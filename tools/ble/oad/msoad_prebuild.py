#!/usr/bin/env python3
"""
MS-OAD Prebuild Script - Run test build to collect objects for slot placement

This script is called as a prebuild step (by CCS or IAR).
It runs a test build with halfModeBuild=true to collect all objects,
then parses the map file and sets slot1ObjectsList in the .syscfg file.
A recursion guard (environment variable) prevents infinite loops.

Usage (TI-Clang / CCS):
    python msoad_prebuild.py <build_dir> <gmake_path>
    python msoad_prebuild.py --compiler ticlang --build-dir <dir> --gmake <path>

Usage (IAR):
    python msoad_prebuild.py --compiler iar --ewp <ewp_path> --config <config> --iarbuild <iarbuild_path> --sysconfig <sysconfig_cli> --sdk <sdk_path>

Where:
    build_dir     - (TI-Clang) The Release or Debug directory containing the makefile.
    gmake_path    - (TI-Clang) Path to gmake executable.
    ewp_path      - (IAR) Path to the .ewp project file.
    config        - (IAR) Build configuration name (e.g., Release, Debug).
    iarbuild      - (IAR) Path to IarBuild.exe.
    sysconfig_cli - (IAR) Path to sysconfig_cli.bat (SysConfig command-line tool).
    sdk_path      - (IAR) Path to the SDK root directory.

IAR Prebuild Command (in IAR IDE Build Actions, "Before compiling/assembling"):
    python "$SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR$/tools/ble/oad/msoad_prebuild.py" --compiler iar --ewp "$PROJ_DIR$/$PROJ_FNAME$.ewp" --config $CONFIG_NAME$ --iarbuild "$TOOLKIT_DIR$/../common/bin/IarBuild.exe" --sysconfig "$SYSCONFIG_ROOT$/sysconfig_cli.bat" --sdk "$SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR$"

Compiling to Standalone Executable:
    This script can be compiled into a standalone executable using PyInstaller.
    This removes the need for Python to be installed on the build machine.

    NOTE: PyInstaller does not support cross-compilation. You must compile on
    each target platform separately (Linux binary on Linux, Windows .exe on Windows).

    Prerequisites:
        pip install pyinstaller

    Linux:
        cd <SDK>/tools/ble/oad
        pyinstaller --onefile --name msoad_prebuild --add-data "mapfile_parser.py:." msoad_prebuild.py
        mv dist/msoad_prebuild .
        rm -rf dist build msoad_prebuild.spec

    Windows (run in Command Prompt or PowerShell):
        cd <SDK>\\tools\\ble\\oad
        pyinstaller --onefile --name msoad_prebuild --add-data "mapfile_parser.py;." msoad_prebuild.py
        move dist\\msoad_prebuild.exe .
        rmdir /s /q dist build
        del msoad_prebuild.spec

    Note: On Windows, --add-data uses semicolon (;) as separator instead of colon (:).

    The resulting executable (~6MB) bundles Python and all dependencies.
"""

import argparse
import os
import re
import subprocess
import sys
import shutil
from pathlib import Path
from typing import Dict, Optional, Set, Tuple

# Import the mapfile parser
from mapfile_parser import extract_objects_from_mapfile


# Environment variable to prevent recursive prebuild
MSOAD_PREBUILD_ENV = "MSOAD_PREBUILD_RUNNING"


def log(message: str, flush: bool = True) -> None:
    """Print a message with prefix."""
    print(f"[MS-OAD Prebuild] {message}", flush=flush)


def find_syscfg_file(project_dir: Path) -> Optional[Path]:
    """Find the .syscfg file in the project directory."""
    syscfg_files = list(project_dir.glob("*.syscfg"))
    if syscfg_files:
        return syscfg_files[0]
    return None


def parse_mapfile_for_slot1_objects(map_file: Path, compiler: str = 'ticlang') -> Tuple[str, Dict, Set]:
    """
    Parse a map file and return the list of objects for slot1ObjectsList.

    Args:
        map_file: Path to the .map file
        compiler: 'ticlang' or 'iar'

    Returns:
        Tuple of:
        - Newline-separated string of library and object names
        - Dict of libraries (for debug)
        - Set of standalone objects (for debug)
    """
    libraries, standalone = extract_objects_from_mapfile(map_file, compiler=compiler)

    # Build the list: libraries first, then standalone objects
    objects_list = []
    for lib in sorted(libraries.keys()):
        objects_list.append(lib)
    for obj in sorted(standalone):
        objects_list.append(obj)

    return '\n'.join(objects_list), libraries, standalone


def set_slot1_objects_list(syscfg_file: Path, objects_list: str) -> bool:
    """
    Set ble.slot1ObjectsList in the syscfg file.

    Args:
        syscfg_file: Path to the .syscfg file
        objects_list: Newline-separated string of library and object names

    Returns:
        True if successful, False otherwise
    """
    try:
        content = syscfg_file.read_text()

        # Escape the objects list for JavaScript string (handle newlines)
        # In syscfg, multiline strings use \n
        # We need to escape: backslashes, quotes, and newlines
        escaped_list = objects_list.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n')

        # Build the new line to insert (don't use regex replacement to avoid escaping issues)
        new_line = f'ble.slot1ObjectsList = "{escaped_list}";'

        # Pattern to match existing slot1ObjectsList (handles multiline values)
        # Use DOTALL to match across newlines in case of corrupted multiline strings
        pattern = r'ble\.slot1ObjectsList\s*=\s*"[^"]*"\s*;?'

        if re.search(pattern, content, re.DOTALL):
            # Line exists - replace with new value using a lambda to avoid escape issues
            new_content = re.sub(pattern, lambda m: new_line, content, flags=re.DOTALL)
            syscfg_file.write_text(new_content)
            log(f"Updated ble.slot1ObjectsList in {syscfg_file.name}")
        else:
            # Line doesn't exist - add it after ble.halfModeBuild or ble.oadMethod
            # Try after halfModeBuild first - use string find/replace to avoid regex escaping issues
            half_mode_match = re.search(r'ble\.halfModeBuild\s*=\s*(true|false)\s*;?', content)
            if half_mode_match:
                insert_pos = half_mode_match.end()
                new_content = content[:insert_pos] + '\n' + new_line + content[insert_pos:]
                syscfg_file.write_text(new_content)
                log(f"Added ble.slot1ObjectsList in {syscfg_file.name}")
            else:
                # Try after oadMethod
                oad_method_match = re.search(r'ble\.oadMethod\s*=\s*"MULTISTEP_OAD"\s*;?', content)
                if oad_method_match:
                    insert_pos = oad_method_match.end()
                    new_content = content[:insert_pos] + '\n' + new_line + content[insert_pos:]
                    syscfg_file.write_text(new_content)
                    log(f"Added ble.slot1ObjectsList in {syscfg_file.name}")
                else:
                    log(f"WARNING: Could not add ble.slot1ObjectsList")
                    return False

        return True

    except Exception as e:
        log(f"ERROR setting slot1ObjectsList: {e}")
        return False


def set_half_mode_build(syscfg_file: Path, value: bool) -> bool:
    """
    Set ble.halfModeBuild in the syscfg file.

    If the line doesn't exist, it will be added after ble.oadMethod.

    Args:
        syscfg_file: Path to the .syscfg file
        value: True or False

    Returns:
        True if successful, False otherwise
    """
    try:
        content = syscfg_file.read_text()
        value_str = "true" if value else "false"

        # Pattern to match: ble.halfModeBuild = true/false (with optional spaces and semicolon)
        pattern = r'ble\.halfModeBuild\s*=\s*(true|false)\s*;?'

        if re.search(pattern, content):
            # Line exists - replace with new value
            new_content = re.sub(pattern, f'ble.halfModeBuild = {value_str};', content)
            syscfg_file.write_text(new_content)
            log(f"Set ble.halfModeBuild = {value_str} in {syscfg_file.name}")
        else:
            # Line doesn't exist - add it after ble.oadMethod
            oad_method_pattern = r'(ble\.oadMethod\s*=\s*"MULTISTEP_OAD"\s*;?)'
            new_content = re.sub(
                oad_method_pattern,
                rf'\1\nble.halfModeBuild = {value_str};',
                content
            )
            if new_content != content:
                syscfg_file.write_text(new_content)
                log(f"Added ble.halfModeBuild = {value_str} in {syscfg_file.name}")
            else:
                log(f"WARNING: Could not find ble.oadMethod to insert halfModeBuild after")
                return False

        return True

    except Exception as e:
        log(f"ERROR setting halfModeBuild: {e}")
        return False


def is_prebuild_running() -> bool:
    """Check if we're already inside a prebuild call."""
    return os.environ.get(MSOAD_PREBUILD_ENV, "0") == "1"


def _find_map_file_ticlang(build_dir: Path) -> Optional[Path]:
    """Find map file for TI-Clang build (in build_dir).

    In CCS, the project folder name matches the project name,
    and build outputs go into <project_name>/Release/ or <project_name>/Debug/.
    The map file is always named <project_name>.map.
    """
    project_name = build_dir.parent.name
    map_file = build_dir / f"{project_name}.map"
    if map_file.exists():
        return map_file
    return None


def _find_map_file_iar(ewp_path: Path, config: str) -> Optional[Path]:
    """Find map file for IAR build (in <ewp_dir>/<config>/List/).

    The map file is named after the project, same as the .ewp file.
    """
    project_name = ewp_path.stem
    ewp_dir = ewp_path.parent

    # Primary: <ewp_dir>/<config>/List/<project_name>.map
    map_file = ewp_dir / config / "List" / f"{project_name}.map"
    if map_file.exists():
        return map_file

    # Fallback: <ewp_dir>/<config>/<project_name>.map
    map_file = ewp_dir / config / f"{project_name}.map"
    if map_file.exists():
        return map_file

    return None


def _process_map_file(map_file: Path, syscfg_file: Optional[Path],
                      compiler: str, context: str, temp_dir: Path) -> bool:
    """Save and parse the map file, then update syscfg. Returns True on success."""

    # Remove and recreate the temp directory to start fresh
    if temp_dir.exists():
        shutil.rmtree(temp_dir)
        log(f"[{context}] Removed existing temp directory: {temp_dir}")
    temp_dir.mkdir(parents=True)

    # Save the map file before it gets overwritten
    test_map_file = temp_dir / map_file.with_suffix(".test.map").name
    shutil.copy2(map_file, test_map_file)
    log(f"[{context}] Saved test build map file to {test_map_file}")

    # Parse the map file to get slot1 objects list
    if syscfg_file:
        log(f"[{context}] Parsing map file for slot1 objects (compiler={compiler})...")
        objects_list, libraries, standalone = parse_mapfile_for_slot1_objects(
            map_file, compiler=compiler
        )
        object_count = len(objects_list.split('\n')) if objects_list else 0
        log(f"[{context}] Found {object_count} objects for slot1 "
            f"({len(libraries)} libraries, {len(standalone)} standalone)")

        # Set slot1ObjectsList in syscfg
        log(f"[{context}] Setting slot1ObjectsList in syscfg...")
        if not set_slot1_objects_list(syscfg_file, objects_list):
            log(f"[{context}] ERROR: Failed to set slot1ObjectsList in syscfg")
            return False

    return True


def run_sysconfig_cli(sysconfig_cli: str, sdk_path: Path,
                      syscfg_file: Path, output_dir: Path,
                      compiler: str = "iar") -> int:
    """
    Run SysConfig CLI to regenerate output files.

    This is needed for IAR because SysConfig runs via .ipcf/ninja BEFORE
    build actions, so after the prebuild modifies .syscfg, we must re-run
    SysConfig to update the generated files.

    Args:
        sysconfig_cli: Path to sysconfig_cli.bat / sysconfig_cli
        sdk_path: Path to the SDK root directory
        syscfg_file: Path to the .syscfg file
        output_dir: Output directory for generated files
        compiler: Compiler type for SysConfig (default: iar)

    Returns:
        0 on success, non-zero on failure
    """
    product_json = sdk_path / ".metadata" / "product.json"

    if not product_json.exists():
        log(f"WARNING: product.json not found at {product_json}")
        return 1

    cmd = [
        sysconfig_cli,
        "-o", str(output_dir),
        "-s", str(product_json),
        "--compiler", compiler,
        str(syscfg_file),
    ]

    log(f"Re-running SysConfig CLI to regenerate output files...")
    log(f"  Command: {' '.join(cmd)}")

    try:
        result = subprocess.run(
            cmd,
            cwd=output_dir,
        )
        if result.returncode != 0:
            log(f"WARNING: SysConfig CLI returned {result.returncode}")
        else:
            log(f"SysConfig CLI completed successfully")
        return result.returncode
    except FileNotFoundError:
        log(f"ERROR: SysConfig CLI not found at '{sysconfig_cli}'")
        return 1
    except Exception as e:
        log(f"ERROR running SysConfig CLI: {e}")
        return 1


def run_test_build_ticlang(build_dir: Path, syscfg_file: Optional[Path],
                           gmake_path: str) -> int:
    """
    Run the test build using gmake (TI-Clang / CCS).

    Args:
        build_dir: Path to the build directory (Release/Debug)
        syscfg_file: Path to the .syscfg file (optional)
        gmake_path: Path to gmake executable

    Returns:
        0 on success, non-zero on failure
    """
    context = "OUTER/MAIN"
    makefile = build_dir / "makefile"

    if not makefile.exists():
        log(f"[{context}] ERROR: makefile not found at {makefile}")
        return 1

    # Set halfModeBuild = true before test build
    if syscfg_file:
        log(f"[{context}] Setting halfModeBuild = true for test build")
        if not set_half_mode_build(syscfg_file, True):
            log(f"[{context}] ERROR: Failed to set halfModeBuild = true")
            return 1

    log(f"[{context}] Running TI-Clang test build in {build_dir}")

    # Set environment variable to prevent recursion
    env = os.environ.copy()
    env[MSOAD_PREBUILD_ENV] = "1"

    # Remove make jobserver variables to avoid conflicts with nested make
    # This is necessary because CCS runs prebuild from within make, and the
    # jobserver authentication tokens don't work for independently spawned make
    for var in ["MAKEFLAGS", "MAKELEVEL", "MFLAGS"]:
        env.pop(var, None)

    try:
        # Run gmake with the test environment (CCS uses gmake on both Linux and Windows)
        result = subprocess.run(
            [gmake_path, "-f", str(makefile), "all"],
            cwd=build_dir,
            env=env,
            # Don't capture output - let it flow to console
        )

        if result.returncode != 0:
            log(f"[{context}] Test build FAILED with return code {result.returncode}")
            if syscfg_file:
                log(f"[{context}] Restoring halfModeBuild = false after failed test build")
                if not set_half_mode_build(syscfg_file, False):
                    log(f"[{context}] ERROR: Failed to restore halfModeBuild = false")
            return result.returncode

        log(f"[{context}] Test build completed successfully")

        # Find and process the map file
        map_file = _find_map_file_ticlang(build_dir)
        if map_file:
            temp_dir = build_dir / "msOadTemp"
            if not _process_map_file(map_file, syscfg_file, 'ticlang', context, temp_dir):
                if syscfg_file:
                    set_half_mode_build(syscfg_file, False)
                return 1
        else:
            log(f"[{context}] ERROR: No map file found in {build_dir}")
            if syscfg_file:
                set_half_mode_build(syscfg_file, False)
            return 1

        # Restore halfModeBuild = false after successful test build
        if syscfg_file:
            log(f"[{context}] Restoring halfModeBuild = false for main build")
            if not set_half_mode_build(syscfg_file, False):
                log(f"[{context}] ERROR: Failed to restore halfModeBuild = false")
                return 1

        return 0

    except FileNotFoundError:
        log(f"[{context}] ERROR: gmake not found at '{gmake_path}'")
        log(f"[{context}] Check that the path is correct. Expected format:")
        log(f"[{context}]   ${{CCS_INSTALL_ROOT}}/utils/bin/gmake")
        return 1
    except Exception as e:
        log(f"[{context}] ERROR: {e}")
        return 1


def run_test_build_iar(ewp_path: Path, config: str, iarbuild_path: str,
                       syscfg_file: Optional[Path],
                       sysconfig_cli: Optional[str] = None,
                       sdk_path: Optional[Path] = None,
                       varfile: Optional[Path] = None) -> int:
    """
    Run the test build using IarBuild (IAR).

    Args:
        ewp_path: Path to the .ewp project file
        config: Build configuration name (e.g., Release, Debug)
        iarbuild_path: Path to IarBuild.exe
        syscfg_file: Path to the .syscfg file (optional)
        sysconfig_cli: Path to sysconfig_cli.bat (optional, for re-running SysConfig)
        sdk_path: Path to SDK root (optional, for re-running SysConfig)

    Returns:
        0 on success, non-zero on failure
    """
    context = "OUTER/MAIN"
    ewp_dir = ewp_path.parent

    if not ewp_path.exists():
        log(f"[{context}] ERROR: .ewp file not found at {ewp_path}")
        return 1

    # Set halfModeBuild = true before test build
    if syscfg_file:
        log(f"[{context}] Setting halfModeBuild = true for test build")
        if not set_half_mode_build(syscfg_file, True):
            log(f"[{context}] ERROR: Failed to set halfModeBuild = true")
            return 1

    log(f"[{context}] Running IAR test build: {ewp_path.name} [{config}]")

    # Set environment variable to prevent recursion
    env = os.environ.copy()
    env[MSOAD_PREBUILD_ENV] = "1"

    # Build IarBuild command. Pass -varfile so the nested build resolves
    # $SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR$ etc. without relying on
    # per-user IAR config (which doesn't exist in CI).
    # Explicit --varfile takes priority; fall back to the SDK-shipped file.
    iarbuild_cmd = [iarbuild_path, str(ewp_path), "-build", config]
    resolved_varfile = varfile or (
        sdk_path / "tools" / "iar" / "SIMPLELINK_LOWPOWER_F3_SDK.custom_argvars"
        if sdk_path else None
    )
    if resolved_varfile is None:
        log(f"[{context}] ERROR: No -varfile provided and no --sdk path to derive one from")
        return 1
    if not resolved_varfile.exists():
        log(f"[{context}] ERROR: varfile not found: {resolved_varfile}")
        return 1
    iarbuild_cmd.extend(["-varfile", str(resolved_varfile)])
    log(f"[{context}] Using -varfile {resolved_varfile}")

    try:
        # Run iarbuild to build the project
        # Syntax: IarBuild.exe <project.ewp> -build <configuration> [-varfile <path>]
        result = subprocess.run(
            iarbuild_cmd,
            cwd=ewp_dir,
            env=env,
            # Don't capture output - let it flow to console
        )

        if result.returncode != 0:
            log(f"[{context}] Test build FAILED with return code {result.returncode}")
            if syscfg_file:
                log(f"[{context}] Restoring halfModeBuild = false after failed test build")
                if not set_half_mode_build(syscfg_file, False):
                    log(f"[{context}] ERROR: Failed to restore halfModeBuild = false")
            return result.returncode

        log(f"[{context}] Test build completed successfully")

        # Find and process the map file (IAR puts it in <config>/List/)
        map_file = _find_map_file_iar(ewp_path, config)
        if map_file:
            temp_dir = ewp_dir / config / "msOadTemp"
            if not _process_map_file(map_file, syscfg_file, 'iar', context, temp_dir):
                if syscfg_file:
                    set_half_mode_build(syscfg_file, False)
                return 1
        else:
            log(f"[{context}] ERROR: No map file found in {ewp_dir / config / 'List'}")
            if syscfg_file:
                set_half_mode_build(syscfg_file, False)
            return 1

        # Restore halfModeBuild = false after successful test build
        if syscfg_file:
            log(f"[{context}] Restoring halfModeBuild = false for main build")
            if not set_half_mode_build(syscfg_file, False):
                log(f"[{context}] ERROR: Failed to restore halfModeBuild = false")
                return 1

        # Re-run SysConfig CLI to regenerate output files with the final
        # .syscfg state (halfModeBuild=false, slot1ObjectsList populated).
        # This is critical for IAR because SysConfig runs via .ipcf/ninja
        # BEFORE build actions, so the initial SysConfig run used the OLD
        # .syscfg content. We must regenerate to get correct linker directives.
        if sysconfig_cli and sdk_path and syscfg_file:
            log(f"[{context}] Re-running SysConfig to regenerate output files...")
            sc_result = run_sysconfig_cli(
                sysconfig_cli, sdk_path, syscfg_file, ewp_dir, compiler="iar"
            )
            if sc_result != 0:
                log(f"[{context}] ERROR: SysConfig re-run failed (rc={sc_result})")
                return sc_result

        return 0

    except FileNotFoundError:
        log(f"[{context}] ERROR: IarBuild not found at '{iarbuild_path}'")
        log(f"[{context}] Check that the path is correct. Expected format:")
        log(f"[{context}]   $TOOLKIT_DIR$\\..\\common\\bin\\IarBuild.exe")
        return 1
    except Exception as e:
        log(f"[{context}] ERROR: {e}")
        return 1


def parse_args():
    """Parse command-line arguments with backward compatibility."""
    parser = argparse.ArgumentParser(
        description="MS-OAD Prebuild Script - Run test build to collect objects for slot placement",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  TI-Clang (positional, backward compatible):
    %(prog)s Release /path/to/gmake

  TI-Clang (named):
    %(prog)s --compiler ticlang --build-dir Release --gmake /path/to/gmake

  IAR:
    %(prog)s --compiler iar --ewp project.ewp --config Release --iarbuild IarBuild.exe --sysconfig sysconfig_cli.bat --sdk C:\\SDK
"""
    )

    parser.add_argument(
        "--compiler",
        choices=["ticlang", "iar"],
        default="ticlang",
        help="Compiler/IDE environment (default: ticlang)"
    )

    # TI-Clang arguments (named)
    parser.add_argument(
        "--build-dir",
        type=Path,
        help="(TI-Clang) Path to Release or Debug directory containing the makefile"
    )
    parser.add_argument(
        "--gmake",
        help="(TI-Clang) Path to gmake executable"
    )

    # IAR arguments
    parser.add_argument(
        "--ewp",
        type=Path,
        help="(IAR) Path to the .ewp project file"
    )
    parser.add_argument(
        "--config",
        help="(IAR) Build configuration name (e.g., Release, Debug)"
    )
    parser.add_argument(
        "--iarbuild",
        help="(IAR) Path to IarBuild.exe"
    )
    parser.add_argument(
        "--sysconfig",
        help="(IAR) Path to sysconfig_cli.bat for re-running SysConfig after prebuild"
    )
    parser.add_argument(
        "--sdk",
        type=Path,
        help="(IAR) Path to SDK root directory (for SysConfig product.json)"
    )
    parser.add_argument(
        "--varfile",
        type=Path,
        help="(IAR) Path to .custom_argvars file for IarBuild. "
             "If omitted, defaults to <sdk>/tools/iar/SIMPLELINK_LOWPOWER_F3_SDK.custom_argvars"
    )

    # Backward-compatible positional args for TI-Clang
    parser.add_argument(
        "positional_args",
        nargs="*",
        help=argparse.SUPPRESS  # Hidden: <build_dir> <gmake_path> for backward compatibility
    )

    args = parser.parse_args()

    # Handle backward-compatible positional args for TI-Clang
    if args.positional_args and args.compiler == "ticlang":
        if len(args.positional_args) >= 2:
            if not args.build_dir:
                args.build_dir = Path(args.positional_args[0])
            if not args.gmake:
                args.gmake = args.positional_args[1]
        elif len(args.positional_args) == 1 and not args.build_dir:
            args.build_dir = Path(args.positional_args[0])

    # Validate required args based on compiler
    if args.compiler == "ticlang":
        if not args.build_dir or not args.gmake:
            parser.error("TI-Clang mode requires build_dir and gmake_path.\n"
                         "  Usage: msoad_prebuild <build_dir> <gmake_path>\n"
                         "  Or:    msoad_prebuild --compiler ticlang --build-dir <dir> --gmake <path>")
    elif args.compiler == "iar":
        if not args.ewp or not args.config or not args.iarbuild:
            parser.error("IAR mode requires --ewp, --config, and --iarbuild.\n"
                         "  Usage: msoad_prebuild --compiler iar --ewp <ewp_path> "
                         "--config <config> --iarbuild <iarbuild_path>")
        if not args.sysconfig or not args.sdk:
            parser.error("IAR mode requires --sysconfig and --sdk for SysConfig re-generation.\n"
                         "  Add: --sysconfig <path/to/sysconfig_cli.bat> --sdk <path/to/sdk>")

    return args


def main():
    # Determine if this is the outer (first) or inner (nested) invocation
    is_inner = is_prebuild_running()
    context = "INNER/NESTED" if is_inner else "OUTER/MAIN"

    log("")
    log("=" * 60)
    log(f">>> MS-OAD Prebuild Script [{context}] <<<")
    log("=" * 60)

    # Check if we're already in a prebuild (recursion guard)
    if is_inner:
        log("")
        log("-" * 60)
        log(f"[{context}] SKIPPING - This is the nested call from test build")
        log(f"[{context}] The outer prebuild spawned a build which triggered")
        log(f"[{context}] this prebuild again. Skipping to avoid infinite recursion.")
        log("-" * 60)
        log(f">>> End MS-OAD Prebuild Script [{context}] <<<")
        log("=" * 60)
        log("")
        return 0

    # Parse arguments
    args = parse_args()

    compiler = args.compiler
    log(f"Compiler: {compiler}")

    if compiler == "ticlang":
        build_dir = args.build_dir.resolve()

        if not build_dir.exists():
            log(f"ERROR: Build directory not found: {build_dir}")
            return 1

        log(f"Using gmake: {args.gmake}")

        # Find the syscfg file (parent of build dir for TI-Clang)
        syscfg_file = find_syscfg_file(build_dir.parent)
        if syscfg_file:
            log(f"Found syscfg file: {syscfg_file.name}")
        else:
            log("WARNING: No .syscfg file found - halfModeBuild toggle will be skipped")

        log("")
        log("-" * 60)
        log(f"[{context}] This is the first prebuild invocation.")
        log(f"[{context}] Will now spawn 'gmake all' with {MSOAD_PREBUILD_ENV}=1")
        log(f"[{context}] to run the TEST BUILD (compiles all objects).")
        log("-" * 60)
        log("")
        log(f"[{context}] vvvvvv TEST BUILD OUTPUT BELOW vvvvvv")
        log("")

        result = run_test_build_ticlang(build_dir, syscfg_file, args.gmake)

    elif compiler == "iar":
        ewp_path = args.ewp.resolve()
        ewp_dir = ewp_path.parent

        if not ewp_path.exists():
            log(f"ERROR: .ewp file not found: {ewp_path}")
            return 1

        log(f"EWP file: {ewp_path}")
        log(f"Config: {args.config}")
        log(f"IarBuild: {args.iarbuild}")
        if args.sysconfig:
            log(f"SysConfig CLI: {args.sysconfig}")
        if args.sdk:
            log(f"SDK path: {args.sdk}")
        if args.varfile:
            log(f"Varfile: {args.varfile}")

        # Find the syscfg file (same directory as .ewp for IAR)
        syscfg_file = find_syscfg_file(ewp_dir)
        if syscfg_file:
            log(f"Found syscfg file: {syscfg_file.name}")
        else:
            log("WARNING: No .syscfg file found - halfModeBuild toggle will be skipped")

        log("")
        log("-" * 60)
        log(f"[{context}] This is the first prebuild invocation.")
        log(f"[{context}] Will now spawn 'iarbuild -build {args.config}' with {MSOAD_PREBUILD_ENV}=1")
        log(f"[{context}] to run the TEST BUILD (compiles all objects).")
        log("-" * 60)
        log("")
        log(f"[{context}] vvvvvv TEST BUILD OUTPUT BELOW vvvvvv")
        log("")

        result = run_test_build_iar(
            ewp_path, args.config, args.iarbuild, syscfg_file,
            sysconfig_cli=args.sysconfig,
            sdk_path=args.sdk,
            varfile=args.varfile,
        )

    log("")
    log(f"[{context}] ^^^^^^ TEST BUILD OUTPUT ABOVE ^^^^^^")
    log("")

    if result == 0:
        log("-" * 60)
        log(f"[{context}] Test build completed successfully!")
        log(f"[{context}] The main build will now continue and find")
        log(f"[{context}] all objects already up-to-date.")
        log("-" * 60)

    log("")
    log("=" * 60)
    log(f">>> End MS-OAD Prebuild Script [{context}] <<<")
    log("=" * 60)
    log("")

    return result


if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3
"""
Commands Runner

Reads a commands file and executes each command after substituting
build-system path placeholders.

The caller supplies the full path to the commands file via --cfg_file.
This makes the same script usable for any build step:

    Pre-build:   --cfg_file <sysconfig_dir>/ti_ble_oad_prebuild.cfg
    Post-build:  --cfg_file <sysconfig_dir>/ti_ble_oad_postbuild.cfg

Lines starting with '#' and empty lines are treated as comments and skipped.
If the commands file does not exist the runner exits silently with code 0.

When --execute_sysconfig is set the runner invokes SysConfig CLI (via
--sysconfig_cli) before reading the commands file.  This is required for
CCS/TI-Clang where pre-build steps run before SysConfig processes the
.syscfg file.  Post-build steps do not need this flag since SysConfig has
already run by then.

Placeholders substituted:
    {{SDK_DIR}}           - SDK installation directory
    {{BUILD_DIR}}         - Build output directory
    {{GMAKE_DIR}}         - (TI-Clang) Path to gmake executable
    {{EWP_DIR}}           - (IAR) Path to the .ewp project file
    {{CONFIG}}            - (IAR) Build configuration name (e.g., Release, Debug)
    {{IAR_BUILD_DIR}}     - (IAR) Path to IarBuild.exe
    {{SYSCONFIG_CLI_DIR}} - Path to sysconfig_cli[.bat]
    {{CG_TOOL_ROOT}}      - Compiler root directory
    {{BASE_NAME}}         - Build artifact base name without extension

Usage (TI-Clang pre-build):
    commands_runner --cfg_file ${SYSCONFIG_TOOL_OUTPUT_DIR}/ti_ble_oad_prebuild.cfg
                    --compiler ticlang
                    --sdk_dir <path>
                    --build_dir <path>
                    --gmake_dir ${CCS_INSTALL_ROOT}/utils/bin/gmake
                    --sysconfig_cli ${SYSCONFIG_TOOL}
                    --execute_sysconfig

Usage (TI-Clang post-build):
    commands_runner --cfg_file ${SYSCONFIG_TOOL_OUTPUT_DIR}/ti_ble_oad_postbuild.cfg
                    --compiler ticlang
                    --sdk_dir <path>
                    --build_dir <path>
                    --base_name <name>
                    --cg_tool_root ${CG_TOOL_ROOT}

Usage (IAR pre-build):
    commands_runner --cfg_file "$$$SYSCONFIG_TOOL_OUTPUT_DIR$$$\ti_ble_oad_prebuild.cfg"
                    --compiler iar
                    --sdk_dir "$SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR$"
                    --ewp "$PROJ_DIR$\$PROJ_FNAME$.ewp"
                    --config "$CONFIG_NAME$"
                    --iarbuild "$TOOLKIT_DIR$\..\common\bin\IarBuild.exe"
                    --sysconfig_cli "$SYSCONFIG_ROOT$\sysconfig_cli.bat"
                    --execute_sysconfig

Usage (IAR post-build):
    commands_runner --cfg_file "$$$SYSCONFIG_TOOL_OUTPUT_DIR$$$\ti_ble_oad_postbuild.cfg"
                    --compiler iar
                    --sdk_dir <path>
                    --cg_tool_root "$TOOLKIT_DIR$"
                    --build_dir "$TARGET_DIR$"
                    --base_name "$TARGET_BNAME$"

Compiling to Standalone Executable:
    See environment/run_oadToolsGen.sh
"""

import argparse
import subprocess
import sys
from pathlib import Path


def log(message: str, flush: bool = True) -> None:
    print(f"[Commands Runner] {message}", flush=flush)


def find_syscfg_file(search_dir: Path):
    """Return the first .syscfg file found in search_dir, or None."""
    files = list(search_dir.glob("*.syscfg"))
    return files[0] if files else None


def invoke_sysconfig_cli(sysconfig_cli: str, output_dir: Path,
                         sdk_dir: str, compiler: str, syscfg_file: Path) -> int:
    """
    Run SysConfig CLI to generate/refresh output files before the commands
    file is read.
    """
    product_json = Path(sdk_dir) / ".metadata" / "product.json"
    if not product_json.exists():
        log(f"ERROR: product.json not found at {product_json}")
        return 1

    output_dir.mkdir(parents=True, exist_ok=True)

    # On Windows sysconfig_cli.bat is needed; try appending .bat if the
    # bare path doesn't exist.
    cli_path = sysconfig_cli
    if not Path(cli_path).exists() and sys.platform == "win32":
        bat = cli_path + ".bat"
        if Path(bat).exists():
            cli_path = bat

    cmd = (f'"{cli_path}" -s "{product_json}" --script "{syscfg_file}" '
           f'-o "{output_dir}" --compiler {compiler}')
    log(f"Running SysConfig CLI...")
    log(f"  {cmd}")

    result = subprocess.run(cmd, shell=True, cwd=str(output_dir))
    if result.returncode != 0:
        log(f"ERROR: SysConfig CLI returned {result.returncode}")
    else:
        log("SysConfig CLI completed successfully")
    return result.returncode


def main():
    parser = argparse.ArgumentParser(
        description='Commands Runner: execute commands from a build-step commands file'
    )

    parser.add_argument('--cfg_file', required=True,
                        help='Full path to the commands file to execute')
    parser.add_argument('--compiler', choices=['ticlang', 'iar'], default='ticlang',
                        help='Compiler toolchain (default: ticlang)')

    # Common
    parser.add_argument('--sdk_dir',
                        help='SDK installation directory ({{SDK_DIR}})')
    parser.add_argument('--build_dir',
                        help='Build output directory ({{BUILD_DIR}})')

    # TI-Clang specific
    parser.add_argument('--gmake_dir',
                        help='Path to gmake executable ({{GMAKE_DIR}})')

    # IAR specific
    parser.add_argument('--ewp',
                        help='Path to .ewp project file ({{EWP_DIR}})')
    parser.add_argument('--config',
                        help='Build configuration name ({{CONFIG}})')
    parser.add_argument('--iarbuild',
                        help='Path to IarBuild.exe ({{IAR_BUILD_DIR}})')

    # Substituted as {{SYSCONFIG_CLI_DIR}}; also used for invocation when
    # --execute_sysconfig is set.
    parser.add_argument('--sysconfig_cli',
                        help='Path to sysconfig_cli[.bat] ({{SYSCONFIG_CLI_DIR}})')
    parser.add_argument('--execute_sysconfig', action='store_true',
                        help='Invoke SysConfig CLI before reading the commands file '
                             '(required for pre-build steps in CCS/TI-Clang)')

    # Post-build specific
    parser.add_argument('--cg_tool_root',
                        help='Compiler root directory ({{CG_TOOL_ROOT}})')
    parser.add_argument('--base_name',
                        help='Build artifact base name without extension ({{BASE_NAME}})')

    args = parser.parse_args()

    cfg_file      = Path(args.cfg_file).resolve()
    sysconfig_dir = cfg_file.parent

    sdk_dir       = str(Path(args.sdk_dir).resolve())      if args.sdk_dir      else ""
    build_dir     = str(Path(args.build_dir).resolve())    if args.build_dir    else ""
    gmake_dir     = args.gmake_dir     or ""
    ewp           = args.ewp           or ""
    config        = args.config        or ""
    iarbuild      = args.iarbuild      or ""
    sysconfig_cli = args.sysconfig_cli or ""
    cg_tool_root  = str(Path(args.cg_tool_root).resolve()) if args.cg_tool_root else ""
    base_name     = args.base_name     or ""

    # Run SysConfig CLI only when explicitly requested.
    if args.execute_sysconfig:
        if build_dir:
            project_dir = Path(build_dir).parent
        elif ewp:
            project_dir = Path(ewp).parent
        else:
            project_dir = sysconfig_dir.parent

        syscfg_file = find_syscfg_file(project_dir)
        if not syscfg_file:
            log(f".syscfg file not found in {project_dir} — no build steps needed")
            return 0

        rc = invoke_sysconfig_cli(sysconfig_cli, sysconfig_dir,
                                  sdk_dir, args.compiler, syscfg_file)
        if rc != 0:
            return rc

    # If the commands file doesn't exist, exit silently
    if not cfg_file.exists():
        log(f"{cfg_file.name} not found in {sysconfig_dir} — no build steps needed")
        return 0

    # Read and parse the commands file
    with open(cfg_file, 'r') as f:
        lines = f.readlines()

    commands = [line.strip() for line in lines
                if line.strip() and not line.strip().startswith('#')]

    if not commands:
        log(f"No commands found in {cfg_file.name} — no build steps needed")
        return 0

    log("")
    log("=" * 60)
    log(f"Commands file:  {cfg_file}")
    log(f"Compiler:       {args.compiler}")
    if sdk_dir:
        log(f"SDK dir:        {sdk_dir}")
    if build_dir:
        log(f"Build dir:      {build_dir}")
    if gmake_dir:
        log(f"Gmake dir:      {gmake_dir}")
    if ewp:
        log(f"EWP:            {ewp}")
    if config:
        log(f"Config:         {config}")
    if iarbuild:
        log(f"IarBuild:       {iarbuild}")
    if sysconfig_cli:
        log(f"SysConfig CLI:  {sysconfig_cli}")
    log(f"Run SysConfig:  {args.execute_sysconfig}")
    if cg_tool_root:
        log(f"CG Tool Root:   {cg_tool_root}")
    if base_name:
        log(f"Base name:      {base_name}")
    log(f"Commands found: {len(commands)}")
    log("=" * 60)
    log("")

    # Substitute placeholders
    substituted = []
    for cmd in commands:
        cmd = cmd.replace("{{SDK_DIR}}",           sdk_dir)
        cmd = cmd.replace("{{BUILD_DIR}}",         build_dir)
        cmd = cmd.replace("{{GMAKE_DIR}}",         gmake_dir)
        cmd = cmd.replace("{{EWP_DIR}}",           ewp)
        cmd = cmd.replace("{{CONFIG}}",            config)
        cmd = cmd.replace("{{IAR_BUILD_DIR}}",     iarbuild)
        cmd = cmd.replace("{{SYSCONFIG_CLI_DIR}}", sysconfig_cli)
        cmd = cmd.replace("{{CG_TOOL_ROOT}}",      cg_tool_root)
        cmd = cmd.replace("{{BASE_NAME}}",         base_name)
        substituted.append(cmd)

    # cwd: use build_dir if provided, fall back to the commands file's directory
    run_dir = build_dir if build_dir else str(sysconfig_dir)

    # Execute each command sequentially
    for i, cmd in enumerate(substituted, 1):
        log(f"[{i}/{len(substituted)}] Executing:")
        log(f"  {cmd}")

        result = subprocess.run(cmd, shell=True, cwd=run_dir)

        if result.returncode != 0:
            log(f"  FAILED with exit code {result.returncode}")
            log("")
            log("=" * 60)
            log(f"FAILED at step {i}/{len(substituted)}")
            log("=" * 60)
            return result.returncode

        log(f"  SUCCESS")

    log("")
    log("-" * 60)
    log("All steps completed successfully!")
    log("-" * 60)
    log("")

    return 0


if __name__ == '__main__':
    sys.exit(main())

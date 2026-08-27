#!/usr/bin/env python3
"""
Map File Parser (TI-Clang and IAR)

Extracts object file names from the MODULE SUMMARY section of a linker map file.
Distinguishes between library objects and standalone object files.
"""

import argparse
import re
import sys
from pathlib import Path
from typing import Dict, List, Set, Tuple
from collections import defaultdict

# IAR section header: e.g. "C:\...\name.dir: [1]" or "OneLib.a: [19]"
_IAR_SECTION_HEADER_RE = re.compile(r'^(.+):\s*\[\d+\]\s*$')


def _read_module_summary_lines(map_path: Path) -> List[str]:
    """Read and return only the lines inside the MODULE SUMMARY section."""
    lines = []
    in_section = False

    with open(map_path, 'r') as f:
        for line in f:
            stripped = line.strip()

            if 'MODULE SUMMARY' in stripped:
                in_section = True
                continue

            if not in_section:
                continue

            # End markers
            if stripped.startswith('Grand Total:'):
                break

            lines.append(stripped)

    return lines


def _skip_to_past_header(lines: List[str]) -> List[str]:
    """Skip column header lines (Module / ------ rows) and return the rest."""
    past_header = False
    result = []
    for line in lines:
        if not past_header:
            if 'Module' in line and 'code' in line:
                past_header = True
                continue
            if line.startswith('------'):
                past_header = True
                continue
            continue
        result.append(line)
    return result


def _is_object_file(name: str) -> bool:
    """Check if a name looks like an object file (.o or .obj)."""
    return name.endswith('.o') or name.endswith('.obj')


def _parse_ticlang(lines: List[str]) -> Tuple[Dict[str, Set[str]], Set[str]]:
    """
    Parse TI-Clang MODULE SUMMARY format.

    Section headers:
        - Directory path ending with ``/`` or ``\\``
        - Library path ending with ``.a``
    Separators: ``+--``
    """
    libraries = defaultdict(set)
    standalone = set()
    current_context = None
    current_library = None

    for line in lines:
        if not line:
            continue

        # Skip separators and totals
        if line.startswith('+--') or line.startswith('Total:'):
            continue

        # Directory header (ends with / or \, or is "<current directory>")
        if line.endswith('/') or line.endswith('\\') or line == '<current directory>':
            current_context = 'directory'
            current_library = None
            continue

        # Library header (ends with .a)
        if line.endswith('.a'):
            current_context = 'library'
            current_library = Path(line).name
            continue

        # Object file line
        parts = line.split()
        if not parts:
            continue

        obj_name = parts[0]
        if not _is_object_file(obj_name):
            continue

        if current_context == 'directory':
            standalone.add(obj_name)
        elif current_context == 'library' and current_library:
            libraries[current_library].add(obj_name)

    return dict(libraries), standalone


def _parse_iar(lines: List[str]) -> Tuple[Dict[str, Set[str]], Set[str]]:
    """
    Parse IAR MODULE SUMMARY format.

    Section headers use ``name: [N]`` syntax:
        - ``OneLib.a: [19]``  -> library
        - ``C:\\...\\dir: [1]`` -> directory (standalone objects)
        - ``command line/config:`` -> skip
    Separators: ``---``
    """
    libraries = defaultdict(set)
    standalone = set()
    current_context = None
    current_library = None

    for line in lines:
        if not line:
            continue

        # Skip IAR decoration and noise
        if line == '***':
            continue
        if line.startswith('---') or line.startswith('Total:'):
            continue
        if line.startswith('Gaps') or line.startswith('Linker created'):
            continue

        # "command line/config:" section — no useful objects
        if line == 'command line/config:':
            current_context = None
            current_library = None
            continue

        # Section header: "path: [N]"
        header_match = _IAR_SECTION_HEADER_RE.match(line)
        if header_match:
            header_path = header_match.group(1).strip()
            if header_path.endswith('.a'):
                current_context = 'library'
                current_library = Path(header_path).name
            else:
                current_context = 'directory'
                current_library = None
            continue

        # Object file line
        parts = line.split()
        if not parts:
            continue

        obj_name = parts[0]
        if not _is_object_file(obj_name):
            continue

        if current_context == 'directory':
            standalone.add(obj_name)
        elif current_context == 'library' and current_library:
            libraries[current_library].add(obj_name)

    return dict(libraries), standalone


def extract_objects_from_mapfile(map_path: Path, compiler: str = 'ticlang') -> Tuple[Dict[str, Set[str]], Set[str]]:
    """
    Extract object file names from the MODULE SUMMARY section of a .map file.

    Args:
        map_path: Path to the .map file
        compiler: ``'ticlang'`` or ``'iar'``

    Returns:
        Tuple of (libraries dict, standalone set)
    """
    raw_lines = _read_module_summary_lines(map_path)
    lines = _skip_to_past_header(raw_lines)

    if compiler == 'iar':
        return _parse_iar(lines)
    return _parse_ticlang(lines)


def print_text_output(libraries: Dict[str, Set[str]], standalone: Set[str]) -> None:
    """Print human-readable text output."""
    # Print standalone objects
    print("=" * 60)
    print(f"STANDALONE OBJECT FILES ({len(standalone)} files)")
    print("=" * 60)
    for obj in sorted(standalone):
        print(f"  {obj}")

    # Print library objects
    print("\n" + "=" * 60)
    print(f"LIBRARY OBJECT FILES ({len(libraries)} libraries)")
    print("=" * 60)
    for lib in sorted(libraries.keys()):
        objs = libraries[lib]
        print(f"\n{lib} ({len(objs)} objects):")
        for obj in sorted(objs):
            print(f"    {obj}")

    # Summary
    total_lib_objs = sum(len(objs) for objs in libraries.values())
    print("\n" + "=" * 60)
    print("SUMMARY")
    print("=" * 60)
    print(f"  Standalone objects: {len(standalone)}")
    print(f"  Libraries:          {len(libraries)}")
    print(f"  Library objects:    {total_lib_objs}")
    print(f"  Total unique:       {len(standalone) + total_lib_objs}")


def main():
    parser = argparse.ArgumentParser(
        description="Extract object file names from a TI-Clang or IAR linker map file."
    )
    parser.add_argument(
        "mapfile",
        type=Path,
        help="Path to the .map file"
    )
    parser.add_argument(
        "--compiler",
        choices=["ticlang", "iar"],
        default="ticlang",
        help="Map file format (default: ticlang)"
    )
    parser.add_argument(
        "--list", "-l",
        action="store_true",
        help="Output library and object file names only (one per line)"
    )

    args = parser.parse_args()

    if not args.mapfile.exists():
        print(f"ERROR: Map file not found: {args.mapfile}", file=sys.stderr)
        return 1

    libraries, standalone = extract_objects_from_mapfile(args.mapfile, args.compiler)

    if args.list:
        for lib in sorted(libraries.keys()):
            print(lib)
        for obj in sorted(standalone):
            print(obj)
    else:
        print_text_output(libraries, standalone)

    return 0


if __name__ == "__main__":
    sys.exit(main())

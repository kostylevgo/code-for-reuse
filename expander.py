#!/usr/bin/env python3
# AI-generated
"""
Expand C++ quote includes (#include "file.h") recursively into a single file.
Resolution order for quote includes:
  1. Relative to the directory of the including file.
  2. Relative to <cmake_root>/lib.
  3. Relative to each directory listed in include_directories() in the CMakeLists.txt
     found at <cmake_root>, in the order they appear.
If none yields an existing file, the original #include line is preserved.
Usage: python expander.py [--no-clipboard] <main_file>
"""

import sys
import re
import argparse
import subprocess
from pathlib import Path

from utils.utils import find_cmake_root, strip_guards_and_pragma

def parse_include_directories(cmake_root: Path) -> list[Path]:
    """
    Parse CMakeLists.txt at cmake_root and extract all directory arguments
    from include_directories() commands. Returns a list of absolute paths.
    Assumes no CMake variables are used and the syntax is simple.
    """
    cmake_file = cmake_root / "CMakeLists.txt"
    if not cmake_file.is_file():
        return []

    try:
        content = cmake_file.read_text(encoding='utf-8')
    except Exception as e:
        print(f"Warning: could not read {cmake_file}: {e}", file=sys.stderr)
        return []

    # Find all include_directories(...) blocks (handles multi-line)
    pattern = r'include_directories\s*\((.*?)\)'
    matches = re.findall(pattern, content, re.DOTALL)

    dirs = []
    for match in matches:
        # Split by whitespace and commas, ignore empty tokens
        tokens = re.split(r'[\s,]+', match.strip())
        for token in tokens:
            if not token:
                continue
            # Strip surrounding quotes if any
            token = token.strip('"\'')
            if token[0] == '/':
                # Resolve absolute path
                path = Path(token).resolve()
            else:
                # Resolve relative to cmake_root (absolute path)
                path = (cmake_root / token).resolve()
            if path.is_dir():
                dirs.append(path)
    return dirs

cwd = Path.cwd()
cmake_root = find_cmake_root(cwd)

# Fallback base directory (old behaviour)
if cmake_root is None:
    print("Warning: Could not find a CMakeLists.txt ancestor. "
          "Falling back to './lib' for second resolution step.", file=sys.stderr)
    base_dir = Path("./lib").resolve()
    include_dirs = []
else:
    base_dir = cmake_root / "lib"
    if not base_dir.is_dir():
        print(f"Warning: {base_dir} does not exist. "
              "Falling back to './lib'.", file=sys.stderr)
        base_dir = Path("./lib").resolve()
    include_dirs = parse_include_directories(cmake_root)
    if include_dirs:
        print(f"Found {len(include_dirs)} include directories from CMakeLists.txt.", file=sys.stderr)

# ----------------------------------------------------------------------
# Recursive file expansion
# ----------------------------------------------------------------------
def expand_file(filepath: Path, included_files: set, seen_angle_includes: set):
    """
    Recursively expand a file:
      - quote includes are resolved with the multi‑step fallback (see module docstring)
      - angle‑bracket includes are kept only the first time they appear
      - #pragma once and inclusion guards are stripped from each file
    Returns a list of lines (without trailing newlines).
    """
    filepath = filepath.resolve()
    if filepath in included_files:
        return []   # already expanded, skip
    included_files.add(filepath)

    try:
        content = filepath.read_text(encoding='utf-8')
    except FileNotFoundError:
        print(f"Warning: file not found: {filepath}", file=sys.stderr)
        return []

    lines = content.splitlines(keepends=False)
    lines = strip_guards_and_pragma(lines)

    output = []
    includer_dir = filepath.parent

    for line in lines:
        # Quote include: #include "file.h"
        m = re.match(r'^\s*#include\s+"([^"]+)"', line)
        if m:
            include_path = m.group(1)

            # Build a list of candidates in order of priority
            candidates = []

            # 1. Relative to includer's directory
            candidates.append((includer_dir / include_path).resolve())

            # 2. Relative to base_dir
            candidates.append((base_dir / include_path).resolve())

            # 3. Relative to each include_directories path
            for inc_dir in include_dirs:
                candidates.append((inc_dir / include_path).resolve())

            resolved = None
            for cand in candidates:
                if cand.is_file():
                    resolved = cand
                    break

            if resolved is None:
                # None of the candidates exist – keep the original include line
                print(f"Warning: Could not resolve include '{include_path}' "
                      f"from file {filepath}. Keeping line.", file=sys.stderr)
                output.append(line)
                continue

            if resolved in included_files:
                continue   # already expanded, skip this include line

            # Recursively expand the included file
            expanded = expand_file(resolved, included_files, seen_angle_includes)
            if expanded:
                output.extend(expanded)
                # Insert a blank line after the expanded chunk
                output.append('')
            # If expanded is empty, we do not add an extra blank line
            continue

        # Angle-bracket include: #include <header>
        m = re.match(r'^\s*#include\s+<([^>]+)>', line)
        if m:
            header_name = m.group(1)
            if header_name in seen_angle_includes:
                continue   # keep only the first occurrence
            seen_angle_includes.add(header_name)
            output.append(line)
            continue

        # Ordinary line: keep it as is
        output.append(line)

    return output

# ----------------------------------------------------------------------
# Post‑processing: strip trailing spaces, compress blank lines,
# remove trailing blank lines, no forced newline at EOF.
# ----------------------------------------------------------------------
def post_process(lines):
    # Strip trailing spaces and tabs from each line
    lines = [line.rstrip(' \t') for line in lines]

    # Compress consecutive blank lines to at most one
    compressed = []
    last_was_empty = False
    for line in lines:
        if line == '':
            if not last_was_empty:
                compressed.append('')
                last_was_empty = True
        else:
            compressed.append(line)
            last_was_empty = False

    # Remove trailing blank lines
    while compressed and compressed[-1] == '':
        compressed.pop()

    return compressed

# ----------------------------------------------------------------------
# Clipboard copy (cross‑platform fallback)
# ----------------------------------------------------------------------
def copy_to_clipboard(text):
    try:
        import pyperclip
        pyperclip.copy(text)
        return
    except ImportError:
        pass

    # Fallback to platform‑specific commands
    try:
        if sys.platform == 'win32':
            p = subprocess.Popen(['clip'], stdin=subprocess.PIPE, shell=True)
            p.communicate(input=text.encode('utf-8'))
        elif sys.platform == 'darwin':
            p = subprocess.Popen(['pbcopy'], stdin=subprocess.PIPE)
            p.communicate(input=text.encode('utf-8'))
        else:
            # Try xclip, then xsel
            try:
                p = subprocess.Popen(['xclip', '-selection', 'clipboard'], stdin=subprocess.PIPE)
                p.communicate(input=text.encode('utf-8'))
            except FileNotFoundError:
                p = subprocess.Popen(['xsel', '--clipboard', '--input'], stdin=subprocess.PIPE)
                p.communicate(input=text.encode('utf-8'))
    except Exception as e:
        print(f"Warning: could not copy to clipboard: {e}", file=sys.stderr)

# ----------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------
def main():
    parser = argparse.ArgumentParser(description='Expand C++ quote includes recursively.')
    parser.add_argument('file', help='Main C++ file to expand')
    parser.add_argument('--no-clipboard', action='store_true',
                        help='Disable copying the result to the clipboard')
    args = parser.parse_args()

    main_file = Path(args.file)
    if not main_file.is_file():
        print(f"Error: file not found: {main_file}", file=sys.stderr)
        sys.exit(1)

    included_files = set()
    seen_angle_includes = set()

    # Expand the main file (it will be added to included_files inside expand_file)
    output_lines = expand_file(main_file, included_files, seen_angle_includes)

    # Post‑process
    output_lines = post_process(output_lines)
    result = '\n'.join(output_lines)

    # Write to stdout (no extra newline)
    sys.stdout.write(result)

    # Copy to clipboard unless disabled
    if not args.no_clipboard:
        copy_to_clipboard(result)

if __name__ == '__main__':
    main()
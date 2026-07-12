#!/usr/bin/env python3
"""
Automatically add #ifndef/#define/#endif include guards to all .h and .hpp files
in the current working directory (recursively).

It removes existing #pragma once and standard #ifndef guards, then inserts a new
guard name constructed as:
    <PROJECT_NAME>_<RELATIVE_PATH_FROM_COMMON_PREFIX>_
where:
    - PROJECT_NAME is the project name extracted from the nearest CMakeLists.txt
      (found by walking up the directory tree) and converted to uppercase.
    - RELATIVE_PATH_FROM_COMMON_PREFIX is the file's path relative to the deepest
      common parent directory that contains all header files found. All non‑alnum
      characters (except underscores) are replaced by underscores ('_').
      (The original specification said '#' but that is not valid in a macro name.)
"""

import sys
import re
from pathlib import Path

# ----------------------------------------------------------------------
# Functions reused from the provided expander script
# ----------------------------------------------------------------------
def find_cmake_root(start_dir: Path) -> Path | None:
    """Return the first ancestor of start_dir that contains a CMakeLists.txt file."""
    current = start_dir.resolve()
    while True:
        if (current / "CMakeLists.txt").is_file():
            return current
        parent = current.parent
        if parent == current:   # reached filesystem root
            break
        current = parent
    return None

def strip_guards_and_pragma(lines):
    """
    Remove #pragma once and standard inclusion guards from the given list of lines.
    Returns a new list of lines with those lines removed.
    """
    # Remove #pragma once lines
    filtered = []
    for line in lines:
        if re.match(r'^\s*#pragma\s+once\s*$', line):
            continue
        filtered.append(line)

    # Look for a standard inclusion guard: #ifndef MACRO ... #define MACRO ... #endif
    guard_macro = None
    ifndef_idx = None
    for i, line in enumerate(filtered):
        m = re.match(r'^\s*#ifndef\s+(\w+)', line)
        if m:
            guard_macro = m.group(1)
            ifndef_idx = i
            break

    if guard_macro is not None and ifndef_idx is not None:
        # find the #define line that follows
        define_idx = None
        for j in range(ifndef_idx + 1, len(filtered)):
            if re.match(r'^\s*#define\s+' + re.escape(guard_macro) + r'\b', filtered[j]):
                define_idx = j
                break

        if define_idx is not None:
            # find the last #endif (likely the matching one)
            endif_idx = None
            for k in range(len(filtered) - 1, -1, -1):
                # Match #endif with optional macro name and/or comment
                if re.match(r'^\s*#endif\b', filtered[k]):
                    endif_idx = k
                    break

            if endif_idx is not None and endif_idx > define_idx:
                # remove the three lines
                remove_set = {ifndef_idx, define_idx, endif_idx}
                filtered = [line for i, line in enumerate(filtered) if i not in remove_set]

    return filtered

# ----------------------------------------------------------------------
# Helper: extract project name from CMakeLists.txt
# ----------------------------------------------------------------------
def find_project_name(cmake_root: Path) -> str | None:
    """Extract the project name from a CMakeLists.txt file."""
    cmake_file = cmake_root / "CMakeLists.txt"
    if not cmake_file.is_file():
        return None

    try:
        content = cmake_file.read_text(encoding='utf-8')
    except Exception as e:
        print(f"Warning: could not read {cmake_file}: {e}", file=sys.stderr)
        return None

    # Match project(...) command, handling possible spaces and parentheses
    m = re.search(r'project\s*\(\s*([^)\s]+)\s*\)', content, re.IGNORECASE)
    if m:
        return m.group(1).strip()
    return None

# ----------------------------------------------------------------------
# Helper: find the deepest common prefix directory of a list of paths
# ----------------------------------------------------------------------
def common_prefix_paths(paths: list[Path]) -> Path:
    """
    Given a list of Path objects (relative to the same base), return the
    deepest common prefix as a Path. If no common prefix exists, return Path('.').
    """
    if not paths:
        return Path('.')

    # Split each path into parts
    parts_list = [list(p.parts) for p in paths]

    # Find common prefix length
    min_len = min(len(p) for p in parts_list)
    common_parts = []
    for i in range(min_len):
        first_part = parts_list[0][i]
        if all(p[i] == first_part for p in parts_list):
            common_parts.append(first_part)
        else:
            break

    if not common_parts:
        return Path('.')
    return Path(*common_parts)

# ----------------------------------------------------------------------
# Main guard insertion routine
# ----------------------------------------------------------------------
def main():
    cwd = Path.cwd()
    print(f"Working directory: {cwd}")

    # Find all .h and .hpp files recursively
    header_files = list(cwd.rglob('*.h')) + list(cwd.rglob('*.hpp'))
    if not header_files:
        print("No .h or .hpp files found.")
        return

    print(f"Found {len(header_files)} header files.")

    # Determine project name from CMakeLists.txt
    cmake_root = find_cmake_root(cwd)
    if cmake_root is None:
        print("Error: Could not find a CMakeLists.txt ancestor.", file=sys.stderr)
        sys.exit(1)

    project_name = find_project_name(cmake_root)
    if project_name is None:
        print(f"Error: No project() directive found in {cmake_root / 'CMakeLists.txt'}", file=sys.stderr)
        sys.exit(1)

    prefix = project_name.upper()
    print(f"Project name (uppercase): {prefix}")

    # Compute common prefix X: deepest directory that contains all header files
    # Use paths relative to cwd
    rel_paths = [p.relative_to(cwd) for p in header_files]
    common_prefix = common_prefix_paths(rel_paths)
    X = cwd / common_prefix
    print(f"Common prefix X: {X}")

    # Process each header file
    for filepath in header_files:
        # Compute relative path from X
        try:
            rel = filepath.relative_to(X)
        except ValueError:
            # Should not happen because X is a prefix of all files,
            # but fallback to relative to cwd if something goes wrong.
            rel = filepath.relative_to(cwd)

        # Convert to guard string: uppercase, replace non-alnum (except underscore) with '_'
        rel_str = str(rel).replace('\\', '/')  # normalize to forward slashes
        # Replace any character that is not alphanumeric or underscore with '_'
        guard_suffix = re.sub(r'[^a-zA-Z0-9_]', '_', rel_str).upper()
        # Ensure it ends with an underscore (already adding at the end)
        guard_name = f"{prefix}_{guard_suffix}_"
        # Double underscores might appear, but that's acceptable.

        print(f"Processing {filepath} -> guard: {guard_name}")

        # Read file content
        try:
            content = filepath.read_text(encoding='utf-8')
        except Exception as e:
            print(f"  Error reading {filepath}: {e}", file=sys.stderr)
            continue

        lines = content.splitlines(keepends=False)

        # Remove existing guards and #pragma once
        stripped_lines = strip_guards_and_pragma(lines)

        # Build new content with guard at the beginning
        # Insert guard lines at the top
        new_lines = [
                        f"#ifndef {guard_name}",
                        f"#define {guard_name}",
                        "",
                    ] + stripped_lines + [
                        "",
                        f"#endif // {guard_name}",
                    ]

        # Write back
        try:
            filepath.write_text('\n'.join(new_lines), encoding='utf-8')
            print(f"  Updated {filepath}")
        except Exception as e:
            print(f"  Error writing {filepath}: {e}", file=sys.stderr)

    print("Done.")

if __name__ == '__main__':
    main()
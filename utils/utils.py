# AI-generated
import re
import subprocess
import sys
from pathlib import Path

# ----------------------------------------------------------------------
# Locate CMake root and parse include directories from its CMakeLists.txt
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

# ----------------------------------------------------------------------
# Helper: strip include guards and #pragma once from a list of lines
# ----------------------------------------------------------------------
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
# Helper: run clang-format on a file
# ----------------------------------------------------------------------
def run_tool(filepath: Path, tool: str, args: list[str]) -> bool:
    """
    Run the given tool on the given file.
    Returns True if successful, False otherwise.
    """
    try:
        # Check if the tool is available
        result = subprocess.run([tool] + ['--version'],
                                capture_output=True, text=True)
        if result.returncode != 0:
            print(f"  Warning: {tool} not found or not working. Skipping formatting.",
                  file=sys.stderr)
            return False
    except FileNotFoundError:
        print(f"  Warning: {tool} not installed or not in PATH. Skipping formatting.",
              file=sys.stderr)
        return False

    try:
        # Run clang-format -i on the file
        subprocess.run([tool] + args + [str(filepath)],
                       check=True,
                       text=True)
    except subprocess.CalledProcessError as e:
        pass
    return True


def run_clang_format(filepath: Path) -> bool:
    return run_tool(filepath, 'clang-format', ['-i'])

def run_clang_tidy(filepath: Path) -> bool:
    return run_tool(filepath, 'clang-tidy', ['-quiet'])

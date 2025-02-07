
# clang-format for R-Type

This document explains how to configure and use **clang-format** to ensure consistent coding standards in the R-Type project.

---

## Why use clang-format?

- Ensures a uniform coding style for all contributors.
- Compatible with most IDEs (Visual Studio Code, CLion, etc.).
- Automates code formatting to focus on development.

---

## Installing clang-format

### **Linux**
1. Install clang-format using your package manager:
   ```bash
   sudo apt install clang-format
   ```

### **MacOS**
1. Install clang-format using Homebrew:
   ```bash
   brew install clang-format
   ```

### **Verification**
1. Make sure clang-format is installed correctly:
   ```bash
   clang-format --version
   ```

---

## Configuration with `.clang-format`

Add the following `.clang-format` file to the root of the R-Type project to define the style rules.

```yaml
BasedOnStyle: LLVM
IndentWidth: 4
ColumnLimit: 100
BreakBeforeBraces: Allman
AllowShortIfStatementsOnASingleLine: false
AlwaysBreakTemplateDeclarations: true
SortIncludes: true
IncludeBlocks: Preserve
SpacesInParentheses: false
```

---

## Using clang-format

### **Manual**
1. Format a specific file:
   ```bash
   clang-format -i path/to/file.cpp
   ```

2. Format the entire project:
   ```bash
   find . -name '*.cpp' -o -name '*.h' | xargs clang-format -i
   ```

### **With IDEs**

#### **VS Code**
1. Install the **C/C++** and **Clang-Format** extensions.
2. Add the following to `settings.json`:
   ```json
   {
       "C_Cpp.clang_format_style": "file",
       "editor.formatOnSave": true
   }
   ```

#### **CLion**
1. Navigate to **File > Settings > Editor > Code Style > C/C++**.
2. Enable **Use clang-format file**.

---

## CI/CD Integration

Add a step in your CI pipeline to validate formatting:
```yaml
- name: clang-format check
  run: |
    find . -name '*.cpp' -o -name '*.h' | xargs clang-format --dry-run --Werror
```

---

## Essential Commands

| Action                          | Command                                        |
|---------------------------------|------------------------------------------------|
| Format a file                   | `clang-format -i file.cpp`                     |
| Format all files                | `find . -name '*.cpp' -o -name '*.h' | xargs clang-format -i` |
| Verify without modifying        | `clang-format --dry-run file.cpp`              |

---

## References

- [Official clang-format documentation](https://clang.llvm.org/docs/ClangFormat.html)
- [Sample .clang-format file](https://github.com/llvm/llvm-project/tree/main/clang/tools/clang-format)

---

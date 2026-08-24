# Qt Calculator

A simple GUI calculator built with **Qt Widgets (C++)**, created in Qt Creator.

## Features
- Standard four-function arithmetic: add, subtract, multiply, divide
- Decimal point support
- Sign toggle (±) and percentage (%)
- Clear (C) and backspace (⌫)
- Basic divide-by-zero error handling
- Clean, dark-themed UI built with `QGridLayout`

## Project structure
```
QtCalculator/
├── QtCalculator.pro   # qmake project file
├── main.cpp           # application entry point
├── mainwindow.h        # MainWindow class declaration
├── mainwindow.cpp       # UI setup + calculator logic
└── README.md
```

## How to open in Qt Creator
1. Open **Qt Creator**.
2. `File > Open File or Project...`
3. Select `QtCalculator.pro`.
4. Choose a Kit (e.g. Desktop Qt 6.x MinGW/GCC/Clang).
5. Click the green **Run** (▶) button to build and launch.

## How to build from the command line (optional)
```bash
qmake QtCalculator.pro
make          # or: mingw32-make / nmake on Windows
./QtCalculator
```

## Requirements
- Qt 5.12+ or Qt 6.x
- A C++17-capable compiler (GCC, Clang, or MSVC)

## Author
Submitted as a course assignment.

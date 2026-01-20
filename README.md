# CineBar

**CineBar** is a **Movie Barcode Generator** - a Qt C++ desktop tool that generates visual barcodes from movies by sampling frames across the film’s duration.

---

## Features

- Generate movie barcodes from video files
- Simple Qt-based GUI
- High-quality image output

---

## Build & Run

### Requirements

- C++17 compatible compiler
- Qt 5 or Qt 6
- CMake 3.16+
- Tested on Windows only

Optional:
- Qt OpenGLWidgets or FFmpeg for media processing

### Clone

```bash
git clone https://github.com/your-username/CineBar.git
cd CineBar
mkdir build
cd build
cmake ..
cmake --build .
```

If Qt is not found:

```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt
```

### Run

```bash
./CineBar.exe
```

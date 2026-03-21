# Quickstart: fx-movies MVP

**Date**: 2026-03-21

## Prerequisites

- CMake 3.20+
- C++20 compatible compiler (GCC 11+, Clang 13+)
- FFmpeg development libraries: `libavformat-dev`, `libavcodec-dev`, `libavutil-dev`, `libswscale-dev`
- OpenGL development headers
- pkg-config

### Install dependencies (Ubuntu/Debian)

```bash
sudo apt install cmake g++ pkg-config \
  libavformat-dev libavcodec-dev libavutil-dev libswscale-dev \
  libgl-dev libglfw3-dev xorg-dev
```

### Install dependencies (Fedora)

```bash
sudo dnf install cmake gcc-c++ pkgconfig \
  ffmpeg-devel mesa-libGL-devel glfw-devel
```

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Run

```bash
./build/fx_movies /path/to/your/videos
```

## Run Tests

```bash
cmake --build build --target tests
cd build && ctest --output-on-failure
```

## Project Structure

```
fx-movies/
├── CMakeLists.txt
├── main.cpp
├── src/
│   ├── app/              # Application setup and main loop
│   ├── asset/            # Asset scanning and management
│   ├── video/            # Video decoding (FFmpeg wrapper)
│   └── ui/               # ImGui panels and layout
├── tests/
│   └── unit/             # GoogleTest unit tests
└── specs/                # Feature specifications
```
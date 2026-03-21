# fx-movies

A lightweight video previewer and asset manager built with C++20, Dear ImGui, and FFmpeg.

## Features

- Browse video files from a specified directory
- Preview and play back video files with basic controls (play/pause)
- Two-panel layout: asset manager (left) and video preview (right)
- Supports common video formats: `.mp4`, `.avi`, `.mkv`, `.mov`, `.webm`

## Prerequisites

- CMake 3.20+
- C++20 compatible compiler (GCC 11+, Clang 13+)
- FFmpeg development libraries
- OpenGL development headers
- pkg-config

### Ubuntu/Debian

```bash
sudo apt install cmake g++ pkg-config \
  libavformat-dev libavcodec-dev libavutil-dev libswscale-dev \
  libgl-dev xorg-dev
```

### Fedora

```bash
sudo dnf install cmake gcc-c++ pkgconfig \
  ffmpeg-devel mesa-libGL-devel
```

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Usage

```bash
./build/fx_movies <directory_path>
```

Point it at a directory containing video files:

```bash
./build/fx_movies ~/Videos
```

## Tests

```bash
cmake --build build
cd build && ctest --output-on-failure
```

## Project Structure

```
src/
├── app/          # Application lifecycle and CLI parsing
├── asset/        # Asset scanning and data types
├── video/        # FFmpeg video decoding
└── ui/           # ImGui panels (asset manager, preview)
tests/
└── unit/         # GoogleTest unit tests
```

## Tech Stack

- **UI**: Dear ImGui (GLFW + OpenGL 3.3)
- **Video**: FFmpeg (libavformat, libavcodec, libavutil, libswscale)
- **Testing**: GoogleTest
- **Build**: CMake with FetchContent

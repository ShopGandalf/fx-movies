# Research: MVP UI with Video Preview and Asset Manager

**Date**: 2026-03-21
**Feature**: 001-mvp-ui-preview

## R-001: ImGui Backend

**Decision**: GLFW 3.x + OpenGL 3.3 core profile

**Rationale**: ImGui ships ready-made backends (`imgui_impl_glfw.cpp` + `imgui_impl_opengl3.cpp`). GLFW is lightweight with no heavy dependencies. OpenGL 3.3 is universally supported on Linux and sufficient for texture rendering. Vulkan would be overkill for this use case.

**Alternatives considered**:
- SDL2/SDL3: Heavier, pulls more dependencies. Viable but unnecessary.
- Vulkan: Overkill for a 2D UI with video textures.

## R-002: Video Decoding Library

**Decision**: FFmpeg libav* suite (system-installed via pkg-config)

**Rationale**: FFmpeg is the industry standard for video decoding in C/C++. Minimum libraries for decode-only playback:
- `libavformat` — container demuxing
- `libavcodec` — video decoding
- `libavutil` — pixel formats, memory, utilities
- `libswscale` — pixel format conversion (YUV→RGBA for OpenGL)

Not needed for MVP: `libavfilter`, `libswresample`, `libavdevice`, `libpostproc`.

**Alternatives considered**:
- GStreamer: Higher-level but heavier abstraction, less control over frame-level access.
- MPV (libmpv): Simpler API but less control over rendering pipeline; harder to integrate with ImGui texture rendering.

## R-003: Video Frame Rendering Pipeline

**Decision**: Decode → sws_scale to RGBA → Upload to OpenGL texture → ImGui::Image()

**Rationale**: Standard pipeline. Decode frames with `avcodec_receive_frame()` (yields YUV420P), convert with `sws_scale()` to RGBA, upload via `glTexImage2D`/`glTexSubImage2D`, display with `ImGui::Image()`. Use RGBA over RGB to avoid GPU alignment issues.

**Alternatives considered**:
- GPU-side YUV→RGB conversion via shader: Better performance but unnecessary complexity for MVP.
- PBO async uploads: Optimization for later.

## R-004: Unit Testing Framework

**Decision**: GoogleTest (gtest + gmock) via FetchContent

**Rationale**: Industry standard for C++, excellent CMake integration via FetchContent. gmock is bundled for mocking interfaces around FFmpeg wrappers.

**Alternatives considered**:
- Catch2: Good alternative, slightly simpler syntax, but GoogleTest has broader ecosystem support and CLion integration.

## R-005: Dependency Management Strategy

**Decision**: Mixed — FetchContent for source-buildable deps, pkg-config for system libs

| Dependency   | Method                   | Rationale                               |
| ------------ | ------------------------ | --------------------------------------- |
| ImGui        | FetchContent (source)    | No official CMake; add sources directly |
| GLFW         | FetchContent             | Builds from source, avoids mismatches   |
| FFmpeg       | pkg_check_modules        | Too large to build from source          |
| GoogleTest   | FetchContent             | Standard practice, pinned version       |

**Alternatives considered**:
- vcpkg/Conan: Adds toolchain complexity unnecessary for this scope.

## R-006: Supported Video Formats

**Decision**: Detection by file extension: `.mp4`, `.avi`, `.mkv`, `.mov`, `.webm`

**Rationale**: These are the most common video container formats. Extension-based filtering is simple and sufficient for directory scanning. Actual playback support depends on FFmpeg's installed codecs. Corrupted or unsupported files are handled gracefully at decode time.
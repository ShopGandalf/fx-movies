# Implementation Plan: MVP UI with Video Preview and Asset Manager

**Branch**: `001-mvp-ui-preview` | **Date**: 2026-03-21 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/specs/001-mvp-ui-preview/spec.md`

## Summary

Build a C++20 desktop application using ImGui (GLFW+OpenGL backend) that accepts a directory path as a command-line argument, scans it for video files, displays them in an asset manager panel, and plays selected videos in a preview panel using FFmpeg for decoding. Unit tests cover core logic (argument parsing, directory scanning, file filtering).

## Technical Context

**Language/Version**: C++20 (as set in CMakeLists.txt)
**Primary Dependencies**: Dear ImGui (GLFW+OpenGL3 backend), GLFW 3.x, FFmpeg (libavformat, libavcodec, libavutil, libswscale)
**Storage**: N/A (in-memory only)
**Testing**: GoogleTest via FetchContent
**Target Platform**: Linux desktop (X11/Wayland via GLFW)
**Project Type**: Desktop application
**Performance Goals**: Directory scan <3s for 100 files, video playback start <2s
**Constraints**: Decode-only (no video editing), single window, no persistent state
**Scale/Scope**: Single-user desktop app, handles directories with up to 100 video files

## Constitution Check

*No constitution file found. No gates to evaluate.*

## Project Structure

### Documentation (this feature)

```text
specs/001-mvp-ui-preview/
├── plan.md              # This file
├── research.md          # Phase 0 output
├── data-model.md        # Phase 1 output
├── quickstart.md        # Phase 1 output
├── contracts/           # Phase 1 output
│   ├── cli-contract.md  # CLI argument interface
│   └── ui-contract.md   # UI layout and interaction spec
└── tasks.md             # Phase 2 output (via /speckit.tasks)
```

### Source Code (repository root)

```text
src/
├── app/                 # Application lifecycle (init, main loop, shutdown)
│   ├── application.h
│   └── application.cpp
├── asset/               # Directory scanning and asset management
│   ├── asset.h          # Asset struct
│   ├── asset_scanner.h  # Directory scanning logic
│   └── asset_scanner.cpp
├── video/               # FFmpeg video decoding wrapper
│   ├── video_player.h
│   └── video_player.cpp
└── ui/                  # ImGui panel rendering
    ├── asset_panel.h    # Asset manager panel
    ├── asset_panel.cpp
    ├── preview_panel.h  # Video preview panel
    └── preview_panel.cpp

tests/
└── unit/
    ├── test_asset_scanner.cpp  # Asset scanning and filtering tests
    └── test_cli_args.cpp       # Argument parsing tests
```

**Structure Decision**: Single-project layout. This is a straightforward desktop application — no need for frontend/backend split or multi-project structure. Source is organized by domain concern (app, asset, video, ui) with a flat test directory.
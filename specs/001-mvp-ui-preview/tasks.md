# Tasks: MVP UI with Video Preview and Asset Manager

**Input**: Design documents from `/specs/001-mvp-ui-preview/`
**Prerequisites**: plan.md (required), spec.md (required for user stories), research.md, data-model.md, contracts/

**Tests**: Included — explicitly requested in feature specification ("use unit tests").

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2, US3)
- Include exact file paths in descriptions

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization, dependency configuration, and directory structure

- [x] T001 Create project directory structure: `src/app/`, `src/asset/`, `src/video/`, `src/ui/`, `tests/unit/`
- [x] T002 Update `CMakeLists.txt` with FetchContent for ImGui, GLFW, GoogleTest; pkg_check_modules for FFmpeg (libavformat, libavcodec, libavutil, libswscale); configure C++20 standard, source targets, and test target
- [x] T003 [P] Create ImGui CMake integration: FetchContent declaration for Dear ImGui repository with GLFW+OpenGL3 backend source files (`imgui_impl_glfw.cpp`, `imgui_impl_opengl3.cpp`) added to the main target in `CMakeLists.txt`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core data types and argument parsing that ALL user stories depend on

**CRITICAL**: No user story work can begin until this phase is complete

- [x] T004 Define `Asset` struct in `src/asset/asset.h` with fields: `std::string filename`, `std::filesystem::path filepath`, `std::string extension`; include `is_supported_extension()` free function that checks against `.mp4`, `.avi`, `.mkv`, `.mov`, `.webm` (case-insensitive)
- [x] T005 [P] Implement CLI argument parsing function in `src/app/cli.h` and `src/app/cli.cpp`: `parse_args(int argc, char* argv[])` returning a struct with the validated directory path or error info; validate path exists and is a directory per CLI contract (exit code 1 for missing arg, exit code 2 for invalid path); write error messages to stderr per `contracts/cli-contract.md`
- [x] T006 [P] Write unit tests for `is_supported_extension()` in `tests/unit/test_asset.cpp`: test all supported extensions, case variations (`.MP4`, `.Mp4`), unsupported extensions (`.txt`, `.jpg`), empty string, no extension
- [x] T007 [P] Write unit tests for `parse_args()` in `tests/unit/test_cli_args.cpp`: test valid directory, missing argument, non-existent path, path is a file (not directory), path with trailing slash

**Checkpoint**: Foundation ready — core types and arg parsing tested and working

---

## Phase 3: User Story 1 - Launch Application with Asset Directory (Priority: P1)

**Goal**: User launches fx-movies with a directory path argument; the application validates the path, scans for video files, and starts the main window.

**Independent Test**: Launch with a test directory containing mixed files; verify only video files are discovered and the window opens.

### Tests for User Story 1

> **NOTE: Write these tests FIRST, ensure they FAIL before implementation**

- [x] T008 [P] [US1] Write unit tests for `AssetScanner` in `tests/unit/test_asset_scanner.cpp`: test scanning a directory with mixed files (returns only video files), empty directory (returns empty vector), directory with only non-video files (returns empty), filenames are correctly extracted, extensions are case-insensitive, subdirectories are skipped (non-recursive)

### Implementation for User Story 1

- [x] T009 [US1] Implement `AssetScanner` in `src/asset/asset_scanner.h` and `src/asset/asset_scanner.cpp`: `scan_directory(const std::filesystem::path& dir) -> std::vector<Asset>` that iterates top-level directory entries using `std::filesystem::directory_iterator`, filters by `is_supported_extension()`, and returns sorted vector of `Asset` structs
- [x] T010 [US1] Implement GLFW+OpenGL initialization in `src/app/application.h` and `src/app/application.cpp`: `Application` class with `init(const std::string& window_title) -> bool` (creates GLFW window, sets up OpenGL 3.3 context, initializes ImGui with GLFW+OpenGL3 backends), `run()` (main loop stub: poll events, ImGui new frame, render, swap buffers), `shutdown()` (cleanup ImGui, GLFW, OpenGL)
- [x] T011 [US1] Wire up `main.cpp`: parse CLI args via `parse_args()`, scan directory via `AssetScanner`, create `Application` instance, pass scanned assets to application, call `init()` then `run()` then `shutdown()`

**Checkpoint**: Application launches with a directory argument, scans for video files, and opens an empty ImGui window. Unit tests for scanning pass.

---

## Phase 4: User Story 2 - Browse Assets in the Asset Manager (Priority: P1)

**Goal**: Display a scrollable panel listing all discovered video filenames with selection highlighting.

**Independent Test**: Launch with a directory containing video files; verify filenames appear in left panel, clicking highlights selection.

### Implementation for User Story 2

- [x] T012 [US2] Implement `AssetPanel` in `src/ui/asset_panel.h` and `src/ui/asset_panel.cpp`: renders an ImGui child window with title "Asset Manager"; takes `const std::vector<Asset>&` and `int& selected_index`; uses `ImGui::Selectable()` for each asset filename with highlight on selection; shows "No supported video files found" when list is empty
- [x] T013 [US2] Integrate `AssetPanel` into `Application::run()` main loop in `src/app/application.cpp`: store `std::vector<Asset>` and `int selected_index` as application state; call `AssetPanel::render()` each frame

**Checkpoint**: Application shows asset list in left panel, items are clickable with visual selection highlight. Works independently even without video preview.

---

## Phase 5: User Story 3 - Preview a Video File (Priority: P1)

**Goal**: When a video file is selected in the asset manager, decode and display it in the preview panel with play/pause controls.

**Independent Test**: Select a video file from asset list; verify video frames appear in preview panel and play/pause button works.

### Implementation for User Story 3

- [x] T014 [US3] Implement `VideoPlayer` in `src/video/video_player.h` and `src/video/video_player.cpp`: wraps FFmpeg decode pipeline; `open(const std::filesystem::path& filepath) -> bool` (opens file with `avformat_open_input`, finds video stream, sets up `AVCodecContext` and `SwsContext` for YUV→RGBA conversion); `decode_next_frame() -> bool` (reads packet, decodes frame, converts to RGBA buffer); `close()` (frees all FFmpeg resources); `get_frame_data() -> const uint8_t*`, `get_width() -> int`, `get_height() -> int`; handle errors gracefully (return false, don't crash)
- [x] T015 [US3] Implement OpenGL texture management in `src/video/video_player.cpp` or `src/ui/preview_panel.cpp`: `create_texture(int width, int height) -> GLuint`, `update_texture(GLuint tex, const uint8_t* data, int width, int height)` using `glTexImage2D`/`glTexSubImage2D` with `GL_RGBA`
- [x] T016 [US3] Implement `PreviewPanel` in `src/ui/preview_panel.h` and `src/ui/preview_panel.cpp`: renders ImGui child window with title "Video Preview"; displays video texture via `ImGui::Image()` scaled to fit panel; shows "Select a video to preview" placeholder when no video selected; shows error message when video fails to decode; includes Play/Pause button using `ImGui::Button()` that toggles `PreviewState.is_playing`
- [x] T017 [US3] Implement frame timing in `PreviewPanel` or `Application`: decode next frame at video framerate (read from `AVStream->r_frame_rate`); skip frames if behind, pause decoding when paused; handle video end (stop or loop)
- [x] T018 [US3] Integrate `PreviewPanel` with asset selection in `src/app/application.cpp`: when `selected_index` changes, call `VideoPlayer::close()` then `VideoPlayer::open()` on new asset; pass `VideoPlayer` state to `PreviewPanel::render()` each frame

**Checkpoint**: Full video preview works — select a file, it plays; click pause, it pauses; select another file, it switches. Corrupted files show error without crashing.

---

## Phase 6: User Story 4 - Main Application Layout (Priority: P2)

**Goal**: Organize asset manager and video preview in a clean two-panel side-by-side layout per UI contract.

**Independent Test**: Launch application and verify two-panel layout; resize window and verify panels adjust proportionally.

### Implementation for User Story 4

- [x] T019 [US4] Implement two-panel layout in `src/app/application.cpp` main loop: create full-window ImGui window (no title bar, no resize, docked to viewport); use `ImGui::Columns(2)` or `ImGui::BeginChild()` with calculated widths for left (asset manager, ~25% width) and right (video preview, ~75% width) panels; set window title to "fx-movies - \<directory_path\>" per UI contract
- [x] T020 [US4] Add window resize handling: panels adjust proportionally when window is resized; maintain minimum panel widths to prevent collapse

**Checkpoint**: Application has clean two-panel layout matching the UI contract wireframe.

---

## Phase 7: Polish & Cross-Cutting Concerns

**Purpose**: Edge case handling, error resilience, and final validation

- [x] T021 [P] Handle edge case: empty directory — ensure "No supported video files found" message appears in asset panel when directory has zero video files
- [x] T022 [P] Handle edge case: corrupted video files — ensure `VideoPlayer::open()` returns false gracefully and `PreviewPanel` shows error message without crashing
- [x] T023 Verify all unit tests pass: run `ctest --output-on-failure` and fix any failures
- [x] T024 Run quickstart.md validation: follow build and run instructions from `specs/001-mvp-ui-preview/quickstart.md` end-to-end on a clean build

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies — can start immediately
- **Foundational (Phase 2)**: Depends on Setup completion — BLOCKS all user stories
- **US1 (Phase 3)**: Depends on Foundational (Phase 2)
- **US2 (Phase 4)**: Depends on US1 (Phase 3) — needs `Application` class and scanned assets
- **US3 (Phase 5)**: Depends on US1 (Phase 3) — needs `Application` class and scanned assets; can run in parallel with US2
- **US4 (Phase 6)**: Depends on US2 (Phase 4) and US3 (Phase 5) — integrates both panels into layout
- **Polish (Phase 7)**: Depends on all user stories being complete

### User Story Dependencies

- **User Story 1 (P1)**: Foundation → US1 (arg parsing + scanning + window init)
- **User Story 2 (P1)**: US1 → US2 (asset panel needs scanned assets and Application)
- **User Story 3 (P1)**: US1 → US3 (preview needs Application; can run parallel with US2)
- **User Story 4 (P2)**: US2 + US3 → US4 (layout combines both panels)

### Within Each User Story

- Tests MUST be written and FAIL before implementation
- Data types before logic
- Core implementation before integration
- Story complete before moving to next priority

### Parallel Opportunities

- T002 and T003 can run in parallel (CMake vs ImGui integration are in same file but distinct sections)
- T005, T006, T007 can all run in parallel (different files)
- T008 can run in parallel with T006, T007 (different test files)
- US2 (Phase 4) and US3 (Phase 5) can run in parallel after US1 completes
- T021 and T022 can run in parallel (different edge cases)

---

## Parallel Example: Foundational Phase

```bash
# Launch these in parallel (different files, no dependencies):
Task: "Implement CLI argument parsing in src/app/cli.h and src/app/cli.cpp"
Task: "Write unit tests for is_supported_extension() in tests/unit/test_asset.cpp"
Task: "Write unit tests for parse_args() in tests/unit/test_cli_args.cpp"
```

## Parallel Example: User Stories 2 & 3

```bash
# After US1 completes, launch these in parallel:
Task: "Implement AssetPanel in src/ui/asset_panel.h and src/ui/asset_panel.cpp"
Task: "Implement VideoPlayer in src/video/video_player.h and src/video/video_player.cpp"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup (T001-T003)
2. Complete Phase 2: Foundational (T004-T007)
3. Complete Phase 3: User Story 1 (T008-T011)
4. **STOP and VALIDATE**: Application launches, scans directory, opens window
5. Tests pass for arg parsing and directory scanning

### Incremental Delivery

1. Setup + Foundational → Foundation ready
2. Add User Story 1 → App launches and scans → **Minimal viable demo**
3. Add User Story 2 → Asset list visible → **Browseable demo**
4. Add User Story 3 → Video playback works → **Full preview demo**
5. Add User Story 4 → Clean layout → **Polished MVP**
6. Polish → Edge cases handled → **Release-ready MVP**

---

## Notes

- [P] tasks = different files, no dependencies
- [Story] label maps task to specific user story for traceability
- Each user story should be independently completable and testable
- Verify tests fail before implementing
- Commit after each task or logical group
- Stop at any checkpoint to validate story independently
- FFmpeg must be installed on the system (`libavformat-dev`, `libavcodec-dev`, `libavutil-dev`, `libswscale-dev`)
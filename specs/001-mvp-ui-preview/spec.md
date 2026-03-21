# Feature Specification: MVP UI with Video Preview and Asset Manager

**Feature Branch**: `001-mvp-ui-preview`
**Created**: 2026-03-21
**Status**: Draft
**Input**: User description: "We are building a video editor called fx-movies. in this first mvp draft, please implement a basic UI with the video preview and a little asset manager based on a given directory we specify on startup with a program argument. Use c++ 20 (as specified in cmake and use imgui and please use unit tests). we do not need any ability to edit the video itself. This is just a first draft."

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Launch Application with Asset Directory (Priority: P1)

As a user, I want to launch fx-movies with a directory path as a program argument so that the application loads and displays the media files found in that directory.

**Why this priority**: Without a valid startup flow and directory loading, no other functionality can work. This is the foundation of the entire application.

**Independent Test**: Can be fully tested by launching the application with a directory path argument and verifying it starts up, validates the path, and scans for media files. Delivers the core entry point for all subsequent features.

**Acceptance Scenarios**:

1. **Given** the application is not running, **When** the user launches it with a valid directory path as argument, **Then** the application starts and displays the main window with the contents of that directory loaded in the asset manager.
2. **Given** the application is not running, **When** the user launches it without any argument, **Then** the application displays a clear error message explaining that a directory path is required and exits gracefully.
3. **Given** the application is not running, **When** the user launches it with an invalid or non-existent directory path, **Then** the application displays a clear error message indicating the path is invalid and exits gracefully.

---

### User Story 2 - Browse Assets in the Asset Manager (Priority: P1)

As a user, I want to see a panel listing all supported media files (video files) from the specified directory so that I can browse my available assets.

**Why this priority**: The asset manager is one of the two core UI panels. Without it, users cannot discover or select media files to preview.

**Independent Test**: Can be tested by launching the application with a directory containing various files and verifying that only supported video files are listed, displayed with their filenames, and organized in a browsable list.

**Acceptance Scenarios**:

1. **Given** the application is running with a valid directory, **When** the asset manager panel is displayed, **Then** it shows a list of all supported video files found in the directory with their filenames.
2. **Given** the directory contains non-video files (e.g., text files, images), **When** the asset manager panel is displayed, **Then** only supported video file formats are shown.
3. **Given** the user selects a video file in the asset manager, **When** the selection is made, **Then** the selected file is visually highlighted.

---

### User Story 3 - Preview a Video File (Priority: P1)

As a user, I want to select a video file from the asset manager and see it play in the video preview panel so that I can review my media content.

**Why this priority**: Video preview is the second core UI panel and the primary way users interact with their media. Together with the asset manager, this completes the MVP experience.

**Independent Test**: Can be tested by selecting a video file from the asset manager and verifying that the preview panel begins playing the video with basic playback visible.

**Acceptance Scenarios**:

1. **Given** the application is running and assets are listed, **When** the user clicks on a video file in the asset manager, **Then** the video preview panel loads and begins playing the selected video.
2. **Given** a video is currently playing in the preview, **When** the user selects a different video from the asset manager, **Then** the preview switches to the newly selected video.
3. **Given** the user has selected a video, **When** the video is playing, **Then** basic playback controls (play/pause) are available.

---

### User Story 4 - Main Application Layout (Priority: P2)

As a user, I want a clean two-panel layout with the asset manager on one side and the video preview on the other so that I can efficiently browse and preview my media.

**Why this priority**: While the individual panels could function independently, a well-organized layout improves usability. This is secondary to the core panel functionality.

**Independent Test**: Can be tested by launching the application and verifying the window displays both panels side by side in a clear, organized layout.

**Acceptance Scenarios**:

1. **Given** the application is running, **When** the main window is displayed, **Then** it shows the asset manager panel and video preview panel in a two-panel layout.
2. **Given** the main window is displayed, **When** the user resizes the window, **Then** the panels adjust proportionally to the available space.

---

### Edge Cases

- What happens when the specified directory is empty (contains no video files)? The asset manager displays a message indicating no supported files were found.
- What happens when a video file is corrupted or cannot be decoded? The preview panel shows an error message for that specific file without crashing the application.
- What happens when the directory contains subdirectories? For this MVP, only top-level files are listed (subdirectories are not recursively scanned).
- What happens when a file is deleted from the directory while the application is running? The asset list reflects the state at startup; a refresh mechanism is out of scope for this MVP.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: Application MUST accept a directory path as a command-line argument at startup.
- **FR-002**: Application MUST validate that the provided path exists and is a directory.
- **FR-003**: Application MUST display a clear error message and exit gracefully if no argument or an invalid path is provided.
- **FR-004**: Application MUST scan the specified directory for supported video file formats (at minimum: .mp4, .avi, .mkv, .mov, .webm).
- **FR-005**: Application MUST display an asset manager panel listing all discovered video files by filename.
- **FR-006**: Application MUST allow users to select a video file from the asset manager list.
- **FR-007**: Application MUST display a video preview panel that plays the selected video file.
- **FR-008**: Application MUST provide basic playback controls (play/pause) for the video preview.
- **FR-009**: Application MUST present the asset manager and video preview in a two-panel layout.
- **FR-010**: Application MUST handle corrupted or unplayable video files gracefully without crashing.
- **FR-011**: Application MUST only list top-level files in the specified directory (no recursive scanning).

### Key Entities

- **Asset**: A video file discovered in the specified directory. Key attributes: filename, file path, file format.
- **Asset Directory**: The root directory specified via command-line argument that contains the user's media files.
- **Preview Session**: The currently active video playback state, including which asset is selected and playback status (playing/paused).

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: Users can launch the application and see their video files listed within 3 seconds of startup for directories containing up to 100 files.
- **SC-002**: Users can select any listed video file and see playback begin within 2 seconds.
- **SC-003**: The application handles all error conditions (missing argument, invalid path, empty directory, corrupted files) without crashing.
- **SC-004**: All core components (argument parsing, directory scanning, file filtering) are covered by unit tests with passing results.

## Assumptions

- **Supported formats**: The MVP supports common video formats (.mp4, .avi, .mkv, .mov, .webm). Additional formats may be added later.
- **No editing capability**: This MVP is strictly for browsing and previewing. No video editing, trimming, or export features are included.
- **No recursive scanning**: Only top-level files in the specified directory are listed. Subdirectory traversal is out of scope.
- **No file watching**: The asset list is populated at startup and does not auto-refresh if directory contents change.
- **Single window**: The application runs in a single window with a fixed two-panel layout.
- **No persistent state**: The application does not save preferences, recent directories, or playback history.
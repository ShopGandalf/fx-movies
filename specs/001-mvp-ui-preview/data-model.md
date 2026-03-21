# Data Model: MVP UI with Video Preview and Asset Manager

**Date**: 2026-03-21
**Feature**: 001-mvp-ui-preview

## Entities

### Asset

Represents a video file discovered in the scanned directory.

| Field     | Type   | Description                                      |
| --------- | ------ | ------------------------------------------------ |
| filename  | string | Display name of the file (basename)              |
| filepath  | path   | Full absolute path to the file                   |
| extension | string | File extension (e.g., ".mp4", ".mkv")            |

**Validation rules**:
- `filepath` must exist and be a regular file
- `extension` must be one of: `.mp4`, `.avi`, `.mkv`, `.mov`, `.webm` (case-insensitive)

**State**: Stateless — assets are scanned once at startup and stored as a list.

---

### AssetDirectory

The root directory provided via command-line argument.

| Field | Type | Description                            |
| ----- | ---- | -------------------------------------- |
| path  | path | Absolute path to the scanned directory |

**Validation rules**:
- Must exist on the filesystem
- Must be a directory (not a file or symlink to file)

---

### PreviewState

The current video playback state.

| Field         | Type   | Description                                     |
| ------------- | ------ | ------------------------------------------------ |
| selected_asset | Asset* | Pointer/reference to the currently selected asset (nullable) |
| is_playing    | bool   | Whether playback is active                       |

**State transitions**:
- `None` → `Selected` (user clicks an asset): loads video, begins playback
- `Selected/Playing` → `Selected/Playing` (user clicks a different asset): switches video
- `Playing` → `Paused` (user clicks pause): pauses playback
- `Paused` → `Playing` (user clicks play): resumes playback

## Relationships

```
AssetDirectory 1──* Asset : contains (scanned at startup)
PreviewState *──1 Asset  : references (currently selected)
```

## Notes

- No persistent storage — all data lives in memory for the application lifetime.
- Asset list is immutable after initial scan (no file watching).
- PreviewState is a singleton — only one video can be previewed at a time.
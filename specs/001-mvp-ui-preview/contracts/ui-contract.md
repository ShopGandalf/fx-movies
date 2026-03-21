# UI Contract: fx-movies

**Date**: 2026-03-21

## Window Layout

```
┌──────────────────────────────────────────────────────────┐
│ fx-movies - <directory_path>                             │
├────────────────────┬─────────────────────────────────────┤
│  Asset Manager     │  Video Preview                      │
│                    │                                     │
│  ┌──────────────┐  │  ┌─────────────────────────────┐    │
│  │ video1.mp4   │  │  │                             │    │
│  │ video2.mkv ◄ │  │  │     [Video Frame]           │    │
│  │ clip3.mov    │  │  │                             │    │
│  │ demo.webm    │  │  │                             │    │
│  │              │  │  └─────────────────────────────┘    │
│  │              │  │                                     │
│  │              │  │      [ ▶ Play / ❚❚ Pause ]         │
│  └──────────────┘  │                                     │
├────────────────────┴─────────────────────────────────────┤
│ Status: Playing video2.mkv                               │
└──────────────────────────────────────────────────────────┘
```

## Panels

### Asset Manager Panel (Left)

- Scrollable list of video filenames
- Selected item is visually highlighted
- Clicking an item selects it and triggers preview
- Shows "No supported video files found" when directory has no matching files

### Video Preview Panel (Right)

- Displays decoded video frames as a texture
- Shows placeholder/empty state when no video is selected
- Play/Pause button below the video frame
- Shows error message if selected video cannot be decoded

## User Interactions

| Action                | UI Element        | Result                              |
| --------------------- | ----------------- | ----------------------------------- |
| Click asset in list   | Asset Manager     | Highlight asset, load in preview    |
| Click Play/Pause      | Preview controls  | Toggle playback state               |
| Resize window         | Window border     | Panels adjust proportionally        |
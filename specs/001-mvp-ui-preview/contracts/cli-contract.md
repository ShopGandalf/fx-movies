# CLI Contract: fx-movies

**Date**: 2026-03-21

## Command Interface

```
fx_movies <directory_path>
```

### Arguments

| Position | Name           | Required | Description                              |
| -------- | -------------- | -------- | ---------------------------------------- |
| 1        | directory_path | Yes      | Path to directory containing video files |

### Exit Codes

| Code | Meaning                                          |
| ---- | ------------------------------------------------ |
| 0    | Normal exit (user closed application)            |
| 1    | No directory argument provided                   |
| 2    | Provided path does not exist or is not a directory |

### Error Messages

| Condition              | Output (stderr)                                            |
| ---------------------- | ---------------------------------------------------------- |
| No argument            | `Error: No directory path provided. Usage: fx_movies <directory_path>` |
| Path does not exist    | `Error: Path does not exist: <path>`                       |
| Path is not a directory | `Error: Path is not a directory: <path>`                   |

### Examples

```bash
# Valid usage
fx_movies /home/user/videos

# Missing argument
fx_movies
# → Error: No directory path provided. Usage: fx_movies <directory_path>

# Invalid path
fx_movies /nonexistent/path
# → Error: Path does not exist: /nonexistent/path
```
<img src="logo.png" align="left"/>

# Singularity

<p>
    <img src="https://img.shields.io/badge/Latest_Release-0.1-green?style=for-the-badge"/>
    <img src="https://img.shields.io/badge/License-MIT-blue?style=for-the-badge"/>
</p>

Simple whiteboard app with an infinite canvas

⚠️ This is a W.I.P, many features are still missing.

![App](images/sc1.png)

## Build

[Binary distributions](https://github.com/Aoutnheub/Singularity/releases) available

**Linux**

Run `build.sh` from the root folder

**Windows**

Run `build.bat` from the root folder

## Keybindings

- `Mouse wheel` : Zoom in / out
- `r` : Reset zoom
- `=` : Increase brush size
- `-` : Decrease brush size
- `0` : Reset brush size
- `Middle mouse` : Pan
- `w/s/a/d` : Pan up / down / left / right
- `Ctrl + z` : Undo
- `Ctrl + y` : Redo
- `0-9` : Select color
- `Tab` : Hide / Show UI
- `Ctrl + s` : Save

## Configuration

No configuration file yet. For now you have to modify the macros starting with `_` and recompile.

## Dependencies

- [Raylib](https://www.raylib.com/index.html) (included)
- [CMake](https://cmake.org/)
- [Git](https://git-scm.com/)

#pragma once
#include "../raylib/build/raylib/include/raylib.h"
#include "stroke.h"

void renderStrokes(Stroke *_strokes);

void renderColors(
    int _win_height, Color _colors[9], int _selected, int _offset
);

void renderBrushSize(int _size, int _offset);

// Returns the width of the ui element
int renderZoomLevel(
    int _win_width, int _win_height, float _level, int _offset
);

// Returns the width of the ui element
int renderStrokeCount(
    int _win_width, int _win_height, unsigned _count,
    int _offset, int _prev_offset
);

// Returns the width of the ui element
int renderPointCount(
    int _win_width, int _win_height, unsigned _count,
    int _offset, int _prev_offset
);

#pragma once
#include "../raylib/build/raylib/include/raylib.h"

typedef struct StrokePoint {
    int x;
    int y;
    struct StrokePoint *next;
} StrokePoint;

StrokePoint* createStrokePoint(int _x, int _y);

typedef struct Stroke {
    int size;
    Color color;
    unsigned point_count;
    struct StrokePoint *points;
    struct Stroke *next;
} Stroke;

Stroke* createStroke(int _size, Color _color);

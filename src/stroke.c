#include <stdlib.h>
#include <stdbool.h>
#include "stroke.h"

StrokePoint* createStrokePoint(int _x, int _y) {
    StrokePoint *point = malloc(sizeof(StrokePoint));
    if(point == NULL) return NULL;
    point->x = _x;
    point->y = _y;
    point->next = NULL;

    return point;
}

Stroke* createStroke(int _size, Color _color) {
    Stroke *stroke = malloc(sizeof(Stroke));
    if(stroke == NULL) return NULL;
    stroke->size = _size;
    stroke->color = _color;
    stroke->point_count = 0;
    stroke->points = NULL;
    stroke->next = NULL;

    return stroke;
}

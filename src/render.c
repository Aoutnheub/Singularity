#include <stdio.h>
#include "macros.h"
#include "render.h"

void renderStrokes(Stroke *_strokes) {
    Stroke *cs = _strokes;
    while(cs != NULL) {
        StrokePoint *cp = cs->points;
        while(cp != NULL) {
            DrawCircle(cp->x, cp->y, (float)cs->size/2, cs->color);
            if(cp->next != NULL) {
                DrawLineEx(
                    (Vector2){cp->x, cp->y},
                    (Vector2){cp->next->x, cp->next->y},
                    cs->size, cs->color
                );
            }
            cp = cp->next;
        }
        cs = cs->next;
    }
}

void renderColors(
    int _win_height, Color _colors[9], int _selected, int _offset
) {
    DrawRectangleRounded(
        (Rectangle){10 - _offset, (float)_win_height / 2 - 130, 40, 280},
        _UI_B_RADIUS, 4, _UI_BG
    );
    int y_offset = 120;
    for(unsigned i = 0; i < 9; ++i) {
        DrawRectangle(
            20 - _offset, _win_height / 2 - y_offset, 20, 20,
            _colors[i]
        );
        y_offset -= 30;
    }
    y_offset = 150 - _selected * 30;
    DrawRectangleLines(
        19 - _offset, _win_height / 2 - y_offset - 1,
        22, 22, WHITE
    );
}

void renderBrushSize(int _size, int _offset) {
    char size[5]; sprintf(size, "%i", _size);
    int txt_w = MeasureText(size, 20);
    DrawRectangleRounded(
        (Rectangle){10, 10 - _offset, 20 + txt_w, 40}, _UI_B_RADIUS, 4, _UI_BG
    );
    DrawText(size, 20, 20 - _offset, 20, WHITE);
}

int renderZoomLevel(
    int _win_width, int _win_height, float _level, int _offset
) {
    char s[5]; sprintf(s, "x%2.1f", _level);
    int txt_w = MeasureText(s, 20);
    DrawRectangleRounded(
        (Rectangle){
            _win_width - 30 - txt_w, _win_height - 50 + _offset,20 + txt_w, 40
        }, _UI_B_RADIUS, 4, _UI_BG
    );
    DrawText(
        s,
        _win_width - 20 - txt_w, _win_height - 40 + _offset,
        20, WHITE
    );

    return txt_w + 20;
}

int renderStrokeCount(
    int _win_width, int _win_height, unsigned _count,
    int _offset, int _prev_offset
) {
    char s[32]; sprintf(s, "Strokes: %i", _count);
    int txt_w = MeasureText(s, 20);
    DrawRectangleRounded(
        (Rectangle){
            _win_width - 30 - txt_w - _prev_offset,
            _win_height - 50 + _offset,20 + txt_w, 40
        }, _UI_B_RADIUS, 4, _UI_BG
    );
    DrawText(
        s,
        _win_width - 20 - txt_w - _prev_offset, _win_height - 40 + _offset,
        20, WHITE
    );

    return txt_w + 20;
}

int renderPointCount(
    int _win_width, int _win_height, unsigned _count,
    int _offset, int _prev_offset
) {
    char s[32]; sprintf(s, "Points: %i", _count);
    int txt_w = MeasureText(s, 20);
    DrawRectangleRounded(
        (Rectangle){
            _win_width - 30 - txt_w - _prev_offset,
            _win_height - 50 + _offset,20 + txt_w, 40
        }, _UI_B_RADIUS, 4, _UI_BG
    );
    DrawText(
        s,
        _win_width - 20 - txt_w - _prev_offset, _win_height - 40 + _offset,
        20, WHITE
    );

    return txt_w + 20;
}

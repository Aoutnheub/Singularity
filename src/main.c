#include <stdlib.h>
#include <stdio.h>
#include "../raylib/build/raylib/include/raylib.h"

#define _FPS 60
#define _MSAA

// Initial window size
#define _INIT_W 1280
#define _INIT_H 720

// Canvas color
#define _BG_COLOR (Color){32, 33, 36, 255}

// Color palette
#define _PALETTE_01 (Color){240, 240, 240, 255}
#define _PALETTE_02 (Color){10, 10, 10, 255}
#define _PALETTE_03 (Color){255, 97, 136, 255}
#define _PALETTE_04 (Color){169, 220, 118, 255}
#define _PALETTE_05 (Color){120, 220, 232, 255}
#define _PALETTE_06 (Color){252, 152, 103, 255}
#define _PALETTE_07 (Color){255, 216, 102, 255}
#define _PALETTE_08 (Color){171, 157, 242, 255}
#define _PALETTE_09 (Color){255, 200, 221, 255}

// Brush size
#define _INIT_BRUSH_SIZE 4
#define _BRUSH_INC 2 // try to use even numbers

// Key bindings
#define _KEY_RESET_ZOOM KEY_R
#define _KEY_INC_BRUSH_SIZE KEY_EQUAL
#define _KEY_DEC_BRUSH_SIZE KEY_MINUS
#define _KEY_RESET_BRUSH_SIZE KEY_ZERO
#define _KEY_PAN_UP KEY_W
#define _KEY_PAN_RIGHT KEY_D
#define _KEY_PAN_DOWN KEY_S
#define _KEY_PAN_LEFT KEY_A
#define _KEY_MODIFIER KEY_LEFT_CONTROL
#define _KEY_UNDO KEY_Z
#define _KEY_REDO KEY_Y
#define _KEY_COLOR01 KEY_ONE
#define _KEY_COLOR02 KEY_TWO
#define _KEY_COLOR03 KEY_THREE
#define _KEY_COLOR04 KEY_FOUR
#define _KEY_COLOR05 KEY_FIVE
#define _KEY_COLOR06 KEY_SIX
#define _KEY_COLOR07 KEY_SEVEN
#define _KEY_COLOR08 KEY_EIGHT
#define _KEY_COLOR09 KEY_NINE
#define _KEY_TOGGLE_UI KEY_TAB

// UI
#define _UI_BG (Color){10, 10, 10, 150}
#define _UI_B_RADIUS 0.2
// -------------------------------------------------------------------------- //

typedef struct StrokePoint {
    int x;
    int y;
    struct StrokePoint *prev;
    struct StrokePoint *next;
} StrokePoint;

// int vec2Length(Vector2 _p1, Vector2 _p2) {
//     int s = pow(_p1.x - _p2.x, 2) + pow(_p1.y - _p2.y, 2);
//     return sqrt(s);
// }

void renderStrokes(
    StrokePoint **_strokes, unsigned _count, int **_sizes, Color **_colors
) {
    for(unsigned i = 0; i < _count; ++i) {
        StrokePoint *cp = &(*_strokes)[i];
        while(cp->next != NULL) {
            DrawCircle(cp->x, cp->y, (*_sizes)[i]/2, (*_colors)[i]);
            DrawLineEx(
                (Vector2){cp->x, cp->y},
                (Vector2){cp->next->x, cp->next->y},
                (*_sizes)[i], (*_colors)[i]
            );
            cp = cp->next;
        }
        DrawCircle(cp->x, cp->y, 2, (*_colors)[i]);
    }
}

void renderColors(
    int _win_height, Color _colors[9], int _selected, int _offset
) {
    DrawRectangleRounded(
        (Rectangle){10 - _offset, _win_height / 2 - 130, 40, 280},
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

// Returns the width of the ui element
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

// Returns the width of the ui element
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

// Returns the width of the ui element
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
// -------------------------------------------------------------------------- //

int main() {
    #ifdef _MSAA
    SetConfigFlags(/*FLAG_WINDOW_RESIZABLE |*/ FLAG_MSAA_4X_HINT);
    #else
    SetConfigFlags(/*FLAG_WINDOW_RESIZABLE |*/ FLAG_MSAA_4X_HINT);
    #endif

    int win_width = _INIT_W, win_height = _INIT_H;
    int brush_size = _INIT_BRUSH_SIZE;
    Color colors[9] = {
        _PALETTE_01,
        _PALETTE_02,
        _PALETTE_03,
        _PALETTE_04,
        _PALETTE_05,
        _PALETTE_06,
        _PALETTE_07,
        _PALETTE_08,
        _PALETTE_09
    };
    unsigned brush_color = 0;
    InitWindow(win_width, win_height, "Singularity");

    Camera2D camera = {
        (Vector2){0, 0}, // offset
        (Vector2){0, 0}, // target
        0, 1 // rotation, zoom
    };

    SetTargetFPS(_FPS);
    HideCursor();

    Vector2 last_mouse_pos = {0, 0};

    // !WARNING Changing the stroke limit will give an error if a save file
    // bigger than the limit is being loaded
    StrokePoint *strokes = malloc(sizeof(StrokePoint) * 100000);
    int *stroke_sizes = malloc(sizeof(int) * 100000);
    Color *stroke_colors = malloc(sizeof(Color) * 100000);
    if(strokes == NULL || stroke_sizes == NULL || stroke_colors == NULL)
        TraceLog(LOG_ERROR, "Oh no!\n");
    unsigned stroke_index = 0;
    StrokePoint *last_point = NULL;
    bool drawing = false;
    int ui_offset = 0;
    bool animating_ui = false;
    bool ui_closed = false;
    unsigned point_count = 0;

    while(!WindowShouldClose()) {
        if(IsWindowResized()) {
            win_width = GetScreenWidth();
            win_height = GetScreenHeight();
        }

        if(GetMouseWheelMove() < 0) {
            camera.zoom -= 0.1;
            if(camera.zoom < 0.1) camera.zoom = 0.1;
        }else if(GetMouseWheelMove() > 0) {
            camera.zoom += 0.1;
            if(camera.zoom > 3) camera.zoom = 3;
        }
        int mouse_x = GetMouseX();
        int mouse_y = GetMouseY();

        // Key bindings
        if(IsKeyPressed(_KEY_RESET_ZOOM)) {
            camera.zoom = 1;
        }
        if(IsKeyPressed(_KEY_INC_BRUSH_SIZE)) {
            brush_size += _BRUSH_INC;
        }else if(IsKeyPressed(_KEY_DEC_BRUSH_SIZE)) {
            brush_size -= _BRUSH_INC;
            if(brush_size <= 0) brush_size = _BRUSH_INC;
        }else if(IsKeyPressed(_KEY_RESET_BRUSH_SIZE)) {
            brush_size = _INIT_BRUSH_SIZE;
        }

        // Pan
        if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            camera.target = (Vector2){
                camera.target.x - (last_mouse_pos.x - mouse_x)
                                * (-1) / camera.zoom,
                camera.target.y - (last_mouse_pos.y - mouse_y)
                                * (-1) / camera.zoom
            };
        }else {
            if(IsKeyDown(_KEY_PAN_UP)) {
                camera.target.y -= 8;
            }
            if(IsKeyDown(_KEY_PAN_RIGHT)) {
                camera.target.x += 8;
            }
            if(IsKeyDown(_KEY_PAN_DOWN)) {
                camera.target.y += 8;
            }
            if(IsKeyDown(_KEY_PAN_LEFT)) {
                camera.target.x -= 8;
            }
        }

        // Colors
        // TODO: Make it not bad
        if(IsKeyPressed(_KEY_COLOR01))
            brush_color = 0;
        else if(IsKeyPressed(_KEY_COLOR02))
            brush_color = 1;
        else if(IsKeyPressed(_KEY_COLOR03))
            brush_color = 2;
        else if(IsKeyPressed(_KEY_COLOR04))
            brush_color = 3;
        else if(IsKeyPressed(_KEY_COLOR05))
            brush_color = 4;
        else if(IsKeyPressed(_KEY_COLOR06))
            brush_color = 5;
        else if(IsKeyPressed(_KEY_COLOR07))
            brush_color = 6;
        else if(IsKeyPressed(_KEY_COLOR08))
            brush_color = 7;
        else if(IsKeyPressed(_KEY_COLOR09))
            brush_color = 8;

        // Start drawing
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            strokes[stroke_index].x = (mouse_x / camera.zoom) + camera.target.x;
            strokes[stroke_index].y = (mouse_y / camera.zoom) + camera.target.y;
            strokes[stroke_index].prev = NULL;
            strokes[stroke_index].next = NULL;
            last_point = &(strokes[stroke_index]);
            stroke_sizes[stroke_index] = brush_size;
            stroke_colors[stroke_index] = colors[brush_color];
            drawing = true;
            ++stroke_index;
            ++point_count;
        }
        // Stop drawing
        else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && drawing) {
            last_point->next = malloc(sizeof(StrokePoint));
            if(last_point->next == NULL)
                TraceLog(LOG_ERROR, "Something's bad\n");
            last_point->next->x = (mouse_x / camera.zoom) + camera.target.x;
            last_point->next->y = (mouse_y / camera.zoom) + camera.target.y;
            last_point->next->prev = last_point;
            last_point->next->next = NULL;
            last_point = NULL;
            drawing = false;
            ++point_count;
        }
        // Draw
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && drawing) {
            last_point->next = malloc(sizeof(StrokePoint));
            if(last_point->next == NULL)
                TraceLog(LOG_ERROR, "Oopsie\n");
            last_point->next->x = (mouse_x / camera.zoom) + camera.target.x;
            last_point->next->y = (mouse_y / camera.zoom) + camera.target.y;
            last_point->next->prev = last_point;
            last_point->next->next = NULL;
            last_point = last_point->next;
            ++point_count;
        }

        BeginDrawing();

            ClearBackground(_BG_COLOR);

            BeginMode2D(camera);

                renderStrokes(
                    &strokes, stroke_index,
                    &stroke_sizes, &stroke_colors
                );

            EndMode2D();

            // UI
            if(IsKeyPressed(_KEY_TOGGLE_UI)) {
                animating_ui = true;
            }
            if(animating_ui) {
                float delta = GetFrameTime();
                if(ui_closed) {
                    ui_offset -= 1000 * delta;
                    if(ui_offset < 0) {
                        ui_offset = 0;
                        animating_ui = false;
                        ui_closed = false;
                    }
                }else {
                    ui_offset += 1000 * delta;
                    if(ui_offset > 50) {
                        ui_offset = 50;
                        animating_ui = false;
                        ui_closed = true;
                    }
                }
            }
            renderColors(win_height, colors, brush_color+1, ui_offset);
            renderBrushSize(brush_size, ui_offset);
            int zoom_lvl_w = renderZoomLevel(
                win_width, win_height, camera.zoom, ui_offset
            );
            int point_cnt_w = renderPointCount(
                win_width, win_height, point_count, ui_offset, zoom_lvl_w + 10
            );
            renderStrokeCount(
                win_width, win_height, stroke_index,
                ui_offset, zoom_lvl_w + point_cnt_w + 20
            );

            // Mouse Cursor
            DrawCircleLines(
                mouse_x, mouse_y, brush_size / 2 * camera.zoom,
                (Color){230, 230, 230, 150}
            );

        EndDrawing();

        last_mouse_pos.x = mouse_x;
        last_mouse_pos.y = mouse_y;
    }

    // Clean up
    for(unsigned i = 0; i < stroke_index; ++i) {
        StrokePoint *cp = strokes[i].next;
        while(cp != NULL) {
            StrokePoint *next_p = cp->next;
            free(cp);
            cp = next_p;
        }
    }
    free(strokes);
    free(stroke_sizes);
    free(stroke_colors);

    CloseWindow();
}

#include <stdlib.h>
#include "../raylib/build/raylib/include/raylib.h"

#define _FPS 60

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
// ---------------------------------------------------------

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

bool arePointsEqual(int _p1x, int _p1y, int _p2x, int _p2y) {
    if(_p1x == _p2x || _p1y == _p2y) return true;
    else return false;
}

void renderStrokes(StrokePoint **_strokes, unsigned _count, int **_sizes) {
    for(unsigned i = 0; i < _count; ++i) {
        StrokePoint *cp = &(*_strokes)[i];
        while(cp->next != NULL) {
            DrawCircle(cp->x, cp->y, (*_sizes)[i]/2, WHITE);
            DrawLineEx(
                (Vector2){cp->x, cp->y},
                (Vector2){cp->next->x, cp->next->y},
                (*_sizes)[i], WHITE
            );
            cp = cp->next;
        }
        DrawCircle(cp->x, cp->y, 2, WHITE);
    }
}
// ---------------------------------------------------------

int main() {
    int win_width = _INIT_W, win_height = _INIT_H;
    int brush_size = _INIT_BRUSH_SIZE;
    InitWindow(win_width, win_height, "Singularity");

    Camera2D camera = {
        (Vector2){0, 0}, // offset
        (Vector2){0, 0}, // target
        0, 1 // rotation, zoom
    };

    SetTargetFPS(_FPS);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    Vector2 last_mouse_pos = {0, 0};

    // !WARNING Changing the stroke limit will give an error if a save file
    // bigger than the limit is being loaded
    StrokePoint *strokes = malloc(sizeof(StrokePoint) * 100000);
    int *stroke_sizes = malloc(sizeof(int) * 100000);
    unsigned stroke_index = 0;
    StrokePoint *last_point = NULL;
    bool drawing = false;
    bool redraw = true;

    while(!WindowShouldClose()) {
        if(GetMouseWheelMove() < 0) {
            camera.zoom -= 0.1;
            if(camera.zoom <= 0) camera.zoom = 0.1;
            redraw = true;
        }else if(GetMouseWheelMove() > 0) {
            camera.zoom += 0.1;
            if(camera.zoom > 3) camera.zoom = 3;
            redraw = true;
        }
        int mouse_x = GetMouseX();
        int mouse_y = GetMouseY();
        // Squeeze out some performance when idle
        if(last_mouse_pos.x != mouse_x && last_mouse_pos.y != mouse_y)
            redraw = true;

        // Key bindings
        if(IsKeyPressed(_KEY_RESET_ZOOM)) {
            camera.zoom = 1;
            redraw = true;
        }else if(IsKeyPressed(_KEY_INC_BRUSH_SIZE)) {
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
        }

        // Start drawing
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            strokes[stroke_index].x = (mouse_x / camera.zoom) + camera.target.x;
            strokes[stroke_index].y = (mouse_y / camera.zoom) + camera.target.y;
            strokes[stroke_index].prev = NULL;
            strokes[stroke_index].next = NULL;
            last_point = &(strokes[stroke_index]);
            stroke_sizes[stroke_index] = brush_size;
            drawing = true;
            ++stroke_index;
            redraw = true;
        }
        // Stop drawing
        else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && drawing) {
            last_point->next = malloc(sizeof(StrokePoint));
            last_point->next->x = (mouse_x / camera.zoom) + camera.target.x;
            last_point->next->y = (mouse_y / camera.zoom) + camera.target.y;
            last_point->next->prev = last_point;
            last_point->next->next = NULL;
            last_point = NULL;
            drawing = false;
            redraw = true;
        }
        // Draw
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && drawing) {
            last_point->next = malloc(sizeof(StrokePoint));
            last_point->next->x = (mouse_x / camera.zoom) + camera.target.x;
            last_point->next->y = (mouse_y / camera.zoom) + camera.target.y;
            last_point->next->prev = last_point;
            last_point->next->next = NULL;
            last_point = last_point->next;
            redraw = true;
        }

        BeginDrawing();

            if(redraw) { ClearBackground(_BG_COLOR);

            BeginMode2D(camera);

                renderStrokes(&strokes, stroke_index, &stroke_sizes);

            EndMode2D(); }

        EndDrawing();

        last_mouse_pos.x = mouse_x;
        last_mouse_pos.y = mouse_y;

        redraw = false;
    }

    CloseWindow();
}
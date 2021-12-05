#pragma once

// #define _DEBUG

#define _FPS 60
#define _MSAA // Disable to increase performance

// Initial window size
#define _INIT_W 1280
#define _INIT_H 720

// Color palette
// Define one of the following
#define _THEME_DEFAULT
// #define _THEME_DEFAULT_LIGHT
// #define _THEME_HARD
// #define _THEME_HARD_LIGHT

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
#define _KEY_SAVE KEY_S

// UI
#define _UI_BG (Color){10, 10, 10, 150}
#define _UI_B_RADIUS 0.2

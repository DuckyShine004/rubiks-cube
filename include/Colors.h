#ifndef COLORS_H
#define COLORS_H

#include "glad/glad.h"

#define ORANGE 1.0, 0.5, 0.0
#define WHITE 1.0, 1.0, 1.0
#define YELLOW 1.0, 1.0, 0.0
#define RED 1.0, 0.0, 0.0
#define GREEN 0.0, 1.0, 0.0
#define BLUE 0.0, 0.0, 1.0

#define ALL_ORANGE ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE
#define ALL_WHITE WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
#define ALL_YELLOW YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW
#define ALL_RED RED, RED, RED, RED, RED, RED
#define ALL_GREEN GREEN, GREEN, GREEN, GREEN, GREEN, GREEN
#define ALL_BLUE BLUE, BLUE, BLUE, BLUE, BLUE, BLUE

extern const GLfloat COLOR_GOW[] = {
    // Left
    ALL_GREEN,

    // Right
    ALL_BLUE,

    // Front
    ALL_ORANGE,

    // Back
    ALL_RED,

    // Bottom
    ALL_WHITE,

    // Top
    ALL_YELLOW};
#endif

//
// Created by Loic_ on 16/02/2023.
//

#ifndef DANTE_INCLUDE_HPP
#define DANTE_INCLUDE_HPP

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ctime>
#include <unistd.h>

#define WALL 'X'
#define CORRIDOR '.'

typedef struct vector2i {
    int x;
    int y;
} vec2i;

typedef struct anchor_s {
    int ptr;
    anchor_s *next;
} anchor_t;

#endif //DANTE_INCLUDE_HPP


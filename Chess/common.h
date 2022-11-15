#ifndef COMMON_H
#define COMMON_H

#include <wchar.h>
#include <stdbool.h>

typedef enum Color{
    white,black,none
}Color;

typedef struct Position{
    int x, y;
}Position;

typedef struct Piece{
    wchar_t type;
    Color color;
    char read;
    Position position;
    Position lastPos;
}Piece;

typedef struct Tile{
    Piece piece;
}Tile;

char*GetColor(Color color);
bool IsPosEqual(Position a, Position b);
void SetPosEqual(Position *a, Position b);
bool IsInBound(int x, int y);

#endif // COMMON_H

#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>

#include "common.h"
#include "legalmoves.h"

char**ReadBoardState(Color *color);
Tile** InitializeBoard(Tile**tiles,Piece*pieces, int db);
void DrawBoard(Tile**tiles,Position selection,Position*legalMoves,int numLegalMoves,Color color,Piece *downedPieces,int downeddb);
void SaveBoard(Tile**tiles);
void ClearSavedBoard(char*fileName);
Piece* ReadDownedPieces(int *downeddb);
void SaveDownedPieces(Piece*pieces, int db);
void SaveLastPosition(Tile**tiles);
Position*ReadLastPosition();
void FreeTiles(Tile**tiles);

#endif // BOARD_H

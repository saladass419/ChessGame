#ifndef LEGALMOVES_H
#define LEGALMOVES_H

#include "common.h"
#include "piece.h"
#include "board.h"

#include <stdbool.h>
#include <string.h>

Position*GetLegalMoves(Piece*selection,Tile**tiles, int *numLegalMoves,Piece*pieces,int db,bool checkCastle);
bool LegalMovesContains(Position*legalMoves,int numLegalMoves,Position position);
bool IsInCheck(Position kingPos, Color color,Piece* pieces,int db, Tile**tiles);
Position*CorrectLegalMoves(Piece*selection,Position*legalMoves,Tile**tiles,Piece*pieces,int *piecesdb,int *movesdb,Piece *downedPiece);

#endif // LEGALMOVES_H

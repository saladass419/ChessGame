#ifndef PIECE_H
#define PIECE_H

#include <string.h>
#include <wchar.h>
#include <ctype.h>
#include <stdbool.h>

#include "common.h"
#include "board.h"

Piece*InitializePieces(char** boardState, int *amount,Position *lastPositions);
Piece*SelectPiece(Piece*pieces,int db, Position input,Color color);
void SetPiecesEqual(Piece*a,Piece b);
Piece* MoveOrCapture(Piece*pieces,int*db,Position from,Position destination,Piece *downedPiece,int *downeddb,bool set);
Piece* MovePiece(Piece*pieces,int db,Position from, Position to, bool set);
Piece* RemovePiece(Piece*pieces,int *db,Position position,Piece *downedPiece,int **downeddb);
Position FindKing(Color color,Piece*pieces,int db);
Piece NullPiece();
Piece Promotion(Piece current);

Piece *GetPieceFromPosition(Piece*pieces,int db,Position position);
Piece*CreatePiece(Piece*pieces,int*db,Piece piece);


#endif // PIECE_H

#include "debugmalloc.h"
#include "legalmoves.h"

Position*GetLegalMoves(Piece*selection,Tile**tiles,int *numLegalMoves,Piece*pieces,int piecesdb,bool checkCastle){
    if(selection==NULL) return NULL;
    int db = -1;
    Position pos=selection->position;
    int x = pos.x;
    int y = pos.y;
    Position tempLegalMoves[64];
    switch(selection->read){
        case 'K':
        case 'k':
            if(IsInBound(x+1,y)&&tiles[x+1][y].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x+1;
                tempLegalMoves[db].y=y;
            }
            if(IsInBound(x,y+1)&&tiles[x][y+1].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x;
                tempLegalMoves[db].y=y+1;
            }
            if(IsInBound(x-1,y)&&tiles[x-1][y].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x-1;
                tempLegalMoves[db].y=y;
            }
            if(IsInBound(x,y-1)&&tiles[x][y-1].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x;
                tempLegalMoves[db].y=y-1;
            }
            if(IsInBound(x+1,y+1)&&tiles[x+1][y+1].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x+1;
                tempLegalMoves[db].y=y+1;
            }
            if(IsInBound(x-1,y-1)&&tiles[x-1][y-1].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x-1;
                tempLegalMoves[db].y=y-1;
            }
            if(IsInBound(x+1,y-1)&&tiles[x+1][y-1].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x+1;
                tempLegalMoves[db].y=y-1;
            }
            if(IsInBound(x-1,y+1)&&tiles[x-1][y+1].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x-1;
                tempLegalMoves[db].y=y+1;
            }
            if(IsInCheck(selection->position,selection->color,pieces,piecesdb,tiles)||checkCastle==false)break;
            if(IsPosEqual(selection->lastPos,selection->position)&&IsPosEqual(tiles[x][7].piece.lastPos,tiles[x][7].piece.position)){
                if(tiles[x][y+1].piece.read==' '&&tiles[x][y+2].piece.read==' '){
                    bool canCastle=true;
                    for(int i=0;i<piecesdb;i++){
                        if(pieces[i].color!=selection->color){
                            int castlingNumLegalMoves=0;
                            Position *castlingLegalMoves = GetLegalMoves(&pieces[i],tiles,&castlingNumLegalMoves,pieces,piecesdb,false);
                            Position betweenRookAndKing;
                            betweenRookAndKing.x=x;
                            betweenRookAndKing.y=y+1;
                            if(LegalMovesContains(castlingLegalMoves,castlingNumLegalMoves,betweenRookAndKing)){
                                canCastle=false;
                                free(castlingLegalMoves);
                                break;
                            }
                            free(castlingLegalMoves);
                        }
                    }
                    if(canCastle==true){
                        db++;
                        tempLegalMoves[db].x=x;
                        tempLegalMoves[db].y=y+2;
                    }
                }
            }
            if(IsPosEqual(selection->lastPos,selection->position)&&IsPosEqual(tiles[x][0].piece.lastPos,tiles[x][0].piece.position)){
                if(tiles[x][y-1].piece.read==' '&&tiles[x][y-2].piece.read==' '&&tiles[x][y-3].piece.read==' '){
                    bool canCastle=true;
                    for(int i=0;i<piecesdb;i++){
                        if(pieces[i].color!=selection->color){
                            int castlingNumLegalMoves=0;
                            Position *castlingLegalMoves = GetLegalMoves(&pieces[i],tiles,&castlingNumLegalMoves,pieces,piecesdb,false);
                            Position betweenRookAndKing;
                            betweenRookAndKing.x=x;
                            betweenRookAndKing.y=y-1;
                            if(LegalMovesContains(castlingLegalMoves,castlingNumLegalMoves,betweenRookAndKing)){
                                canCastle=false;
                                free(castlingLegalMoves);
                                break;
                            }
                            free(castlingLegalMoves);
                        }
                    }
                    if(canCastle==true){
                        db++;
                        tempLegalMoves[db].x=x;
                        tempLegalMoves[db].y=y-2;
                    }
                }
            }
            break;
        case 'Q':
        case 'q':
            for(int i = x+1; i<8;i++){
                if(IsInBound(i,y)&&tiles[i][y].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=i;
                    tempLegalMoves[db].y=y;
                }
                if(tiles[i][y].piece.color!=none) break;
            }
            for(int i = x-1; i>-1;i--){
                if(IsInBound(i,y)&&tiles[i][y].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=i;
                    tempLegalMoves[db].y=y;
                }
                if(tiles[i][y].piece.color!=none) break;
            }
            for(int i = y+1; i<8;i++){
                if(IsInBound(x,i)&&tiles[x][i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x;
                    tempLegalMoves[db].y=i;
                }
                if(tiles[x][i].piece.color!=none) break;
            }
            for(int i = y-1; i>-1;i--){
                if(IsInBound(x,i)&&tiles[x][i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x;
                    tempLegalMoves[db].y=i;
                }
                if(tiles[x][i].piece.color!=none) break;
            }
            for(int i = 1; i<8;i++){
                if(IsInBound(x+i,y+i)&&tiles[x+i][y+i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x+i;
                    tempLegalMoves[db].y=y+i;
                }
                if(!IsInBound(x+i,y+i)||tiles[x+i][y+i].piece.color!=none) break;
            }
            for(int i = 1; i<8;i++){
                if(IsInBound(x+i,y-i)&&tiles[x+i][y-i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x+i;
                    tempLegalMoves[db].y=y-i;
                }
                if(!IsInBound(x+i,y-i)||tiles[x+i][y-i].piece.color!=none) break;
            }
            for(int i = 1; i<8;i++){
                if(IsInBound(x-i,y+i)&&tiles[x-i][y+i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x-i;
                    tempLegalMoves[db].y=y+i;
                }
                if(!IsInBound(x-i,y+i)||tiles[x-i][y+i].piece.color!=none) break;
            }
            for(int i = 1; i<8;i++){
                if(IsInBound(x-i,y-i)&&tiles[x-i][y-i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x-i;
                    tempLegalMoves[db].y=y-i;
                }
                if(!IsInBound(x-i,y-i)||tiles[x-i][y-i].piece.color!=none) break;
            }
            break;
        case 'R':
        case 'r':
            for(int i = x+1; i<8;i++){
                if(IsInBound(i,y)&&tiles[i][y].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=i;
                    tempLegalMoves[db].y=y;
                }
                if(tiles[i][y].piece.color!=none) break;
            }
            for(int i = x-1; i>-1;i--){
                if(IsInBound(i,y)&&tiles[i][y].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=i;
                    tempLegalMoves[db].y=y;
                }
                if(tiles[i][y].piece.color!=none) break;
            }
            for(int i = y+1; i<8;i++){
                if(IsInBound(x,i)&&tiles[x][i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x;
                    tempLegalMoves[db].y=i;
                }
                if(tiles[x][i].piece.color!=none) break;
            }
            for(int i = y-1; i>-1;i--){
                if(IsInBound(x,i)&&tiles[x][i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x;
                    tempLegalMoves[db].y=i;
                }
                if(tiles[x][i].piece.color!=none) break;
            }
            break;
        case 'B':
        case 'b':
            for(int i = 1; i<8;i++){
                if(IsInBound(x+i,y+i)&&tiles[x+i][y+i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x+i;
                    tempLegalMoves[db].y=y+i;
                }
                if(!IsInBound(x+i,y+i)||tiles[x+i][y+i].piece.color!=none) break;
            }
            for(int i = 1; i<8;i++){
                if(IsInBound(x+i,y-i)&&tiles[x+i][y-i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x+i;
                    tempLegalMoves[db].y=y-i;
                }
                if(!IsInBound(x+i,y-i)||tiles[x+i][y-i].piece.color!=none) break;
            }
            for(int i = 1; i<8;i++){
                if(IsInBound(x-i,y+i)&&tiles[x-i][y+i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x-i;
                    tempLegalMoves[db].y=y+i;
                }
                if(!IsInBound(x-i,y+i)||tiles[x-i][y+i].piece.color!=none) break;
            }
            for(int i = 1; i<8;i++){
                if(IsInBound(x-i,y-i)&&tiles[x-i][y-i].piece.color!=selection->color){
                    db++;
                    tempLegalMoves[db].x=x-i;
                    tempLegalMoves[db].y=y-i;
                }
                if(!IsInBound(x-i,y-i)||tiles[x-i][y-i].piece.color!=none) break;
            }
            break;
        case 'N':
        case 'n':
            if(IsInBound(x+1,y+2)&&tiles[x+1][y+2].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x+1;
                tempLegalMoves[db].y=y+2;
            }
            if(IsInBound(x+2,y+1)&&tiles[x+2][y+1].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x+2;
                tempLegalMoves[db].y=y+1;
            }
            if(IsInBound(x-1,y+2)&&tiles[x-1][y+2].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x-1;
                tempLegalMoves[db].y=y+2;
            }
            if(IsInBound(x+2,y-1)&&tiles[x+2][y-1].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x+2;
                tempLegalMoves[db].y=y-1;
            }
            if(IsInBound(x-2,y+1)&&tiles[x-2][y+1].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x-2;
                tempLegalMoves[db].y=y+1;
            }
            if(IsInBound(x+1,y-2)&&tiles[x+1][y-2].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x+1;
                tempLegalMoves[db].y=y-2;
            }
            if(IsInBound(x-2,y-1)&&tiles[x-2][y-1].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x-2;
                tempLegalMoves[db].y=y-1;
            }
            if(IsInBound(x-1,y-2)&&tiles[x-1][y-2].piece.color!=selection->color){
                db++;
                tempLegalMoves[db].x=x-1;
                tempLegalMoves[db].y=y-2;
            }
            break;
        case 'P':
            if(IsInBound(x-1,y)&&tiles[x-1][y].piece.color==none){
                db++;
                tempLegalMoves[db].x=x-1;
                tempLegalMoves[db].y=y;
                if(x==6&&IsInBound(x-2,y)&&tiles[x-2][y].piece.color==none){
                    db++;
                    tempLegalMoves[db].x=x-2;
                    tempLegalMoves[db].y=y;
                }
            }
            if(IsInBound(x-1,y+1)&&tiles[x-1][y+1].piece.color==black){
                db++;
                tempLegalMoves[db].x=x-1;
                tempLegalMoves[db].y=y+1;
            }
            if(IsInBound(x-1,y-1)&&tiles[x-1][y-1].piece.color==black){
                db++;
                tempLegalMoves[db].x=x-1;
                tempLegalMoves[db].y=y-1;
            }
            if(IsInBound(x-1,y-1)&&tiles[x][y-1].piece.read=='p'&&tiles[x][y-1].piece.lastPos.x==x-2){ 
                db++;
                tempLegalMoves[db].x=x-1;
                tempLegalMoves[db].y=y-1;
            }
            if(IsInBound(x-1,y+1)&&tiles[x][y+1].piece.read=='p'&&tiles[x][y+1].piece.lastPos.x==x-2){
                db++;
                tempLegalMoves[db].x=x-1;
                tempLegalMoves[db].y=y+1;
            }
            break;
        case 'p':
            if(IsInBound(x+1,y)&&tiles[x+1][y].piece.color==none){
                db++;
                tempLegalMoves[db].x=x+1;
                tempLegalMoves[db].y=y;
                if(x==1&&IsInBound(x+2,y)&&tiles[x+2][y].piece.color==none){
                    db++;
                    tempLegalMoves[db].x=x+2;
                    tempLegalMoves[db].y=y;
                }
            }
            if(IsInBound(x+1,y+1)&&tiles[x+1][y+1].piece.color==white){
                db++;
                tempLegalMoves[db].x=x+1;
                tempLegalMoves[db].y=y+1;
            }
            if(IsInBound(x+1,y-1)&&tiles[x+1][y-1].piece.color==white){
                db++;
                tempLegalMoves[db].x=x+1;
                tempLegalMoves[db].y=y-1;
            }
            if(IsInBound(x+1,y-1)&&tiles[x][y-1].piece.read=='P'&&tiles[x][y-1].piece.lastPos.x==x+2){
                db++;
                tempLegalMoves[db].x=x+1;
                tempLegalMoves[db].y=y-1;
            }
            if(IsInBound(x+1,y+1)&&tiles[x][y+1].piece.read=='P'&&tiles[x][y+1].piece.lastPos.x==x+2){
                db++;
                tempLegalMoves[db].x=x+1;
                tempLegalMoves[db].y=y+1;
            }
            break;
        default:
            break;
    }
    Position nullPos;
    nullPos.x = -1;
    nullPos.y = -1;
    for(int i = db+1;i<64;i++){
        SetPosEqual(&tempLegalMoves[i],nullPos);
    }
    Position *legalMoves = (Position*)malloc((db+1)*sizeof(Position));
    int idx=0;
    for (int i = 0; i < 64; ++i)
    {
        if(!IsPosEqual(tempLegalMoves[i],nullPos)){
            SetPosEqual(&legalMoves[idx++],tempLegalMoves[i]);
        }
    }
    *numLegalMoves=idx;
    return legalMoves;
}
bool LegalMovesContains(Position*legalMoves,int numLegalMoves,Position position){
    for(int i = 0; i<numLegalMoves;i++) 
        if(IsPosEqual(legalMoves[i],position)) return true;
    return false;
}
bool IsInCheck(Position kingPos, Color color,Piece* pieces,int db, Tile**tiles){
    int numLegalMoves = 0;
    Position*legalMoves = NULL;

    for(int i = 0; i < db; i++){
        if(pieces[i].color!=color&&tolower(pieces[i].read)!='k'){
            legalMoves=GetLegalMoves(&pieces[i],tiles,&numLegalMoves,pieces,db,true);
            if(LegalMovesContains(legalMoves,numLegalMoves,kingPos)) { free(legalMoves); return true; } //Check if the king is in check
            if(numLegalMoves!=0) free(legalMoves);
        }
    }
    return false;
}
Position*CorrectLegalMoves(Piece*selection,Position*legalMoves,Tile**tiles,Piece*pieces,int *piecesdb,int *movesdb,Piece *downedPiece){
    if(selection==NULL) return legalMoves;
    Position tempPos;
    SetPosEqual(&tempPos,selection->position);

    Position tempLegalMoves[*movesdb];
    Position kingPos = FindKing(selection->color,pieces,*piecesdb);
    int newdb=0;

    Piece tempSelection;
    SetPiecesEqual(&tempSelection,*selection);
    Piece*tempPiece=NULL;
    Piece hitPiece;

    for(int i = 0; i < *movesdb; i++){
        tempPiece = GetPieceFromPosition(pieces,*piecesdb,legalMoves[i]); //Check if there is a piece on the square
        if(tempPiece!=NULL){ // If there is, make a copy of that piece
            SetPiecesEqual(&hitPiece,*tempPiece);

            pieces = MoveOrCapture(pieces,piecesdb,tempPos,hitPiece.position,NULL,NULL,false); //Capture that piece with the selection
            tiles = InitializeBoard(tiles,pieces,*piecesdb);
            
            if(tolower(tempSelection.read)=='k'){
                SetPosEqual(&kingPos,hitPiece.position);
            }
            
            if(!IsInCheck(kingPos,selection->color,pieces,*piecesdb,tiles)){ //See if the player is still in check
                SetPosEqual(&tempLegalMoves[newdb++],legalMoves[i]); //If not, add that move to the legalmoves
            }   
            
            if(tolower(tempSelection.read)=='k'){
                SetPosEqual(&kingPos,tempPos);
            }
            
            pieces = MovePiece(pieces,*piecesdb,hitPiece.position,tempPos,false); //Move back the selection
            pieces = CreatePiece(pieces,piecesdb,hitPiece); //Recreate the piece that was taken

            tiles = InitializeBoard(tiles,pieces,*piecesdb);
            
        }
        else{
            pieces = MovePiece(pieces,*piecesdb,tempPos,legalMoves[i],false); //If there is no pother piece on the square, move the selection there
            tiles = InitializeBoard(tiles,pieces,*piecesdb);
            
            if(tolower(tempSelection.read)=='k'){
                SetPosEqual(&kingPos,legalMoves[i]);
            }

            if(!IsInCheck(kingPos,selection->color,pieces,*piecesdb,tiles)){ //See if the player is still in check
                SetPosEqual(&tempLegalMoves[newdb++],legalMoves[i]); //If not, add that move to the legalmoves
            }
            
            if(tolower(tempSelection.read)=='k'){
                SetPosEqual(&kingPos,tempPos);
            }
            
            pieces = MovePiece(pieces,*piecesdb,legalMoves[i],tempPos,false); //Move the selection back to it's original position
            tiles = InitializeBoard(tiles,pieces,*piecesdb);
        }
    }
    for(int i = newdb;i<*movesdb;i++){
        tempLegalMoves[i].x = -1;
        tempLegalMoves[i].y = -1;
    }

    free(legalMoves);
    Position*correctMoves = (Position*)malloc(newdb*sizeof(Position));
    *movesdb = newdb;
    for(int i = 0; i < newdb; i++){
        SetPosEqual(&correctMoves[i],tempLegalMoves[i]);
    }
    return correctMoves;
}
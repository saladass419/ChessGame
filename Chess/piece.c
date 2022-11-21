#include "debugmalloc.h"
#include "piece.h"

Piece*InitializePieces(char** boardState,int *amount,Position *lastPositions){
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            if(isspace(boardState[x][y])==0)++(*amount); //Checking for non-free squares
        }
    }
    Piece*pieces = (Piece*)malloc((*amount)*sizeof(Piece));
    int db = 0;
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            pieces[db].position.x = x;
            pieces[db].position.y = y;
            if(lastPositions[x*8+y].x==-1||lastPositions[x*8+y].y==-1){
                pieces[db].lastPos.x = x;
                pieces[db].lastPos.y = y;
            }
            else{
                SetPosEqual(&pieces[db].lastPos,lastPositions[x*8+y]);
            }
            pieces[db].read = boardState[x][y];
            switch(pieces[db].read){
            case 'K':
                pieces[db].type=0x2654;
                pieces[db].color=white;
                break;
            case 'Q':
                pieces[db].type=0x2655;
                pieces[db].color=white;
                break;
            case 'R':
                pieces[db].type=0x2656;
                pieces[db].color=white;
                break;
            case 'B':
                pieces[db].type=0x2657;
                pieces[db].color=white;
                break;
            case 'N':
                pieces[db].type=0x2658;
                pieces[db].color=white;
                break;
            case 'P':
                pieces[db].type=0x2659;
                pieces[db].color=white;
                break;
            case 'k':
                pieces[db].type=0x265A;
                pieces[db].color=black;
                break;
            case 'q':
                pieces[db].type=0x265B;
                pieces[db].color=black;
                break;
            case 'r':
                pieces[db].type=0x265C;
                pieces[db].color=black;
                break;
            case 'b':
                pieces[db].type=0x265D;
                pieces[db].color=black;
                break;
            case 'n':
                pieces[db].type=0x265E;
                pieces[db].color=black;
                break;
            case 'p':
                pieces[db].type=0x265F;
                pieces[db].color=black;
                break;
            default:
                db--;
                break;
            }
            db++;
            if(db==*amount) break;
        }
    }
    for(int i = 0; i<8;i++){
        free(boardState[i]);
    }
    free(boardState);
    free(lastPositions);
    return pieces;
}
Piece*SelectPiece(Piece*pieces,int db, Position input,Color color){
    for(int i = 0; i< db; i++){
        if(IsPosEqual(pieces[i].position,input)&&color==pieces[i].color){ //Finding the selection
            return &pieces[i];
        }
    }
    return NULL;
}
void SetPiecesEqual(Piece*a,Piece b){
    a->color = b.color;
    a->type = b.type;
    a->read = b.read;
    SetPosEqual(&a->position,b.position);
    SetPosEqual(&a->lastPos,b.lastPos);
}
Piece* MoveOrCapture(Piece*pieces,int*db,Position from, Position to,Piece *downedPiece,int *downeddb,bool set){
    if(GetPieceFromPosition(pieces,*db,to)!=NULL){
        pieces = RemovePiece(pieces,db,to,downedPiece,&downeddb);
        pieces = MovePiece(pieces,*db,from,to,set);
    }
    else{
        Piece selection =*GetPieceFromPosition(pieces,*db,from);
        if(selection.read=='P'||selection.read=='p'){ //Checking for En Passaint
            if(to.y!=from.y){
                Position toBeRemoved;
                toBeRemoved.x=from.x;
                toBeRemoved.y=to.y;
                pieces = RemovePiece(pieces,db,toBeRemoved,downedPiece,&downeddb);
            }
        }
        else if(selection.read=='K'||selection.read=='k'){ //Checking for castling
            if(to.y==from.y+2){
                Position toBeMoved;
                toBeMoved.x=from.x;
                toBeMoved.y=from.y+3;
                Position toMoveTo;
                toMoveTo.x=from.x;
                toMoveTo.y=to.y-1;
                pieces = MovePiece(pieces,*db,toBeMoved,toMoveTo,set);
            }
            if(to.y==from.y-2){
                Position toBeMoved;
                toBeMoved.x=from.x;
                toBeMoved.y=from.y-4;
                Position toMoveTo;
                toMoveTo.x=from.x;
                toMoveTo.y=to.y+1;
                pieces = MovePiece(pieces,*db,toBeMoved,toMoveTo,set);
            }
        }
        pieces = MovePiece(pieces,*db,from,to,set);
    }
    Piece selection =*GetPieceFromPosition(pieces,*db,to);
    if(set==true&&((selection.read=='P'&&selection.position.x==0)||(selection.read=='p'&&selection.position.x==7))){ //Checking for promotion
        Piece promoted;
        SetPiecesEqual(&promoted,Promotion(selection));
        int idx = -1;
        for(int i = 0; i < *db; i++){
            if(IsPosEqual(pieces[i].position,selection.position)) {idx = i; break;}
        }
        pieces = RemovePiece(pieces,db,selection.position,downedPiece,&downeddb);
        pieces = CreatePiece(pieces,db,promoted,idx);
    }
    return pieces;
}
Piece Promotion(Piece current){
    wprintf(L"What would you like to promote to?\n'Q,R,B,N'\n");
    char choice=' ';
    do{
        scanf(" %c",&choice);
        if(tolower(choice)!='q'&&tolower(choice)!='r'&&tolower(choice)!='b'&&tolower(choice)!='n') wprintf(L"Incorrect piece!\n");
    }while(tolower(choice)!='q'&&tolower(choice)!='r'&&tolower(choice)!='b'&&tolower(choice)!='n');
    Piece promotedPiece;

    promotedPiece.color = current.color;
    SetPosEqual(&promotedPiece.position,current.position);
    SetPosEqual(&promotedPiece.lastPos,current.lastPos);

    switch (tolower(choice)){
        case 'q':
            if(current.color==white){
                promotedPiece.read='Q';
                promotedPiece.type=0x2655;
            }
            else{
                promotedPiece.read='q';
                promotedPiece.type=0x265B;
            }
         break;
         case 'r':
            if(current.color==white){
                promotedPiece.read='R';
                promotedPiece.type=0x2656;
            }
            else{
                promotedPiece.read='r';
                promotedPiece.type=0x265C;
            }
         break;
         case 'b':
            if(current.color==white){
                promotedPiece.read='B';
                promotedPiece.type=0x2657;
            }
            else{
                promotedPiece.read='b';
                promotedPiece.type=0x265D;
            }
         break;
         case 'n':
            if(current.color==white){
                promotedPiece.read='N';
                promotedPiece.type=0x2658;
            }
            else{
                promotedPiece.read='n';
                promotedPiece.type=0x265E;
            }
         break;
    }
    return promotedPiece;
}
Piece* MovePiece(Piece*pieces,int db,Position from, Position to, bool set){
    for(int i = 0; i<db;i++){
        if(IsPosEqual(pieces[i].position,from)){
            if(set==true)
                SetPosEqual(&pieces[i].lastPos,from);
            SetPosEqual(&pieces[i].position,to);
            break;
        }
    }
    return pieces;
}
Piece* RemovePiece(Piece*pieces,int *db,Position position,Piece *downedPiece,int **downeddb){
    Piece*tempPieces = NULL;
    for(int i = 0; i<*db;i++){
        if(IsPosEqual(pieces[i].position,position)) { 
            tempPieces = (Piece*)malloc((*db-1)*sizeof(Piece)); 
            if(downedPiece!=NULL){
                SetPiecesEqual(downedPiece,pieces[i]);
                **downeddb+=1;
            }
            break; 
        }
    }
    if(tempPieces==NULL) return pieces;
    int newdb=0;
    for(int i = 0; i< *db;i++){
        if(!IsPosEqual(pieces[i].position,position)){
            SetPiecesEqual(&tempPieces[newdb++],pieces[i]);
        }
    }
    *db=newdb;
    free(pieces);
    return tempPieces;
}
Position FindKing(Color color,Piece*pieces,int db){
    for(int i = 0; i<db;i++){
        if(pieces[i].color==color&&tolower(pieces[i].read)=='k') return pieces[i].position;
    }
}

Piece*GetPieceFromPosition(Piece*pieces,int db,Position position){
    for (int i = 0; i < db; ++i)
    {
        if(IsPosEqual(pieces[i].position,position)) return &pieces[i];
    }
    return NULL;
}
Piece* CreatePiece(Piece*pieces,int*db,Piece piece, int idx){
    Piece*tempPieces = (Piece*)malloc((*db+1)*sizeof(Piece));
    int tempIdx=0;
    for(int i = 0; i < *db+1; i++){
        if(i==idx){
            SetPiecesEqual(&tempPieces[i],piece);
        }
        else{
            SetPiecesEqual(&tempPieces[i],pieces[tempIdx++]);
        }
    }
    free(pieces);
    *db+=1;
    return tempPieces;
}
int PiecesCount(Piece**pieces){
    int db = 0;
    for(int i = 0; pieces[i]!=NULL; i++){
        db++;
    }
    return db;
}
Piece NullPiece(){
    Piece piece;
    piece.position.x = -1;
    piece.position.y = -1;
    piece.type = 0x2800;
    piece.color = none;
    piece.read=' ';
    piece.lastPos.x = -1;
    piece.lastPos.y = -1;
    return piece;
}


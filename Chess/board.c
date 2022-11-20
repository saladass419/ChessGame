#include "debugmalloc.h"
#include "board.h"

char**ReadBoardState(Color *color){
    FILE* file;
    file = fopen("boardstate.txt", "r");

    if (file == NULL) {
        printf("File can't be opened! \n");
        return NULL;
    }
    char*line = NULL;
    size_t len = 0;
    int db=0;
    while(getline(&line,&len,file)!=-1) db++;

    char **_boardState = (char**)malloc(8*sizeof(char*));
    for(int i = 0; i < 8; i++){
        _boardState[i] = (char*)malloc(9*sizeof(char));
    }
    if(strlen(line)==0){ //Setting the board up if there are no saved moves
        strcpy(_boardState[0],"rnbqkbnr");
        strcpy(_boardState[1],"pppppppp");
        strcpy(_boardState[2],"        ");
        strcpy(_boardState[3],"        ");
        strcpy(_boardState[4],"        ");
        strcpy(_boardState[5],"        ");
        strcpy(_boardState[6],"PPPPPPPP");
        strcpy(_boardState[7],"RNBQKBNR");
        *color = white;
    }
    else{ //Setting the board up if there are saved moves
        int i=0;
        for(int x = 0;x<8;x++){
            for(int y=0;y<8;y++){
                _boardState[x][y]=line[i++];
            }
        }
        *color = (db-1)%2;
    }
    fclose(file);
    return _boardState;
}
Position*ReadLastPosition(){
    FILE*file = fopen("lastposition.txt","r");
    if (file == NULL) {
        printf("File can't be opened! \n");
        return NULL;
    }
    char*line = NULL;
    size_t len = 0;
    Position*lastPositions = (Position*)malloc(64*sizeof(Position));
    int idx=0;
    while(getline(&line,&len,file)!=-1){
        int x,y;
        sscanf(line,"%d,%d ",&x,&y);
        lastPositions[idx].x=x;
        lastPositions[idx++].y=y;
    }
    Position nullPos;
    nullPos.x=-1;
    nullPos.y=-1;
    for (int i = idx; i < 64; i++)
    {
        SetPosEqual(&lastPositions[i],nullPos);
    }
    return lastPositions;
}
Piece *ReadDownedPieces(int *downeddb){
    Piece *pieces = (Piece*)malloc(30*sizeof(Piece));
    FILE*file = fopen("downedpieces.txt","r");

    if (file == NULL) {
        printf("File can't be opened! \n");
        return NULL;
    }
    wchar_t piece;
    while((piece = fgetwc(file))!=WEOF){
        SetPiecesEqual(&pieces[*downeddb],NullPiece());
        pieces[*downeddb].type=piece;
        if(pieces[*downeddb].type==0x2655||pieces[*downeddb].type==0x2656||pieces[*downeddb].type==0x2657||pieces[*downeddb].type==0x2658||pieces[*downeddb].type==0x2659)
            pieces[*downeddb].color = white;
        else pieces[*downeddb].color = black;
        *downeddb+=1;
    }
    for(int i  = *downeddb; i < 30; i++){
        SetPiecesEqual(&pieces[i],NullPiece());
    }
    fclose(file);
    return pieces;
}
void SaveDownedPieces(Piece*pieces, int db){
    FILE*file = fopen("downedpieces.txt","w");
    if (file == NULL) {
        printf("File can't be opened! \n");
        return;
    }
    for (int i = 0; i < db; ++i)
    {
        fputwc(pieces[i].type,file);
    }
    fclose(file);
}   
void SaveLastPosition(Tile**tiles){
    FILE*file = fopen("lastposition.txt","w");
    if (file == NULL) {
        printf("File can't be opened! \n");
        return;
    }
    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            fprintf(file,"%d,%d\n",tiles[x][y].piece.lastPos.x,tiles[x][y].piece.lastPos.y);
        }
    }
    fclose(file);
}
Tile** InitializeBoard(Tile**tiles,Piece*pieces, int db){
    FreeTiles(tiles);
    tiles = (Tile**)malloc(8*sizeof(Tile*));
    for(int i = 0; i<8;i++){
        tiles[i] = (Tile*)malloc(8*sizeof(Tile));
    }

    Piece temp;
    SetPiecesEqual(&temp,NullPiece());

    Position tempPos;
    for(int x = 0; x<8;x++){
        for(int y = 0; y < 8; y++){
            tempPos.x=x; tempPos.y=y;
            bool didFind=false;
            for(int i = 0; i<db;i++){
                if(IsPosEqual(pieces[i].position,tempPos)){ //Setting up non-free squares
                    SetPiecesEqual(&tiles[x][y].piece,pieces[i]);
                    didFind=true;
                    break;
                }
            }
            if(didFind==false)
            {
                SetPiecesEqual(&tiles[x][y].piece,temp); //Setting up free squares
            }
        }
    }
    return tiles;
}
void DrawBoard(Tile**tiles, Position selection,Position*legalMoves,int numLegalMoves,Color color,Piece* downedPieces, int downeddb){
    wprintf(L"    --- --- --- --- --- --- --- ---\n");
    if(color == white){
        Position tempPos;
        bool foundLegalMove=false;
        for(int x = 0; x < 8; x++){
            wprintf(L" %d ",(x-8)*(-1));
            for(int y = 0; y < 8; y++){
                tempPos.x=x; tempPos.y=y;
                if(IsInBound(selection.x,selection.y)){
                    if(IsPosEqual(selection,tempPos)) //Drawing the outline of a selected tile
                        wprintf(L"|(%lc)",tiles[x][y].piece.type);
                    else{
                        if(LegalMovesContains(legalMoves,numLegalMoves,tempPos)){
                            wprintf(L"|[%lc]",tiles[x][y].piece.type);   
                        }
                        else wprintf(L"| %lc ",tiles[x][y].piece.type);
                    }
                }
                else wprintf(L"| %lc ",tiles[x][y].piece.type);
            }
            wprintf(L"|\n    --- --- --- --- --- --- --- ---\n");
        }
        wprintf(L"     a   b   c   d   e   f   g   h\n");
    }
    else{
        Position tempPos;
        bool foundLegalMove=false;
        for(int x = 7; x > -1; x--){
            wprintf(L" %d ",(x-8)*(-1));
            for(int y = 7; y > -1; y--){
                tempPos.x=x; tempPos.y=y;
                if(IsInBound(selection.x,selection.y)){
                    if(IsPosEqual(selection,tempPos)) //Drawing the outline of a selected tile
                        wprintf(L"|(%lc)",tiles[x][y].piece.type);
                    else{
                        if(LegalMovesContains(legalMoves,numLegalMoves,tempPos)){
                            wprintf(L"|[%lc]",tiles[x][y].piece.type);   
                        }
                        else wprintf(L"| %lc ",tiles[x][y].piece.type);
                    }
                }
                else wprintf(L"| %lc ",tiles[x][y].piece.type);
            }
            wprintf(L"|\n    --- --- --- --- --- --- --- ---\n");
        }
        wprintf(L"     h   g   f   e   d   c   b   a\n");
    }
    for(int i = 0; i < downeddb; i++){
        if(downedPieces[i].color==white)
            wprintf(L"%lc ",downedPieces[i].type);
    }
    wprintf(L"\n");
    for(int i = 0; i < downeddb; i++){
        if(downedPieces[i].color==black)
            wprintf(L"%lc ",downedPieces[i].type);
    }
    wprintf(L"\n");
}

void SaveBoard(Tile**tiles){
    FILE* file;
    file = fopen("boardstate.txt", "a");

    if (file == NULL) {
        printf("File can't be opened! \n");
        return;
    }
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            fprintf(file, "%c",tiles[x][y].piece.read);
        }
    }
    fprintf(file,"\n");
    fclose(file);
}
void ClearSavedBoard(char*fileName){
    FILE* file;
    file = fopen(fileName, "w");
    fclose(file);
}

void FreeTiles(Tile**tiles){
    if(tiles==NULL) return;
    for(int i = 0; i<8;i++){
        if(tiles[i]!=NULL) free(tiles[i]);
    }
    free(tiles);
}

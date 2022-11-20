#include "debugmalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>
#include <stdbool.h>

#include "common.h"
#include "piece.h"
#include "legalmoves.h"
#include "board.h"

Position GetInput(char* text,Piece*selection,Tile**tiles);
bool CheckMate(Piece* pieces, int db, Position kingPos,Tile**tiles, Color color);
char**ReadPlayBackBoardState(int move);
void PlayBack();

int main(void)
{
    setlocale(LC_CTYPE, "");

<<<<<<< HEAD
    wprintf(L"New Game: '1'\nContinue previously started game: '0'\nWatch playback:'2'\nIn-game:\nSwitch selection: 'P' | Forfeit game: 'L' | Offer draw: 'S' | Quit game: 'Q'\n");
=======
    wprintf(L"New Game: '1'\nContinue previously started game: '0'\nForfeit game: 'L'\nOffer draw: 'S'\nQuit game: 'Q'\n");
>>>>>>> parent of ee8b04d (almost finished game)
    int in;
    do{
        scanf("%d",&in);
        if(in == 1) {
            ClearSavedBoard("boardstate.txt");
            ClearSavedBoard("downedpieces.txt");
            ClearSavedBoard("lastposition.txt");
        }
        else if(in==2){
            PlayBack();
            exit(0);
        }
        else if(in!=0){
            wprintf(L"Invalid input!\n");
        }
    }while(in!=0&&in!=1&&in!=2);
    Color player;

    int downeddb = 0;
    Piece *downedPieces = ReadDownedPieces(&downeddb);

    Piece* pieces = NULL;
    Tile**tiles = NULL;
    Piece*selection = NULL;
    Position*legalMoves = NULL;

    int numLegalMoves=0;
    int db = 0;
    Position input;

    bool wantsToBreak=false;
<<<<<<< HEAD
    bool wantsToChangeSelection=false;
    int repetitionCount=0;
=======
>>>>>>> parent of ee8b04d (almost finished game)

    Position nullPos;
    nullPos.x = -1;
    nullPos.y = -1;

    pieces = InitializePieces(ReadBoardState(&player),&db,ReadLastPosition());
    tiles = InitializeBoard(tiles,pieces,db);
    if(in==1) SaveBoard(tiles);
    DrawBoard(tiles,nullPos,legalMoves,numLegalMoves,player,downedPieces,downeddb); //Drawing the board
    do{
        Position kingPos = FindKing(player,pieces,db);
        if(IsInCheck(kingPos,player,pieces,db,tiles)){
            if(CheckMate(pieces,db,kingPos,tiles,player)){
                wprintf(L"%s gets checkmated!\n",GetColor(player));
                break;
            }
            else
                wprintf(L"%s is in check!\n",GetColor(player));  
        }
        else{
            if(CheckMate(pieces,db,kingPos,tiles,player)||repetitionCount>=6){
                wprintf(L"It's a stalemate!\n");
                break;
            }
        }
        wprintf(L"%s's turn!\n",GetColor(player));

        do{ //Selecting a piece
            selection = NULL;
            input = GetInput("Select a piece! ",selection,tiles);
            if(input.x==-1&&input.y==-1){ //Checking the input for forfeiting, quitting, offering draw
                wprintf(L"Game forfeited! %s lost!\n",GetColor(player));
                wantsToBreak=true;
                break;
            }else if(input.x==-2&&input.y==-2){
                wprintf(L"Game drawn!\n");
                wantsToBreak=true;
                break;
            }else if(input.x==-3&&input.y==-3){
                wantsToBreak=true;
                break;
            }
            selection = SelectPiece(pieces,db,input,player);
            if(selection==NULL) {
                wprintf(L"Selection is invalid!\n"); 
                continue;
            }
            Piece tempSelection = *selection;
            legalMoves = GetLegalMoves(selection,tiles,&numLegalMoves,pieces,db,true);
            legalMoves = CorrectLegalMoves(selection,legalMoves,tiles,pieces,&db,&numLegalMoves,NULL); //Getting the legal moves of the selected piece
            selection = &tempSelection;
            
            if(legalMoves==NULL) wprintf(L"That piece cannot move!\n");
        } while(selection==NULL||legalMoves==NULL);
        if(wantsToBreak) break;
        DrawBoard(InitializeBoard(tiles,pieces,db),selection->position,legalMoves,numLegalMoves,player,downedPieces,downeddb); //Drawing the board

        do{ //Moving a piece
            input = GetInput("Select a destination!\n",selection,tiles);
            if(!LegalMovesContains(legalMoves,numLegalMoves,input)) wprintf(L"That piece cannot move there!\n");
        } while(!LegalMovesContains(legalMoves,numLegalMoves,input));

        pieces = MoveOrCapture(pieces,&db,selection->position,input,&downedPieces[downeddb],&downeddb,true); //Moving piece and capturing if needed

        if(tolower(selection->read)!='p'){
            if(IsPosEqual(selection->lastPos,input))repetitionCount+=1;
            else repetitionCount=0;
        }

        free(legalMoves);
        numLegalMoves=0;
        selection = NULL;

        player = (player+1)%2;
        DrawBoard(InitializeBoard(tiles,pieces,db),nullPos,legalMoves,numLegalMoves,player,downedPieces,downeddb); //Drawing the board

        SaveLastPosition(tiles);
        SaveDownedPieces(downedPieces,downeddb);
        SaveBoard(tiles); //Saving the board
    } while(true);

    free(downedPieces);
    free(pieces);
    FreeTiles(tiles);
}

Position GetInput(char* text,Piece*selection,Tile**tiles){
    Position input;
    bool isCorrect;
    int number = -1;
    char character = ' ';
    int i = 0;
    do{
        wprintf(L"%s\n",text);
        scanf(" %c",&character);
        if(selection==NULL){
            if(tolower(character)=='l'){
                wprintf(L"If you want to forfeit the match, press '1'\nIf you want to cancel, press '0'\n");
                scanf(" %d",&number);
                if(number==1){
                    input.x=-1;
                    input.y=-1;
                    return input;
                }
                else if(number==0){
                    continue;
                }
                else{
                    wprintf(L"Incorrect input!\n");
                }
            }
            else if(tolower(character)=='s'){
                wprintf(L"If you want to offer a draw, press '1'\nIf you want to cancel, press '0'\n");
                scanf(" %d",&number);
                if(number==1){
                    input.x=-2;
                    input.y=-2;
                    return input;
                }
                else if(number==0){
                    continue;
                }
                else{
                    wprintf(L"Incorrect input!\n");
                }
            }
            else if(tolower(character) == 'q'){
                wprintf(L"If you want to quit, press '1'\nIf you want to cancel, press '0'\n");
                scanf(" %d",&number);
                if(number==1){
                    input.x=-3;
                    input.y=-3;
                    return input;
                }
                else if(number==0){
                    continue;
                }
                else{
                    wprintf(L"Incorrect input!\n");
                }
            }
        }
        scanf("%d",&number);

        input.y=tolower(character)-97; //Converting chess squares to coordinates
        input.x=(number-8)*-1;

        if(!IsInBound(input.x,input.y)){ //Checking if the input is outside of boundaries
            wprintf(L"Outside of table!\n");
            continue;
        }
        if(selection!=NULL&&(selection->color==tiles[input.x][input.y].piece.color)) { //Checking if the piece can move to the desired destination
            isCorrect = false;
            wprintf(L"Incorrent tile!\n");
        }
        else isCorrect = true;
    }while(!IsInBound(input.x,input.y)||!isCorrect); //Keeping the pieces within the broders
    return input;
}
bool CheckMate(Piece* pieces, int db, Position kingPos,Tile**tiles, Color color){
    Position*legalMoves=NULL;
    int numLegalMoves=0;
    int totalCount = 0;
    for (int i = 0; i < db; ++i)
    {
        if(pieces[i].color==color){
            legalMoves = GetLegalMoves(&pieces[i],tiles,&numLegalMoves,pieces,db,true);
            legalMoves = CorrectLegalMoves(&pieces[i],legalMoves,tiles,pieces,&db,&numLegalMoves,NULL); //Checking for each piece if it has any legal moves
            totalCount+=numLegalMoves;
            free(legalMoves);
        }
    }
    if(totalCount==0) return true;
    return false;
}
char**ReadPlayBackBoardState(int move){
    FILE*file=fopen("boardstate.txt","r");
    if (file == NULL) {
        printf("File can't be opened! \n");
        return NULL;
    }
    char*line = NULL;
    size_t len = 0;

    for (int i = 0; i <= move; i++)
    {
        getline(&line,&len,file);
    }
    
    char **_boardState = (char**)malloc(8*sizeof(char*));
    for(int i = 0; i < 8; i++){
        _boardState[i] = (char*)malloc(9*sizeof(char));
    }

    int i=0;
    for(int x = 0;x<8;x++){
        for(int y=0;y<8;y++){
            _boardState[x][y]=line[i++];
        }
    }
    fclose(file);
    return _boardState;
}
void PlayBack(){
    int move = 0;

    FILE*file = fopen("boardstate.txt","r");
    int max=-1;
    char*line = NULL;
    size_t len = 0;
    while(getline(&line,&len,file)!=-1) {
        max++; //Counting how many moves are there in the save file
    }
    fclose(file);

    if(max==0){
        wprintf(L"There is no previously saved game!\n");
        return;
    }

    char**boardState = ReadPlayBackBoardState(move);

    Color player = white;

    Piece* pieces = NULL;
    Tile**tiles = NULL;
    int db = 0;

    Position nullPos;
    nullPos.x=-1;
    nullPos.y=-1;

    char input;
    pieces = InitializePieces(boardState,&db,ReadLastPosition());
    tiles = InitializeBoard(tiles,pieces,db);

    DrawBoard(tiles,nullPos,NULL,0,player,NULL,0); //Drawing the board

    wprintf(L"To move forward: 'E'\nTo move backward: 'Q'\nTo flip view: 'R'\nTo exit: 'W'\n");
    do{
        scanf(" %c",&input);
        if(tolower(input)=='e') {
            if(move<max)
                move+=1;
            else wprintf(L"There is no more moves in that direction!\n");
        }
        else if(tolower(input)=='q') {
            if(move>0)
                move-=1;
            else wprintf(L"There is no more moves in that direction!\n");
        }
        else if(tolower(input)=='r') {
            player = (player+1)%2;
        }
        else if(tolower(input)=='w') break;
        else {
            wprintf(L"Invalid input!\n");
            continue;
        }
        wprintf(L"Current move: %d / Max moves: %d\n",move,max);
        free(pieces);
        db=0;
        pieces = InitializePieces(ReadPlayBackBoardState(move),&db,ReadLastPosition());
        tiles = InitializeBoard(tiles,pieces,db);
        DrawBoard(tiles,nullPos,NULL,0,player,NULL,0); //Drawing the board
    }while(tolower(input)!='w');
    free(pieces);
    FreeTiles(tiles);
    return;
}

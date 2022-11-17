#include "debugmalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>

#include "common.h"
#include "piece.h"
#include "legalmoves.h"
#include "board.h"

Position GetInput(char* text,Piece*selection,Tile**tiles);
bool CheckMate(Piece* pieces, int db, Position kingPos,Tile**tiles, Color color);

int main(void)
{
    setlocale(LC_CTYPE, "");

    wprintf(L"New Game: '1'\nContinue previously started game: '0'\nForfeit game: 'F'\nOffer draw: 'D'\nQuit game: 'Q'\n");
    int in;
    scanf("%d",&in);
    if(in == 1) {
        ClearSavedBoard("boardstate.txt");
        ClearSavedBoard("downedpieces.txt");
        ClearSavedBoard("lastposition.txt");
    }
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

    Position nullPos;
    nullPos.x = -1;
    nullPos.y = -1;

    pieces = InitializePieces(ReadBoardState(&player),&db,ReadLastPosition());
    tiles = InitializeBoard(tiles,pieces,db);
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
            if(CheckMate(pieces,db,kingPos,tiles,player)){
                wprintf(L"It's a stalemate!\n",GetColor(player));
                break;
            }
        }
        wprintf(L"%s's turn!\n",GetColor(player));

        do{ //Selecting a piece
            selection = NULL;
            input = GetInput("Select a piece! ",selection,tiles);
            if(input.x==-1&&input.y==-1){
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
            if(tolower(character)=='f'){
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
            else if(tolower(character)=='d'){
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

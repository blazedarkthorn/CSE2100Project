// Tic-tac-toe (T3) Network Game
// Jason Losh

//-----------------------------------------------------------------------------
// Compile notes for C99 code
//-----------------------------------------------------------------------------

// gcc -o t3 t3.c udp.c

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdlib.h>          // EXIT_ codes
#include <stdbool.h>         // bool
#include <stdio.h>           // printf, scanf
#include <string.h>          // strlen, strcmp

#include "udp.h"

#define CLIENT_PORT 1111
#define SERVER_PORT 1112

void clearBoard(char board[3][3])
{
    for (int r = 0; r < 3; r++){
        for (int c = 0; c < 3; c++){
            board[r][c] = '.';}
    }
}

// TODO: Add code to display the game board
void showBoard(char board[3][3])
{
    for (int r = 0; r < 3; r++){
        for (int c = 0; c < 3; c++){
            if(c==0){printf("\n");}
            printf("%c",board[r][c]);}
    }
    printf("\n");
}

// TODO: Add code to determine if a winner (xo) has occurred
bool isWinner(char board[3][3], char xo)
{   
    bool win = false;

    if((board[0][0]==board[0][1]&&board[0][1]==board[0][2])){
        if(board[0][0]== xo){printf( "\n%c wins!\n",xo); showBoard(board); return true;}
    }
    if((board[1][0]==board[1][1]&&board[1][1]==board[1][2])){
        if(board[1][0]== xo){printf( "\n%c wins!\n",xo); showBoard(board); return true;}
    }
    if((board[2][0]==board[2][1]&&board[2][1]==board[2][2])){
        if(board[2][0]== xo){printf( "\n%c wins!\n",xo); showBoard(board); return true;}
    }
    if((board[0][0]==board[1][0]&&board[1][0]==board[2][0])){
        if(board[0][0]== xo){printf( "\n%c wins!\n",xo); showBoard(board); return true;}
    }
    if((board[0][1]==board[1][1]&&board[1][1]==board[2][1])){
        if(board[0][1]== xo){printf( "\n%c wins!\n",xo); showBoard(board); return true;}
    }
    if((board[0][2]==board[1][2]&&board[1][2]==board[2][2])){
        if(board[0][2]== xo){printf( "\n%c wins!\n",xo); showBoard(board); return true;}
    }
    if((board[0][0]==board[1][1]&&board[1][1]==board[2][2])){
        if(board[0][0]== xo){printf( "\n%c wins!\n",xo); showBoard(board); return true;}
    }
    if((board[0][2]==board[1][1]&&board[0][1]==board[2][0])){
        if(board[0][2]== xo){printf( "\n%c wins!\n",xo); showBoard(board); return true;}
    }

    return win;
}

// TODO: Add code to test if an x or o (xo) is a valid move
//       and then record in the game board
bool addMove(char board[3][3], char move[], char xo)
{
    bool ok = (strlen(move) == 2);
    if(strcmp(move,"A1") == 0 && board[0][0]=='.'){
        board[0][0]= xo;
        ok = true;
    }
    else if(strcmp(move,"A2") == 0 && board[0][1]=='.'){
        board[0][1]= xo;
        ok = true;
    }
    else if(strcmp(move,"A3") == 0 && board[0][2]=='.'){
        board[0][2]= xo;
        ok = true;
    }
    else if(strcmp(move,"B1") == 0 && board[1][0]=='.'){
        board[1][0]= xo;
        ok = true;
    }
    else if(strcmp(move,"B2") == 0 && board[1][1]=='.'){
        board[1][1]= xo;
        ok = true;
    }
    else if(strcmp(move,"B3") == 0 && board[1][2]=='.'){
        board[1][2]= xo;
        ok = true;
    }
    else if(strcmp(move,"C1") == 0 && board[2][0]=='.'){
        board[2][0]= xo;
        ok = true;
    }
    else if(strcmp(move,"C2") == 0 && board[2][1]=='.'){
        board[2][1]= xo;
        ok = true;

    }
    else if(strcmp(move,"C3") == 0 && board[2][2]=='.'){
        board[2][2]= xo;
        ok = true;
    }
    else {
        printf("Please insert a valid move");
        return false;
    }
    return ok;
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    char *remoteIp;
    char *role;
    char *test;
    char str[100], str2[100];
    char board[3][3];
    bool validMove;
    bool winner = false;
    int moveCount = 0;
    char move[2];
    bool trigger = false;
    
    bool myTurn;
    char myLetter, opponentLetter;
    int remotePort;

    // TODO: Verify arguments are valie
    bool goodArguments;
    if(argc == 3){
        remoteIp = argv[1];
        if((strcmp(argv[2],"invite")==0)||(strcmp(argv[2],"accept")==0)){
            role = argv[2];
            goodArguments=true;
        }
    }
    if (!goodArguments)
    {
        printf("usage: t3 IPV4_ADDRESS ROLE\n");
        printf("  where:\n");
        printf("  IPV4_ADDRESS is address of the remote machine\n");
        printf("  ROLE is either invite or accept\n");
        exit(EXIT_FAILURE);
    }
    // TODO: Determine if client or server
    // A server will wait to accept an invitation to play
    // A client will send an invitation to play
    bool client = false;
    if(strcmp(role, "invite")==0){
        client = true;
        remotePort=SERVER_PORT;
        openListenerPort(remoteIp,CLIENT_PORT);
    }
    else if(strcmp(role, "accept")==0){
        remotePort=CLIENT_PORT;
        openListenerPort(remoteIp,SERVER_PORT);
    }
    //openListenerPort(remoteIp,remotePort);
    printf("%d\n",remotePort);
    printf("%s\n",remoteIp);
    
    // TODO: Open listener port number dependent on client/server role
    //printf("%d\n",openListenerPort(remoteIp,remotePort));
    
    // TODO: Determine remote port that you will send data to
    //       If you are server, send to client port, and vice versa
    if(client == true){
        sendData(remoteIp,remotePort,"invite");
        printf("sent\n");
    }
    else if(client == false){
        printf("waiting\n"); 
        receiveData(str, 10);
        if(strcmp(str, "invite") == 0){
            printf("Invitation accepted\n");
        }
        else
            return 0;
    }
    
    // Setup game
    clearBoard(board);

    // TODO: Determine whether it is your turn or not
    myTurn=true;
    if(client== true){
        myTurn=false;
    }
    // TODO: Determine your letter (x or o) and your opponent's letter
    if(client== true){
        myLetter='x';
        opponentLetter='o';
    }
    else
    {
        myLetter='o';
        opponentLetter='x';
    }
    
    // TODO: Add code to send an invite or wait to accept an invite
    
    // Start game loop, alternating turns
    while(winner == false && moveCount != 9)
    {
        // get my move
        if (myTurn==true)
        {
            // TODO: add code your move here to get the move, validate move,
            //       show board, send move to remote port, and check for a winner
            printf("\nYour Turn!\n");
            showBoard(board);
            while(validMove==false){
            gets(move);
            validMove = addMove(board,move,myLetter);
            }
            validMove=false;
            myTurn=false;
            sendData(remoteIp,remotePort,move);
            winner=isWinner(board,opponentLetter);
            if(winner == true){
                closeListenerPort();
                return 0;
            }
            winner=isWinner(board,myLetter);
            if(winner == true){
                closeListenerPort();
                return 0;
            }
        }
        // get opponent's move
        if (myTurn==false)
        {
            receiveData(move,2);
            printf("\nOpponent Moved\n");
            // TODO: add code to receive your opponent's move, validate move,
            //       show board, sizeof move, stdin);, and check for a winner
            showBoard(board);
            addMove(board,move,opponentLetter);
            myTurn=true;
            winner=isWinner(board,opponentLetter);
            if(winner == true){
                closeListenerPort();
                return 0;
            }
            winner=isWinner(board,myLetter);
            if(winner == true){
                closeListenerPort();
                return 0;
            }
        }
        moveCount++;

        
    }
    if (winner == false){
        printf("The game was a draw\n");}

    // TO DO: Close listener port
    closeListenerPort();
    return EXIT_SUCCESS;
}

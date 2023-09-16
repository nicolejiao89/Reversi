/**
 * @file reversi.c
 * @author <-- Nicole Jiao -->
 * @brief This file is used for APS105 Lab 8. 2023W version
 * @date 2023-03-14
 * 
 */

#include <stdio.h>
#include <stdbool.h>

// DO NOT REMOVE THE FOLLOWING LINE
#if !defined(TESTER_P1) && !defined(TESTER_P2)
// DO NOT REMOVE THE ABOVE LINE

// DO NOT REMOVE THE FOLLOWING LINE
#endif
// DO NOT REMOVE THE ABOVE LINE

void printBoard(char board[][26], int n) {
    // prints board with row and column labels
    
    for (int row = -1; row < n; row++){
        for (int col = -2; col < n; col++){
            // prints blank at top left corner and second column
            if ((row == -1 && col == -2) || col == -1){
                printf(" ");
            }
            // prints characters on first row
            else if (row == -1){
                printf("%c", col + 'a');
            }
            // prints characters on first col
            else if (col == -2){
                printf("%c", row + 'a');
            }
            // prints board
            else {
                printf("%c", board[row][col]);
            }
        }
        printf("\n");
    }
}

bool positionInBounds(int n, int row, int col) {
    // returns true if (row, col) is within the board

    return (row >= 0 && row < n && col >= 0 && col < n);
}

bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol) {
    // returns true if, in the specified direction, there is a line of tile(s) of the
    // opposite colour and a tile of the initial colour immediately following
    // note: does not check initial tile 

    // increment coordinate in specified direction
    row += deltaRow;
    col += deltaCol; 

    bool inBounds = positionInBounds(n, row, col);

    // initial tile is black
    if (colour == 'B'){  
        // next tile is out of bounds, unoccupied or black 
        if (!inBounds || !(board[row][col] == 'W')){
            return false;
        }   

        // loop through line of white tiles 
        while (inBounds && board[row][col] == 'W'){
            row += deltaRow;
            col += deltaCol; 
            inBounds = positionInBounds(n, row, col);
        }

        // no black tile was found following the line of white tiles
        // return false
        if (!inBounds || board[row][col] == 'U'){
            return false;
        }

        // black tile was found
        // return true
        return true;
    }

    // initial tile is white
    else {  

        // next tile is out of bounds, unoccupied or white 
        if (!inBounds || !(board[row][col] == 'B')){
            return false;
        }

        // next tile is black
        // loop through line of black tiles 
        while (inBounds && board[row][col] == 'B'){
            row += deltaRow;
            col += deltaCol; 
            inBounds = positionInBounds(n, row, col);
        }

        // no white tile was found following the line of black tiles 
        // return false
        if (!inBounds || board[row][col] == 'U'){
            return false;
        }

        // white tile was found
        // return true
        return true;
    }
}

bool validMove(char board[][26], int n, int row, int col, char colour){
    // returns true if (row, col) is an available move for specified colour
    bool available = false;

    // user input coordinate is out of bounds
    // user input colour is not W and B
    // initial tile is occupied
    // return false
    if (!positionInBounds(n, row, col) || (colour != 'W' && colour != 'B') || board[row][col] != 'U'){
        return available;
    }

    // loops through eight directions
    for (int deltaRow = -1; deltaRow <= 1 && !available; deltaRow++){
        for (int deltaCol = -1; deltaCol <= 1 && !available; deltaCol++){
            if (!(deltaRow == 0 && deltaCol == 0)){
                available = checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol);
            }
        }
    }
    return available;
}

bool AvailableMoves(char board[][26], int n, char colour){
    // returns true if there is an available move for specified colour
    // on the entire board

    for (int row = 0; row < n; row++){
        for (int col = 0; col < n; col++){
            if (validMove(board, n, row, col, colour)){
                return true;
            }
        }
    }
    return false;
}

void flipTiles(char board[][26], int n, int row, int col, char colour){
    // flips the middle tiles and adds the user selected tile
    // note: assume (row, col) is a valid move for specified colour

    // save initial coordinate
    int ROW = row;
    int COL = col;

    // add user selected tile
    board[row][col] = colour;
    
    // flips middle tiles
    for (int deltaRow = -1; deltaRow <= 1; deltaRow++){
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++){
            if (!(deltaRow == 0 && deltaCol == 0)){
                
                // reset row and col
                row = ROW;
                col = COL;
                
                if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)){
                    row += deltaRow;
                    col += deltaCol;
                    
                    if (colour == 'W'){
                        while (board[row][col] == 'B'){
                            board[row][col] = 'W';
                            row += deltaRow;
                            col += deltaCol;
                        }
                    }

                    else {
                        while (board[row][col] == 'W'){
                            board[row][col] = 'B';
                            row += deltaRow;
                            col += deltaCol;
                        }
                    }
                }
            }
        }
    }
}

int getScore(char board[][26], int n, int row, int col, char colour){
    // calculate the total number of tiles of the opposite colour 
    // that will be flipped at the given coordinate

    int score = 0;

    int ROW = row;
    int COL = col;

    // loops through eight directions
    for (int deltaRow = -1; deltaRow <= 1; deltaRow++){
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++){
            
            if (!(deltaRow == 0 && deltaCol == 0)){

                // reset coordinates
                row = ROW;
                col = COL;

                if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)){
                    row += deltaRow;
                    col += deltaCol;

                    while (board[row][col] != colour){
                        score++;
                        row += deltaRow;
                        col += deltaCol;
                    }
                }
            }
        }
    }
    return score;
}

int makeMove(char board[][26], int n, char turn, int *row, int *col){
    // computes the best move for the specified colour 
    // returns the location thorugh pointer variables

    int score = 0;
    int max_score = 0;

    int weight[8][8] = {
        {10, 3, 5, 5, 5, 5, 3, 10},
        {3, -1, 1, 1, 1, 1, -1, 3},
        {5, 1, 1, 1, 1, 1, 1, 5},
        {5, 1, 1, 1, 1, 1, 1, 5},
        {5, 1, 1, 1, 1, 1, 1, 5},
        {5, 1, 1, 1, 1, 1, 1, 5},
        {3, -1, 1, 1, 1, 1, -1, 3},
        {10, 3, 5, 5, 5, 5, 3, 10},
    };

    // loop through each tile on the board
    for (int row_int = 0; row_int < n; row_int++){
        for (int col_int = 0; col_int < n; col_int++){
            if (validMove(board, n, row_int, col_int, turn)){
                
                score = weight[row_int][col_int] * getScore(board, n, row_int, col_int, turn);

                // update maximum score found
                if (score > max_score){
                    max_score = score;
                    *row = row_int;
                    *col = col_int;
                }
            }
        }
    }
    return 0;
}

void checkWin(char board[][26], int n){
    // check which player won the game or if there is a draw
    // print the results

    int scoreW = 0, scoreB = 0;

    for (int row = 0; row < n; row++){
        for (int col = 0; col < n; col++){
            if (board[row][col] == 'W'){
                scoreW++;
            }
            else if (board[row][col] == 'B'){
                scoreB++;
            }
        }
    }

    if (scoreW > scoreB){
        printf("W player wins.");
    }
    else if (scoreB > scoreW){
        printf("B player wins.");
    }
    else{
        printf("Draw!");
    }
}

//*******************************************************
// DO NOT REMOVE THE FOLLOWING LINE
#ifndef TESTER_P2
// DO NOT REMOVE THE ABOVE LINE

int main(void) {
    char compColour, userColour;
    char userRow, userCol;
    int compRow, compCol;
    bool compTurn;
    bool gameEnd = false;

    // get board dimensions
    int n = 0;
    printf("Enter the board dimension: ");
    scanf("%d", &n);

    // generate a blank board
    char board[26][26];
    for (int row = 0; row < 26; row++) {
        for (int col = 0; col < 26; col++) {
            if(row == n/2 - 1 && col == n/2 - 1){
                board[row][col] = 'W';
            }
            else if(row == n/2 && col == n/2){
                board[row][col] = 'W';
            }
            else if(row == n/2 - 1 && col == n/2){
                board[row][col] = 'B';
            }
            else if(row == n/2 && col == n/2 - 1){
                board[row][col] = 'B';
            }
            else{
                board[row][col] = 'U';
            }
        }
    }

    // ask user if computer is B or W
    printf("Computer plays (B/W): ");
    scanf(" %c", &compColour);

    // determine user userColour and compTurn
    if (compColour == 'B'){
        userColour = 'W';
        compTurn = true;
    }
    else{
        userColour = 'B';
        compTurn = false;
    }

    // print initialized board
    printBoard(board, n);

    // loop until game ends
    while (!gameEnd){

        // computer's turn 
        if(compTurn){
            // computer makes move if available
            if (AvailableMoves(board, n, compColour)){
                makeMove(board, n, compColour, &compRow, &compCol);
                flipTiles(board, n, compRow, compCol, compColour);
                printf("Computer places %c at %c%c.\n", compColour, compRow + 'a', compCol + 'a');
                printBoard(board, n);
            }
            else{
                printf("%c player has no valid move.\n", compColour);
            }
            // flip to user's turn 
            compTurn = false;
        } 

        // user's turn 
        else if (!compTurn){
            // user makes move if available
            if (AvailableMoves(board, n, userColour)){
                printf("Enter move for colour %c (RowCol): ", userColour);
                scanf(" %c %c", &userRow, &userCol);

                // check move validity
                if (validMove(board, n, userRow - 'a', userCol - 'a', userColour)){
                    flipTiles(board, n, userRow - 'a', userCol - 'a', userColour); 
                    printBoard(board, n);
                }
                else{
                    printf("Invalid move.\n");
                    checkWin(board, n);
                    gameEnd = true;
                }
            }
            else{
                printf("%c player has no valid move.\n", userColour);
            }
            // flip to computer's turn 
            compTurn = true;
        }

        // games ends when no available moves for computer and user 
        if (!AvailableMoves(board, n, compColour) && !AvailableMoves(board, n, userColour)){
            checkWin(board, n);
            gameEnd = true;
        }
    }
    return 0;
}

// DO NOT REMOVE THE FOLLOWING LINE
#endif
// DO NOT REMOVE THE ABOVE LINE
//*******************************************************

//
//  minimaxV2.cpp
//
//
//  Created by Andrew Wang on 2019-11-20.
//

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include <time.h>
#include <stdlib.h>
using namespace std;

class Node {
public:
    //ATTRIBUTES//
    
    int boardState[8][8];
    int place[2];
    int depth;
    int maxDepth;
    float value;
    bool isMax;
    int childIndex;
    vector <Node> children;
    
    //CONSTRUCTOR//
    
    Node(int a, int b, int state[8][8]) {
        maxDepth = a;
        depth = b;
        childIndex = 0;
        place[0] = 0;
        place[1] = 0;
        
        if (depth % 2 == 0) {//if the tree depth is even, it's the player's turn
            isMax = 0;
        } else { //otherwise it's the computer's turn
            isMax = 1;
        }
        
        for (int i = 0; i < 8; i++) { //Idk if we should use memcpy
            for (int j = 0; j < 8; j++) {
                boardState[i][j] = state[i][j];
            }
        }
        
    }
    
    //METHODS//
    
    float evaluate() { //Dummy eval function
        float val =rand() % 100; // note the smaller the range of numbers, the faster
        return val;
    }
    
    void getBoard(int** board) { //dummy getBoard function
        //NICK'S CODE
        
        //END OF NICK'S CODE
        /*int board[8][8]={
        {0,2,0,2,0,2,0,2},//red side of the board initially
        {2,0,2,0,2,0,2,0},
        {0,2,0,2,0,2,0,2},
        {0,0,0,0,0,0,0,0},//middle of the board is initially empty
        {0,0,0,0,0,0,0,0},
        {1,0,1,0,1,0,1,0},//white side of the board initially
        {0,1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1,0}};*/
        int newBoard[8][8];
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                newBoard[i][j] = board[i][j];
            }
        }
        
        Node *child;
        child = new Node (maxDepth,depth+1,newBoard);
        children.push_back(*child);
        delete child;
    }
    
    //NICK'S CODE//
    int validMove(int *action, int currentstate[8][8], int turn ){//a version of checkmove used in the minimax that doesn't move any pieces
        int cr, cc, mr, mc;//current row, column and move row, column
        int columnDirection=0, rowDirection=0;

        cc=action[0];//current row&column is the first entry from player
        cr=action[1];

        mc=action[2];//move row&column is the second entry from player
        mr=action[3];
        
        if(mr>7 || mr<0){
            return -1;
        }
        
        if(mc>7 || mc<0){
            return -1;
        }
        
        //removed the check of choosing an empty square
        //cout << "Got here\n";
        //cout << cr <<" "<<cc <<" "<< mr << " " << mc << " " << currentstate[mr][mc] << "\n";
        if(currentstate[mr][mc]!=0){
            return -1;//this -1 return value indicates that the selected move spot is already occupied and thus the move is not valid
        }
        if(currentstate[cr][cc]==1){//if the spot selected to move has a white piece
            if(turn%2==0){//only player one can move odd numbered checkers, thus the move number must be even since p1 goes first
                if(mr>=cr){//white checkers can only move upwards
                    return -1;//they need to enter another move
                }
                if(mr==cr-1){//if the player demands to move their white checker to the row directly above
                    if(mc==cc+1 || mc==cc-1){
                        //removed the auto king
                        return 1;//this move is thus valid because the right player is moving the right piece to an empty index in the right direction
                    }
                }
                if(mr==cr-2){//if the player demands to move their checker two rows up (taking something)
                    columnDirection=mc-cc;//the column direction of the move is determined via the difference in mc and cc
                    if(columnDirection>0){
                        columnDirection=1;
                    }
                    else {
                        columnDirection=-1;
                    }
                    if(currentstate[cr-1][cc+columnDirection]==2 || currentstate[cr-1][cc+columnDirection]==4){//if the diagonally adjacent spot contains an opposite piece or king
                        //took out the automatic piece removal
                        return 1;//the move is valid, and the piece has been removed thus return 1 and move the piece in main
                    }
                }
            }
            else{
                return -1; //-1 return value indicated an illegal inital selection, in this case the player didnt choose their own piece
            }
        }
        if(currentstate[cr][cc]==2){//if the selected spot contains a red checker
            if(turn%2==1){//only player two can move even numbered checkers, thus the move count must be odd since they go second
                if(mr<=cr){//red checkers can only move downwards
                    return -1;
                }
                if(mr==cr+1){//if the player demands to move their red piece down to the next row
                    if(mc==cc+1 || mc==cc-1){
                        //removed the auto king
                        return 1;//this 3x3 move is thus valid for same reasons as mentioned above
                    }
                }
                if(mr==cr+2){//if the player demands to move their checker two rows down (taking something)
                    columnDirection=mc-cc;//the column direction of the move is determined via the difference in mc and cc

                    if(columnDirection>0){
                        columnDirection=1;
                    }
                    else{
                        columnDirection=-1;
                    }

                    if(currentstate[cr+1][cc+columnDirection]==1 || currentstate[cr+1][cc+columnDirection]==3){//if the diagonally adjacent spot contains an opposite piece or king
                        //removed the auto piece removal
                        return 1;//the move is valid, and the piece has been removed thus return 1 and move the piece in main
                    }
                }
            }
            else {
                return -1;//-1 return value indicated an illegal inital selection, in this case the player didnt choose their own piece
            }
        }
        else if(currentstate[cr][cc]==3){//if a white king is selected for movement
            if(turn%2==0){//the turn must match white player
                if(abs(mc-cc)==1 && abs(mr-cr)==1){//if the king is trying to move in 3x3 allow
                    return 1;
                }
                else if(abs(mc-cc)==2 && abs(mr-cr)==2){//if a 5x5 jump move is requested
                    columnDirection=mc-cc;//determine the move direction as positive or negative
                        if(columnDirection>0){
                            columnDirection=1;
                        }
                        else{
                            columnDirection=-1;
                        }

                    rowDirection=mr-cr;
                        if(rowDirection>0){
                            rowDirection=1;
                        }
                        else{
                            rowDirection=-1;
                        }
                    if(currentstate[cr+rowDirection][cc+columnDirection]==2 || currentstate[cr+rowDirection][cc+columnDirection]==4){
                    //if the space being hopped over belongs to the opponent

                        //removed the auto piece removal

                        return 1;
                    }
                }
            }
        }
        else if(currentstate[cr][cc]==4){//if a red king is selected for movement
            if(turn%2==1){//the turn must match red player
                if(abs(mc-cc)==1 && abs(mr-cr)==1){//if the king is trying to move in 3x3 allow
                    return 1;
                }
                else if(abs(mc-cc)==2 && abs(mr-cr)==2){//if a 5x5 jump move is requested
                    columnDirection=mc-cc;//determine the move direction as positive or negative
                        if(columnDirection>0){
                            columnDirection=1;
                        }
                        else{
                            columnDirection=-1;
                        }

                    rowDirection=mr-cr;
                        if(rowDirection>0){
                            rowDirection=1;
                        }
                        else{
                            rowDirection=-1;
                        }

                    if(currentstate[cr+rowDirection][cc+columnDirection]==1 || currentstate[cr+rowDirection][cc+columnDirection]==3){
                        //if the space hopped over belongs to the opponent

                        //removed the auto piece removal

                        return 1;
                    }
                }
            }
        }
    return -1;
    }
    
    
    void removePiece(int **arr,int cc,int cr){
    arr[cr][cc]=0;
    }
    
    void movePiece(int **arr, int cc, int cr, int nc, int nr){ //gets board, current row, current column, new row, new column indices
        cout << "\nBefore\n";
        for (int i = 0; i < 8; i++) { //Idk if we should use memcpy
            for (int j = 0; j < 8; j++) {
                cout << arr[i][j] << " ";
            }
            cout << "\n";
        }
    int piece=arr[cr][cc];
    arr[cr][cc]=0;
    arr[nr][nc]=piece;
        cout << "\nAfter\n";
        for (int i = 0; i < 8; i++) { //Idk if we should use memcpy
            for (int j = 0; j < 8; j++) {
                cout << arr[i][j] << " ";
            }
            cout << "\n";
        }
        
    }


    
    void newBoard( int *move, int **newState, int turn){

        int cc=move[0];//current row&column is the first entry from computer
        int cr=move[1];

        int mc=move[2];//move row&column is the second entry from computer
        int mr=move[3];
        
        cout << "Moving " << cc << " "<<cr << " to " << mc << " " << mr << "\n";
        
        /*cout << "Before movePiece\n";
        for (int i = 0; i < 8; i++) { //Idk if we should use memcpy
            for (int j = 0; j < 8; j++) {
                cout << newState[i][j] << " ";
            }
            cout << "\n";
        }*/

        int columnDirection =0, rowDirection=0;
        if(mr==cr+1){//if the approved move wants to move into a king postion
            if(mc==cc+1 || mc==cc-1){

                if(mr==7){
                    newState[mc][mr]=4;//king that spot
                }
                removePiece(newState,cc,cr);//remove the pawn
            }
        }
        else if(mr==cr-1){//if the approved move wants to move into a king postion
            if(mc==cc+1 || mc==cc-1){
                if(mr==0){
                    newState[mc][mr]=3;//king that spot
                }
                removePiece(newState,cc,cr);//remove the pawn
            }
        }
        else{//move the piece normally
            if(abs(mc-cc)==2 && abs(mr-cr)==2){//if a 5x5 jump move is requested
                    columnDirection=mc-cc;//determine the move direction as positive or negative
                        if(columnDirection>0){
                            columnDirection=1;
                        }
                        else{
                            columnDirection=-1;
                        }
                    rowDirection=mr-cr;
                        if(rowDirection>0){
                            rowDirection=1;
                        }
                        else{
                            rowDirection=-1;
                        }
                    if(newState[cr+rowDirection][cc+columnDirection]==1 || newState[cr+rowDirection][cc+columnDirection]==3){
                        //if the space hopped over belongs to the opponent
                        removePiece(newState, cc+columnDirection, cr+rowDirection);//removes that piece
                        //the attacking piece is actually moved below
                    }
                    else if(newState[cr+rowDirection][cc+columnDirection]==2 || newState[cr+rowDirection][cc+columnDirection]==4){
                        //if the space hopped over belongs to the opponent
                        removePiece(newState, cc+columnDirection, cr+rowDirection);//removes that piece
                        //the attacking piece is actually moved below
                    }
                movePiece(newState, cc, cr, mc, mr);//moves the piece, this move is already valid,otherwise the funciton would not have run
                }
            
        }
         
        /*cout << "After movePiece\n";
        for (int i = 0; i < 8; i++) { //Idk if we should use memcpy
            for (int j = 0; j < 8; j++) {
                cout << newState[i][j] << " ";
            }
            cout << "\n";
        }*/

    }
    
    void findNext(int *position, int currentState[8][8], int turn){
      int i=0, j=0, finishing=1, cut=0;

      if(position[0]>=7 && position[1]>=7){//if the position is currently on the very last piece of the board, do nothing at all in the projection state
          return;
      }
      for(i=position[0];i<8 && cut!=1;i++){
        if(finishing==1){//you must continue searching the current row the first time through
            if(position[0]==0 && position[1]==0){//if its starting from the very first spot dont increment j before starting
                for(j=position[1];j<8 && cut!=1; j++){
                    if(currentState[i][j]!=0){//finds a non empty square
                        if(currentState[i][j]==1 || currentState[i][j]==3){//it belongs to the player
                            cout << currentState[i][j] << " Player's piece\n";
                            if(turn==0){

                                position[0]=i;
                                position[1]=j;
                                cut=1;
                                break;
                            }
                        }
                        else if(currentState[i][j]==2 || currentState[i][j]==4){
                            cout << currentState[i][j] << " CPU's piece\n";
                            if(turn==1){

                                position[0]=i;
                                position[1]=j;
                                cut=1;
                                break;
                            }
                        }
                    }
                }
            } else {//if the spot isnt 0,0 increment j to prevent calling on the same piece as before
                for(j=position[1]+1;j<8 && cut!=1; j++){
                    if(currentState[i][j]!=0){//finds a non empty square
                        if(currentState[i][j]==1 || currentState[i][j]==3){//it belongs to the player
                            if(turn==0){
                                position[0]=i;
                                position[1]=j;
                                cut=1;
                                break;
                            }
                        }
                        else if(currentState[i][j]==2 || currentState[i][j]==4){
                            if(turn==1){
                                position[0]=i;
                                position[1]=j;
                                cut=1;
                                break;
                            }
                        }
                    }
                }
            }
        } else {//it is now on a completely new row from the one that it started on so index j from 0
            for(j=0;j<8 && cut!=1;j++){
                if(currentState[i][j]!=0){//finds a non empty square
                    if(currentState[i][j]==1 || currentState[i][j]==3){//it belongs to the player
                        if(turn==0){
                            position[0]=i;
                            position[1]=j;
                            cut=1;
                            break;
                        }
                    }
                    else if(currentState[i][j]==2 || currentState[i][j]==4){
                        if(turn==1){
                            position[0]=i;
                            position[1]=j;
                            cut=1;
                            break;
                        }
                    }
                }
            }
        }
        finishing=0;//after one iteration of the column check loop, it must have finished the current column so never begin j from 0;
      }
    }
    
    void project(int *position, int currentState[8][8], int turn) {
        //position is the "place" current state of the parent board
        
        int i=0, j=0, births = 0;
        
        //now a new piece has been found and its moves can be projected
        
        int *action= new int[4];//sets the initial piece position to the first two spots for action pointer
        action[0]=position[0];
        action[1]=position[1];
        //cout <<"Got here\n";
        /*
        0 - move up right
        1- move up left
        2 - move down right
        3 - move down left
        4 - jump up right
        5 - jump up left
        6 - jump down right
        7 - jump down left
        8 - same spot
        */
        
        while (births == 0) {
            findNext(position, currentState, turn);
            action[0]=position[0];
            action[1]=position[1];
            
            if(currentState[position[0]][position[1]]!=0){
                for(i=0;i<8;i++){//tests each of the 9 possible moves
                    switch(i){
                    case 0: action[2]=position[0]-1;
                            action[3]=position[1]+1;
                            break;
                    case 1: action[2]=position[0]-1;
                            action[3]=position[1]-1;
                            break;
                    case 2: action[2]=position[0]+1;
                            action[3]=position[1]+1;
                            break;
                    case 3: action[2]=position[0]+1;
                            action[3]=position[1]-1;
                            break;
                    case 4: action[2]=position[0]-2;
                            action[3]=position[1]+2;
                            break;
                    case 5: action[2]=position[0]-2;
                            action[3]=position[1]-2;
                            break;
                    case 6: action[2]=position[0]+2;
                            action[3]=position[1]+2;
                            break;
                    case 7: action[2]=position[0]+2;
                            action[3]=position[1]-2;
                            break;
                    /*case 8: action[2]=position[0];//check the check move function for actions regarding this move
                            action[3]=position[1];
                            break;*/
                    }
                    //cout <<"Got here\n";
                    if(validMove(action, currentState, turn)==1){
                        
                        births++;
                        //cout <<"Got here\n";
                        int **childBoard = new int*[8]; //allocates the new board array on the heap ,8x8 continuous memory block
                        for (int i = 0; i < 8; i++){
                            childBoard[i] = new int[8];
                        }
                        int a=0, b=0;
                        //use newBoard funciton
                        for(a=0;a<8;a++){
                            for(b=0;b<8;b++){
                                childBoard[a][b]=currentState[a][b];//copies the entire current state into this newboard
                            }
                        }
                       
                        /*cout <<"Before:\n";
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j <8; j++) {
                                cout << childBoard[i][j] << " ";
                            }
                            cout << "\n";
                        }*/
                        
                        newBoard(action, childBoard, turn);
                        
                        /*cout << "After:\n";
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j <8; j++) {
                                cout << childBoard[i][j] << " ";
                            }
                            cout << "\n";
                        } cout << "\n";*/

                        //CALL GET BOARD
                        getBoard(childBoard);
                        
                        for (int i = 0; i < 8; i++) {
                            delete [] childBoard[i];
                        }
                        delete[] childBoard;//frees the pointer since its already been transfered
                    }
                }
            }
        }
    }
    
    
    //END OF NICK'S CODE//
};

void printBoard(Node node, int childNum) {
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << node.children[childNum].boardState[i][j] << " ";
        }
        cout << "\n";
    }
}


float minimax(Node node, int childNum, float alpha, float beta) { //childNum keeps track of the index of the children
    cout << "\nDepth:" << node.depth << "\n";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << node.boardState[i][j] << " ";
        }
        cout << "\n";
    }
    
    if (node.depth == node.maxDepth) { //Base case, need to also consider when game is won or there are no more valid moves
        node.value = node.evaluate();
        cout <<"Value is " << node.value  << " child num is " << childNum << "\n\n";
        return node.value;
    }
    if (node.isMax) {
        cout << "Is max\n";
        node.value = -10000;
        //for each child of node recursively max out the values
        int start = 1;
        for (int i = 0; i < node.children.size() || start == 1; i++) { //terminating cond should be i < node.children.size()
            
            node.project(node.place, node.boardState, node.isMax);
            
            node.value = max(node.value, minimax(node.children[i],i,alpha,beta));
            alpha = max(alpha, node.value);
            if (alpha >= beta) {
                cout << "Pruned cause isMax is guaranteed " << alpha << " and isMin is guaranteed " << beta << "\n";
                break;
            }
        }
        cout << "Chose the max value as " << node.value << "\n";
        return node.value;
    } else {
        cout << "Is min\n";
        node.value = 10000;
        //for each child of node recursively min out the values
        int start = 1;
        for (int i = 0; i < node.children.size() || start == 1; i++) { // i < node.children.size()
            start = 0;
            
            node.project(node.place, node.boardState, node.isMax);
            
            int oldV = node.value;
            node.value = min(node.value, minimax(node.children[i],i,alpha,beta));
            if (oldV != node.value) node.childIndex++;
            beta = min(beta, node.value);
            
            if (alpha >= beta) {
                cout << "Pruned cause isMin is guaranteed " << beta << " and isMax is guaranteed " << alpha << "\n";
                break;
            }
        }
        if (node.depth == 0) {
            --node.childIndex; //prevent off by one error
            cout << "Chose the min value as " << node.value << "\n";
            cout << "The index is " << node.childIndex << "\n";
            return node.childIndex;
        }
        
        cout << "Chose the min value as " << node.value << "\n";
        return node.value;
    }
}



int main(void) {
    srand(time(NULL));
    int board[8][8]={
    {0,2,0,2,0,2,0,2},//red side of the board initially
    {2,0,2,0,2,0,2,0},
    {0,2,0,2,0,2,0,2},
    {0,0,0,0,0,0,0,0},//middle of the board is initially empty
    {0,0,0,0,0,0,0,0},
    {1,0,1,0,1,0,1,0},//white side of the board initially
    {0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,0}};
    
    Node f(4,0,board);
    f.project(f.place,board,f.isMax);
    printBoard(f,0);
    //cout << minimax(f,f.childIndex,-10001,10001) << "\n";
    return 0;
}


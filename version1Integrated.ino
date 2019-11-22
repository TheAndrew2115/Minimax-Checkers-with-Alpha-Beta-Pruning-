#include <RGBmatrixPanel.h>

#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

//matrix.drawPixel(x,y,matrix.Color333(r,g,b));
//matrix.fillRect(topleftx, toplefty, width, height, colour);
//matrix.drawRect^^ same as above
//matrix.drawLine(

const int yes = 2;
const int no = 3;
const int up = 4;
const int down = 5;
const int left = 6;
const int right = 7;

int lastButtonState = 0;
int currentButtonState = 0;

int tLx = 0;
int tLy = 0;

int deltacolumn;

int board[8][8] = {
{0,0,0,2,0,0,0,2},//red side of the board initially
{2,0,2,0,2,0,0,0},
{0,2,0,0,0,2,0,1},
{0,0,0,0,0,0,0,0},//middle of the board is initially empty
{0,0,0,2,0,0,0,0},
{1,0,0,0,1,0,1,0},//white side of the board initially
{0,2,0,1,0,1,0,1},
{1,0,1,0,1,0,1,0}};

int *selection = &board[0][0];
int numSelections = 0;
int selections[4] = {0};

int selectionStage = 0;

int *afterCheckAttack;

int turnnumber = 0;

int moveable=0;

void drawBoard(int board[8][8], int *selection){

  for (int i = 0; i < 8; ++i) {
    tLy = i*4;
    tLx = 0;
    for (int j = 0; j < 8; ++j) {
      tLx = j*4;
      if ((j % 2 != 0 && i % 2 != 0) || (j%2 == 0 && i % 2 == 0)) { //if its a white square
        if (&board[i][j] == selection){ //if it is selected
          matrix.drawRect(tLx, tLy, 4, 4, matrix.Color333(0,7,0));
          matrix.drawRect(tLx + 1, tLy + 1, 2, 2, matrix.Color333(7,7,7));
        } 
        else { //if not
          matrix.fillRect(tLx, tLy, 4, 4, matrix.Color333(7,7,7));
        }
      } 
      else { //its a black square
        if (&board[i][j] == selection && board[i][j] > 2) { //if it is selected and not empty, king
          matrix.drawLine(tLx + 1, tLy, tLx + 2, tLy, matrix.Color333(0,7,0));
          matrix.drawLine(tLx + 1, tLy + 3, tLx + 2, tLy + 3, matrix.Color333(0,7,0));
          matrix.drawLine(tLx, tLy + 1, tLx, tLy + 2, matrix.Color333(0,7,0));
          matrix.drawLine(tLx + 3, tLy + 1, tLx + 3, tLy + 2, matrix.Color333(0,7,0));
          if (board[i][j] == 3){
            matrix.drawLine(tLx, tLy, tLx + 3, tLy + 3, matrix.Color333(0,0,7));
            matrix.drawLine(tLx, tLy + 3, tLx + 3, tLy, matrix.Color333(0,0,7));
          }
          if (board[i][j] == 4){
            matrix.drawLine(tLx, tLy, tLx + 3, tLy + 3, matrix.Color333(7,0,0));
            matrix.drawLine(tLx, tLy + 3, tLx + 3, tLy, matrix.Color333(7,0,0));
          }
        }
        else if (board[i][j] > 2) { //if it is a king, not selected
          matrix.drawLine(tLx + 1, tLy, tLx + 2, tLy, matrix.Color333(0,0,0));
          matrix.drawLine(tLx + 1, tLy + 3, tLx + 2, tLy + 3, matrix.Color333(0,0,0));
          matrix.drawLine(tLx, tLy + 1, tLx, tLy + 2, matrix.Color333(0,0,0));
          matrix.drawLine(tLx + 3, tLy + 1, tLx + 3, tLy + 2, matrix.Color333(0,0,0));
          if (board[i][j] == 3){
            matrix.drawLine(tLx, tLy, tLx + 3, tLy + 3, matrix.Color333(0,0,7));
            matrix.drawLine(tLx, tLy + 3, tLx + 3, tLy, matrix.Color333(0,0,7));
          }
          if (board[i][j] == 4){
            matrix.drawLine(tLx, tLy, tLx + 3, tLy + 3, matrix.Color333(7,0,0));
            matrix.drawLine(tLx, tLy + 3, tLx + 3, tLy, matrix.Color333(7,0,0));
          }
        }
        else if (&board[i][j] == selection && (board[i][j] == 1 || board[i][j] == 2)){ //if it is selected and not empty, no king
          matrix.drawRect(tLx, tLy, 4, 4, matrix.Color333(0,7,0));
          if (board[i][j] == 1){
            matrix.drawRect(tLx + 1, tLy + 1, 2, 2, matrix.Color333(0,0,7));
          }
          if (board[i][j] == 2){
            matrix.drawRect(tLx + 1, tLy + 1, 2, 2, matrix.Color333(7,0,0));
          }
        } 
        else if (board[i][j] == 1 || board[i][j] == 2){ //if it is not empty but not selected, no king
          matrix.drawRect(tLx, tLy, 4, 4, matrix.Color333(0,0,0));
          if (board[i][j] == 1){
            matrix.drawRect(tLx + 1, tLy + 1, 2, 2, matrix.Color333(0,0,7));
          }
          if (board[i][j] == 2){
            matrix.drawRect(tLx + 1, tLy + 1, 2, 2, matrix.Color333(7,0,0));
          }
        }
        else if (&board[i][j] == selection){ //if it is empty and selected
          matrix.drawRect(tLx, tLy, 4, 4, matrix.Color333(0,7,0));
          matrix.fillRect(tLx+1, tLy+1, 2, 2, matrix.Color333(0,0,0));
        }
        else { //it is empty
          matrix.fillRect(tLx, tLy, 4, 4, matrix.Color333(0,0,0));
        }
      }
    }
  }
  
}

int *buttonCheck(int *selection, int board[8][8]){
  if (digitalRead(up) == LOW){
    //Serial.println("up");
    lastButtonState = LOW;
    currentButtonState = LOW;
    while(lastButtonState == currentButtonState){
      currentButtonState = digitalRead(up);
    }
    if (selection >= &board[1][0]) {
      selection -= 8;
    }
  }
  if (digitalRead(left) == LOW){
    //Serial.println("left");
    lastButtonState = LOW;
    currentButtonState = LOW;
    while(lastButtonState == currentButtonState){
      currentButtonState = digitalRead(left);
    }
    if (selection > &board[0][0]) {
      selection -= 1;
    }
  }
  
  if (digitalRead(down) == LOW){
    //Serial.println("down");
    lastButtonState = LOW;
    currentButtonState = LOW;
    while(lastButtonState == currentButtonState){
      currentButtonState = digitalRead(down);
    }
    if (selection < &board[7][0]) {
      selection += 8;
    }
  }
  if (digitalRead(right) == LOW){
    //Serial.println("right");
    lastButtonState = LOW;
    currentButtonState = LOW;
    while(lastButtonState == currentButtonState){
      currentButtonState = digitalRead(right);
    }
    if (selection < &board[7][7]) {
      selection += 1;
    }
  }
  return selection;
}

/*
int rankBoard(int arr[8][8]){
    //returns 1 if player one has more pieces
    //returns 2 if player two has more pieces
    //returns 0 if they both have the same number of pieces
    //normal piece = 1      king piece = 2
    int player1Count=0;
    int player2Count=0;
    for(int x=0;x<8;x++){
        for(int y=0;y<8;y++){
            switch(arr[x][y]){
                case 1:
                    player1Count++;
                    break;
                case 2:
                    player2Count++;
                    break;
                case 3:
                    player1Count += 2;
                    break;
                case 4:
                    player2Count += 2;
                    break;
            }
        }
    }
    if(player1Count>player2Count){
        return 1;
    }
    if(player1Count<player2Count){
        return 2;
    }
    return 0;
}
*/
int isGameOver(int arr[8][8]){
    int player1Count=0;
    int player2Count=0;
    for(int x=0;x<8;x++){
        for(int y=0;y<8;y++){
            switch(arr[x][y]){
                case 1:
                    player1Count++;
                    break;
                case 2:
                    player2Count++;
                    break;
                case 3:
                    player1Count++;
                    break;
                case 4:
                    player2Count++;
                    break;
            }
        }
    }
    if(player1Count==0||player2Count==0){
        return 0; //game over
    }
    return 1; //game still going
}

int checkmove(int *action, int currentstate[8][8], int turn ){
    int cr, cc, mr, mc;//current row, column and move row, column

    int columnDirection=0, rowDirection=0;

    cc=action[0];//current row&column is the first entry from player
    cr=action[1];

    mc=action[2];//move row&column is the second entry from player
    mr=action[3];

    if(currentstate[cr][cc]==0){
        return -1;//a negative one return value makes the player choose another starting point because the one selected was empty
    }

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
                    if(mr==0){
                        currentstate[cr][cc]=3;//if the move is valid into row 0 for a white piece, king the piece so that a king is moved in the move function
                    }
                    return 1;//this move is thus valid because the right player is moving the right piece to an empty index in the right direction
                }
            }

            if(mr==cr-2){//if the player demands to move their checker two rows up (taking something)
                columnDirection=mc-cc;//the column direction of the move is determined via the difference in mc and cc

                if(columnDirection>0){
                    columnDirection=1;
                }
                else{
                    columnDirection=-1;
                }

                if(currentstate[cr-1][cc+columnDirection]==2 || currentstate[cr-1][cc+columnDirection]==4){//if the diagonally adjacent spot contains an opposite piece or king
                    removePiece(currentstate, cc+columnDirection, cr-1);//deletes the piece that was jumped over
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
                    if(mr==7){
                        currentstate[cr][cc]=4;//if the move is valid into row 0 for a white piece, king the piece so that a king is moved in the move function
                    }
                    return 1;//this 3x3 move is thus valid for same reasons as mentioned above
                }
            }
            
            //CHECK THE THING BELOW BECAUSE IT WAS SIMPLY COPPIED AND MODIFIED FROM ABOVE
            if(mr==cr+2){//if the player demands to move their checker two rows down (taking something)
                columnDirection=mc-cc;//the column direction of the move is determined via the difference in mc and cc

                if(columnDirection>0){
                    columnDirection=1;
                }
                else{
                    columnDirection=-1;
                }

                if(currentstate[cr+1][cc+columnDirection]==1 || currentstate[cr+1][cc+columnDirection]==3){//if the diagonally adjacent spot contains an opposite piece or king
                    removePiece(currentstate, cc+columnDirection, cr+1);
                    return 1;//the move is valid, and the piece has been removed thus return 1 and move the piece in main
                }

            }

        }
        else{
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
                    removePiece(currentstate, cc+columnDirection, cr+rowDirection);//removes the enemy piece that was jumped over
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
                    removePiece(currentstate, cc+columnDirection, cr+rowDirection);//removes the enemy piece that was jumped over
                    return 1;
                }
            }

        }
    }

    //Everything is done except the repeated attack choice


return -1;
}

void removePiece(int arr[8][8],int cc,int cr){
arr[cr][cc]=0;
}

void movePiece(int arr[8][8], int cc, int cr, int nc, int nr){ //gets board, current row, current column, new row, new column indices

int piece=arr[cr][cc];
arr[cr][cc]=0;
arr[nr][nc]=piece;

}

int *canAttack(int arr[8][8], int cc, int cr, int nc, int nr){
int *returnvalues = (int*)malloc(3*sizeof(int));
if(arr[cr][cc]==1 && cr==0){ arr[cr][cc]=3; } //if the white pawn is on the other side of the board, king it
if(arr[cr][cc]==2 && cr==7){ arr[cr][cc]=4; } //red pawn being kinged like above

if(arr[cr][cc]==1){ //normal white piece
    if(cc==0||cc==1){//if the piece is one the left side of the board
        if((arr[cr-1][cc+1]==2||arr[cr-1][cc+1]==4)&&(arr[cr-2][cc+2]==0)){ //loop until shits done. //maybe return a pointer w/ new coords and loop in the check function
            while(true){ //do an or statement for both left and right
            //newSpot = &board[cc][cr];
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2]=1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc+2&&nr==cr-2){ //user wants to jump to the right
                removePiece(arr,cc+1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
            }
        }
        else{ //if there isnt an available jump
            returnvalues[0]=cc;
            returnvalues[1]=cr;
            returnvalues[2]=0;
            return returnvalues;
        }
    }
    else if(cc==7||cc==6){ //if its on the right
        if((arr[cr-1][cc-1]==2||arr[cr-1][cc-1]==4)&&(arr[cr-2][cc-2]==0)){ //if theres an valid, available jump to the left or right
            while(true){ //loop until they enter correct data
            
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2]=1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc-2&&nr==cr-2){ //user wants to jump over to the left
                removePiece(arr,cc-1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
            }
        }
        else{ //if there isnt an available jump
            returnvalues[0]=cc;
            returnvalues[1]=cr;
            returnvalues[2]=0;
            return returnvalues;
        }
    }
    else{ //no index restriction on checking to the left or right
        if(((arr[cr-1][cc+1]==2||arr[cr-1][cc+1]==4)&&arr[cr-2][cc+2]==0)||((arr[cr-1][cc-1]==2||arr[cr-1][cc-1]==4)&&(arr[cr-2][cc-2]==0))){ //if theres an valid, available jump to the left or right
            while(true){ //loop until they enter correct data
            
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2]=1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc+2&&nr==cr-2){
                removePiece(arr,cc+1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc-2&&nr==cr-2){
                removePiece(arr,cc-1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
            }
        }
        else{ //if there isnt an available jump
            returnvalues[0]=cc;
            returnvalues[1]=cr;
            returnvalues[2]=0;
            return returnvalues;
        }
    }
} //there could be an extra paranthesis somwhere
else if(arr[cr][cc]==2){ //normal red piece
    if(cc==0||cc==1){//if the piece is one the left side of the board
        if((arr[cr+1][cc+1]==1||arr[cr+1][cc+1]==3)&&(arr[cr+2][cc+2]==0)){ //loop until shits done. //maybe return a pointer w/ new coords and loop in the check function
            while(true){ //do an or statement for both left and right
            
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2] = 1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc+2&&nr==cr+2){ //user wants to jump to the right
                removePiece(arr,cc+1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
            }
        }
        else{ //if there isnt an available jump
            returnvalues[0]=cc;
            returnvalues[1]=cr;
            returnvalues[2]=0;
            return returnvalues;
        }
    }
    else if(cc==7||cc==6){ //if its on the right YOU NEED TO ADD IT HERE AS THIS IS WHERE IT IS
        if((arr[cr+1][cc-1]==1||arr[cr+1][cc-1]==3)&&(arr[cr+2][cc-2]==0)){ //if theres an valid, available jump to the left or right
            while(true){ //loop until they enter correct data
            
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2] = 1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc-2&&nr==cr+2){ //user wants to jump over to the left
                removePiece(arr,cc-1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
            }
        }
        else{ //if there isnt an available jump
            returnvalues[0]=cc;
            returnvalues[1]=cr;
            returnvalues[2]=0;
            return returnvalues;
        }
    }
    else{ //no index restriction on checking to the left or right
        if(((arr[cr+1][cc+1]==1||arr[cr+1][cc+1]==3)&&arr[cr+2][cc+2]==0)||((arr[cr+1][cc-1]==1||arr[cr+1][cc-1]==3)&&(arr[cr+2][cc-2]==0))){ //if theres an valid, available jump to the left or right. ADD SHIT SO IF ITS AT COLUMN 6 AND CHECKS 2 RIGHT
            while(true){ //loop until they enter correct data
            
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2] = 1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc+2&&nr==cr+2){
                removePiece(arr,cc+1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc-2&&nr==cr+2){
                removePiece(arr,cc-1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
            }
        }
        else{ //if there isnt an available jump
            returnvalues[0]=cc;
            returnvalues[1]=cr;
            returnvalues[2]=0;
            return returnvalues;
        }
    }
}
else if(arr[cr][cc]==3){ //white king
    if(cc==0||cc==1){
        if(((arr[cr-1][cc+1]==2||arr[cr-1][cc+1]==4)&&arr[cr-2][cc+2]==0)||((arr[cr+1][cc+1]==2||arr[cr+1][cc+1]==4)&&arr[cr+2][cc+2]==0)){
        while(true){ //loop until they enter correct data
            
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2] = 1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc+2&&nr==cr+2){
                removePiece(arr,cc+1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc+2&&nr==cr-2){
                removePiece(arr,cc+1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
        }
    }
    else{
        returnvalues[0]=cc;
        returnvalues[1]=cr;
        returnvalues[2]=0;
        return returnvalues;
    }
    }
    else if(cc==6||cc==7){
        if(((arr[cr-1][cc-1]==2||arr[cr-1][cc-1]==4)&&(arr[cr-2][cc-2]==0))||((arr[cr+1][cc-1]==2||arr[cr+1][cc-1]==4)&&(arr[cr+2][cc-2]==0))){
        while(true){ //loop until they enter correct data
            
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2] = 1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc-2&&nr==cr+2){
                removePiece(arr,cc-1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc-2&&nr==cr-2){
                removePiece(arr,cc-1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
        }
    }
    else{
        returnvalues[0]=cc;
        returnvalues[1]=cr;
        returnvalues[2]=0;
        return returnvalues;
    }
    }
    else{
    if(((arr[cr-1][cc+1]==2||arr[cr-1][cc+1]==4)&&arr[cr-2][cc+2]==0)||((arr[cr-1][cc-1]==2||arr[cr-1][cc-1]==4)&&(arr[cr-2][cc-2]==0))||((arr[cr+1][cc+1]==2||arr[cr+1][cc+1]==4)&&arr[cr+2][cc+2]==0)||((arr[cr+1][cc-1]==2||arr[cr+1][cc-1]==4)&&(arr[cr+2][cc-2]==0))){
        while(true){ //loop until they enter correct data
            
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2] = 1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc+2&&nr==cr+2){
                removePiece(arr,cc+1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc-2&&nr==cr+2){
                removePiece(arr,cc-1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc+2&&nr==cr-2){
                removePiece(arr,cc+1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc-2&&nr==cr-2){
                removePiece(arr,cc-1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
        }
    }
    else{
        returnvalues[0]=cc;
        returnvalues[1]=cr;
        returnvalues[2]=0;
        return returnvalues;
    }
    }
}
else if(arr[cr][cc]==4){ //red king
    if(cc==0||cc==1){
        if(((arr[cr-1][cc+1]==1||arr[cr-1][cc+1]==3)&&arr[cr-2][cc+2]==0)||((arr[cr+1][cc+1]==1||arr[cr+1][cc+1]==3)&&arr[cr+2][cc+2]==0)){
        while(true){ //loop until they enter correct data
            
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2] = 1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc+2&&nr==cr+2){
                removePiece(arr,cc+1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc+2&&nr==cr-2){
                removePiece(arr,cc+1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
        }
    }
    else{
        returnvalues[0]=cc;
        returnvalues[1]=cr;
        returnvalues[2]=0;
        return returnvalues;
    }
    }
    else if(cc==6||cc==7){
        if(((arr[cr-1][cc-1]==1||arr[cr-1][cc-1]==3)&&(arr[cr-2][cc-2]==0))||((arr[cr+1][cc-1]==1||arr[cr+1][cc-1]==3)&&(arr[cr+2][cc-2]==0))){
        while(true){ //loop until they enter correct data
            
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2] = 1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc-2&&nr==cr+2){
                removePiece(arr,cc-1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc-2&&nr==cr-2){
                removePiece(arr,cc-1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
        }
    }
    else{
        returnvalues[0]=cc;
        returnvalues[1]=cr;
        returnvalues[2]=0;
        return returnvalues;
    }
    }
    else{
    if(((arr[cr-1][cc+1]==1||arr[cr-1][cc+1]==3)&&arr[cr-2][cc+2]==0)||((arr[cr-1][cc-1]==1||arr[cr-1][cc-1]==3)&&(arr[cr-2][cc-2]==0))||((arr[cr+1][cc+1]==1||arr[cr+1][cc+1]==3)&&arr[cr+2][cc+2]==0)||((arr[cr+1][cc-1]==1||arr[cr+1][cc-1]==3)&&(arr[cr+2][cc-2]==0))){
        while(true){ //loop until they enter correct data
            
            returnvalues[0]=nc;
            returnvalues[1]=nr;
            if(nc==cc&&nr==cr){
                returnvalues[2] = 1;
                return returnvalues; //tells the check function that it can stop looping
            }
            else if(nc==cc+2&&nr==cr+2){
                removePiece(arr,cc+1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc-2&&nr==cr+2){
                removePiece(arr,cc-1,cr+1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2] = 2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc+2&&nr==cr-2){
                removePiece(arr,cc+1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            else if(nc==cc-2&&nr==cr-2){
                removePiece(arr,cc-1,cr-1);
                movePiece(arr,cc,cr,nc,nr);
                returnvalues[2]=2;
                return returnvalues; //returns 1 so that in the check function, it will know to continue looping
            }
            printf("Incorrect Input\n");
        }
    }
    else{
        returnvalues[0]=cc;
        returnvalues[1]=cr;
        returnvalues[2]=0;
        return returnvalues;
    }
    }
}
returnvalues[0]=cc;
returnvalues[1]=cr;
returnvalues[2]=0; //just in case something goes wrong
return returnvalues;
}

void setup() {
  // put your setup code here, to run once:
  
  matrix.begin();
  Serial.begin(9600);
  pinMode(yes, INPUT_PULLUP);
  pinMode(no, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  
}



void loop() {
  if(isGameOver(board)==1){
  selection = buttonCheck(selection, board);
  drawBoard(board, selection);
  /*if (afterCheckAttack[2] == 0){
    numSelections = 0;
  }*/
  /*afterCheckAttack = canAttack(board, selections[0], selections[1], selections[0], selections[1]);
  if (afterCheckAttack[2] == 0 && selectionStage == 1) {
    numSelections = 0;
  }*/
  
  if(digitalRead(yes) == LOW){
    lastButtonState = LOW;
    currentButtonState = LOW;
    while(lastButtonState == currentButtonState){
      currentButtonState = digitalRead(yes);
    }
    if(numSelections == 0) {
      selections[1] = (selection - &board[0][0])/8;
      selections[0] = (selection - &board[0][0])-selections[1]*8;
      numSelections = 1;
      Serial.println("Selected Piece");
    }
    else {
      selections[3] = (selection - &board[0][0])/8;
      selections[2] = (selection - &board[0][0])-selections[3]*8;
      Serial.println("Selected Move");
      if(selectionStage == 1){
        moveable = 0;
        free(afterCheckAttack);
        afterCheckAttack = canAttack(board, selections[0], selections[1], selections[2], selections[3]);
        if(afterCheckAttack[2] == 0){
          numSelections = 0;
          selectionStage = 0;
          turnnumber += 1;
          Serial.println("numSelections reset to 0");
          }
        if(afterCheckAttack[2] == 1){
          numSelections = 0;
          selectionStage = 0;
          turnnumber += 1;
        }
        if(afterCheckAttack[2] == 2){
          selections[0] = afterCheckAttack[0];
          selections[1] = afterCheckAttack[1];
          free(afterCheckAttack);
          afterCheckAttack = canAttack(board, selections[0], selections[1], selections[0], selections[1]);
          if(afterCheckAttack[2] == 0){
              selectionStage = 0;
              numSelections = 0;
              turnnumber+=1;
            }
        }
       
      } 
      else if (selectionStage == 0) {
        moveable = checkmove(selections, board, turnnumber);
        Serial.println(selections[0]);
        Serial.println(selections[1]);
        Serial.println(selections[2]);
        Serial.println(selections[3]);
      }
      
      if (moveable == -1){
        numSelections = 0;
        Serial.println("Illegal Move");
      }
      
      else if (moveable == 1) {
        movePiece(board, selections[0], selections[1], selections[2], selections[3]);
        deltacolumn = selections[2] - selections[0];
        if(deltacolumn<0){
          deltacolumn *= -1;
          }
        Serial.println("Moved");
        free(afterCheckAttack);
        afterCheckAttack = canAttack(board, selections[2], selections[3], selections[2], selections[3]);
        if (afterCheckAttack[2] != 0 && deltacolumn>1){ //have to fix can attack where it wont let you attack if you just moved previously instead of attacked
           selections[0] = selections[2];
           selections[1] = selections[3];
           selectionStage = 1;
           numSelections = 1;
           turnnumber-=1;
        } else{
          numSelections =0;
        }
        turnnumber +=1;
      }
      //numSelections = 0;
      //turnnumber +=1;
    }
  }
  } else {
    Serial.print("Winner");
  }
}

/*
void project(int *position, int currentstate[8][8], int turn){

    int *action=malloc(sizeof(int)*4);//sets the initial piece position to the first two spots for action pointer
    action[0]=postion[0];
    action[1]=position[1];

    int i=0;*/
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
    9 - move up
    10 - move down
    11 - move right
    12 - move left
    *//*
    if(currentstate[postion[0]][position[1]]==1 || currentstate[postion[0]][position[1]]==2 || currentstate[postion[0]][position[1]]==3 || currentstate[postion[0]][position[1]]==4 ){

        for(i=0;i<9;i++){//tests each of the 13 possible moves
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
            case 8: action[2]=position[0];//check the check move function for actions regarding this move
                    action[3]=position[1];
                    break;

            }

        if(checkmove(action, currentstate, turn)==1){
            //birth child
        }


        }
    }

    if(currentstate[postion[0]][position[1]]==3 || currentstate[postion[0]][position[1]]==4 ){// if its a king

        for(i=9;i<13;i++){//tests each of the 13 possible moves

            switch(i){

                case 9: action[2]=position[0]-1;
                        action[3]=position[1];
                        break;
                case 10:action[2]=position[0]+1;
                        action[3]=position[1];
                        break;
                case 11:action[2]=position[0];
                        action[3]=position[1]+1;
                        break;
                case 12:action[2]=position[0];
                        action[3]=position[1]-1;
                        break;

                }

        if(checkmove(action, currentstate, turn)==1){
            //birth child
        }

        }
    }

}


#include <vector>
#include <iostream>

using namespace std;

class Node{  
public:
    int depth;
    int board[8][8];
    int numChildren;
    vector<int*> children;
    float evalValue;
    bool isMax;
    //bool isMin;
    node(int depthh, int boardd[8][8], int numChildrenn, vector<int*> childrenn, float evalValuee, bool isMaxx){ //constructor
        depth = depthh;
        for(int x = 0; x<8; x++){
            for(int y = 0; y<8; y++){
                board[x][y] = boardd[x][y];
            }
        }
        numChildren = numChildrenn;
        children = childrenn;
        evalValue = evalValuee;
        isMax = isMaxx;
    }
    ~node(){} //deconstructor

};
*/

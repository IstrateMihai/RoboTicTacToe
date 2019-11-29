#define l1 0.0935
#define l2 0.151
#define ID1 1
#define ID2 14
#define ID3 18
static double RadToDeg = 180/M_PI;
static double DegToVal = 3.41;
static int Speed = 100;
struct pozitie_cuple {
    int p1 , p2;
};

pozitie_cuple InverseKinematics(double x , double y){ //inverse kinematics for a 2dof robot arm
       pozitie_cuple temp;
     double px2=-acos((x*x+y*y-(l1*l1)-(l2*l2))/(2*l1*l2));
       double px1 =atan2(y,x)-atan2((l2*sin(px2)),(l1+l2*cos(px2)));
       temp.p1=abs(px1*DegToVal*RadToDeg+205);
       temp.p2=abs(px2*DegToVal*RadToDeg+512);
       return temp;
}
void moveTo(double x , double y){       //move end effector to the desired x,y coordonates
 
    pozitie_cuple temp = InverseKinematics(x,y);
       ax12a.moveSpeed(ID1,temp.p1,Speed);
     ax12a.moveSpeed(ID2,temp.p2,Speed);
       int px2 = ax12a.readPosition(ID2);
       int px1 = ax12a.readPosition(ID1);
while(!((px1>=temp.p1-1  || px1<temp.p1+1 )&& (px2>=temp.p2-1 || px2<temp.p2+1)));
}
void pencilUp(){   
     ax12a.moveSpeed(18,650,100);
     while(ax12a.readPosition(ID3)!=650);
    }
void pencilDown(){  
     ax12a.moveSpeed(18,500,100);
while(ax12a.readPosition(ID3)!=500);

}
void Line(double x1 , double y1 , double x2 , double y2){ //draw line from (x1,y1) to (x2,y2)
     int desc=50;
     double dx = x2-x1;
     double pasX = dx/desc;
     double dy=y2-y1;
     double pasY = dy/desc;
     moveTo(x1,y1);
pencilDown();
     for(int i=0;i<=desc;i++)
     {
      moveTo(x1+pasX*i,y1+pasY*i);     
     }
pencilUp();

}
   void Squere(double x , double y,double l){//draw a squere
   Line2(x,y,x+l,y);
   Line(x+l,y,x+l,y+l);
   Line2(x+l,y+l,x,y+l);
   Line(x,y+l,x,y);
  }
void Board(double x, double y,double tableSize,double cellSize)//draw the game board
{
    Squere(x, y, tableSize);
    pencilUp();
    Line(x + cellSize, y, x + cellSize, y + tableSize);
pencilUp();
    Line(x + cellSize * 2, y, x + cellSize * 2, y + tableSize);
   pencilUp();
    Line(x, y + cellSize, x + tableSize, y + cellSize);
   pencilUp();
    Line(x, y + cellSize * 2, x + tableSize, y + cellSize * 2);
    pencilUp();
}

void PutX(double x,double y ,int poz){ //put X in disired position
  int dif = poz%3;
  int dif2 = poz/3;
  Line(x+cellSize*dif,y+tableSize-cellSize/4-cellSize*dif2,x+cellSize+cellSize*dif,y+cellSize*2+cellSize/4-cellSize*dif2);
  Line(x+cellSize+cellSize*dif,y+tableSize-cellSize/4-cellSize*dif2,x+cellSize*dif,y+cellSize*2+cellSize/4-cellSize*dif2);
  }

void Put0(double x, double y , int poz){  //put 0 in disired position
   int dif=poz%3;
   int dif2=poz/3;
 Squere(x+cellSize/2+cellSize*dif,y+tableSize-cellSize/2-cellSize*dif2,cellSize/3);//to test
  
}
/*check for winners , return number of winning row/column/diag */
int rowWinner(char *GameBoard){
   if(GameBoard[0]==GameBoard[1]&& GameBoard[1]==GameBoard[2] && GameBoard[2]!=' ') return 1;
     if(GameBoard[3]==GameBoard[4]&& GameBoard[4]==GameBoard[5] && GameBoard[5]!=' ') return 2;
      if(GameBoard[6]==GameBoard[7]&& GameBoard[7]==GameBoard[8] && GameBoard[8]!=' ') return 3;
     return 0;
}
int columnWinner(char *GameBoard){
if(GameBoard[0]==GameBoard[3]&& GameBoard[3]==GameBoard[6] && GameBoard[6]!=' ') return 1;
if (GameBoard[1]==GameBoard[4]&& GameBoard[4]==GameBoard[7] && GameBoard[7]!=' ') return 2;
if(GameBoard[2]==GameBoard[5]&& GameBoard[5]==GameBoard[8] && GameBoard[8]!=' ') return 3;
     return 0;
}
int diagWinner(char *GameBoard){
if (GameBoard[0]==GameBoard[4] && GameBoard[4]==GameBoard[8] && GameBoard[8] != ' ') 
        return 1; 
        
   else if (GameBoard[6]==GameBoard[4] && GameBoard[4]==GameBoard[2] && GameBoard[2] != ' ') 
        return 2; 
  return 0;
/*---------------------------------------------------------------------------------*/
}
bool Winner(char *GameBoard){  //checks if game has a winner
  return (diagWinner(GameBoard)>0 || rowWinner(GameBoard)>0 || columnWinner(GameBoard)>0);
}

int  TicTacToe(double x , double y,double tableSize,double cellSize  ){
   char GameBoard[9]={"        "};   //intialise game board , all blank
   
   int moves =0;   //number of made moves
   int poz;
   Board(x,y,tableSize,cellSize);
   while (!Winner(GameBoard)&& moves <9)
   {
      if(moves%2==0){ 
    while(true){//get random position untill position is empty on game board
      poz=random(0,9);
      if(GameBoard[poz]!='X' && GameBoard[poz]!='0') break;
    
      }
      GameBoard[poz]='X';
     
      PutX(x,y,poz);
      }
      else{ 
             while(true){
      poz=random(0,9);
      if(GameBoard[poz]!='X' && GameBoard[poz]!='0') break;
      }
     
      GameBoard[poz]='0';
    Put0(x,y,poz);
      }
      moves++; //max 9 moves 
   }
  
 
   if(Winner(GameBoard)){//checks for winner and draws line over winning combination
    
      if(rowWinner(GameBoard)){
    if(rowWinner(GameBoard)==3) Line2(x,y+cellSize/2,x+tableSize,y+cellSize/2);
    if(rowWinner(GameBoard)==2) Line2(x,y+cellSize*1.5,x+tableSize,y+cellSize*1.5);
    if(rowWinner(GameBoard)==1) Line2(x,y+cellSize*2.5,x+tableSize,y+cellSize*2.5);
       }
       else  if(columnWinner(GameBoard)){
    if(columnWinner(GameBoard)==1) Line2(x+cellSize/2,y,x+cellSize/2,y+tableSize);
    if(columnWinner(GameBoard)==2) Line2(x+cellSize*1.5,y,x+cellSize*1.5,y+tableSize);
    if(columnWinner(GameBoard)==3) Line2(x+cellSize*2.5,y,x+cellSize*2.5,y+tableSize);
       }
       else if(diagWinner(GameBoard)){
           if(diagWinner(GameBoard)==1) LineD(x,y+tableSize,x+tableSize,y);
           if(diagWinner(GameBoard)==2) LineD(x,y,x+tableSize,y+tableSize);
       }
   }
   else {
  
     return -1;/*return -1 for draw , 1 if X wins or 0 if 0 wins*/
   }
  return moves%2;
}

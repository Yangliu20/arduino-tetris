#include <IRremote.h>

int row[8]={5,4,3,2,6,7,8,9};
int col[8]={10,11,12,13,14,15,16,17};
int fixAppear[8][8]={{0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0}};       //底部所有已经固定的积木
int currentBlock[11][8]={{0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0}};   //正在下落中的积木，前三行是积木待落下时所处的区域，假设所有积木都只占据三行
int totalAppear[8][8]={{0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0}};     //所有显示出来的积木

int center[10][2]={{1,4},{1,3},{2,3},{1,3},{1,3},{1,3},{2,3},{1,3},{1,3},{1,4}};

int blockModel[10][3][8]={{{0,0,0,0,0,0,0,0},
                          {0,0,0,0,1,0,0,0},
                          {0,0,0,1,1,0,0,0}},
                         {{0,0,0,0,0,0,0,0},
                          {0,0,0,1,0,0,0,0},
                          {0,0,0,1,1,0,0,0}},
                         {{0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,1,1,1,0,0,0}},
                         {{0,0,0,0,0,0,0,0},
                          {0,0,0,1,0,0,0,0},
                          {0,0,1,1,1,0,0,0}},
                         {{0,0,0,0,0,0,0,0},
                          {0,0,0,1,1,0,0,0},
                          {0,0,1,1,0,0,0,0}},
                         {{0,0,0,0,0,0,0,0},
                          {0,0,1,1,0,0,0,0},
                          {0,0,0,1,1,0,0,0}},
                         {{0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,1,0,0,0,0}},
                         {{0,0,0,0,0,0,0,0},
                          {0,0,0,1,1,0,0,0},
                          {0,0,0,1,1,0,0,0}},
                         {{0,0,0,0,0,0,0,0},
                          {0,0,0,0,1,0,0,0},
                          {0,0,1,1,1,0,0,0}},
                         {{0,0,0,0,0,0,0,0},
                          {0,0,0,1,0,0,0,0},
                          {0,0,0,1,1,1,0,0}}};


int RECV_PIN=19;//左移，右移，顺时针转，逆时针转
IRrecv irrecv(RECV_PIN);
decode_results results;

//int movingDownCounting=49;
int score=0;
int buttonStateL,buttonStateR,buttonC,buttonAC;
int beforeL=0;
int beforeR=0;
long velocityControl=600;
unsigned long long lastT;
int num;

int currentC[2];

void setup() {
  //Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  //Serial.println("Enabled IRin");
  randomSeed(analogRead(4));
  
  for(int i=0;i<8;i++){
    pinMode(row[i],OUTPUT);
    pinMode(col[i],OUTPUT);
    digitalWrite(row[i],LOW);
    digitalWrite(col[i],HIGH);
  }

  num=random(10);
  for(int i=0;i<3;i++){
    for(int j=0;j<8;j++){
      currentBlock[i][j]=blockModel[num][i][j];}}  //随机选择一个积木放下，进入待落下的区域
  for(int i=0;i<2;i++){
    currentC[i]=center[num][i];
  }
  lastT=millis();
}

void loop() {
  for(int i=3;i<11;i++){
        for(int j=0;j<8;j++){
          totalAppear[i-3][j]=(currentBlock[i][j] || fixAppear[i-3][j]);}
  }
  show(totalAppear);

  if (irrecv.decode(&results)) {
    //Serial.println(results.value);
    if(results.value==16724175){   //press 1 ->move left
      buttonStateL=1;}
    if(results.value==16743045){   //press 3 ->move right
      buttonStateR=1;}
    if(results.value==16716015){   //press 4 ->rotate anticlockwise
      buttonAC=1;}
    if(results.value==16734885){   //press 6
      buttonC=1;}
    irrecv.resume();
  }

  
  if(buttonStateL==1 /*and beforeL==0*/)
    if (checkL(currentBlock,fixAppear,currentC)==0){//左移
    movingL(currentBlock,currentC);
    currentC[1]-=1;}
  if(buttonStateR==1 /*and beforeR==0*/)
    if(checkR(currentBlock,fixAppear,currentC)==0){//右移
    movingR(currentBlock,currentC);
    currentC[1]+=1;}
  if(buttonAC==1)
    rotationAntiClockwise(currentBlock,currentC,fixAppear);

  buttonStateR=0;
  buttonStateL=0;
  buttonAC=0;
  buttonC=0;
    
  if(millis()-lastT > velocityControl){//判断是否能下落，能下落的话进入if,下落一格，下落需要考虑时间，可以之后加一个count条件
    lastT=millis();
    
    int d=checkDown(currentBlock,fixAppear,currentC);
    if(d==0) {
      movingDown(currentBlock,currentC);
      currentC[0]+=1;}    
    
    else if(d==1){   //如果是不能下落了
      for(int i=0;i<3;i++){
        for(int j=0;j<8;j++){
          if(currentBlock[i][j]==1){                 //积木已经超出界面范围，游戏结束，一直停留在ending界面
            //Serial.print("Game over. Congratulations! You have scored ");
            //Serial.print(score);
            //Serial.println(".");
            endGame();}}
      }

      
      for(int i=3;i<11;i++){
        for(int j=0;j<8;j++){
          fixAppear[i-3][j]=(currentBlock[i][j] || fixAppear[i-3][j]);}
      }                                            //把currentBlock和fixAppear结合起来     //游戏没有结束
        
      while(clearing(fixAppear)!=100){
        score+=1;}                                 //消去整行，直到没有可以消去的了

      num=random(10);
      for(int i=0;i<3;i++){
        for(int j=0;j<8;j++){
          currentBlock[i][j]=blockModel[num][i][j];}}
      for(int i=3;i<11;i++){
        for(int j=0;j<8;j++){
          currentBlock[i][j]=0;                    //放下一个新的积木
        }
      }
      for(int i=0;i<2;i++){
        currentC[i]=center[num][i];
      }
    }
    }
    
}






void show(int appear[8][8]){  //显示此时的矩阵
    for(int i=0;i<8;i++){     //i stands for row
      for(int j=0;j<8;j++){   //j stands for column
        if(appear[i][j]==1){
          digitalWrite(row[i],HIGH);
          digitalWrite(col[j],LOW);
          delayMicroseconds(500);
          digitalWrite(row[i],LOW);  //row[i]和col[j]还要给其他灯使用，要先关掉，不然会出问题的
          digitalWrite(col[j],HIGH);}}}
    }



void movingL(int smallBlock[11][8],int currentC[2]){  //用户操控左右移动，只移动上方的小积木块，需要另外判断是否到达边界！
    for(int i=(currentC[0]-1);i<=(currentC[0]+1);i++){
      for(int j=(currentC[1]-1);j<=(currentC[1]+1);j++){
        if(j<8) smallBlock[i][j-1]=smallBlock[i][j];
      }
      smallBlock[i][currentC[1]+1]=0;
      smallBlock[i][7]=0;
    }
}

int checkL(int smallBlock[11][8],int fixAppear[8][8],int currentC[2]){   //判断是否到达左极限，到达返回1
  for(int i=(currentC[0]-1);i<=(currentC[0]+1);i++){
    for(int j=(currentC[1]-1);j<=(currentC[1]+1);j++){
      if(smallBlock[i][j]==1){
        if(j==0){
        return 1;}
        if((i-3)>=0){
        if(fixAppear[i-3][j-1]==1)
        return 1;}
      }
    }
  }
  return 0;
}


void movingR(int smallBlock[11][8],int currentC[2]){
    for(int i=(currentC[0]-1);i<=(currentC[0]+1);i++){
      for(int j=(currentC[1]+1);j>=(currentC[1]-1);j--){
        if(j>=0) smallBlock[i][j+1]=smallBlock[i][j];
      }
      smallBlock[i][currentC[1]-1]=0;
      smallBlock[i][0]=0;
    } 
}

int checkR(int smallBlock[11][8],int fixAppear[8][8],int currentC[2]){   //判断是否到达右极限，到达返回1
  for(int i=(currentC[0]-1);i<=(currentC[0]+1);i++){
    for(int j=(currentC[1]+1);j>=(currentC[1]-1);j--){
      if(smallBlock[i][j]==1){
        if(j==7){
        return 1;}
        if((i-3)>=0){
        if(fixAppear[i-3][j+1]==1)
        return 1;}
      }
    }
  }
  return 0;
}

void movingDown(int smallBlock[11][8],int currentC[2]){    //向下移动一格
  for(int j=(currentC[1]-1);j<=(currentC[1]+1);j++){
    for(int i=(currentC[0]+1);i>=(currentC[0]-1);i--){
      smallBlock[i+1][j]=smallBlock[i][j];
    }
    smallBlock[currentC[0]-1][j]=0;
  } 
}



int checkDown(int currentBlock[11][8],int fixAppear[8][8],int currentC[2]){   //判断是否到达下落极限，到达极限的可能条件有：A.已经到达底部 B.与fix的积木相遇了;到达极限返回1，没有到达返回0
  for(int i=(currentC[0]+1);i>=(currentC[0]-1);i--){
    for(int j=(currentC[1]-1);j<=(currentC[1]+1);j++){
      if(currentBlock[i][j]==1){
        if(i==10){
        return 1;}
        if((i-2)>=0){
        if(fixAppear[i-3+1][j]==1)
        return 1;}
      }
    }
  }
  return 0;
}


int clearing(int totalAppear[8][8]){    //判断是否要消去一行，如果要消去的话就直接消去，返回被消去的行的index
  int i,j;
  int targetRow=100;    //随便赋一个离谱的初值
  int counts;
  
  for(i=7;i>=0;i--){
    counts=0;
    for(j=0;j<8;j++){
      if(totalAppear[i][j]==0){counts+=1;}}
    if(counts==8){
      break;}
    if(counts==0){
      targetRow=i;
      break;}
    }

  if(targetRow<8){   //valid target row number
    for(j=0;j<8;j++){
      for(i=targetRow;i>0;i--){  //不包括最上面一行
        totalAppear[i][j]=totalAppear[i-1][j];}
    totalAppear[0][j]=0;   //最上面一行自动为0
    }
  }

  return targetRow;
  }

void endGame(){
  int endingFig[8][8]={{0,0,0,0,0,0,0,0},
                       {0,1,1,0,0,1,1,0},
                       {0,1,1,0,0,1,1,0},
                       {0,0,0,0,0,0,0,0},
                       {1,0,0,0,0,0,0,1},
                       {0,1,0,0,0,0,1,0},
                       {0,0,1,1,1,1,0,0},
                       {0,0,0,0,0,0,0,0}};
  while(1){
    show(endingFig);
  }
}


int rotationAntiClockwise(int currentBlock[11][8],int center[2],int fixAppear[8][8]){
  int currentBlockTemp[11][8]={{0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0}};
  for(int i=(center[0]-1);i<=(center[0]+1);i++){
    for(int j=(center[1]-1);j<=(center[1]+1);j++){
      if((i>=0 and i<11) and (j>=0 and j<8)){
        if(currentBlock[i][j]==1){
          if(((center[0]+center[1]-j)<0 or (center[0]+center[1]-j)>10) or ((center[1]-center[0]+i)<0 or (center[1]-center[0]+i)>7))           //超出范围
            return 0;
          if((center[0]+center[1]-j-3)>=0){
            if(fixAppear[center[0]+center[1]-j-3][center[1]-center[0]+i]==1)            //撞到了
            return 0;}
          
          currentBlockTemp[center[0]+center[1]-j][center[1]-center[0]+i]=currentBlock[i][j];}
      }
    }
  }
  for(int i=(center[0]-1);i<=(center[0]+1);i++){
    for(int j=(center[1]-1);j<=(center[1]+1);j++){
      currentBlock[i][j]=currentBlockTemp[i][j];
    }
  }
  return 1;
}

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <cmath>
#include <windows.h>
using namespace std;
 
 
const WORD FORE_RED =BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |FOREGROUND_RED ;	//用于绘制红色边框 
const WORD FORE_YELLOW = FOREGROUND_INTENSITY |FOREGROUND_RED | FOREGROUND_GREEN ;	//用于绘制黄色文字 
struct node
{
    int x,y;
}snake[1000];

HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
COORD coo;
 
bool isStar=false;
int hard; 
int life=10;
const int mapX=25,mapY=40;   //地图的长宽 
bool rebegin=false; //重玩游戏 
int slength=3,direction=3;
node food;
int direct[4][2]={{-1,0},{1,0},{0,-1},{0,1}};  //四个方向的xy变量 
 //绘制红色边框，用的颜色组合 
 void redPaint() 
 {
 	SetConsoleTextAttribute(hout, FORE_RED);  
 }
 //绘制蛇和文字，用的颜色组合 
 void yellowPaint()
 {
 	SetConsoleTextAttribute(hout, FORE_YELLOW);
 }
void setCursor(int x,int y)
{
    coo.X=y;
    coo.Y=x;
    SetConsoleCursorPosition(hout,coo);
};
//隐藏光标 
void hide()
{
    CONSOLE_CURSOR_INFO cursor_info={1,0};   //第一个参数光标的百分比厚度，第二个参数光标的厚度，设置为0，光标不可见。 
    SetConsoleCursorInfo(hout, &cursor_info);
}
 
//自定义生成随机数，用于生成随机位置的食物 
double random(double start, double end)
{
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}
//绘制选择关卡 
 void chooseLevel()
{
	setCursor(5,13);
	cout << "请输入关卡（1-6）:" << endl;  
 } 
//绘制墙 
void drawWall()
{
	redPaint() ;
    for (int i=0;i<=mapY/5;i++)
        cout << "█";
        	yellowPaint();
        cout<<": 贪吃蛇 :";
		redPaint() ;  
    for (int i=14;i<=mapY;i++)
        cout << "█"; 
    cout << endl;
    for (int j=0;j<=mapX;j++)
    {
         cout <<  "█";
		yellowPaint();
        for (int i=1;i<=mapY;i++) cout << " ";
        	redPaint() ;  
        cout << "█";
       yellowPaint();
        for (int i=1;i<mapY-3;i++)
		{
			if(j==11){
				if(i%2==0){
				redPaint() ;   
					cout << "█"; 
					yellowPaint();
				}			
			}else{
				cout << " ";
			}
		 }
        	redPaint() ;  
        cout << "█"<<endl; 
    }
    for (int i=0;i<=mapY;i++)
        cout << "█";
        	yellowPaint();
      setCursor(14,60); 
	  cout << "提示"; 
	  setCursor(15,45);
	  cout<<"向上：↑           向左：←";
	  setCursor(16,45);
	  cout<<"向下：↓           向右：→";
	  setCursor(17,45);
	  cout<<"暂停/开始： 确定键"	;
	  setCursor(18,45);
	  cout<<"重新选关:  ESC键"	;
	  setCursor(19,60);
	  cout<<"注意！"	; 
	  setCursor(21,45);
	  cout<<"1：撞到障碍物或者墙生命值减一"; 
	  setCursor(22,45);
	  cout<<"2：吃到小星星生命值加一";  
	  chooseLevel();
}
void drawSnake()
{
    setCursor(snake[0].x,snake[0].y);  //光标设置到蛇头的位置输出‘@ ’ 
    cout << "@";
    for (int i=1;i<=slength-1;i++)    //光标遍历蛇身的每一个位置，输出‘* ’ 
    {
        setCursor(snake[i].x,snake[i].y);
        cout << "*";
    }
}
//撞墙后，方向向反方向 
void hitWall()
{
	if(snake[0].x==1 &&direction==0)  //1 ,,,上 
	{
		life=life-1;
		direction=1;
	}else if(snake[0].x==mapX+1&&direction==1)  //0 下 
	{		 
	    life=life-1;
		direction=0;
	}else if(snake[0].y==2&&direction==2)  //3 左 
	{
		life=life-1;	 
		direction=3;
	}else if(snake[0].y==mapY+1&&direction==3) //2 右
	{
		life=life-1;	 
		direction=2;
	}
}
 
//产生食物，随机生成位置 
bool produceFood()
{
    srand((unsigned)time(0));   	//设置随机数种子 
    bool isValid;  //食物生成的位置是否合法 
    while (1)
    {
        isValid=true;  
        food.x=(int) random(0,mapX)+1;food.y=(int) random(0,mapY)+2;  //随机生成食物的位置 
        for (int k=0;k<=slength-1;k++)           //如果随机生成的食物位置，在当前蛇身的位置，则标志位为false，继续while循环，重新生成食物位置 
        {
            if (snake[k].x==food.x && snake[k].y==food.y)
            {
                isValid=false;break;
            }
        }
        if (isValid) break;   //随机生成的食物位置不在蛇身处，跳出while循环 
    }
    setCursor(food.x,food.y);   //光标定义到食物的位置 
    if(rand()%10>8)
    {
    	cout<<"#";
    	isStar=true;
	}else{
		 cout << "$";      //输出打印食物 
		 isStar=false;
	}
   
    return true;
}
//蛇移动 
bool move()
{
    node temp;
    bool isEat=false; //是否吃到食物 
    temp=snake[slength-1];
    for (int i=slength-1;i>=1;i--)
        snake[i]=snake[i-1];                   //遍历蛇身的每一个位置，蛇身的每个位置都等于它前一个的位置 
    snake[0].x+=direct[direction][0];
    snake[0].y+=direct[direction][1];
    setCursor(snake[1].x,snake[1].y);    //光标移动到紧挨蛇头后面的位置，输出蛇身‘* ’ 
    cout << "*";
    if (snake[0].x==food.x && snake[0].y==food.y)  //吃到食物 
    {
        slength++;								//蛇长度+1 
        isEat=true;									//标志位为true 
        snake[slength-1]=temp;				//蛇增加的长度，蛇尾的位置等于吃到食物之前的位置
		if(isStar==true)    
		{
			life+=1;
		 } 
    }
    if (!isEat)								//没有吃到食物 
    {
        setCursor(temp.x,temp.y);		
        cout << " ";
    }
    else
        produceFood();   			//吃到了食物，要重新生成新的食物 
    setCursor(snake[0].x,snake[0].y);  //光标定位到蛇头位置，输出蛇头标志。 
    cout << "@";
    hitWall();
	if(life==0)
	{
		setCursor(5,13);
		cout << "生命值为0!" << endl;
		setCursor(7,13);
		cout << "你输了!" << "最终长度: " << slength << endl;
		setCursor(9,13);
		cout << "按回车键重新开始..."<< endl;
		setCursor(4,mapY+5);  
    	cout << "生命值:   "<<life<<endl; 
		char c=10;
    	while(c!=13){
        c=getch();
		}
		rebegin=true; 
	}
    return true;
};
void configureLevel()  //选择关卡 
{
	setCursor(7,13);
	cin>>hard;
	hard=hard*2; 	
	setCursor(5,13);
	cout << "                  " << endl; 
	setCursor(7,13);
	cout << "   " << endl; 
} 
void printMenu()
{
    setCursor(4,mapY+5);  
    cout<<"              "  ;   //清理生命值区域 
    setCursor(4,mapY+5);  
    cout << "生命值:   "<<life<<endl; 
    setCursor(6,mapY+5);  
    cout << "当前长度: ";
    cout << slength;
    setCursor(8,mapY+5); 
    cout << "已吃食物: ";
    cout << slength-3<<"\n";
    setCursor(10,mapY+5); 
    cout << "第   "<<hard/2<<"   关 ";
}
void pauseFuntion() 
{
	char c=10;
    while(c!=13){
        c=getch();
	}
}
void controlSnake() 
{
	char control;   				//上下左右。 
	if (kbhit()){
    	control=getch();
        if (control==-32){
        	control=getch();
            switch(control){
            case 72:
                if (direction==2 || direction==3)
                    direction=0;break;  //上 
            case 80:
                if (direction==2 || direction==3)
                    direction=1;break;  //下 
            case 75:
                if (direction==0 || direction==1)
                    direction=2;break;  //左 
            case 77:
                if (direction==0 || direction==1)
                    direction=3;break;  //右 
                }
            }
          	else if(control==13){		pauseFuntion();}
		  	else if(control==27){		rebegin=true;} 
        }
}
/*** 主函数 ***/
int main()
{
	while(1)
	{
	rebegin=false;//重玩标志位恢复为false 
   
    clock_t a,b;   
    double hard_len;		//动态的难度，难度跟随时间而变化。 
    for (int i=0;i<=3;i++) 
    {
        snake[i].x=10;
        snake[i].y=10-i;
    }
    system("cls");     			//清理屏幕 
    hide();
    drawWall();
    configureLevel();
    produceFood();
    drawSnake();
    clock_t s_time, e_time;   //用于计算程序的运行时间 
		s_time=clock();
    while (!rebegin)
    {
    	 rebegin=false;
        hard_len=(double)slength/(double) (mapX*mapY); //游戏难度逐步提高 
        a=clock();
        while (1)
        {
            b=clock();
            if (b-a>=(int)(400-30*hard)*(1-sqrt(hard_len))) break;
        }
        controlSnake() ; //控制蛇的运动 
        if (!move()) break;
         e_time=clock();        		//最新时间 
        setCursor(2,mapY+5);           	//光标移动到最后，输出蛇的长度 
        cout<<"已用时间:"<<(double)(e_time-s_time)/CLOCKS_PER_SEC<<"S"<<endl; //最新时间-初始时间 
         printMenu();
    }
	}
    system("pause");
    return 0;
}

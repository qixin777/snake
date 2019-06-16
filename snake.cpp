#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <cmath>
#include <windows.h>
using namespace std;
 
 
const WORD FORE_RED =BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |FOREGROUND_RED ;	//���ڻ��ƺ�ɫ�߿� 
const WORD FORE_YELLOW = FOREGROUND_INTENSITY |FOREGROUND_RED | FOREGROUND_GREEN ;	//���ڻ��ƻ�ɫ���� 
struct node
{
    int x,y;
}snake[1000];

HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
COORD coo;
 
bool isStar=false;
int hard; 
int life=10;
const int mapX=25,mapY=40;   //��ͼ�ĳ��� 
bool rebegin=false; //������Ϸ 
int slength=3,direction=3;
node food;
int direct[4][2]={{-1,0},{1,0},{0,-1},{0,1}};  //�ĸ������xy���� 
 //���ƺ�ɫ�߿��õ���ɫ��� 
 void redPaint() 
 {
 	SetConsoleTextAttribute(hout, FORE_RED);  
 }
 //�����ߺ����֣��õ���ɫ��� 
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
//���ع�� 
void hide()
{
    CONSOLE_CURSOR_INFO cursor_info={1,0};   //��һ���������İٷֱȺ�ȣ��ڶ����������ĺ�ȣ�����Ϊ0����겻�ɼ��� 
    SetConsoleCursorInfo(hout, &cursor_info);
}
 
//�Զ�������������������������λ�õ�ʳ�� 
double random(double start, double end)
{
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}
//����ѡ��ؿ� 
 void chooseLevel()
{
	setCursor(5,13);
	cout << "������ؿ���1-6��:" << endl;  
 } 
//����ǽ 
void drawWall()
{
	redPaint() ;
    for (int i=0;i<=mapY/5;i++)
        cout << "��";
        	yellowPaint();
        cout<<": ̰���� :";
		redPaint() ;  
    for (int i=14;i<=mapY;i++)
        cout << "��"; 
    cout << endl;
    for (int j=0;j<=mapX;j++)
    {
         cout <<  "��";
		yellowPaint();
        for (int i=1;i<=mapY;i++) cout << " ";
        	redPaint() ;  
        cout << "��";
       yellowPaint();
        for (int i=1;i<mapY-3;i++)
		{
			if(j==11){
				if(i%2==0){
				redPaint() ;   
					cout << "��"; 
					yellowPaint();
				}			
			}else{
				cout << " ";
			}
		 }
        	redPaint() ;  
        cout << "��"<<endl; 
    }
    for (int i=0;i<=mapY;i++)
        cout << "��";
        	yellowPaint();
      setCursor(14,60); 
	  cout << "��ʾ"; 
	  setCursor(15,45);
	  cout<<"���ϣ���           ���󣺡�";
	  setCursor(16,45);
	  cout<<"���£���           ���ң���";
	  setCursor(17,45);
	  cout<<"��ͣ/��ʼ�� ȷ����"	;
	  setCursor(18,45);
	  cout<<"����ѡ��:  ESC��"	;
	  setCursor(19,60);
	  cout<<"ע�⣡"	; 
	  setCursor(21,45);
	  cout<<"1��ײ���ϰ������ǽ����ֵ��һ"; 
	  setCursor(22,45);
	  cout<<"2���Ե�С��������ֵ��һ";  
	  chooseLevel();
}
void drawSnake()
{
    setCursor(snake[0].x,snake[0].y);  //������õ���ͷ��λ�������@ �� 
    cout << "@";
    for (int i=1;i<=slength-1;i++)    //�����������ÿһ��λ�ã������* �� 
    {
        setCursor(snake[i].x,snake[i].y);
        cout << "*";
    }
}
//ײǽ�󣬷����򷴷��� 
void hitWall()
{
	if(snake[0].x==1 &&direction==0)  //1 ,,,�� 
	{
		life=life-1;
		direction=1;
	}else if(snake[0].x==mapX+1&&direction==1)  //0 �� 
	{		 
	    life=life-1;
		direction=0;
	}else if(snake[0].y==2&&direction==2)  //3 �� 
	{
		life=life-1;	 
		direction=3;
	}else if(snake[0].y==mapY+1&&direction==3) //2 ��
	{
		life=life-1;	 
		direction=2;
	}
}
 
//����ʳ��������λ�� 
bool produceFood()
{
    srand((unsigned)time(0));   	//������������� 
    bool isValid;  //ʳ�����ɵ�λ���Ƿ�Ϸ� 
    while (1)
    {
        isValid=true;  
        food.x=(int) random(0,mapX)+1;food.y=(int) random(0,mapY)+2;  //�������ʳ���λ�� 
        for (int k=0;k<=slength-1;k++)           //���������ɵ�ʳ��λ�ã��ڵ�ǰ�����λ�ã����־λΪfalse������whileѭ������������ʳ��λ�� 
        {
            if (snake[k].x==food.x && snake[k].y==food.y)
            {
                isValid=false;break;
            }
        }
        if (isValid) break;   //������ɵ�ʳ��λ�ò�������������whileѭ�� 
    }
    setCursor(food.x,food.y);   //��궨�嵽ʳ���λ�� 
    if(rand()%10>8)
    {
    	cout<<"#";
    	isStar=true;
	}else{
		 cout << "$";      //�����ӡʳ�� 
		 isStar=false;
	}
   
    return true;
}
//���ƶ� 
bool move()
{
    node temp;
    bool isEat=false; //�Ƿ�Ե�ʳ�� 
    temp=snake[slength-1];
    for (int i=slength-1;i>=1;i--)
        snake[i]=snake[i-1];                   //���������ÿһ��λ�ã������ÿ��λ�ö�������ǰһ����λ�� 
    snake[0].x+=direct[direction][0];
    snake[0].y+=direct[direction][1];
    setCursor(snake[1].x,snake[1].y);    //����ƶ���������ͷ�����λ�ã��������* �� 
    cout << "*";
    if (snake[0].x==food.x && snake[0].y==food.y)  //�Ե�ʳ�� 
    {
        slength++;								//�߳���+1 
        isEat=true;									//��־λΪtrue 
        snake[slength-1]=temp;				//�����ӵĳ��ȣ���β��λ�õ��ڳԵ�ʳ��֮ǰ��λ��
		if(isStar==true)    
		{
			life+=1;
		 } 
    }
    if (!isEat)								//û�гԵ�ʳ�� 
    {
        setCursor(temp.x,temp.y);		
        cout << " ";
    }
    else
        produceFood();   			//�Ե���ʳ�Ҫ���������µ�ʳ�� 
    setCursor(snake[0].x,snake[0].y);  //��궨λ����ͷλ�ã������ͷ��־�� 
    cout << "@";
    hitWall();
	if(life==0)
	{
		setCursor(5,13);
		cout << "����ֵΪ0!" << endl;
		setCursor(7,13);
		cout << "������!" << "���ճ���: " << slength << endl;
		setCursor(9,13);
		cout << "���س������¿�ʼ..."<< endl;
		setCursor(4,mapY+5);  
    	cout << "����ֵ:   "<<life<<endl; 
		char c=10;
    	while(c!=13){
        c=getch();
		}
		rebegin=true; 
	}
    return true;
};
void configureLevel()  //ѡ��ؿ� 
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
    cout<<"              "  ;   //��������ֵ���� 
    setCursor(4,mapY+5);  
    cout << "����ֵ:   "<<life<<endl; 
    setCursor(6,mapY+5);  
    cout << "��ǰ����: ";
    cout << slength;
    setCursor(8,mapY+5); 
    cout << "�ѳ�ʳ��: ";
    cout << slength-3<<"\n";
    setCursor(10,mapY+5); 
    cout << "��   "<<hard/2<<"   �� ";
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
	char control;   				//�������ҡ� 
	if (kbhit()){
    	control=getch();
        if (control==-32){
        	control=getch();
            switch(control){
            case 72:
                if (direction==2 || direction==3)
                    direction=0;break;  //�� 
            case 80:
                if (direction==2 || direction==3)
                    direction=1;break;  //�� 
            case 75:
                if (direction==0 || direction==1)
                    direction=2;break;  //�� 
            case 77:
                if (direction==0 || direction==1)
                    direction=3;break;  //�� 
                }
            }
          	else if(control==13){		pauseFuntion();}
		  	else if(control==27){		rebegin=true;} 
        }
}
/*** ������ ***/
int main()
{
	while(1)
	{
	rebegin=false;//�����־λ�ָ�Ϊfalse 
   
    clock_t a,b;   
    double hard_len;		//��̬���Ѷȣ��Ѷȸ���ʱ����仯�� 
    for (int i=0;i<=3;i++) 
    {
        snake[i].x=10;
        snake[i].y=10-i;
    }
    system("cls");     			//������Ļ 
    hide();
    drawWall();
    configureLevel();
    produceFood();
    drawSnake();
    clock_t s_time, e_time;   //���ڼ�����������ʱ�� 
		s_time=clock();
    while (!rebegin)
    {
    	 rebegin=false;
        hard_len=(double)slength/(double) (mapX*mapY); //��Ϸ�Ѷ������ 
        a=clock();
        while (1)
        {
            b=clock();
            if (b-a>=(int)(400-30*hard)*(1-sqrt(hard_len))) break;
        }
        controlSnake() ; //�����ߵ��˶� 
        if (!move()) break;
         e_time=clock();        		//����ʱ�� 
        setCursor(2,mapY+5);           	//����ƶ����������ߵĳ��� 
        cout<<"����ʱ��:"<<(double)(e_time-s_time)/CLOCKS_PER_SEC<<"S"<<endl; //����ʱ��-��ʼʱ�� 
         printMenu();
    }
	}
    system("pause");
    return 0;
}

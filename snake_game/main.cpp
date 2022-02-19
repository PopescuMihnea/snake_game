#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;
fstream f;
string directory;
short mousestate,C,R,llimit=0,lines=1,border=1,meneu=1,topscor[11],vmaxX,vmaxY,vmin,n,specfoodX,specfoodY;
short l=4,lmax,foodX,foodY,headX[1921],headY[1081],poz;
float snaker,snakeg,snakeb;
unsigned char c1[10],nume[51],aux12[10]="Scor:";
char aux[51],topnume[11][51],c[59];
short i,a;
short score,tscore;
bool resume,init=false,gameover=false,window=0,numeintrodus,tasta,specfoodgen,anticheat;
auto t1=steady_clock::now(),t2=steady_clock::now(),t3=steady_clock::now();
bool genfruit=true;
short W,H,gridX,gridY;
float sFPS=8.0,FPS=sFPS,fFPS=sFPS+40.0,fpsmod,difmod;
enum directie {up=1,down,lft,rght};
short sd=up;
vector <pair<int,int>> coada;
void writescore()
{
    short i=0,j=0;
    for (i=0; i<10; i++)
        if (score>topscor[i])
        {
            for (j=9; j>i; j--)
            {
                topscor[j]=topscor[j-1];
                strcpy(topnume[j],topnume[j-1]);
            }
            strcpy(topnume[i],aux);
            topscor[i]=score;
            n++;
            break;
        }
}
void writedata()
{
    short i=0,n=0;
    for (i=0; i<=10; i++)
        if (topscor[i]==0)
            break;
    n=--i;
    f.open(directory,fstream::out | fstream::in);
    f<<llimit<<" "<<lines<<" "<<border<<"\n";
    f<<n<<"\n";
    for (i=0; i<=n; i++)
        f<<topnume[i]<<"\n"<<topscor[i]<<"\n";
    f.close();
}

void initGrid(int x,int y)
{
    gridX=x;
    gridY=y;
}
void dettriangle(int i)
{
    switch (sd)
    {
    case up:
        glVertex2f(headX[i],headY[i]);
        glVertex2f(headX[i]+1,headY[i]);
        glVertex2f(headX[i]+0.5,headY[i]+1);
        break;
    case down:
        glVertex2f(headX[i],headY[i]+1);
        glVertex2f(headX[i]+1,headY[i]+1);
        glVertex2f(headX[i]+0.5,headY[i]);
        break;
    case lft:
        glVertex2f(headX[i]+1,headY[i]);
        glVertex2f(headX[i]+1,headY[i]+1);
        glVertex2f(headX[i],headY[i]+0.5);
        break;
    case rght:
        glVertex2f(headX[i],headY[i]);
        glVertex2f(headX[i],headY[i]+1);
        glVertex2f(headX[i]+1,headY[i]+0.5);
        break;
    }
}

void maininit()
{
    W=glutGet(GLUT_WINDOW_WIDTH);
    H=glutGet(GLUT_WINDOW_HEIGHT);
    C=W/20;
    R=H/20;
    initGrid(R,C);
    headX[0]=C/2;
    headY[0]=R/2;
    for (i=1; i<l; i++)
    {
        switch (sd)
        {
        case up:
            headY[i]=headY[i-1]-1;
            headX[i]=headX[0];
            break;
        case down:
            headY[i]=headY[i-1]+1;
            headX[i]=headX[0];
            break;
        case lft:
            headY[i]=headY[0];
            headX[i]=headX[i-1]+1;
            break;
        case rght:
            headY[i]=headY[0];
            headX[i]=headX[i-1]-1;
        }
    }
    if (!llimit)
    {
        if (border) lmax=C*R-2*C-2*(R-2)-1;
        else lmax=C*(R-2)-1;
    }
    else
    {
        if (border)
            lmax=(C-1)*(R-3)/4;
        else
            lmax=C*(R-2)/4;
    }
    if (l>lmax)
        l=lmax;
    if (foodX<0+border || foodY<0+border || foodX>=C-border || foodY>=R-border-2)
        genfruit=1;
    init=0;
}

void randomgen(short &x,short &y)
{
    srand(time(0)+rand()%10);
    if (!init)
    {
        if (border)
        {
            vmaxX=gridX-2;
            vmaxY=gridY-4;
            vmin=1;
        }
        else
        {
            vmaxX=gridX-1;
            vmaxY=gridY-3;
            vmin=0;
        }
        init=1;
    }
    do
    {
        x=vmin+rand()%(vmaxX-vmin);
        y=vmin+rand()%(vmaxY-vmin);
    }
    while (x==foodX && y==foodY);
}

void timer_callback(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/FPS,timer_callback,0);
    FPS=sFPS;
}
void RenderString(float x, float y, void *font, const unsigned char* string,float r,float g,float b)
{
    glColor3f(r,g,b);
    glRasterPos2f(x, y);
    glutBitmapString(font, string);
}
void fmeneu()
{
    short i;
    unsigned char aux1[12]="1) Joc Nou",aux2[12]="2) Optiuni",aux3[12]="Optiuni:",aux4[16]="q/esc) Iesire",aux5[12]="4) Inapoi",aux6[20]="1) Bariera margine",aux7[25]="2) Linii ajutatoare",aux8[26]="3) Lungime sarpe limitata";
    unsigned char auxy[3]="Da",auxn[3]="Nu",aux9[20]="3) Schema Taste",aux10[10]="Taste:",aux11[20]="Joc pus pe pauza",aux13[100]="Apasa tasta p pentru a opri pauza sau tasta 4 pentru a reveni la meniul principal";
    unsigned char aux14[50]="W/A/S/D sau sageti-schimba directia sarpelui",aux15[100]="ATENTIE!Cat timp tasta este apasata mai mult de o secunda sarpele se va misca mai repede",aux16[15]="p-pauza joc",aux17[73]="4/4+alt-revenire la meniu si abandonare joc sau accesare meniu precedent",aux18[21]="q/esc-iesire din joc";
    unsigned char aux19[17]="4)Lista scoruri",aux25[8]="Top 10:";
    unsigned char aux20[13]="Joc Terminat",aux21[6]="Nume:",aux22[62]="Enter-introdu numele alt+4-meniu principal esc-inchide joc",aux23[15]="Nume introdus!",aux24[32]="Scorul tau nu se afla in top 10";
    unsigned char aux26[7]="1)Usor",aux27[8]="2)Mediu",aux28[7]="3)Greu";
    if (meneu==1)
    {
        if (mousestate==1)
            RenderString (2,R-1,GLUT_BITMAP_HELVETICA_18,aux1,0,1.0,0);
        else
            RenderString (2,R-1,GLUT_BITMAP_HELVETICA_18,aux1,1.0,1.0,1.0);
        if (mousestate==2)
            RenderString (2,R-2,GLUT_BITMAP_HELVETICA_18,aux2,0,1.0,0);
        else
            RenderString (2,R-2,GLUT_BITMAP_HELVETICA_18,aux2,1.0,1.0,1.0);
        if (mousestate==3)
            RenderString (2,R-3,GLUT_BITMAP_HELVETICA_18,aux9,0,1.0,0);
        else
            RenderString (2,R-3,GLUT_BITMAP_HELVETICA_18,aux9,1.0,1.0,1.0);
        if (mousestate==4)
            RenderString (2,R-4,GLUT_BITMAP_HELVETICA_18,aux19,0,1.0,0);
        else
            RenderString (2,R-4,GLUT_BITMAP_HELVETICA_18,aux19,1.0,1.0,1.0);
        if (mousestate==5)
            RenderString (2,R-5,GLUT_BITMAP_HELVETICA_18,aux4,0,1.0,0);
        else
            RenderString (2,R-5,GLUT_BITMAP_HELVETICA_18,aux4,1.0,1.0,1.0);
    }
    else if (meneu==2)
    {
        RenderString (2,R-1,GLUT_BITMAP_HELVETICA_18,aux3,1.0,1.0,1.0);
        if (mousestate==1)
            RenderString (2,R-2,GLUT_BITMAP_HELVETICA_18,aux6,1.0,0.64,0.0);
        else
            RenderString (2,R-2,GLUT_BITMAP_HELVETICA_18,aux6,1.0,1.0,1.0);
        if (border)
            RenderString (10,R-2,GLUT_BITMAP_HELVETICA_18,auxy,0.0,1.0,0.0);
        else
            RenderString (10,R-2,GLUT_BITMAP_HELVETICA_18,auxn,1.0,0.0,0.0);
        if (mousestate==5)
            RenderString (2,R-3,GLUT_BITMAP_HELVETICA_18,aux7,1.0,0.64,0.0);
        else
            RenderString (2,R-3,GLUT_BITMAP_HELVETICA_18,aux7,1.0,1.0,1.0);
        if (lines)
            RenderString (9.5,R-3,GLUT_BITMAP_HELVETICA_18,auxy,0.0,1.0,0.0);
        else
            RenderString (9.5,R-3,GLUT_BITMAP_HELVETICA_18,auxn,1.0,0.0,0.0);
        if (mousestate==3)
            RenderString (2,R-4,GLUT_BITMAP_HELVETICA_18,aux8,1.0,0.64,0.0);
        else
            RenderString (2,R-4,GLUT_BITMAP_HELVETICA_18,aux8,1.0,1.0,1.0);
        if (llimit)
            RenderString (12.5,R-4,GLUT_BITMAP_HELVETICA_18,auxy,0.0,1.0,0.0);
        else
            RenderString (12.5,R-4,GLUT_BITMAP_HELVETICA_18,auxn,1.0,0.0,0.0);
        if (mousestate==4)
            RenderString (2,R-6,GLUT_BITMAP_HELVETICA_18,aux5,1.0,0.64,0.0);
        else
            RenderString (2,R-6,GLUT_BITMAP_HELVETICA_18,aux5,1.0,1.0,1.0);
    }
    else if (meneu==3)
    {
        RenderString (2,R-1,GLUT_BITMAP_HELVETICA_18,aux10,1.0,1.0,1.0);
        RenderString (2,R-2,GLUT_BITMAP_HELVETICA_18,aux14,1.0,1.0,1.0);
        RenderString (2,R-3,GLUT_BITMAP_HELVETICA_18,aux15,1.0,0.64,0.0);
        RenderString (2,R-4,GLUT_BITMAP_HELVETICA_18,aux16,1.0,1.0,1.0);
        RenderString (2,R-5,GLUT_BITMAP_HELVETICA_18,aux17,1.0,1.0,1.0);
        RenderString (2,R-6,GLUT_BITMAP_HELVETICA_18,aux18,1.0,1.0,1.0);
        if (mousestate==1)
            RenderString (2,R-8,GLUT_BITMAP_HELVETICA_18,aux5,0,1.0,0);
        else
            RenderString (2,R-8,GLUT_BITMAP_HELVETICA_18,aux5,1.0,1.0,1.0);
    }
    else if (meneu==4)
    {
        RenderString (C/2,R/2,GLUT_BITMAP_HELVETICA_18,aux11,1.0,1.0,1.0);
        RenderString (C/2,R/2-1,GLUT_BITMAP_HELVETICA_18,aux12,1.0,1.0,1.0);
        itoa(score,c,10);
        memcpy(c1,c,strlen(c));
        RenderString (C/2+3,R/2-1,GLUT_BITMAP_HELVETICA_18,c1,1.0,1.0,1.0);
        RenderString (C/2-13,R/2-2,GLUT_BITMAP_HELVETICA_18,aux13,1.0,1.0,1.0);
    }
    else if (meneu==5)
    {
        RenderString (C/2,R-3,GLUT_BITMAP_TIMES_ROMAN_24,aux25,1.0,1.0,1.0);
        for (i=0; i<=n && i<10; i++)
        {
            itoa (i+1,c,10);
            short aux=strlen(c);
            memcpy(c1,c,strlen(c));
            memcpy(c1+strlen(c),")",1);
            memcpy(c1+strlen(c)+1,topnume[i],strlen(topnume[i]));
            itoa(topscor[i],c,10);
            memcpy(c1+aux+strlen(topnume[i])+1,": ",2);
            memcpy(c1+aux+strlen(topnume[i])+3,c,strlen(c));
            switch(i)
            {
            case 0:
                RenderString (C/2,R-5-i,GLUT_BITMAP_HELVETICA_18,c1,1,0.874,0);
                break;
            case 1:
                RenderString (C/2,R-5-i,GLUT_BITMAP_HELVETICA_18,c1,0.752,0.752,0.752);
                break;
            case 2:
                RenderString (C/2,R-5-i,GLUT_BITMAP_HELVETICA_18,c1,0.803,0.498,0.196);
                break;
            default:
                RenderString (C/2,R-5-i,GLUT_BITMAP_HELVETICA_18,c1,1.0,1.0,1.0);
                break;
            }
            memset(c1,0,51);
        }
        if (mousestate==1)
            RenderString (5,R-4-i-1,GLUT_BITMAP_HELVETICA_18,aux5,0,1,0);
        else
            RenderString (5,R-4-i-1,GLUT_BITMAP_HELVETICA_18,aux5,1,1,0.94);
        poz=4+i;
    }
    else if (meneu==6)
    {
        RenderString (C/2-2.2,R/2+7,GLUT_BITMAP_TIMES_ROMAN_24,aux20,1.0,0.0,0.0);
        if (score>topscor[9])
            if (!numeintrodus)
            {
                RenderString (C/2-2.2,R/2+5.5,GLUT_BITMAP_HELVETICA_18,aux21,1.0,1.0,1.0);
                RenderString (C/2+1,R/2+5.5,GLUT_BITMAP_HELVETICA_18,nume,1.0,1.0,1.0);
            }
            else RenderString (C/2-2.2,R/2+5.5,GLUT_BITMAP_HELVETICA_18,aux23,1.0,1.0,1.0);
        else
            RenderString (C/2-2.2,R/2+5.5,GLUT_BITMAP_HELVETICA_18,aux24,1.0,1.0,1.0);
        RenderString (C/2-10,R/2+4.5,GLUT_BITMAP_HELVETICA_18,aux22,1.0,1.0,1.0);
    }
    else if (meneu==7)
    {
        if (mousestate==5)
            RenderString (2.2,R-1,GLUT_BITMAP_HELVETICA_18,aux26,0,1.0,0);
        else
            RenderString (2.2,R-1,GLUT_BITMAP_HELVETICA_18,aux26,1.0,1.0,1.0);
        if (mousestate==2)
            RenderString (2,R-2,GLUT_BITMAP_HELVETICA_18,aux27,0,1.0,0);
        else
            RenderString (2,R-2,GLUT_BITMAP_HELVETICA_18,aux27,1.0,1.0,1.0);
        if (mousestate==3)
            RenderString (2,R-3,GLUT_BITMAP_HELVETICA_18,aux28,0,1.0,0);
        else
            RenderString (2,R-3,GLUT_BITMAP_HELVETICA_18,aux28,1.0,1.0,1.0);
        if (mousestate==4)
            RenderString (2,R-4,GLUT_BITMAP_HELVETICA_18,aux5,0,1.0,0);
        else
            RenderString (2,R-4,GLUT_BITMAP_HELVETICA_18,aux5,1.0,1.0,1.0);
    }
}
void keyboard_callback(int key,int,int)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        if (sd!=down)
        {
            if (sd==up)
                FPS=fFPS;
            else FPS=sFPS;
            sd=up;
        }
        break;
    case GLUT_KEY_DOWN:
        if (sd!=up)
        {
            if (sd==down)
                FPS=fFPS;
            else FPS=sFPS;
            sd=down;
        }
        break;
    case GLUT_KEY_LEFT:
        if (sd!=rght)
        {
            if (sd==lft)
                FPS=fFPS;
            else FPS=sFPS;
            sd=lft;
        }
        break;
    case GLUT_KEY_RIGHT:
        if (sd!=lft)
        {
            if (sd==rght)
                FPS=fFPS;
            else FPS=sFPS;
            sd=rght;
        }
        break;
    default:
        break;
    }
}
void keyboard_callback2(unsigned char key,int,int)
{
    if (meneu==1)
        switch (key)
        {
        case 'q':
        case 'Q':
        case 27:
            writedata();
            exit(0);
            break;
        case '1':
            meneu=7;
            break;
        case '2':
            meneu=2;
            break;
        case '3':
            meneu=3;
            break;
        case '4':
            meneu=5;
            break;
        }
    else if (meneu==2)
        switch (key)
        {
        case 'q':
        case 'Q':
        case 27:
            writedata();
            exit(0);
            break;
        case '1':
            border=!border;
            break;
        case '2':
            lines=!lines;
            break;
        case '3':
            llimit=!llimit;
            break;
        case '4':
            meneu=1;
            break;
        }
    else if (meneu==3)
        switch(key)
        {
        case '4':
            meneu=1;
            break;
        case'q':
        case 'Q':
        case 27:
            writedata();
            exit(0);
            break;
        }
    else if (meneu==4)
        switch (key)
        {
        case 'p':
        case 'P':
            resume=1;
            meneu=0;
            break;
        case 'q':
        case 'Q':
        case 27:
            writedata();
            exit(0);
            break;
        case '4':
            sd=up;
            headX[0]=C/2;
            headY[0]=R/2;
            maininit();
            l=4;
            tscore=score=0;
            genfruit=1;
            sFPS=10;
            meneu=1;
            break;
        }
    else if (meneu==5)
        switch(key)
        {
        case 'q':
        case 'Q':
        case 27:
            writedata();
            exit(0);
            break;
        case '4':
            meneu=1;
            break;
        }
    else if (meneu==6)
    {
        switch (key)
        {
        case 27:
            score=0;
            writedata();
            exit(0);
            break;
        case 13:
            if (strlen(aux))
            {
                if (!numeintrodus)
                    writescore();
                numeintrodus=1;
            }
            break;
        case 127:
        case 8:
            memset(nume+(strlen(aux)-1),0,1);
            memset(aux+(strlen(aux)-1),0,1);
            break;
        default:
            if (strlen(aux)<50)
            {
                aux[strlen(aux)]=key;
                memcpy(nume,aux,strlen(aux));
            }
            break;
        }
        int mod=glutGetModifiers();
        if (key=='4')
            if (mod==GLUT_ACTIVE_ALT)
            {
                gameover=0;
                window=0;
                numeintrodus=0;
                memset(aux,0,50);
                memset(nume,0,50);
                sd=up;
                headX[0]=C/2;
                headY[0]=R/2;
                maininit();
                l=4;
                sFPS=10;
                tscore=score=0;
                genfruit=1;
                meneu=1;
            }
    }
    else if (!meneu)
        switch (key)
        {
        case 'p':
        case 'P':
            meneu=4;
            break;
        case 'q':
        case 'Q':
        case 27:
            writedata();
            exit(0);
            break;
        case '4':
            sd=up;
            headX[0]=C/2;
            headY[0]=R/2;
            maininit();
            l=4;
            tscore=score=0;
            genfruit=1;
            sFPS=10;
            meneu=1;
            break;
        case 'w':
        case 'W':
            if (sd!=down)
            {
                if (sd==up)
                    FPS=fFPS;
                else FPS=sFPS;
                sd=up;
            }
            break;
        case 's':
        case 'S':
            if (sd!=up)
            {
                if (sd==down)
                    FPS=fFPS;
                else FPS=sFPS;
                sd=down;
            }
            break;
        case 'a':
        case 'A':
            if (sd!=rght)
            {
                if (sd==lft)
                    FPS=fFPS;
                else FPS=sFPS;
                sd=lft;
            }
            break;
        case 'd':
        case 'D':
            if (sd!=lft)
            {
                if (sd==rght)
                    FPS=fFPS;
                else  FPS=sFPS;
                sd=rght;
            }
            break;
        default:
            break;
        }
    else if (meneu==7)
        switch(key)
        {
        case '1':
            fpsmod=0.175;
            difmod=0.7;
            meneu=0;
            break;
        case '2':
            fpsmod=0.30;
            difmod=1;
            meneu=0;
            break;
        case '3':
            fpsmod=0.45;
            difmod=1.5;
            meneu=0;
            break;
        case '4':
            meneu=1;
            break;
        }
    if (key=='4')
        memset(c1,0,4);
}

void mouse_callback(int x,int y)
{
    x/=20;
    y/=20;
    if (meneu==1)
        if (x>=2 && x<=6 && !y)
            mousestate=1;
        else if (x>=2 && x<=5 && y==1)
            mousestate=2;
        else if (x>=2 && x<=8 && y==2)
            mousestate=3;
        else if (x>=2 && x<=7 && y==3)
            mousestate=4;
        else if (x>=2 && x<=6 && y==4)
            mousestate=5;
        else mousestate=0;
    if (meneu==2)
        if (x>=2 && x<=11 && y==1)
            mousestate=1;
        else if (x>=2 && x<=10 && y==2)
            mousestate=5;
        else if (x>=2 && x<=13 && y==3)
            mousestate=3;
        else if (x>=2 && x<=5 && y==5)
            mousestate=4;
        else
            mousestate=0;
    if (meneu==3)
        if(x>=2 && x<=5 && y==7 )
            mousestate=1;
        else
            mousestate=0;
    if (meneu==5)
        if (x>=5 && x<=8 && y==poz)
            mousestate=1;
        else
            mousestate=0;
    if (meneu==7)
        if (x>=2 && x<=4 && y==0)
            mousestate=5;
        else if (x>=2 && x<=5 && y==1)
            mousestate=2;
        else if (x>=2 && x<=4 && y==2)
            mousestate=3;
        else if (x>=2 && x<=5 && y==3)
            mousestate=4;
        else
            mousestate=0;
}

void mouse_callback2(int button,int state,int x,int y)
{
    if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        if (meneu==1)
            if (mousestate==1)
                meneu=7;
            else if (mousestate==2 || mousestate==3)
                meneu=mousestate;
            else if (mousestate==4)
                meneu=5;
            else if (mousestate==5)
            {
                writedata();
                exit(0);
            }
        if (meneu==2)
            if (mousestate==1)
                border=!border;
            else if (mousestate==5)
                lines=!lines;
            else if (mousestate==3)
                llimit=!llimit;
            else if (mousestate==4)
                meneu=1;
        if (meneu==3)
            if (mousestate==1)
                meneu=1;
        if (meneu==5)
            if (mousestate==1)
                meneu=1;
        if (meneu==7)
            if (mousestate==5)
            {
                fpsmod=0.175;
                difmod=0.7;
                meneu=0;
            }
            else if (mousestate==2)
            {
                fpsmod=0.30;
                difmod=1;
                meneu=0;
            }
            else if (mousestate==3)
            {
                fpsmod=0.45;
                difmod=1.5;
                meneu=0;
            }
            else if (mousestate==4)
                meneu=1;
    }
}

void drawSquare(int x,int y)
{
    glLineWidth(1.0);
    if (border &&(x==0 || y==0 || x==C-1 || y==R-3))
    {
        glColor3f(0.7,0.0,0.0);
        glBegin(GL_POLYGON);
    }
    else if (lines)
    {
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_LINE_LOOP);
    }
    glVertex2f(x,y);
    glVertex2f(x+1,y);
    glVertex2f(x+1,y+1);
    glVertex2f(x,y+1);
    glEnd();
}
void drawGrid()
{
    for (int i=0; i<gridY; i++)
        for (int j=0; j<gridX-2; j++)
            drawSquare(i,j);

}
void drawSnake()
{
    if (headX[0]==-1 || headY[0]==-1 || headX[0]==C || headY[0]==R-2)
    {
        switch(sd)
        {
        case up:
            headY[0]=0;
            break;
        case down:
            headY[0]=R-3;
            break;
        case lft:
            headX[0]=C;
            break;
        case rght:
            headX[0]=0;
            break;
        }
    }
    for (int i=1; i<l; i++)
        if (headX[0]==headX[i] && headY[0]==headY[i])
            gameover=true;
    if (border)
        if (headX[0]==0 || headX[0]==C-1 || headY[0]==0 || headY[0]==R-3)
            gameover=true;
    for (i=l-1+gameover; i>0; i--)
    {
        headX[i]=headX[i-1];
        headY[i]=headY[i-1];
    }
    switch (sd)
    {
    case up:
        headY[0]++;
        break;
    case down:
        headY[0]--;
        break;
    case lft:
        headX[0]--;
        break;
    case rght:
        headX[0]++;
        break;
    default:
        break;
    }
    for (i=0+2*gameover; i<l+gameover; i++)
    {
        if (i==0)
        {
            glColor3f(0.0,1.0,0.0);
            glBegin(GL_TRIANGLES);
            dettriangle(i);
            glEnd();
        }
        else
        {
            if (gameover)
                snaker=snakeg=snakeb=0.5;
            else
            {
                if (1<l*0.008)
                {
                    snaker=snakeg=0;
                    snakeb=1;
                }
                else
                {
                    snaker=snakeg=1-l*0.008;
                    snakeb=1;
                }
            }
            glColor3f(snaker,snakeg,snakeb);
            glRectf(headX[i],headY[i],headX[i]+1,headY[i]+1);
        }
    }
    if (gameover)
    {
        glColor3f(0.0,0.5,0.0);
        glBegin(GL_TRIANGLES);
        dettriangle(1);
        glEnd();
    }
    auto duration=duration_cast<milliseconds>(t2-t1);
    if (headX[0]==specfoodX && headY[0]==specfoodY && l%5==0 && duration.count()/1000.0<=6.01)
    {
        coada.push_back(make_pair(0,2));
        tscore+=5;
        l++;
        if (l>lmax)
            l=lmax;
        score=(int)(tscore*difmod);
    }
    if ((headX[0]==foodX && headY[0]==foodY))
    {
        genfruit=true;
        coada.push_back(make_pair(0,1));
        tscore++;
        l++;
        sFPS+=fpsmod;
        if (l>lmax)
            l=lmax;
        score=(int)(tscore*difmod);
    }
    for (i=0; i<coada.size(); i++)
    {
        if (coada[i].second==1)
            glColor3f(0,0,0.7);
        else
            glColor3f(1,0.75,0.79);
        if (coada[i].first>0)
            glRectf(headX[coada[i].first]-0.25,headY[coada[i].first]-0.25,headX[coada[i].first]+1.25,headY[coada[i].first]+1.25);
        coada[i].first++;
        if (i==0 && coada[i].first>=l)
            coada.erase(coada.begin(),coada.begin()+1);
    }
}
void genFruit()
{
    glColor3f(0.0,0.0,0.7);
    for (int i=1; i<l; i++)
    {
        if (foodX==headX[i] && foodY==headY[i])
            randomgen(foodY,foodX);
        if (specfoodX==headX[i] && specfoodY==headY[i] && l%5==0)
            randomgen(specfoodY,specfoodX);
    }

    if (foodX<0+border || foodY<0+border || foodX>=C-border || foodY>=R-border-2)
        genfruit=1;
    auto duration=duration_cast<milliseconds>(t2-t1);
    if (genfruit)
    {
        randomgen(foodY,foodX);
        genfruit=false;
    }
    if (l%5==0)
    {
        if (!specfoodgen)
        {
            t1=t2=steady_clock::now();
            randomgen(specfoodY,specfoodX);
            randomgen(specfoodY,specfoodX);
            specfoodgen=1;
            anticheat=0;
        }
        if ((foodX==specfoodX && foodY==specfoodY) || specfoodX<0+border || specfoodY<0+border || specfoodX>=C-border || specfoodY>=R-border-2)
        {
            t1=t2=steady_clock::now();
            randomgen(specfoodY,specfoodX);
        }
    }
    glRectf(foodX,foodY,foodX+1,foodY+1);
    if (l%5==0 && duration.count()/1000.0<=6.01)
    {
        t2=steady_clock::now();
        duration = duration_cast<milliseconds>(t2-t1);
        glColor3f(1,0.75,0.79);
        glRectf(specfoodX,specfoodY,specfoodX+1,specfoodY+1);
    }
    else if (l%5!=0)
        specfoodgen=0;

}
void display_callback()
{
     if (resume)
        if (!anticheat)
       t1=steady_clock::now();
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    if (meneu)
        fmeneu();
    else
    {
        char aux[3];
        drawGrid();
        drawSnake();
        genFruit();
        itoa(score,c,10);
        memcpy(c1,c,strlen(c));
        RenderString (2,R-1,GLUT_BITMAP_HELVETICA_18,aux12,1.0-1.0*score/50,0.0+1.0*score/50,0);
        RenderString (4.5,R-1,GLUT_BITMAP_HELVETICA_18,c1,1.0-1.0*score/50,0.0+1.0*score/50,0);
        auto duration=duration_cast<milliseconds>(t2-t1);
        if (duration.count()/1000.0<=6.01 && l%5==0)
        {
            itoa(6000-duration.count(),c,10);
            memcpy(c1,c,strlen(c));
            memcpy(aux,c1+1,2);
            memcpy(c1+1,".",1);
            memcpy(c1+2,aux,2);
            RenderString (C/2,R-1,GLUT_BITMAP_HELVETICA_18,c1,1.0-(6-duration.count()/1000.0)*0.20,0.0+(6-duration.count()/1000.0)*0.20,0);
            memset(c1,0,50);
        }
    }
    glutSwapBuffers();
    if (gameover)
    {
        char c[10];
        itoa(score,c,10);
        char msg[20]="Scor: ";
        strcat(msg,c);
        if (!window)
        {
            MessageBox(NULL,msg,"Joc Terminat",0);
            window=1;
        }
        meneu=6;
    }
    if (resume)
    {
        if (!anticheat)
       t1=steady_clock::now();
        _sleep(2000);
       if (!anticheat)
       {t1=steady_clock::now();
       anticheat=!anticheat;
       }
        resume=!resume;
    }
}

void closefunc()
{
    writedata();
            exit(0);
}

void start()
{
    glClearColor(0.0,0.0,0.0,1.0);
    initGrid(R,C);
}
void reshape_callback(int W,int H)
{
    maininit();
    glViewport(0,0,(GLsizei)W,(GLsizei)H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,C,0.0,R,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
}
int main (int argc, char **argv)
{
    char ch[51];
    directory=argv[0];
    directory.erase(directory.find_last_of('\\')+1);
    directory+="data.txt";
    f.open(directory,fstream::in);
    f.getline(ch,6);
    llimit=ch[0]-'0';
    lines=ch[2]-'0';
    border=ch[4]-'0';
    f.getline(ch,2);
    n=ch[0]-'0';
    if (llimit<0)
        llimit=lines=border=1;
    if (n<0)
        n=-1;
    for (i=0; i<=n; i++)
    {
        f.getline(topnume[i],51);
        f.getline(ch,4);
        topscor[i]=atoi(ch);
    }
    f.close();
    f.open(directory,fstream::out);
    f.close();
    glutInit(&argc,argv);
    W=glutGet(GLUT_SCREEN_WIDTH)-200;
    H=glutGet(GLUT_SCREEN_HEIGHT)-160;
    C=W/20;
    R=H/20;
    headX[0]=C/2;
    headY[0]=R/2;
    if (!llimit)
    {
        if (border) lmax=C*R-2*C-2*R-1;
        else lmax=C*R-1;
    }
    else
    {
        if (border)
            lmax=(C-1)*(R-1)/4;
        else
            lmax=C*R/4;
    }
    for (i=1; i<l; i++)
    {
        headY[i]=headY[i-1]-1;
        headX[i]=headX[0];
    }
    glutInitWindowSize(W,H);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-W)/2,(glutGet(GLUT_SCREEN_HEIGHT)-H)/2-40);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Joc Snake");
    start();
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutTimerFunc(0,timer_callback,0);
    glutSpecialFunc(keyboard_callback);
    glutKeyboardFunc(keyboard_callback2);
    glutPassiveMotionFunc(mouse_callback);
    glutMouseFunc(mouse_callback2);
    glutCloseFunc(closefunc);
    glutMainLoop();
}


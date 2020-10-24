#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <string.h>

int temporizador=0, flag=0, solo[3][75], cactos[4][92], tela[17][77], score=0, hi_score=0;

char mensagem[23]={"Press Spacebar to begin"}, pausa[5]={"Pause"}, perdeu[9]={"Game Over"}, close[22]={"Press Spacebar to exit"}, placar[6]={"Score:"};

void configtela(){ //fun��o q configura o tamanho da tela e do buffer
    COORD outbuff;
    outbuff.X = 77; // tem que ser 1 maior que o tamanho X
    outbuff.Y = 17; // tem que ser 1 maior que o tamanho Y
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(hConsole, outbuff);
    Sleep(130);
    SMALL_RECT windowSize = {0, 0, 77, 17};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void moldura(int i){
    int a,b, cor1=176, cor2=177, cor3=178;
    if(i==1){
        for(a=0;a<17;a++){
            for(b=0;b<77;b++){
                if(a==0 || a==16){
                    tela[a][b]=cor1;
                }else{
                    if(b==0 || b==76){
                        tela[a][b]=cor1;
                    }
                }
            }
        }
    }else if(i==2){
        for(a=0;a<17;a++){
            for(b=0;b<77;b++){
                if(a==0 || a==16){
                    tela[a][b]=cor2;
                }else{
                    if(b==0 || b==76){
                        tela[a][b]=cor2;
                    }
                }
            }
        }
    }else if(i==3){
        for(a=0;a<17;a++){
            for(b=0;b<77;b++){
                if(a==0 || a==16){
                    tela[a][b]=cor3;
                }else{
                    if(b==0 || b==76){
                        tela[a][b]=cor3;
                    }
                }
            }
        }
    }
}

int dinossauro(int i, int x, int y){
    if(i==1){
    int dinossauro1[6][9]={{32,32,32,32,32,201,205,79,187},{32,32,32,32,32,204,203,205,188},{32,32,32,201,205,188,204,205,187},{204,205,205,188,32,32,186,32,32},{200,205,203,205,205,203,188,32,32},{32,32,186,32,32,186,32,32,32}};
    return(dinossauro1[x][y]);
    }else if(i==2){
    int dinossauro2[6][9]={{32,32,32,32,32,201,205,79,187},{32,32,32,32,32,204,203,205,188},{32,32,32,201,205,188,204,205,187},{204,205,205,188,32,32,186,32,32},{200,205,203,205,205,203,188,32,32},{196,196,186,196,196,200,196,196,196}};
    return(dinossauro2[x][y]);
    }else if(i==3){
    int dinossauro3[6][9]={{32,32,32,32,32,201,205,79,187},{32,32,32,32,32,204,203,205,188},{32,32,32,201,205,188,204,205,187},{204,205,205,188,32,32,186,32,32},{200,205,203,205,205,203,188,32,32},{196,196,200,196,196,186,196,196,196}};
    return(dinossauro3[x][y]);
    }else if(i==4){
    int dinossauro4[6][9]={{32,32,32,32,32,201,205,88,187},{32,32,32,32,32,204,203,205,188},{32,32,32,201,205,188,204,205,187},{204,205,205,188,32,32,186,32,32},{200,205,203,205,205,203,188,32,32},{32,32,186,32,32,186,32,32,32}};
    return(dinossauro4[x][y]);
    }else if(i==0){
    int dinossauro1[6][9]={{32,32,32,32,32,201,205,79,187},{32,32,32,32,32,204,203,205,188},{32,32,32,201,205,188,204,205,187},{204,205,205,188,32,32,186,32,32},{200,205,203,205,205,203,188,32,32},{196,196,186,196,196,186,196,196,196}};
    return(dinossauro1[x][y]);
    }
}

int cacto(int i, int x, int y){
    if(i==1){
        int cacto1[4][15]={{32,32,32,32,32,32,32,32,32,32,32,32,32,32,32},{176,32,176,32,176,32,176,32,176,32,176,176,32,176,176},{32,176,176,32,176,32,176,176,32,176,32,176,32,176,32},{196,196,176,176,196,176,196,176,196,176,176,196,176,196,196}};
        return(cacto1[x][y]);
    }else if(i==2){
        int cacto2[4][15]={{32,32,32,32,32,32,32,32,32,32,32,32,32,32,32},{32,32,32,32,32,176,32,176,176,32,176,32,32,32,32},{32,32,32,32,32,32,176,176,176,176,32,32,32,32,32},{196,196,196,196,196,196,196,176,176,196,196,196,196,196,196}};
        return(cacto2[x][y]);
    }else if(i==3){
        int cacto3[4][15]={{32,32,32,32,32,176,32,176,176,32,176,32,32,32,32},{32,32,32,32,32,32,176,32,32,176,32,32,32,32,32},{32,32,32,32,32,32,32,176,176,32,32,32,32,32,32},{196,196,196,196,196,196,196,176,176,196,196,196,196,196,196}};
        return(cacto3[x][y]);
    }else if(i==4){
        int cacto4[4][15]={{176,32,176,32,32,176,32,176,32,32,176,176,32,176,176},{176,32,176,32,176,32,176,32,176,32,176,176,32,176,176},{32,176,176,32,176,32,176,176,32,176,32,176,32,176,32},{196,196,176,176,196,176,196,176,196,176,176,196,176,196,196}};
        return(cacto4[x][y]);
    }
}

void pista(int i){
    int chao[75], pedras1[75], pedras2[75], a, b, pedra[75], solo_reserva[3][1];
    /*pista est�tica*/if(i==0){
        //cria ch�o e pedras
            for(a=0;a<75;a++){
                chao[a]=196;
            }
            srand(time(NULL));
            for(b=0;b<75;b++){pedra[b] = (rand() % 101);}
            for(b=0;b<75;b++){
                if(pedra[b]<33){pedras1[b]=45;}else if(pedra[b]>=33 && pedra[b]<66){pedras1[b]=46;}else{pedras1[b]=32;}
            }
            for(b=0;b<75;b++){
                if(pedra[b]<33){pedras2[b]=32;}else if(pedra[b]>=33 && pedra[b]<66){pedras2[b]=45;}else{pedras2[b]=46;}
            }


        //monta a matriz solo(int)
        for(a=0;a<75;a++){solo[0][a]=chao[a];}
        for(a=0;a<75;a++){solo[1][a]=pedras1[a];}
        for(a=0;a<75;a++){solo[2][a]=pedras2[a];}
    }
    /*pista dinamica*/
    else if(i==1){
        for(a=0;a<3;a++){
            for(b=0;b<75;b++){
                if(b==0){
                    solo_reserva[a][b]=solo[a][b];
                    solo[a][b]=solo[a][b+1];
                }else if(b==74){
                    solo[a][b]=solo_reserva[a][0];
                }else{
                    solo[a][b]=solo[a][b+1];
                }
            }
        }
    }
}

void obstaculo(int i){
    int x,y,temp1;
    /*printa cacto*/if(i==1){
        srand(time(NULL));
        temp1=(rand()%4)+1;
        for(x=0;x<4;x++){for(y=0;y<15;y++){cactos[x][y+76]=cacto(temp1,x,y);}}
    }/*atualiza*/else if(i==2){
        for(x=0;x<4;x++){
            for(y=0;y<92;y++){
                if(y!=91){cactos[x][y]=cactos[x][y+1];}else if(y==91 && x==3){cactos[x][y]=196;}
            }
        }
    }/*cria um risco*/else if(i==0){
        for(x=0;x<92;x++){cactos[3][x]=196;}
    }
}

void setup_program(){
    int x,y,a=1,b=1;
    char t;
    pista(0);
    while(1){
    //imprime moldura, ch�o, obstaculo, nuvem, dino e score parados
    /*configura console*/ configtela();
    system("cls");
    /*limpa*/for(x=0;x<17;x++){for(y=0;y<77;y++){tela[x][y]=32;}}
    /*moldura*/moldura(b);
    /*pista*/for(x=0;x<3;x++){for(y=0;y<75;y++){tela[x+13][y+1]=solo[x][y];}}
    /*dinossauro*/for(x=0;x<6;x++){for(y=0;y<9;y++){tela[x+8][y+3]=dinossauro(0,x,y);}}
    /*mensagem*/if(a==1){x=8; for(y=0;y<23;y++){tela[x][y+27]=mensagem[y];}}
    /*printa tela cheia*/for(x=0;x<17;x++){for(y=0;y<77;y++){printf("%c", tela[x][y]);}printf("\n");}
    /*espera tecla espa�o para come�ar*/t=0; if(kbhit()){t=getch();} if(t==32){break;}
    if(a==1){a--;}else{a++;}
    if(b==3){b=1;}else{b++;}
    Sleep(50);
    }
}

int main(void){
    ///variav�is
    int flag1,x,y,dino_run=2, dino_sprite=0, posicao=0, t, temp2=0;
    float a=0, temp1=0;
    obstaculo(0);

    ///setup
    setup_program();

    ///loop principal
    while(1){

        ///imprime tela
        system("cls");
        for(x=0;x<17;x++){for(y=0;y<77;y++){tela[x][y]=32;}}
        /*moldura*/moldura(2);
        /*pista*/for(x=0;x<3;x++){for(y=0;y<75;y++){tela[x+13][y+1]=solo[x][y];}}
        /*obstaculo*/for(x=0;x<4;x++){for(y=0;y<75;y++){tela[x+10][y+1]=cactos[x][y];}}
        /*dinossauro*/for(x=0;x<6;x++){for(y=0;y<9;y++){tela[x+8-posicao][y+3]=dinossauro(dino_sprite,x,y);}}
        /*printa tela cheia*/for(x=0;x<17;x++){for(y=0;y<77;y++){printf("%c", tela[x][y]);}printf("\n");}

        ///zera var da tecla e verifica tecla
        fflush(stdin);
        t=0; if(kbhit()){t=getch();}
        fflush(stdin);

        ///comandos de tecla
        if/*sair*/(t==27 || t==13){
            return (0);
        }/*pulo*/else if(t==32){
            flag1=1;
        }/*pause*/else if(t=='p'){
            while(1){
                system("cls");
                /*gameover*/x=8; for(y=0;y<5;y++){tela[x][y+36]=pausa[y];}
                /*printa tela cheia*/for(x=0;x<17;x++){for(y=0;y<77;y++){printf("%c", tela[x][y]);}printf("\n");}
                fflush(stdin);
                t=0; if(kbhit()){t=getch();}
                if(t=='p'){break;}
                Sleep(temporizador);
            }
        }
        //executa fun��o de movimento
        if(flag1==1){
            dino_sprite=1;
            a+=0.1;
            temp1 = (((-28)*(pow(a,2)/2.25))+((28)*(a)/1.5));
            posicao = lroundf(temp1);
            fflush(stdin);
            t=0; if(kbhit()){t=getch();}
            if(posicao==0){flag1=0;a=0.0;}
        }

        ///verifica colis�o
        //verifica sobreposi��o entre matriz obst�culo e matriz dinossauro
        /*obstaculo*/for(x=0;x<4;x++){for(y=0;y<75;y++){tela[x+10][y+1]=cactos[x][y];}}
        for(x=0;x<6;x++){
            for(y=0;y<9;y++){
                if((tela[x+8-posicao][y+3])==176){
                    while(1){
                        system("cls");
                        /*dinossauro*/for(x=0;x<6;x++){for(y=0;y<9;y++){tela[x+8-posicao][y+3]=dinossauro(dino_sprite,x,y);}}
                        /*gameover*/x=8; for(y=0;y<9;y++){tela[x][y+34]=perdeu[y];}
                        /*perdeu*/x=9; for(y=0;y<22;y++){tela[x][y+27]=close[y];}
                        /*printa tela cheia*/for(x=0;x<17;x++){for(y=0;y<77;y++){printf("%c", tela[x][y]);}printf("\n");}
                        fflush(stdin);
                        t=0; if(kbhit()){t=getch();}
                        if(t==32){return 0;}
                        Sleep(temporizador);
                    }
                }
            }
        }

        ///score e temporizador
        score++;
        if(score>225){
            temporizador=30;
        }else if(score>450){
            temporizador=20;
        }else if(score>675){
            temporizador=10;
        }else if(score<=225){
            temporizador=50;
        }

        ///atualiza ch�o
        pista(1); pista(1); pista(1);

        ///atualiza obstaculo
        srand(time(NULL));
        if(temp2==0){
            temp2=((rand()%15)+21);
        }else{
            temp2--;
            if(temp2==0){
                obstaculo(1);
            }
        }
        obstaculo(2); obstaculo(2); obstaculo(2);

        ///atualiza dinossauro
        if(dino_run==2){dino_run++;}else{dino_run--;}
        if(posicao==0){dino_sprite=dino_run;}

        Sleep(temporizador);
    }
    return(0);
}

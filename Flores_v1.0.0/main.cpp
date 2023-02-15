#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <ctime>

#define  ARIIBA    72
#define  ABAJO     80
#define  IZQUIERDA 75
#define  DERECHA   77
#define  ESC       27
using namespace std;
void gotoxy(short x, short y)
{
   COORD pos = { x, y };
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
/*Colorear*/
void color(WORD col)
{
   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),col);
}
void ocultar_cursor(bool bandera)
{
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 2; //tamanho del cursor
    cci.bVisible = bandera; // TRUE: muestra el cursosr FALSE: no muestra el cursosr
    SetConsoleCursorInfo(hCon,&cci);
}


void menu_principal();
void mover_resorte();
void juego(string,bool);
/*ESTRUCTURAS*/


typedef struct{
    int dia;
    int mes;
    int anhio;
}tipoFecha;

typedef struct{
    string name;
    string fecha;
    int puntos;
}tipoPuntuaciones;
typedef struct{
    int repeticion;
    int nivel;
    bool condicion;
}tipoNivel;
typedef struct{
    int posX;
    int posY;
    int color;
}tipoCordenadas;

typedef struct{
    int posX;
    int posY;
    int dx;
    bool bandera;
}tipoBala;
/*Dibujara el resorte*/
char resorte_1[]={'*',' ','*',0};
char resorte_2[]={'|','-','|',0};

/*Borrara el resorte*/
char borrar_resorte[]={' ',' ',' ',0};

/*DIBUJO FLOR*/
char flor_1[]={'(','*',')',0};
char flor_2[]={' ','|',' ',0};

/*CORDENADAS*/
//resortera
tipoCordenadas resortera={35,24,9};
//asteroides
tipoCordenadas aves[99];
//bala
tipoBala bala={35,24,0,false};
//Flores
tipoCordenadas flores[99];
//niveles
tipoNivel nivel={0,1,false};
void vidas(int vidas){

    gotoxy(2,1); printf("VIDAS: %d",vidas);
}
void barra_salud(int n)
{
        gotoxy(70+1,2);printf(" ");
        gotoxy(70+2,2);printf(" ");
        gotoxy(70+3,2);printf(" ");
        gotoxy(70+4,2);printf(" ");
        gotoxy(70+5,2);printf(" ");
        gotoxy(70+6,2);printf(" ");
    for(int v=0;v<n;v++)
    {
        gotoxy(70+v,2);
        printf("%c",3);
    }

}
void hoy(tipoFecha &aux){
    time_t t;
    t = time(NULL);
    struct tm*f;
    f = localtime(&t);
    aux.dia   = f->tm_mday;
    aux.mes   = f->tm_mon + 1;
    aux.anhio = f->tm_year + 1900;
}


void dibujo_menu()
{
    fstream logo;
    string linea;
    color(2);
    logo.open("logo.txt", ios::in);
    while(!logo.eof())
    {
        getline(logo,linea);
        cout<<linea<<endl;
    }
    logo.close();

}

void pintar_limites()
{
    for(int x = 2; x<78; x++){
        gotoxy(x,3);printf("%c",205);
        gotoxy(x,28);printf("%c",247);
        gotoxy(x,29);printf("%c",205);
    }
    for(int y = 4; y<29; y++){
        gotoxy(2,y);printf("%c",186);
        gotoxy(77,y);printf("%c",186);
    }
    gotoxy(2,3);printf("%c",201);
    gotoxy(2,29);printf("%c",200);
    gotoxy(77,3);printf("%c",187);
    gotoxy(77,29);printf("%c",188);
}

void  pintar_resortera()
{
    color(resortera.color);
    gotoxy(resortera.posX,resortera.posY); printf(resorte_1);
    color(resortera.color);
    gotoxy(resortera.posX,resortera.posY+1); printf(resorte_2);
}
void  borrar_resorte_resortera()
{
    gotoxy(resortera.posX,resortera.posY); printf(borrar_resorte);
    gotoxy(resortera.posX,resortera.posY+1);printf(borrar_resorte);
}

void pintar_flor(tipoCordenadas flor [],int tam)
{
    for(int i = 0; i<tam; i++){
        color(flor[i].color);
        gotoxy(flor[i].posX,flor[i].posY);
        puts(flor_1);
        gotoxy(flor[i].posX,flor[i].posY+1);
        puts(flor_2);
    }
}

void pintar_ave(tipoCordenadas ave[],int tam)
{
    for(int i = 0; i<tam; i++)
    {
    gotoxy(ave[i].posX,ave[i].posY); printf("%c",94);
    }
}
void pintar_SoloAve(tipoCordenadas ave)
{
    color(ave.color);
    gotoxy(ave.posX,ave.posY); printf("%c",94);
}
void pintar_bala()
{
    gotoxy(bala.posX,bala.posY); printf("*");
}

void abrirAch_flor(tipoCordenadas flores[],int &tamFlores)
{
    fstream archFlor;
    int x,y,c;
    int flor_tam=0;
    archFlor.open("cordenadas_flores.txt", ios::in);
    while(!archFlor.eof() && archFlor>>x>>y>>c){
    flores[flor_tam].posX=x;
    flores[flor_tam].posY=y;
    flores[flor_tam].color=c;
    flor_tam++;
    }
    archFlor.close();
    tamFlores=flor_tam;
}

void abrirAch_aves(tipoCordenadas aves[],int &tamAve)
{
    fstream archAve;
    int x,y,c;
    int ave_tam=0;
    archAve.open("cordenadas_aves.txt", ios::in);
    while(!archAve.eof() && archAve>>x>>y>>c){
    aves[ave_tam].posX=x;
    aves[ave_tam].posY=y;
    aves[ave_tam].color=c;
    ave_tam++;
    }
    archAve.close();
    tamAve=ave_tam;
}

void pintar_florGuardado(int comprobante[],tipoCordenadas flor[],int tamFlores)
{
    for (int i=0;i<tamFlores;i++){
        if(comprobante[i] == 1){
            gotoxy(flor[i].posX + 1,flor[i].posY); printf(" ");
        }
    }
}

void ordenar(tipoPuntuaciones usuario[], int n)
{
    tipoPuntuaciones aux;
    for(int i=0; i<n ; i++){
        for(int j =0; j<n; j++){
            if(usuario[j].puntos<usuario[j+1].puntos){
                aux = usuario[j];
                usuario[j] = usuario[j+1];
                usuario[j+1]= aux;
            }
        }
    }
}

void mostrar_Mejores10(tipoPuntuaciones garden[], int n){
    for(int i=0; i<n; i++)
    {
        cout<<"\n"<<garden[i].name<<"\t"<<garden[i].fecha<<"\t"<<garden[i].puntos<<endl;
    }
}


void opciones_menu()
{
    color(4);
    gotoxy(45,16);
    printf("1) Jugar");
    gotoxy(45,17);
    printf("2) Continuar jugando");
    gotoxy(45,18);
    printf("3) salir");
}
void crear_top(tipoPuntuaciones usuarios[], int &contUsuarios,bool top)
{
    if(top){
        fstream archPuntuacion;
        tipoPuntuaciones garden;
        archPuntuacion.open("puntuacion.txt",ios::in);
        cout<<"Nombre\tFecha\t\tPuntos"<<endl;
        while(!archPuntuacion.eof() && archPuntuacion>>garden.name>>garden.fecha>>garden.puntos)
            {
                usuarios[contUsuarios].name = garden.name;
                usuarios[contUsuarios].fecha = garden.fecha;
                usuarios[contUsuarios].puntos = garden.puntos;
                contUsuarios++;
            }
            archPuntuacion.close();
            ordenar(usuarios,contUsuarios);
            top = false;
    }
}




int main()
{
    menu_principal();
    getch();
    return 0;
}


void menu_principal()
{
    string nick;
    tipoPuntuaciones usuarios[99];
    int contUsuarios=0;
    int mejores10=10;
    bool repite = true;
    bool jugador_guardado = false;
    bool top = true;
    bool bucle = true;
    int opcion;
    gotoxy(28,19);
    cout<<"GARDEN IDENTIFIQUESE: ";
    getline(cin,nick);
    do{
        system("cls");
        dibujo_menu();
        opciones_menu();
        gotoxy(50,21);
        cout<<"GARDEN: "<<nick<<endl;
        gotoxy(28,22);
        cout<<"Ingrese su opcion: ";
        color(2);
        cin>>opcion;
        crear_top(usuarios,contUsuarios,top);
        system("cls");
        switch(opcion)
        {
            case 1:
                    gotoxy(3,0);
                    cout<<"Garden: "<<nick<<endl;
                    ocultar_cursor(false);
                    juego(nick,jugador_guardado);

                break;
            case 2:
                    jugador_guardado = true;
                    juego(nick,jugador_guardado);
                    break;
            case 3:
                    ocultar_cursor(false);
                    gotoxy(45,15);
                    cout<<"DON'T LET DOWN YOUR GUARD"<<endl;
                    gotoxy(50,16);
                    cout<<" SEE YOU LATEAR"<<endl;
                    repite = false;
                    break;
        }
    }while(repite);

}


void mover_resorte(bool &disparo,bool &Terminar_juego){

            if(kbhit()){
                char tecla = getch();
                borrar_resorte_resortera();
                if(tecla == IZQUIERDA && resortera.posX>3)resortera.posX--;
                if(tecla == DERECHA   && resortera.posX+3<77)resortera.posX++;
                if (tecla == ' ') disparo = true;
                if (tecla == ESC) Terminar_juego = true;
                pintar_resortera();
            }
}



void mover_ave(tipoCordenadas ave[],int tamNivel,int dd,int &point,tipoCordenadas flor[]
               ,int &live, int comprobante[], int &contNiv)
{
    Sleep(30);
    for(int a=0; a<tamNivel; a++)
    {
        gotoxy(ave[a].posX,ave[a].posY); printf(" ");
        ave[a].posY++;
        if (ave[a].posY > 27 || ave[a].posX == bala.dx+1 && resortera.posY+dd <= ave[a].posY)//20 es la cordenada donde esta mas o menos en el piso
        {
            if(ave[a].posX == bala.dx+1 && resortera.posY+dd <= ave[a].posY){
                point+=10;
            }
            for(int i=0; i<tamNivel;i++)
            {
                if(flor[i].posX+1 == ave[a].posX && comprobante[i]==0)
                {
                    live--;
                    comprobante[i]=1;
                    flor[i].color = 4;
                }
            }
        ave[a].posX = /*21;*/((rand()%71)+4);
        ave[a].posY = 4;
        contNiv++;
        if(contNiv == 20)
        {
            nivel.condicion = true;
            contNiv = 0;
        }
        }
        pintar_flor(flor,tamNivel);
        pintar_SoloAve(ave[a]);
    }
}
void juego(string nick,bool guardado)
{
    string auxNick;
    fstream archPuntuacion;
    fstream archGuardar1;
    fstream archGuardar2;
    fstream archAbrirGuardar;
    bool game_over = false;
    bool disparo = false;
    bool disparo_powerUP = false;
    bool terminar_juego = false;
    int live = 1,auxlive;
    int dd=0;
    int point=0,auxpoint;
    int tamFlores;
    int tamAves;
    int comprobante[99]={0,0,0,0,0,0};
    int control=0;
    int buleano;
    int contNiv=0;
    tipoNivel aux;

    tipoFecha fecha;
    hoy(fecha);
    pintar_limites();
    pintar_resortera();
    abrirAch_aves(aves,tamAves);
    abrirAch_flor(flores,tamFlores);
    if(guardado){
         archAbrirGuardar.open("guardar_juego.txt", ios::in);
         while(!archAbrirGuardar.eof() && archAbrirGuardar>>auxNick>>auxlive>>auxpoint>>aux.nivel){
            nick = auxNick;
            live = auxlive;
            point = auxpoint;
            nivel.nivel = aux.nivel;
         }
         archAbrirGuardar.close();
         archAbrirGuardar.open("guardarFlores_comprobante.txt", ios::in);
         while(!archAbrirGuardar.eof() && archAbrirGuardar>>buleano){
               comprobante[control]= buleano;
               if(buleano == 1){
                flores[control].color=4;
               }
               control++;
         }
         archAbrirGuardar.close();
         ocultar_cursor(false);
         pintar_florGuardado(comprobante,flores,tamFlores);
         color(2);
         gotoxy(3,0);
         cout<<"Garden: "<<auxNick<<endl;
    }
    gotoxy(35,1); printf("Nivel %i", nivel.nivel);
    color(4);
    while(!game_over){
            color(15);
            barra_salud(live);
            gotoxy(4,2);printf("Puntos: %d",point);
            gotoxy(4,1);printf("vidas: %d",live);
            color(4);
            if(!disparo) bala.dx = resortera.posX;
            gotoxy(bala.dx+1,resortera.posY+dd);
            printf("*");
            mover_ave(aves,nivel.nivel,dd,point,flores,live,comprobante,contNiv);
            Sleep(10);
            mover_resorte(disparo,terminar_juego);
            gotoxy(bala.dx+1,resortera.posY+dd);
            printf(" ");
            if (disparo == true)dd--;
                if (resortera.posY + dd < 5)
                    {
                        dd = 0;
                        disparo = false;
                    }
            if(live == 0 || live<0 || nivel.nivel == 11)
            {
                game_over = true;
            }
            if(nivel.condicion)
            {
                nivel.nivel++;
                gotoxy(35,1); printf("Nivel %i", nivel.nivel);
                nivel.condicion = false;
                live++;
            }
                if (terminar_juego){

                    archGuardar1.open("guardar_juego.txt", ios::out);
                    archGuardar1<<nick<<" "<<live<<" "<<point<<" "<<nivel.nivel<<endl;
                    archGuardar1.close();

                    archGuardar2.open("guardarFlores_comprobante.txt", ios::out);
                    for(int i=0; i<tamFlores;i++){
                        archGuardar2<<comprobante[i]<<endl;
                    }
                    archGuardar2.close();
                    break;
                }
            }
            if(game_over && terminar_juego == false){
            archPuntuacion.open("puntuacion.txt",ios::app | ios::out);
            archPuntuacion<<nick<<" "<<fecha.dia<<"-"<<fecha.mes<<"-"<<fecha.anhio<<" "<<point<<endl;
            archPuntuacion.close();
            }
            gotoxy(50,15);
            if(nivel.nivel >11){
            printf("WIN");
            }else{
            printf("GAME OVER");
            }
            nivel.nivel=1;
            resortera={35,24,9};

            Sleep(1500);

    }

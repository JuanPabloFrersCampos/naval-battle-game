#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define CANTIDADBARCOS 3
#define TAMANOX 6
#define TAMANOY 3
#define NOMBREMAX 32
#define TAMANOXY [TAMANOX] [TAMANOY]
#define COLOR_DEFAULT "\x1b[0m"
#define ROJO_T "\x1b[31m"
#define VERDE_T "\x1b[32m"
char equivaleX = 'F'; //En caso de modificar el tamaño X del mapa, colocar el equivalente al tamaño aquí.

void bienvenida (char[]);
void prepararMapa(int [] [TAMANOY], int [] [TAMANOY], int [] [TAMANOY], int [] [TAMANOY]);
void generarBarcosEnemigos(int [] [TAMANOY]);
void generarBarcosUsuario(int [] [TAMANOY], int [] [TAMANOY], int [] [TAMANOY], int [] [TAMANOY]);
void imprimirMapa (int [] [TAMANOY], int [] [TAMANOY], int [] [TAMANOY], int [] [TAMANOY]);
void disparos(int [] [TAMANOY], int [] [TAMANOY], int [] [TAMANOY], int [] [TAMANOY]);
int comprobarFinalizacionJuego (int);
int conversionLetraANumero(char);

int main()
{
    //Variables globales:
    int mapaEnemigo TAMANOXY, mapaUsuario TAMANOXY, disparosMapaEnemigo TAMANOXY;
    int disparosMapaUsuario TAMANOXY;
    char nombreUsuario [NOMBREMAX];

    //Ejecución del programa ordenadamente:
    prepararMapa(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
    bienvenida(nombreUsuario);
    generarBarcosEnemigos(mapaEnemigo);
    generarBarcosUsuario(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
    imprimirMapa(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
    disparos(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
    return 0;
}

void prepararMapa (int mapaEnemigo TAMANOXY, int mapaUsuario TAMANOXY, int disparosMapaEnemigo TAMANOXY,
                   int disparosMapaUsuario TAMANOXY)
{
    /*Setea todos los índices de los arreglos, según el tamaño del mapa, con valor 0. Posteriormente,
    estos valores pueden ser modificados, en caso de que contengan un barco, o sean disparados*/
    int x, y;
    for (y = 0; y <= TAMANOY - 1; y++)
    {
        for (x = 0; x <= TAMANOX - 1; x++)
        {
            mapaEnemigo[x][y] = 0;
            mapaUsuario[x][y] = 0;
            disparosMapaEnemigo[x][y] = 0;
            disparosMapaUsuario[x][y] = 0;
        }
    }
}

void bienvenida (char nombreUsuario[])
{
    int i=0;
    char caracter;
    printf("Bienvenido capitan. Cual es su nombre?: ");
    while (i <= NOMBREMAX - 1  &&  caracter != '\r')
    {
        caracter = getche();
        if (caracter != '\r')
        {
            nombreUsuario[i] = caracter;
            i++;
        }
        nombreUsuario[i] = '\0';
    }
    system("cls");
    printf("\n Muy bien, capitan %s, es un honor tenerlo a bordo.", nombreUsuario);
    printf("\n Su objetivo es hundir todos los barcos del enemigo, antes de que ellos lo hagan, mucha");
    printf(" suerte!");

    printf("\n\n En esta ocasion dispones de %d barcos, y su enemigo tambien. ", CANTIDADBARCOS);
    printf("\n Distribuyelos estrategicamente para hacerte con la victoria.");

    printf("\n\n Presiona una tecla para continuar");
    getche();
    system("cls");
}

void generarBarcosEnemigos(int mapaEnemigo TAMANOXY)
{
    int i, x, y;
    srand(time(NULL));
    for (i = 0; i <= CANTIDADBARCOS - 1; i++)
    {
        x = (rand() % TAMANOX);
        y = (rand() % TAMANOX);
        mapaEnemigo [x][y] = 1;
    }
}

void generarBarcosUsuario(int mapaEnemigo TAMANOXY, int mapaUsuario TAMANOXY,
                          int disparosMapaEnemigo TAMANOXY,int disparosMapaUsuario TAMANOXY)
{
    int i, x, y, aux;
    char ingreso;
    for (i = 0; i <= CANTIDADBARCOS - 1; i++)
    {
        do
        {
            system("cls");
            imprimirMapa(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
            printf("\n\n En que columna colocaras el el barco numero %d? (A - %c): ", i + 1, equivaleX);
            ingreso = getche();
            x = conversionLetraANumero(ingreso);
        }
        while (x == 999); /*Este valor lo retorna la función conversionLetraANumero,
                                 ... al analizar un caracter no previsto.*/

        do
        {
            /*No se guarda realmente la columna introducida por el usuario, si no que se guarda el valor -1,
           ...de esta manera es más intuitivo para el usuario, ya que nunca tendrá que utilizar la columna
           ...0. En todo caso introducirá la columna 1, pero realmente se colocará en la columna 0*/
            printf("\n Y en que fila? (1 - %d): ", TAMANOY);
            scanf("%d", &aux);
            aux -= 1;
            y = aux;
            mapaUsuario[x][y] = 1;
        }
        while (aux < 0  ||  aux > TAMANOY );


    }
}

int comprobarFinalizacionJuego(int aciertos)
{
    int v;
    v = (aciertos == CANTIDADBARCOS) ? 1: 0;
    return v;
}

void disparos (int mapaEnemigo TAMANOXY, int mapaUsuario TAMANOXY, int disparosMapaEnemigo TAMANOXY,
               int disparosMapaUsuario TAMANOXY)
/*Genera (en el caso de la máquina)/solicita (para el usuario) las coordenadas en donde se efectua un disparo
, y verifica si se acierta a algún barco. En el caso de acertar, actualiza el mapa, suma un acierto, y
mediante la función comprobarFinalizacionJuego, despliega o no un mensaje de finalización del juego.*/
{
    int aciertosEnemigo=0, aciertosUsuario=0, turno=1, ingreso, x, y, aux;
    srand(time(NULL));

    while (aciertosEnemigo < CANTIDADBARCOS  &&  aciertosUsuario  <  CANTIDADBARCOS)
    {
        if (turno % 2 == 0) // Turno de la máquina
        {
            imprimirMapa(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
            printf("\n\n Hector Bonzo da la orden de disparar y...");
            do
            {
                x = (rand() % TAMANOX);
                y = (rand() % TAMANOY);
            }
            while (disparosMapaUsuario[x][y] == 1); //En caso de que genere unas coordenadas en donde
            disparosMapaUsuario [x][y] = 1;         //previamente se disparó, genera otras diferentes.
            getche();

            if (mapaUsuario[x][y] == 1  &&  disparosMapaUsuario[x][y] == 1) //Si aciertan a un barco
            {
                system("cls");
                imprimirMapa(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
                printf("\n\n Un barco aliado fue hundido!");
                aciertosEnemigo++;
                getche();

                if (comprobarFinalizacionJuego(aciertosEnemigo))
                {
                    printf("\nPerdiste!");
                }
            }

            else if (mapaUsuario[x][y] == 0  &&  disparosMapaUsuario[x][y] == 1)
            {
                system("cls");
                imprimirMapa(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
                printf("\n\n Hector Bonzo fallo!.");
                printf("\n Se detecto una explosion en %c%d", x+65, y+1);
                getche();
            }
        }

        else //Turno del usuario
        {
            do
            {
            system("cls");
            imprimirMapa(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
                do
                {
                    if (disparosMapaEnemigo[x][y] == 1)
                    {
                        printf("\n Tu primer oficial te informa que dicha coordenada ya fue atacada previamente");
                        printf("\n Rapido, vuelve a informarle a tu primer oficial en donde atacar");
                    }
                    printf("\n\n En que columna quieres atacar? (A - %c): ", equivaleX);
                    ingreso = getche();
                    x = conversionLetraANumero(ingreso);
                }
                while (x ==  999); //999 es devuelto por conversionLetraANumero, si se le pasa un valor no previsto

                do
                {
                    printf("\n Y en que fila?: ");
                    scanf("%d", &aux);
                    y = aux - 1;
                }
                while (y < 0  ||  y > TAMANOY );
            }
            while (disparosMapaEnemigo[x][y] == 1); // Comprueba que ya no se haya disparado en dicha coordenada.
            disparosMapaEnemigo[x][y] = 1;

            printf("\n Se prepara el disparo y...");
            getche();
            if (mapaEnemigo[x][y] == 1  &&  disparosMapaEnemigo[x][y] == 1)
            {
                system("cls");
                aciertosUsuario++;
                imprimirMapa(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
                printf("\n\n Un barco enemigo fue hundido!");
                if (comprobarFinalizacionJuego(aciertosUsuario)) //Verifica si el juego debe finalizar.
                {
                    imprimirMapa(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
                    printf("\n Felicitaciones, ganaste!");
                }
            }
            else if (mapaEnemigo[x][y] == 0  &&  disparosMapaEnemigo[x][y] == 1)
            {
                imprimirMapa(mapaEnemigo, mapaUsuario, disparosMapaEnemigo, disparosMapaUsuario);
                system("cls");
                printf("\n No le diste a ningun barco!\n");
            }
        }
        turno++;
    }
}

int conversionLetraANumero(char c)
/*Convierte una letra a su posición numérica equivalente. Si analiza un caracter
no previsto, devuelve 999.
Las pruebas lógicas planteadas, se basan en el código ASCII. Se  incluye una prueba lógica OR + 32 en
todas las instancias, ya que el equivalente de la letra en minúscula.
65 es el identificador correspondiente a la letra "a". i aumenta en 1 cada vez que se realiza el ciclo,
para así recorrer todo el abecedario.*/
{
    int devuelve, i = 0;

    while (i <= 26)
    {
        if (c == 65 + i  ||  c == 65 + 32 + i)
        {
            devuelve = i;
            break;
        }
        i++;
    }
    if (i == 26) // Si i recorrio todo el abecedario, sin encontrar coincidencias.
    {
        devuelve = 999;
    }
    return devuelve;
}



void imprimirMapa (int mapaEnemigo TAMANOXY, int mapaUsuario TAMANOXY, int disparosMapaEnemigo TAMANOXY,
                   int disparosMapaUsuario TAMANOXY)
//Imprime tanto mapa enemigo como aliado, se debe actualizar después de cada turno.
{
    int x, y;
    //Impresión mapa enemigo:
    printf("\n\t\t\t\tCuadricula enemiga:\n\n\t\t");
    for (x = 65; x <= 65 + TAMANOX - 1; x++) // Fila que muestra letras según corresponda sobre los numeros
    {
        printf("\t%c", x);
    }
    printf("\n");
    for (y = 0; y <= TAMANOY - 1; y++)  // Columna a la izquierda de los números, que indica la posición
    {                                   // al usuario
        printf("\n\n\t\t%d", y+1);
        for (x = 0; x <= TAMANOX - 1; x++)
        {
            if (mapaEnemigo [x][y] == 1  &&  disparosMapaEnemigo [x][y] == 1)
            {
                printf(ROJO_T"\t=>"COLOR_DEFAULT);
            }
            else if (mapaEnemigo [x][y] == 0  &&  disparosMapaEnemigo [x][y] == 1)
            {
                printf("\tX");
            }
            else
            {
                printf("\t-");
            }
        }
    }

    printf("\n\n\t\t\t-----------------------------------------\n\n");

    //Impresión mapa del usuario:
    printf("\t\t\t\tCuadricula aliada:\n\n\t\t");
    for (x = 65; x <= 65 + TAMANOX - 1; x++)
    {
        printf("\t%c", x);
    }
    printf("\n");
    for (y = 0; y <= TAMANOY - 1; y++)
    {
        printf("\n\n\t\t%d", y+1);
        for (x = 0; x <= TAMANOX - 1; x++)
        {
            if (mapaUsuario [x][y] == 1  &&  disparosMapaUsuario [x][y] == 1)
            {
                printf(ROJO_T"\t=>"COLOR_DEFAULT);
            }
            else if (mapaUsuario [x][y] == 0  &&  disparosMapaUsuario [x][y] == 1)
            {
                printf("\tX");
            }
            else if (mapaUsuario [x][y] == 1  &&  disparosMapaUsuario [x][y] == 0)
            {
                printf(VERDE_T"\t=>"COLOR_DEFAULT);
            }
            else
            {
                printf("\t-");
            }
        }
    }
    printf("\n");
}

/*!
\mainpage CiLog
\author  ilusion
\version 0.01
\date Septiembre 2014
\brief Compilador e Interprete Logico.

\section intro CiLog
Herramienta desarrollada para practicas en el area de Inteligencia Artificial, Compiladores, Logica y Lenguajes Formales.

Cambiando los tokens del sistema, puedes utilizar la sintaxis de tu lenguaje preferido, por lo que CiLog tiene la ventaja de poder a coplar su sintaxis a tus gustos.

\section opciones Opciones Validas
<b>-i</b><br/>
    Ver la informacion disponible<br/>
<b>-v IDI.idioma</b><br/>
    Ver el valor de un parametro de configuracion dado.  Utilizar el valor TODO para listar lodas las configuraciones con su valor respectivo y valores adminitidos.<br/>
<b>-m IDI.idioma:ES</b><br/>
<b>-c</b><br/>
    Compilar la base de conocimiento*/
#include <cstring>
#include <ncurses.h>

#include "CiLog.h"

//!Constante de salto de linea
#define SALTO "\n"

//!Variable utilizada para controlar el estado del reconocimiento de los posibles paramentros
int estado;
//!Variable para contener un parametro a modificar
char * clave;
//!Variable para contener un valor de un parametro
char * valor;

 //!Ejecuta un parametro enviado en la ejecucion de CiLog
 /*!Esta funcion recibe los argumentos pasados por linea de comandos, para su procesamiento antes de ejecutar CiLog.
 \param chrArgumento El argumento a ejecutar*/
void ejecutarParametros(char *);
 //!Muestra informacion acerca del sistema
 /*!Muestra informacion acerca del sistema, el autor y contactos para sugerencias y dudas.*/
void mostarInformacion();

int main(int argC, char * argV[])
{
    initscr();
	keypad(stdscr,TRUE);
	scrollok(stdscr,TRUE);
	cbreak();
	noecho();

    estado=0;
    CiLog ciLog;

    for(int x=1; x<argC; x++)
    {
        ejecutarParametros(argV[x]);
    }

    if(estado==0)
    {
        ciLog.modoInterprete();
    }
    else
    {
        printw("Definicion incorrecta de parametros.");
        printw(SALTO);
    }

    endwin();
    return 0;
}
void ejecutarParametros(char * argumento)
{
    if(estado==0)
    {
        if(strcasecmp(argumento, "-i")==0)
        {
            mostarInformacion();
        }
        else if(strcasecmp(argumento, "-v")==0)
        {
            estado=1;
        }
    }
    else if(estado==1)
    {
    }
    else if(estado==2)
    {
    }
}
void mostarInformacion()
{
}

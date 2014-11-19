#include "Util.h"

Util::Util(){}
Util::Util(string comando, string sentencia, string ayuda)
{
	this->comando=comando;
	this->sentencia=sentencia;
	this->ayuda=ayuda;

	configuracion=new Configuracion;
	configuracion->cargarHistorial();
}
Util::~Util()
{
    configuracion->cargarConfiguracion();
    configuracion->actualizarHistorial();
}
//Funciones Publicas----------------------------------------------------
string Util::aMinuscula(string cadena)
{
	string retorno="";

	for(unsigned int x=0; x<cadena.length(); ++x)
	{
		retorno+=tolower(cadena[x]);
	}

	return retorno;
}
void Util::getText(string * texto)
{
    *texto=procesarPulsacion();
}
int Util::lineaValida(int linea)
{
    int y, x;
    getmaxyx(stdscr, y, x);

    if(linea>y)
    {
        linea=y-1;
    }

    return linea;
}
void Util::limpiarPantalla()
{
    clear();
}
//Funciones Privadas----------------------------------------------------
string Util::procesarPulsacion()
{
    int linea=0;
    int codigo=0;
    int estado=0;
    bool leer=true;
    char * tecla=new char;
    string procesamiento="";

	while(leer==true)
	{
	    codigo=getch();
	    *tecla=codigo;

	    if(codigo==KEY_DOWN)
        {
            procesamiento=configuracion->abajo();
            moverHistorial(&procesamiento, &estado, &linea);
        }
        else if(codigo==KEY_UP)
        {
            procesamiento=configuracion->arriba();
            moverHistorial(&procesamiento, &estado, &linea);
        }
        else if(codigo==ENTER)
        {
            if(estado==2 || estado==3 || ayuda.compare(procesamiento)==0)
            {
                leer=false;
                configuracion->agregarHistorial(procesamiento);
            }
            else
            {
                if(linea>0)
                {
                    procesamiento+=" ";
                    printw("%c     #", ENTER);
                }
                else
                {
                    printw("%ccilog#", ENTER);
                }
            }

            configuracion->reiniciarHistorial();
        }
        else if(isprint(*tecla))
        {
            linea+=1;
            printw("%c", codigo);
            procesamiento+=(*tecla);
            estado=procesarTecla(estado, *tecla);
        }
        else if(codigo==KEY_BACKSPACE)
        {
            if(linea>0)
            {
                linea-=1;
                int y, x;
                getyx(stdscr, y, x);
                move(y, x-1);
                clrtoeol();

                procesamiento=procesamiento.substr(0, linea);

                if(linea==0)
                {
                    estado=0;
                }
            }
        }
	}

	return procesamiento;
}
int Util::procesarTecla(int estado, char tecla)
{
    if(estado==0)
    {
        estado=1;

        if(comando[0]==tecla)
        {
            estado=2;
        }
    }

    if(estado==1 || estado==3)
    {
        estado=1;

        if(sentencia[0]==tecla)
        {
            estado=3;
        }
    }

    return estado;
}
void Util::moverHistorial(string * procesamiento, int * estado, int * columna)
{
    int y, x;
    getyx(stdscr, y, x);
    move(y, 6);
    clrtobot();

    if(procesamiento->compare(INICIO)==0)
    {
        *estado=0;
        *columna=0;
        *procesamiento="";
    }
    else
    {
        printw(procesamiento->c_str());
        refresh();
        *columna=procesamiento->length();

        if(comando.compare(procesamiento->substr(0, 1))==0)
        {
            *estado=2;
        }
        else if(sentencia.compare(procesamiento->substr(procesamiento->length()-1, 1))==0)
        {
            *estado=3;
        }
    }
}

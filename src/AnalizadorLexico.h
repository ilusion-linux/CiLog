#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H

#include <iostream>
#include <fstream>

#include "Configuracion.h"

#define ENTER   10
#define TABH     9
#define ESPACIO 32
#define ESCAPETEXTO "[s]"
#define PUNTOTEXTO  "."

using std::ios;
using std::ifstream;
using std::string;

//!Proceso de analisis lexico
/*!Ejecuta el analisis lexico a partir de un archivo o una cadena de texto ingresada como consulta, y genera una estructura del tipo lexico.*/
class AnalizadorLexico
{
    public:
        AnalizadorLexico(string);
        ~AnalizadorLexico();

        void * darLexico();
        string analizarArchivo(string, int);
        string analizarConsulta(string);

    private:
        int estado;
        int fila;
        int columna;
        string buffer;

        string tknFinSentencia;
        string tknSeparadorParametro;
        string tknInicioAgrupacion;
        string tknFinAgrupacion;
        string tknOperadorAnd;
        string tknOperadorOr;
        string tknOperadorNegacion;
        string tknOperadorIgualdad;
        string tknDefinicionFuncion;
        string tknComentarioSimple;
        string tknIndicadorVariable;
        string infCaracterInvalido;
        string infErrorPosicion;
        string infResultadoOK;
        string infArchivoVacio;

        Configuracion * configuracion;

        struct lexico{
            int linea;
            string elemento;
            struct lexico * siguiente;
        };

        struct lexico * lexicos;
        struct lexico * iniLexcios;

        string leerArchivo(string, int);
        string leerConsulta(string);
        string procesar(char);
        void analizador(char);
        void analizadorGenerico(char);
        bool caracterValido(char);
        bool agregar(string);
};
#endif

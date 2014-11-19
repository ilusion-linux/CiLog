#ifndef ANALIZADORSINTACTICO_H
#define ANALIZADORSINTACTICO_H

#include <iostream>

#include "Configuracion.h"

#define ESCAPETEXTO "[s]"
#define PUNTOTEXTO  "."

using std::string;

//!Proceso de analisis sintactico
/*!Ejecuta el analisis sintactico sobre una estructura del tipo lexico y termina generando una estructura del tipo sintactico.*/
class AnalizadorSintactico
{
    public:
        AnalizadorSintactico(string);
        ~AnalizadorSintactico();

        void * darSintactico();
        string analizar(void *);

    private:
        int estado;

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
        string infElementoNoEsperado;
        string infResultadoOK;
        string infErrorLinea;

        Configuracion * configuracion;

        struct lexico{
            int linea;
            string elemento;
            struct lexico * siguiente;
        };
        struct sintactico{
            int linea;
            string elemento;
            struct sintactico * elementoComplejo;
            struct sintactico * siguiente;
        };

        struct lexico * lexicos;

        struct sintactico * sintacticos;
        struct sintactico * iniSintacticos;
        struct sintactico * sintacticosComplejo;
        struct sintactico * iniSintacticosComplejo;

        string procesar(string, int);
        string analizador(string, int);
        bool esReservado(string);
        template<class estructura>
                bool agregar(estructura **, string, int, estructura *);
};
#endif

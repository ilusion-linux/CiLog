#ifndef ANALIZADORSEMANTICO_H
#define ANALIZADORSEMANTICO_H

#include <iostream>

#include "Configuracion.h"

#define ESCAPETEXTO "[s]"
#define PUNTOTEXTO  "."

using std::string;

//!Proceso de analisis semantico
/*!Ejecuta el analisis semantica sobre una estructura del tipo sintactico, esta genera una estructura del tipo semantico, la cual es un punto de partida para la creacion de la base de conocimientos.*/
class AnalizadorSemantico
{
    public:
        AnalizadorSemantico(string);
        ~AnalizadorSemantico();

        void * darSemantico();
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

        struct sintactico{
            int linea;
            string elemento;
            struct sintactico * elementoComplejo;
            struct sintactico * siguiente;
        };

        struct semantico{
            string elemento;
            struct semantico * elementoComplejo;
            struct semantico * siguiente;
        };

        struct sintactico * sintacticos;
        struct sintactico * sintacticosComplejo;

        struct semantico * semanticos;
        struct semantico * iniSemanticos;
        struct semantico * semanticosComplejo;
        struct semantico * iniSemanticosComplejo;

        string procesar(string, int);
        string analizador(string);
        bool esReservado(string);
        template<class estructura>
                bool agregar(estructura **, string, estructura *);
};
#endif

#ifndef ANALIZADOR_H
#define ANALIZADOR_H

#include <iostream>
#include <ncurses.h>

#include "Directorio.h"
#include "Configuracion.h"
#include "BaseConocimiento.h"
#include "AnalizadorLexico.h"
#include "AnalizadorSintactico.h"
#include "AnalizadorSemantico.h"

#define SALTO     "\n"
#define LSTINICIO "Inicio Lista"

using std::string;

//!Manejador del proceso de compilacion
/*!Lleva a cabo el proceso compilacion lexica,sintactica y semantica, manejando los errores y la posterior ejecucion del a funcion que crea la base de concocimiento.*/
class Analizador
{
    public:
        //!Constructor default de la clase
        Analizador();
        Analizador(BaseConocimiento **);
        //!Destructor default de la clase
        ~Analizador();

        string compilar();
        string compilar(string);
        string compilarConsulta(string);
        void * darSemantico();

    private:
        string dirPath;
        string dirExtension;
        string infInicioAnalisis;
        string infResultadoOK;
        string infResultadoERROR;
        string infResultadoFinalOK;
        string infResultadoFinalERROR;

        Directorio * directorio;
        Configuracion * configuracion;
        BaseConocimiento * conocimiento;
        AnalizadorLexico * analizaLexico;
        AnalizadorSintactico * analizaSintactico;
        AnalizadorSemantico * analizaSemantico;

        struct archivo{
			string valor;
			int tamanio;
			struct archivo * siguiente;
		};
		struct archivo * archivos;

		void inicializar();
		string analizar();
		string analisisLexico(string);
		string analisisLexico(string, int);
		string analisisSintactico(void *, int);
		string analisisSemantico(void *, int);
		void formarConocimiento(void *);
};
#endif

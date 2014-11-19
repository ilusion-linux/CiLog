#ifndef MOTORINFERENCIA_H
#define MOTORINFERENCIA_H

#include <iostream>
#include <ncurses.h>

#include "BaseConocimiento.h"
#include "Analizador.h"
#include "Configuracion.h"
#include "Util.h"

#define SALTO          "\n"
#define ESCAPETEXTO    "[s]"
#define INICIOBUSQUEDA "---"

using std::string;

//!Proceso de busquedas en la base de conocimiento
/*!Ejecuta las consultas a la base de concoimiento, en base a consultas estructuras como entrada a traves del interprete de CiLog.

Busca tanto la existencia de predicados como, la consulta a elementos complejos pertenecientes al predicado, compuesto por uno o varios sujetos.*/
class MotorInferencia
{
    public:
        MotorInferencia(BaseConocimiento **);
        ~MotorInferencia();

        void ejecutarConsulta(string consulta);

    private:
        int conVariables;
        string predicado;

        string opcOtroResultado;
        string tknIndicadorVariable;
        string infResultadoOK;
        string infErrorPredicado;
        string resResSi;
        string resResNo;
        string resResTodo;

        BaseConocimiento * conocimiento;
        Analizador * analizador;
        Configuracion * configuracion;
        Util * util;

        struct semantico{
            string elemento;
            struct semantico * elementoComplejo;
            struct semantico * siguiente;
        };

        struct busquedaSujeto{
            string valor;      //Si es valor a buscar
            string variable;   //Si es variable a encontrar
            string buscar;     //Valor a buscar de un registro identico
            string igualacion; //Valores reemplazando variables
            bool   resultado;  //Resultado de comparacion
            struct busquedaSujeto * siguiente;
        };

        struct semantico * semanticos;

        struct busquedaSujeto * busqueda;
        struct busquedaSujeto * iniBusqueda;

        struct elementoCompuesto{
		    string elemento;
		    struct elementoCompuesto * siguiente;
		};
		struct sujeto{
			struct elementoCompuesto * elemento;
			struct sujeto * siguiente;
		};
		struct predicado{
			string valor;
			struct predicado * siguiente;
			struct sujeto * inicioSujeto;
			struct sujeto * finSujeto;
		};

		struct predicado * predicados;

        void formarValoresBusqueda(void *);
        void ejecutarBusqueda();
        bool agregar(string);
        string esVariable(string);
        void busquedaDetalle(struct sujeto *);
        bool busquedaCompuesta(struct elementoCompuesto *);
        bool comprobarBusqueda();
        bool mostarMensaje();
};
#endif

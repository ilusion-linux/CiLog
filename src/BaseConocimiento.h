#ifndef BASECONOCIMIENTO_H
#define BASECONOCIMIENTO_H

#include <iostream>

#define PRDINICIO       "Base de predicados"
#define INICIOCOMPUESTO "---"

using std::string;

//!Base de conocimiento de CiLog
/*!Conforma la parte de inteligencia artificial correspondiente a la Base de Conocimiento. Aqui se analizan, crean y guardan las estructuras utilizadas para la representacion de los sujetos, predicados y estructuras de cacheado.*/
class BaseConocimiento
{
	public:
		BaseConocimiento();
		~BaseConocimiento();

		void formarBaseDeHechos(void *);
		void mostrarBaseDeHechos();
		void * predicadoUnico(string);

	private:
        struct semantico{
            string elemento;
            struct semantico * elementoComplejo;
            struct semantico * siguiente;
        };

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
		struct cacheSujeto{
			struct sujeto * sujetoM;
			struct predicado * predicadoM;
			struct cacheSujeto * siguiente;
		};

		struct semantico * semanticos;
        struct semantico * semanticosComplejo;

		struct predicado * predicados;
		struct predicado * predicadosActual;

		struct sujeto * sujetos;

		struct elementoCompuesto * iniElementoCompuesto;

		struct predicado * iniPredicados;

		bool agregarPredicado(string);
};
#endif

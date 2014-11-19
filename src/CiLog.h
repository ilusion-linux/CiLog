#ifndef CILOG_H
#define CILOG_H

#include <iostream>
#include <ncurses.h>

#include "Util.h"
#include "BaseConocimiento.h"
#include "Analizador.h"
#include "MotorInferencia.h"

using std::string;

#define AYUDA "?"
#define SALTO "\n"

//!Compilador e Interprete Logico
/*!Esta clase se encarga de la administracion y ejecucion de la logica de la aplicacion.

Es la encargada de la interaccion entre los usuarios y el programa.*/
class CiLog
{
	public:
	    //!Constructor default de la clase
		CiLog();
		//!Destructor default de la clase
		~CiLog();

		//!Funcion para inicializar el modo interprete del programa
		/*!Al llamar a esta funcion se inicia la interraccion en modo intreprete del sistema, en la que el usuario podra ingresar los parametros a ejecutar o realizar las consultas necesarias a la base de conocimientos.  Esta funcion inicializa el modo grafico de la libreria ncurses.*/
		void modoInterprete();

	private:
	    //!Variable que almacena el comando utilizado para salir del sistema
		string opcSalir;
		//!Variable que almacena el caracter de inicio de comandos
		string opcComando;
		//!Variable que almacena el comando para compilar la base de conocimiento
		string opcCompilar;
		//!Variable que almacena el comando utilizado para mostar la ayuda del sistema
		string opcAyuda;
		//!Variable que almacena el caracter que indica el final de una sentencia en el interprete
		string opcFinSentencia;
		//!Variable que almacena el comparo para mostrar el historial de comandos utilizados
		string opcHistorial;
		//!Variable con comando para limpiar la pantalla de trabajo
		string opcLimpiar;
		//!Variable que almacena el comando para visualizar archivos del path de trabajo
		string opcVer;
		//!Varaible que almacena el comando para ver la base de conocimiento por pantalla
		string opcHechos;
		//!Variable que almacena el caracter para mostrar mas resultados en busquedas
		string opcOtroResultado;
		//!Variable que contiene informacion de la version actual del programa
		string desVersion;
		//!Variable que contiene informacion descriptiva del programa
		string desInformacion;
		//!Variable con informacion del autor
		string desAutor;
		//!Variable con informacion del comando utilzado para salir del programa
		string desSalir;
		//!Variable con informacion del comando para ver la ayuda del programa
		string desAyuda;
		//!Variable con informacion del comando utilizado para compilar la base de conocimiento
		string desCompilar;
		//!Variable con informacion del comando utilizado para ver el historial de comandos ejecutados
		string desHistorial;
		//!Variable con informacion del comando utilizado para limpiar la pantalla
		string desLimpiar;
		//!Varaible con informacion del comando utlizado para ver el contenido de un archivo del directorio de trabajo
		string desVer;
		//!Variable con informacion del comando utilizado para ver la base de hechos
		string desHechos;
		//!Variable con informacion del caracter utilizado para mostrar mas resultados de las busquedas realizadas
		string desOtroResultado;
		//!Variable que almacena la leyenda de las opciones del sistemas
		string opcOpciones;
		//!Variable que almacena la leyenda de comando invalido
		string infComandoInvalido;

		//!Objeto con la logica de la libreria Util.h
		Util * util;
		//!Objeto con la logica de la libreria Configuracion.h
		Configuracion * configuracion;
		//!Objeto con la logica de la libreria BaseConocimiento.h
		BaseConocimiento * conocimiento;
		//!Objeto con la logica de la libreria Analizador.h
		Analizador * analizador;
		//!Objeto con la logica de la libreria MotorInferencia.h
		MotorInferencia * motor;

		//!Interpretra una orden ingresada por el usuario
		/*!Se encarga de analizar si el usuario a ingresado la ejecucion de un comando o ha ingresado una consulta a la base de conocimientos.*/
		void interpretarEntrada(string *);
		//!Identifica y ejecuca un comando ingresado por el usuario
		/*!Una vez identifica el comando ingresado por el usuario, en esta funcion se ejecuta la logica correspondiente al comando ingresado.*/
		void ejecutarComando(string);
		//!Ejecuta una sentencia ingresada por el usuario
		/*!Encargada iniciar el analisis y ejecucion de una consulta ingresada por el usuario.*/
		void ejecutarSentencia(string);
		//!Muestra la ayuda del sistema
		/*!Al llamar a esta funcion se listan las opciones disponibles del sistema y una breve descripcion del usuo de estos.*/
		void mostrarAyuda();
};
#endif

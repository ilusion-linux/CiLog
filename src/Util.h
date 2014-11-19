#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <ncurses.h>

#include "Configuracion.h"

#define ENTER 10
#define INICIO " "

using std::string;

//!Utilidades diversas para CiLog
/*!Coleccion de diferentes funciones de caracter general para el programa.*/
class Util
{
	public:
	    //!Constructor default de la clase
	    Util();
	    //!Constructor especializado
	    /*!\param comando cadena que indica que se ejecutar un comando pre establecido
	    \param sentencia cadena que indica que se ejecutara una consulta al motor de inferencia
	    \param ayudar simbolo de ayuda del sistema*/
		Util(string, string, string);
		//!Destructor default de la clase
		~Util();

		//!Convierte una cadena a minusculas
		/*!Convierte una cadena a minusculas, retornando un nuevo string con la nueva cadena.
		\param cadena cadena a convertir
		\return La nueva cadena convertida*/
		string aMinuscula(string);
        //!Captura pulsaciones del teclado
        /*!Funcion especializada para la deteccion y captura de datos desde el teclado. Esta funcion no retorna ningun valor ya que lo que recibe es una referencia a un string siendo esta referencia la que sufre modificaciones en la ejecucion de esta funcion.
        \param texto referencia a variable donde se almacena la nueva pulsacion detectada*/
		void getText(string *);
		//!Determina si la linea sobre la que se va a imprimir texto es valida
		/*!Determina si la linea en la que se imprima es una linea valida ya que al estar modo ncurses, la dimension de la terminar es limitada.  Si la linea donde se desea imprimir no es validad, se realiza un calculo obteniendo la maxima linea de la consola actual y se le resta uno para poder posicionarse en la ultima fila de esta consola.
		\param linea linea en la que se desea realizar la impresion de texto
		\return Linea validad para la impresion del texto.*/
		int lineaValida(int);
		//!Limpia la pantalla
		/*!Realiza una limpieza de la pantalla, posicionando el prompt en el a primera fila*/
		void limpiarPantalla();

	private:
	    //!Objeto con la logica de Configuracion.h
        Configuracion * configuracion;

        //!Cadena con el simbolo de ayuda del sistemas
        string ayuda;
        //!Cadena con el identificar de comandos
		string comando;
		//!Cadena con el identificador de sentencias
		string sentencia;

		//!Procesa la entrada desde el teclado, mostrando solo los caracteres validos
		/*!Esta funcion detecta el juego de caracteres valido para el modo interprete.  Una vez inicia esta funcion no se detiene hasta que se encuentra el simbolo de fin de sentencia o fin de comando.
		\return string con la nueva cadena ingresada*/
		string procesarPulsacion();
		//!Procesa segun el estado correspondiente el valor de la tecla ingresado
		/*!Analisando el estado en que se encuentra el modo interprete, segun sea el valor de la tecla a procesar se cambiara el estado del modo interprete para indicar el final del procesamiento.
		\param estado el estado actual del modo interprete
		\param caracter el valor de la tecla a procesar
		\return el nuevo estado luego del procesamiento*/
		int procesarTecla(int, char);
		//!Mueve el historial de registro para arriba o para abajo
		/*!Recorre el historial segun las teclas de ARRIBA o ABAJO, en esta funcion se muestra en pantalla el elemento siguiente del historial y se asigna a la variable actual que recibe los comandos o sentencias del interprete.
		\param procesamiento guarda las pulsacion en el modo interprete
		\param estado indica el estatus en que se puede encontrar el modo interprete
		\param columna retorna el largo del elemento seleccionado del historia*/
		void moverHistorial(string *, int *, int *);
};
#endif

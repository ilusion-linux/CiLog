#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <iostream>
#include <ncurses.h>
#include <sqlite3.h>

#define SALTO         "\n"
#define DBNOMBRE      "CiLog.cnf"
#define LSTNOMBRE     "Lista de configuraciones"
#define QRYCONFIG     "select atributo, valor from configuracion_basica;"
#define QRYHISTORY    "select sentencia from historial order by correlativo;"
#define QRYDELHISTORY "delete from historial;"
#define QRYTOKENS     "select token, valor from configuracion_token;"

using std::string;
using std::to_string;

//!Gestiona la actualizacion y carga de las opciones del sistemas
/*!Aqui se gestiona todas las actividades correspondientes a configuracion del sistema.  Desde configuracion opciones como idioma, o cantidad de registro de historial a guardar hasta poder cambiar el lexico del lenguaje o poder cambiar los comandos del sistemas segun la preferencia de cada usuario.*/
class Configuracion
{
	public:
	    //!Constructor default de la clase
		Configuracion();
		//!Destructor default de la clase
		~Configuracion();

        //!Se ejecuta cuando se presiona la tecla <b>ABAJO</b>
        /*!Recorre el historial hacia abajo, uno a uno los registro encontrados, pudiendo editar el registro seleccionado.
        \return El siguiente elemento del historial*/
		string abajo();
		//!Se ejecuta cuando se presiona la tecla <b>ARRIBA</b>
		/*!Recorre el historial hacia arriba, uno a uno los registro encontrados, pudiendo editar el registro seleccionado.
		\return El siguiente elemento del historial*/
        string arriba();
        //!Carga los tokens a utilizar
        /*!Recupera los tokens guardados y los guarda en una estructura para su posterior utilizacion.
        \return true | false*/
        bool cargarTokens();
        //!Carga el historial guardado
        /*!Recupera el historial y lo guarda en una estructura para su posterior utilizacion.
        \return true | false*/
        bool cargarHistorial();
        //!Carga la configuracion del programa
        /*!Recuepra la configuracion del sistema y lo guarda en una estrusctura para su posterior utilizacion.*/
        bool cargarConfiguracion();
        //!Muestra el historial
        /*!Muestra el pantalla el listado de los comandos ejecutados anteriormente.*/
        void mostrarHistorial();
        //!Actualiza el historial
        /*!Actualiza el historial de comandos ejecutados, con los nuevos comando que se ejecutaron desde que se abrio el programa.*/
        void actualizarHistorial();
        //!obtiene el valor de una clave
        /*!Obtieene le valor de una clave correspondiente a una opcion de configuracion o token.
           \param  clave solicitada
           \return valor de la clave solicitada*/
		string obtenerValor(string);
		//!Agrega un nuevo comando al historial
		/*!Se ejecuta luego de que el usuario a ejecutado un comando.
		   \return true | false*/
		bool agregarHistorial(string);
		//!Actualiza la configuracion
		/*!Actualiza la configuracion del sistema, para salvar cualquier cambio realizado.
		   \return true | false*/
		void actualizarConfiguracion();
		//!Modifica el valor de una clave
		/*!Modifica el valor de una opcion del sistema, pudiendo ser inclusive un token o comando.
		\param clave identificador de la opcion a modificar
		\param valor nuevo valor para la opcion
		\return ture | false*/
		bool modificarValor(string, string);
		//!Restablece el inicio del historial
		/*!Restablece la variable que recorre el historial, para que vuelva de nuevo al inicio*/
		void reiniciarHistorial();

	private:
	    //!Conteo de los nuevos comando ejecutados por el usuario
        int noHistorial;
        //!Utilizada para devolver errores de la libreria de sqlite3
		char * msjError;
		//!Instancia de libreria de sqlite3 para manipulacion de datos en CiLog.cnf
		sqlite3 * datos;

		struct valor{
			string clave;
			string valor;
			struct valor * siguiente;
		};
		struct valor * valores;
		struct valor * iniValores;

		struct historial{
            string comando;
            struct historial * siguiente;
            struct historial * anterior;
		};
		struct historial * comandos;
		struct historial * iniComandos;
		struct historial * recorrer;

		//!Conecta con base de datos sqlite3
		/*!Se encarga de realizar la coneccion con la base de datos del programa.
		\return true | false si la conexcion se realizo correctamente o no*/
		bool conectar();
		//!Recorre el historial segun la indicacion dada Arriba o Abajo
		/*!Partiendo del elemento inicial se recorre el historial de comandos ejecutados.
		\param movimiento donde 0 realiza el movimiento haci arriba y 1 lo realiza hacia abajo
		\return string con el nuevo elemento que se acaba de recorrer*/
		string recorrido(int);
		//!Obtiene el valor especifico de una clave
		/*!Envidando el nombre especifico de una opcion de configuracion o token, se obtiene su valor.
		\param clave nombre de la opcion que se desea obtener su valor
		\return cadena con el valor de la clave deseada*/
		string obtener(string);
		//!Ejecuta la sentencia sql especificada
		/*!Ejecuta sentencias sql del tipo insert, delete y update, con el query especifico.
		\param sql cadena con la sentencia sql a ejecutar*/
		void ejecutarSQL(string);
		//!Modifica un parametro de configuracion
		/*!Permite la personalizacion de las diferentes opciones de configuracion del sistemas, afectado al parametro indicado con el valor deseado.
		\param clave nombre de la opcion que se desea obtener su valor
		\param valor nuevo valor que se desea para la clave especifica
		\return true | false si la modificacion se realizo con exito o no*/
		bool modificar(string, string);
		//!Agrega un elemento a una lista enlazada generica
		/*!Permite el poder realizar la inserccion de un nuevo elemento en una lista enzalada sin necesidad de definir el tipo de lista que se afectara.
		\param estructura tipo generico de estructra en la que se realizara la operacion de inserccion
		\param clave nombre de la opcion que se agregara en la lista enlazada
		\param valor valor de la clave indicada
		\return true | false si la inserccion se realiza con exito o no*/
		template<class estructura>
                bool agregar(estructura **, string, string);
};
#endif

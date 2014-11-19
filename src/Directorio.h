#ifndef DIRECTORIO_H
#define DIRECTORIO_H

#include <iostream>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>

#define SEPARADOR ":"

using std::string;
using std::size_t;

//!Manejo de archivos en el directorio de trabajo
/*!Funciones necesarias para obtener el directorio de trabajo y sus archivos correspondientes, buscando recursivamente dentro de cualquier subcarpeta.

Tambien se incluye la obtencion de las extensiones a utilizar para los archivos a incluir en la lista generada de archivos para su posterior utlizacion.*/
class Directorio
{
	public:
	    //!Constructor especializado
	    /*!\param extension listado de extensiones validas
	    \param inicio cadena de inicio para lista enalaza de directorios*/
		Directorio(string, string);
		//!Destructor default de la clase
		~Directorio();

		//!Analiza y obtiene los archivos del path de trabajo
		/*!Una vez analizado y recorrido el directorio de trabajo, devuelve la direccion inicial de la estructura donde se encuentran los archivos encontrados.
		\param path directorio de trabajo
		\return direccion generica del inicio de la estructura donde se almacenan los archivos validos*/
		void * darArchivos(string);

	private:
	    //!Almacena las extensiones validas de los archivos que se analizaran
		string extensiones;

		struct archivo{
			string valor;
			int tamanio;
			struct archivo * siguiente;
		};
		struct archivo * archivos;
		struct archivo * iniArchivos;

		//!Obtiene los archivos que se encuentran en el path de trabajo
		/*!Recorre recursivamente el path de trabajo buscando todos los archivos que se encuentren dentro de este.
		\param path cadena con el directorio de trabajo donde se buscaran los archivos con base de conocimiento*/
		void listarArchivos(string);
		//!Analiza los elementos encontrados del path de trabajo
		/*!Si un elemento del path de trabajo es un directorio llama recursivamente a la funcion listarArchivos.
		\param archivo cadena con el elemento a analizar*/
		void agregarArchivo(string);
		//!Comprueba la extension de un archivo
		/*!Compueba la extension de cada archivo encontrado en el path de trabajo contra las extensiones validad.
		\param archivo cadena con el elemento a analizar
		\return true | false si el archivo contiene la extension valida o no*/
		bool extensionValida(string);
		//!Agrega el elemento a la estructura
		/*!Agrega el nuevo elemento, a la estructura de los archivos que seran procesados
		\param elemento cadena con el elemento a registrar
		\param tamanio tamanio del archivo que se registrara*/
		void agregarElemento(string, int);
};
#endif

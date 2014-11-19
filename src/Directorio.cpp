#include "Directorio.h"

Directorio::Directorio(string extension, string inicio)
{
	this->extensiones=extension;

	archivos=new struct archivo;
	archivos->valor=inicio;
	archivos->siguiente=NULL;

	iniArchivos=archivos;
}
Directorio::~Directorio()
{
}
//Funciones Publicas----------------------------------------------------
void * Directorio::darArchivos(string path)
{
	listarArchivos(path);

	/*archivos=iniArchivos;

	while(archivos!=NULL)
    {
        printw("%s\n", archivos->valor.c_str());
        archivos=archivos->siguiente;
    }*/

	return iniArchivos;
}
//Funciones Privadas----------------------------------------------------
void Directorio::listarArchivos(string path)
{
	DIR * dir=opendir(path.c_str());

	if(dir)
	{
		struct dirent * listado;

		while((listado=readdir(dir))!=NULL)
		{
			char * elemento=listado->d_name;

			if(strcmp(elemento, ".")!=0 && strcmp(elemento, "..")!=0)
			{
			    string ruta=string(path)+string("/")+
					string(listado->d_name);

				agregarArchivo(ruta);
			}
		}

		closedir(dir);
	}
	else
	{
		agregarArchivo(path);
	}
}
void Directorio::agregarArchivo(string archivo)
{
	struct stat atributos;

	if(stat(archivo.c_str(), &atributos)==0)
	{
		if(S_ISDIR(atributos.st_mode))
		{
			listarArchivos(archivo);
		}
		else
		{
		    int tamanio=atributos.st_size;

			if(extensionValida(archivo)==true)
			{
				agregarElemento(archivo, tamanio);
			}
		}
	}
}
bool Directorio::extensionValida(string archivo)
{
    char * aux=new char[extensiones.length()];
    strcpy(aux, extensiones.c_str());

    char * extension=strtok(aux, SEPARADOR);

    while(extension!=NULL)
    {
        size_t valida=archivo.find(extension);

        if(valida!=string::npos)
        {
            return true;
        }

        extension=strtok(NULL, SEPARADOR);
    }

	return false;
}
void Directorio::agregarElemento(string elemento, int tamanio)
{
	struct archivo * aux=archivos;
	archivos=new struct archivo;

	aux->siguiente=archivos;

	archivos->valor=elemento;
	archivos->tamanio=tamanio;
	archivos->siguiente=NULL;
}

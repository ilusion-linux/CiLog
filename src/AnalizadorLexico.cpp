#include "AnalizadorLexico.h"

AnalizadorLexico::AnalizadorLexico(string inicio)
{
    lexicos=new struct lexico;
	lexicos->elemento=inicio;
	lexicos->siguiente=NULL;

	iniLexcios=lexicos;

    configuracion=new Configuracion();
    configuracion->cargarTokens();
    configuracion->cargarConfiguracion();

    tknFinSentencia=configuracion->obtenerValor("finSentencia");
    tknSeparadorParametro=configuracion->obtenerValor("separadorParametro");
    tknInicioAgrupacion=configuracion->obtenerValor("inicioAgrupacion");
    tknFinAgrupacion=configuracion->obtenerValor("finAgrupacion");
    tknOperadorAnd=configuracion->obtenerValor("operadorAnd");
    tknOperadorOr=configuracion->obtenerValor("operadorOr");
    tknOperadorNegacion=configuracion->obtenerValor("operadorNegacion");
    tknOperadorIgualdad=configuracion->obtenerValor("operadorIgualdad");
    tknDefinicionFuncion=configuracion->obtenerValor("definicionFuncion");
    tknComentarioSimple=configuracion->obtenerValor("comentarioSimple");
    tknIndicadorVariable=configuracion->obtenerValor("indicadorVariable");
    infCaracterInvalido=configuracion->obtenerValor("INF.caracterInvalido");
    infCaracterInvalido=configuracion->obtenerValor("INF.caracterInvalido");
    infErrorPosicion=configuracion->obtenerValor("INF.errorPosicion");
    infResultadoOK=configuracion->obtenerValor("INF.resultadoOK");
    infArchivoVacio=configuracion->obtenerValor("INF.archivoVacio");
}
AnalizadorLexico::~AnalizadorLexico()
{
}
//Funciones Publicas----------------------------------------------------
string AnalizadorLexico::analizarArchivo(string archivo, int tamanio)
{
    estado=0;
    fila=1;
    columna=1;
    buffer="";

    return leerArchivo(archivo, tamanio);
}
string AnalizadorLexico::analizarConsulta(string consulta)
{
    estado=0;
    fila=1;
    columna=1;
    buffer="";

    return leerConsulta(consulta);
}
void * AnalizadorLexico::darLexico()
{
    return lexicos=iniLexcios;
    //Destruir lista antes de iniciar analisis de otro archivo
}
//Funciones Privadas----------------------------------------------------
string AnalizadorLexico::leerArchivo(string archivo, int tamanio)
{
    char caracter;
    string retorno=infArchivoVacio;
    ifstream lectura(archivo.c_str(), ios::binary);

    while(tamanio>0)
	{
	    lectura.read(&caracter, sizeof(char));string infResultadoOK;
	    retorno=procesar(caracter);

        int posicion=retorno.find(PUNTOTEXTO);

	    if(retorno.substr(0, posicion).compare(infCaracterInvalido.substr(0, posicion))==0)
        {
            lexicos=iniLexcios;
            return retorno;
        }

	    ++columna;
        --tamanio;
	}

	lectura.close();

    return retorno;
}
string AnalizadorLexico::leerConsulta(string consulta)
{
    char caracter;
    string retorno=infArchivoVacio;

    for(unsigned int x=0; x<consulta.length(); x++)
    {
        caracter=consulta[x];
        retorno=procesar(caracter);

        int posicion=retorno.find(PUNTOTEXTO);

	    if(retorno.substr(0, posicion).compare(infCaracterInvalido.substr(0, posicion))==0)
        {
            lexicos=iniLexcios;
            return retorno;
        }

	    ++columna;
    }

    return retorno;
}
string AnalizadorLexico::procesar(char caracter)
{
    string retorno=infResultadoOK;

    if(caracterValido(caracter)==false)
    {
        retorno=infCaracterInvalido;
        size_t posicion=retorno.find(ESCAPETEXTO);

        if(posicion!=string::npos)
        {
            retorno=retorno.replace(posicion, 3, &caracter, 0, 1);
            retorno+=infErrorPosicion+to_string(fila)+", "+to_string(columna);
        }
    }
    else
    {
        analizador(caracter);
    }

    return retorno;
}
void AnalizadorLexico::analizador(char caracter)
{
    //Estado de lectura de caracteres acumulativos
    if(estado==0)
    {
        if(caracter==ESPACIO)
        {
            if(buffer.length()>0)
            {
                estado=1;
            }
        }
        else if(tknComentarioSimple[0]==caracter)
        {
            estado=3;
        }
        else
        {
            analizadorGenerico(caracter);
        }
    }
    else if(estado==1)
    {
        if(caracter!=ESPACIO)
        {
            analizadorGenerico(caracter);
            estado=0;
        }
    }
    else if(estado==3)
    {
        if(caracter==ENTER)
        {
            estado=0;
            analizadorGenerico(caracter);
        }
    }
}
void AnalizadorLexico::analizadorGenerico(char caracter)
{
    if(isalpha(caracter) || caracter==tknIndicadorVariable[0])
    {
        if(estado==1)
        {
            buffer+=ESPACIO;
        }

        buffer+=caracter;
    }
    else if(caracter==ENTER)
    {
        ++fila;
    }
    else if(caracter==TABH)
    {

    }
    else
    {
        if(buffer.length()>0)
        {
            agregar(buffer);
            buffer="";
        }

        string guardar;
        agregar(guardar.insert(0, &caracter, 0, 1));
    }
}
bool AnalizadorLexico::caracterValido(char valido)
{
    if(estado==3)
    {
        return true;
    }

    if(isalpha(valido) ||
       valido==ENTER ||
       valido==TABH ||
       valido==ESPACIO ||
       tknFinSentencia[0]==valido ||
       tknSeparadorParametro[0]==valido ||
       tknInicioAgrupacion[0]==valido ||
       tknFinAgrupacion[0]==valido ||
       tknOperadorAnd[0]==valido ||
       tknOperadorOr[0]==valido ||
       tknOperadorNegacion[0]==valido ||
       tknOperadorIgualdad[0]==valido ||
       tknDefinicionFuncion[0]==valido ||
       tknComentarioSimple[0]==valido ||
       tknIndicadorVariable[0]==valido)
    {
        return true;
    }

    return false;
}
bool AnalizadorLexico::agregar(string token)
{
    try
    {
        struct lexico * aux=lexicos;
        lexicos=new struct lexico;

        aux->siguiente=lexicos;

        lexicos->elemento=token;
        lexicos->linea=fila;
        lexicos->siguiente=NULL;
    }
    catch(...)
    {
        return false;
    }

    return true;
}

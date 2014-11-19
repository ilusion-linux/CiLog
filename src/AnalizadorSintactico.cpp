#include "AnalizadorSintactico.h"

AnalizadorSintactico::AnalizadorSintactico(string inicio)
{
    sintacticos=new struct sintactico;
	sintacticos->elemento=inicio;
	sintacticos->elementoComplejo=NULL;
	sintacticos->siguiente=NULL;

	iniSintacticos=sintacticos;

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
	infElementoNoEsperado=configuracion->obtenerValor("INF.elementoNoEsperado");
	infResultadoOK=configuracion->obtenerValor("INF.resultadoOK");
	infErrorLinea=configuracion->obtenerValor("INF.errorLinea");
}
AnalizadorSintactico::~AnalizadorSintactico()
{
}
//Funciones Publicas----------------------------------------------------
string AnalizadorSintactico::analizar(void * structLexico)
{
    estado=0;
    string retorno="";
    lexicos=(struct lexico *)structLexico;
    lexicos=lexicos->siguiente;

    while(lexicos!=NULL)
    {
        retorno=procesar(lexicos->elemento, lexicos->linea);

        int posicion=retorno.find(PUNTOTEXTO);

        if(retorno.substr(0, posicion).compare(infElementoNoEsperado.substr(0, posicion))==0)
        {
            sintacticos=iniSintacticos;
            return retorno;
        }

        lexicos=lexicos->siguiente;
    }

    return retorno;
}
void * AnalizadorSintactico::darSintactico()
{
    //
    /*
    printw("\n------------------------------\n");
    sintacticos=iniSintacticos->siguiente;

    while(sintacticos!=NULL)
    {
        if(sintacticos->elemento.length()>0)
        {
            printw("%s\n", sintacticos->elemento.c_str());
        }
        else
        {
            sintacticosComplejo=sintacticos->elementoComplejo;
            printw("     ");

            while(sintacticosComplejo!=NULL)
            {
                printw("%s ", sintacticosComplejo->elemento.c_str());
                sintacticosComplejo=sintacticosComplejo->siguiente;
            }
            printw("\n");
        }

        sintacticos=sintacticos->siguiente;
    }
    printw("\n------------------------------\n");
    //
    */

    return sintacticos=iniSintacticos;
}
//Funciones Privadas----------------------------------------------------
string AnalizadorSintactico::procesar(string elemento, int linea)
{
    string retorno=analizador(elemento, linea);

    size_t posicion=retorno.find(ESCAPETEXTO);

    if(posicion!=string::npos)
    {
        retorno=retorno.replace(posicion, 3, elemento, 0, elemento.length());
        retorno+=infErrorLinea+to_string(linea);
    }

    return retorno;
}
string AnalizadorSintactico::analizador(string elemento, int linea)
{
    string retorno=infResultadoOK;
    struct sintactico * nullSintacticos=NULL;

    if(estado==0)
    {
        if(esReservado(elemento)==true)
        {
            retorno=infElementoNoEsperado;
        }
        else
        {
            estado=1;
            agregar(&sintacticos, elemento, linea, nullSintacticos);

            sintacticosComplejo=new struct sintactico;
            sintacticosComplejo->elemento="---";
            sintacticosComplejo->elementoComplejo=NULL;
            sintacticosComplejo->siguiente=NULL;

            iniSintacticosComplejo=sintacticosComplejo;
        }
    }
    else if(estado==1)
    {
        if(elemento.compare(tknInicioAgrupacion)==0)
        {
            estado=2;
            agregar(&sintacticos, elemento, linea, nullSintacticos);
        }
        else
        {
            retorno=infElementoNoEsperado;
        }
    }
    else if(estado==2)
    {

        if(esReservado(elemento)==true)
        {
            retorno=infElementoNoEsperado;
        }
        else
        {
            estado=3;
            agregar(&sintacticosComplejo, elemento, linea, nullSintacticos);
        }
    }
    else if(estado==3)
    {
        if(elemento.compare(tknSeparadorParametro)==0)
        {
            estado=2;
            agregar(&sintacticosComplejo, elemento, linea, nullSintacticos);
        }
        else if(elemento.compare(tknFinAgrupacion)==0)
        {
            estado=4;
            agregar(&sintacticos, "", linea, iniSintacticosComplejo->siguiente);
            agregar(&sintacticos, elemento, linea, nullSintacticos);
        }
        else
        {
            retorno=infElementoNoEsperado;
        }
    }
    else if(estado==4)
    {
        if(elemento.compare(tknFinSentencia)==0)
        {
            estado=0;
            agregar(&sintacticos, elemento, linea, nullSintacticos);
        }
        else
        {
            retorno=infElementoNoEsperado;
        }
    }

    return retorno;
}
bool AnalizadorSintactico::esReservado(string elemento)
{
    if(elemento.compare(tknDefinicionFuncion)==0 ||
       elemento.compare(tknFinAgrupacion)==0 ||
       elemento.compare(tknFinSentencia)==0 ||
       elemento.compare(tknIndicadorVariable)==0 ||
       elemento.compare(tknInicioAgrupacion)==0 ||
       elemento.compare(tknOperadorAnd)==0 ||
       elemento.compare(tknOperadorIgualdad)==0 ||
       elemento.compare(tknOperadorNegacion)==0 ||
       elemento.compare(tknOperadorOr)==0 ||
       elemento.compare(tknSeparadorParametro)==0)
    {
        return true;
    }

    return false;
}
template<class estructura>
bool AnalizadorSintactico::agregar(estructura ** lista, string elemento, int linea, estructura * elementoComplejo)
{
    try
	{
		estructura * aux=*lista;
		*lista=new estructura;
		aux->siguiente=*lista;

		(*lista)->elemento=elemento;
		(*lista)->linea=linea;
		(*lista)->elementoComplejo=elementoComplejo;

		(*lista)->siguiente=NULL;
		aux=*lista;
	}catch(...)
	{
		return false;
	}

	return true;
}

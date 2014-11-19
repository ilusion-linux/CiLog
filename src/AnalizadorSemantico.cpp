#include "AnalizadorSemantico.h"

AnalizadorSemantico::AnalizadorSemantico(string inicio)
{
    semanticos=new struct semantico;
    semanticos->elemento=inicio;
    semanticos->elementoComplejo=NULL;
    semanticos->siguiente=NULL;

    iniSemanticos=semanticos;

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
AnalizadorSemantico::~AnalizadorSemantico()
{
}
//Funciones Publicas----------------------------------------------------
string AnalizadorSemantico::analizar(void * structSintactico)
{
    estado=0;
    string retorno="";
    sintacticos=(struct sintactico *)structSintactico;
    sintacticos=sintacticos->siguiente;
    //printw("<*>%s<*>", sintacticos->elemento.c_str());

    while(sintacticos!=NULL)
    {
        //printw("<|>%s<|>", sintacticos->elemento.c_str());
        if(sintacticos->elemento.length()>0)
        {
            //printw("<->%s<->", sintacticos->elemento.c_str());
            retorno=procesar(sintacticos->elemento, sintacticos->linea);
        }
        else
        {
            sintacticosComplejo=sintacticos->elementoComplejo;
            //printw("<<>>%s<<>>", sintacticosComplejo->elemento.c_str());

            while(sintacticosComplejo!=NULL)
            {
                //printw("<+>%s<+>", sintacticosComplejo->elemento.c_str());
                retorno=procesar(sintacticosComplejo->elemento, sintacticosComplejo->linea);

                sintacticosComplejo=sintacticosComplejo->siguiente;
            }
        }

        int posicion=retorno.find(PUNTOTEXTO);

        if(retorno.substr(0, posicion).compare(infElementoNoEsperado.substr(0, posicion))==0)
        {
            semanticos=iniSemanticos;
            return retorno;
        }

        sintacticos=sintacticos->siguiente;
    }

    return retorno;
}
void * AnalizadorSemantico::darSemantico()
{
    //
        /*
    printw("\n------------------------------\n");
    semanticos=iniSemanticos->siguiente;

    while(semanticos!=NULL)
    {
        if(semanticos->elemento.length()>0)
        {
            printw("%s\n", semanticos->elemento.c_str());
        }
        else
        {
            semanticosComplejo=semanticos->elementoComplejo;
            printw("     ");

            while(semanticosComplejo!=NULL)
            {
                printw("%s ", semanticosComplejo->elemento.c_str());
                semanticosComplejo=semanticosComplejo->siguiente;
            }
            printw("\n");
        }

        semanticos=semanticos->siguiente;
    }
    printw("\n------------------------------\n");
    //    */

    return semanticos=iniSemanticos;
}
//Funciones Privadas----------------------------------------------------
string AnalizadorSemantico::procesar(string elemento, int linea)
{
    string retorno=analizador(elemento);

    size_t posicion=retorno.find(ESCAPETEXTO);

    if(posicion!=string::npos)
    {
        retorno=retorno.replace(posicion, 3, elemento, 0, elemento.length());
        retorno+=infErrorLinea+to_string(linea);
    }

    return retorno;
}
string AnalizadorSemantico::analizador(string elemento)
{
    string retorno=infResultadoOK;
    struct semantico * nullSemanticos=NULL;

    if(estado==0)
    {
        if(esReservado(elemento)==true)
        {
            retorno=infElementoNoEsperado;
        }
        else
        {
            estado=1;
            agregar(&semanticos, elemento, nullSemanticos);

            semanticosComplejo=new struct semantico;
            semanticosComplejo->elemento="---";
            semanticosComplejo->elementoComplejo=NULL;
            semanticosComplejo->siguiente=NULL;

            iniSemanticosComplejo=semanticosComplejo;
        }
    }
    else if(estado==1)
    {
        if(elemento.compare(tknInicioAgrupacion)==0)
        {
            estado=2;
            //agregar(&semanticos, elemento, linea, nullSemanticos);
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
            agregar(&semanticosComplejo, elemento, nullSemanticos);
        }
    }
    else if(estado==3)
    {
        if(elemento.compare(tknSeparadorParametro)==0)
        {
            estado=2;
            //agregar(&semanticosComplejo, elemento, nullSemanticos);
        }
        else if(elemento.compare(tknFinAgrupacion)==0)
        {
            estado=4;
            agregar(&semanticos, "", iniSemanticosComplejo->siguiente);
            //agregar(&semanticos, elemento, linea, nullSemanticos);
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
            //agregar(&semanticos, elemento, nullSemanticos);
        }
        else
        {
            retorno=infElementoNoEsperado;
        }
    }

    return retorno;
}
bool AnalizadorSemantico::esReservado(string elemento)
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
bool AnalizadorSemantico::agregar(estructura ** lista, string elemento, estructura * elementoComplejo)
{
    try
	{
		estructura * aux=*lista;
		*lista=new estructura;
		aux->siguiente=*lista;

		(*lista)->elemento=elemento;
		(*lista)->elementoComplejo=elementoComplejo;

		(*lista)->siguiente=NULL;
		aux=*lista;
	}catch(...)
	{
		return false;
	}

	return true;
}

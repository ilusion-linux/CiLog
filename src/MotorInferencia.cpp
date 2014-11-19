#include "MotorInferencia.h"

MotorInferencia::MotorInferencia(BaseConocimiento ** conocimiento)
{
    this->conocimiento=*conocimiento;

    analizador=new Analizador();

    configuracion=new Configuracion();
    configuracion->cargarConfiguracion();
    configuracion->cargarTokens();

    opcOtroResultado=configuracion->obtenerValor("OPC.otroResultado");
    infResultadoOK=configuracion->obtenerValor("INF.resultadoOK");
    infErrorPredicado=configuracion->obtenerValor("INF.errorPredicado");
    tknIndicadorVariable=configuracion->obtenerValor("indicadorVariable");
    resResSi=configuracion->obtenerValor("RES.resSi");
    resResNo=configuracion->obtenerValor("RES.resNo");
    resResTodo=configuracion->obtenerValor("RES.resTodo");

    busqueda=new struct busquedaSujeto;
    busqueda->valor=INICIOBUSQUEDA;
    busqueda->variable=INICIOBUSQUEDA;
    busqueda->siguiente=NULL;

    iniBusqueda=busqueda;

    util=new Util();
}

MotorInferencia::~MotorInferencia()
{
}
//Funciones Publicas----------------------------------------------------
void MotorInferencia::ejecutarConsulta(string consulta)
{
    conVariables=0;
    string retorno=analizador->compilarConsulta(consulta);

    if(retorno.compare(infResultadoOK)==0)
    {
        formarValoresBusqueda(analizador->darSemantico());
        ejecutarBusqueda();
    }
    else
    {
        printw(SALTO);
        printw(retorno.c_str());
    }
}
//Funciones Privadas----------------------------------------------------
void MotorInferencia::formarValoresBusqueda(void * structSemantico)
{
    semanticos=((struct semantico *)structSemantico)->siguiente;

    predicado=semanticos->elemento;

    //printw(SALTO);
    //printw(predicado.c_str());

    busqueda=iniBusqueda;
    semanticos=semanticos->siguiente;
    struct semantico * semanticosComplejo=semanticos->elementoComplejo;
    refresh();

    while(semanticosComplejo!=NULL)
    {
        //printw(SALTO);
        //printw("Elemento busqueda:", semanticosComplejo->elemento.c_str());
        agregar(semanticosComplejo->elemento);
        semanticosComplejo=semanticosComplejo->siguiente;
    }
}
void MotorInferencia::ejecutarBusqueda()
{
    predicados=(struct predicado *)conocimiento->predicadoUnico(predicado);

    if(predicados==NULL)
    {
        string msj=infErrorPredicado;

        size_t posicion=msj.find(ESCAPETEXTO);

        if(posicion!=string::npos)
        {
            msj=msj.replace(posicion, 3, predicado, 0, predicado.length());
        }

        printw(SALTO);
        printw(msj.c_str());
    }
    else
    {
        busquedaDetalle(predicados->inicioSujeto);
    }
}
void MotorInferencia::busquedaDetalle(struct sujeto * recorrer)
{
    bool hayResultado=false;

    while(recorrer!=NULL)
    {
        //printw(SALTO);
        //printw("Buscando Sujeto");

        if(busquedaCompuesta(recorrer->elemento)==true)
        {
            //printw(SALTO);
            //printw("Mostrar msj");
            //refresh();
            hayResultado=true;

            if(mostarMensaje()==false)
            {
                recorrer=NULL;
            }
        }

        if(recorrer!=NULL)
        {
            recorrer=recorrer->siguiente;
        }
    }

    if(hayResultado==false)
    {
        printw(SALTO);
        printw(resResNo.c_str());
    }
}
bool MotorInferencia::busquedaCompuesta(struct elementoCompuesto * recorrer)
{
    busqueda=iniBusqueda->siguiente;

    while(busqueda!=NULL)
    {
        busqueda->buscar="";
        busqueda->igualacion="";
        busqueda=busqueda->siguiente;
    }

    busqueda=iniBusqueda->siguiente;

    while(busqueda!=NULL)
    {
        //printw(SALTO);
        //printw("  Sujeto %s", recorrer->elemento.c_str());

        if(recorrer==NULL)
        {
            return false;
        }
        else
        {
            busqueda->buscar=recorrer->elemento;

            busqueda=busqueda->siguiente;
            recorrer=recorrer->siguiente;
        }
    }

    if(recorrer!=NULL)
    {
        return false;
    }

    return comprobarBusqueda();
}
bool MotorInferencia::comprobarBusqueda()
{
    busqueda=iniBusqueda->siguiente;

    while(busqueda!=NULL)
    {
        if(busqueda->valor.length()>0)
        {
            //printw("<<IGUALCION DE VALOR>>");
            busqueda->igualacion=busqueda->valor;
        }
        else if(busqueda->igualacion.length()==0)
        {
            //printw("<<IGUALCION>>");
            busqueda->igualacion=busqueda->buscar;

            struct busquedaSujeto * aux=busqueda->siguiente;

            while(aux!=NULL)
            {
                if(aux->variable.compare(busqueda->variable)==0)
                {
                    aux->igualacion=busqueda->igualacion;
                }

                aux=aux->siguiente;
            }
        }

        busqueda->resultado=false;

        if(util->aMinuscula(busqueda->igualacion).compare(util->aMinuscula(busqueda->buscar))==0)
        {
            busqueda->resultado=true;
        }

        busqueda=busqueda->siguiente;
    }

    busqueda=iniBusqueda->siguiente;

    while(busqueda!=NULL)
    {
        //
        /*
        printw(SALTO);
        printw("----------------------");
        printw("[%s]  ", busqueda->valor.c_str());
        printw("[%s]  ", busqueda->variable.c_str());
        printw("[%s]  ", busqueda->igualacion.c_str());
        printw("[%s]  ", busqueda->buscar.c_str());
        printw("[%d]  ", busqueda->resultado);
        printw(SALTO);
        printw("----------------------");
        //*/
        if(busqueda->resultado==false)
        {
            return false;
        }

        busqueda=busqueda->siguiente;
    }

    return true;
}
bool MotorInferencia::mostarMensaje()
{
    //printw(SALTO);
    //printw("<<Mostrando msj>>");

    if(conVariables==0)
    {
        printw(SALTO);
        printw(resResSi.c_str());

        return false;
    }
    else
    {
        printw(SALTO);
        busqueda=iniBusqueda->siguiente;

        while(busqueda!=NULL)
        {
            if(busqueda->variable.length()>0)
            {
                printw("    %s=%s", busqueda->variable.substr(1, busqueda->variable.length()).c_str(), busqueda->igualacion.c_str());
            }

            busqueda=busqueda->siguiente;
        }

        int otras=getch();

        if(opcOtroResultado[0]==otras)
        {
            return true;
        }

        return false;
    }
}
bool MotorInferencia::agregar(string valor)
{
    try
    {
        struct busquedaSujeto * aux=busqueda;
        busqueda=new struct busquedaSujeto;

        aux->siguiente=busqueda;

        busqueda->variable=esVariable(valor);

        if(busqueda->variable.length()==0)
        {
            busqueda->valor=valor;
        }
        else
        {
            conVariables=1;
        }

        busqueda->siguiente=NULL;
    }
    catch(...)
    {
        return false;
    }

    return true;
}
string MotorInferencia::esVariable(string valor)
{
    if(tknIndicadorVariable[0]==valor[0])
    {
        return valor;
    }

    return "";
}

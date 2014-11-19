#include "BaseConocimiento.h"

#include <ncurses.h>

BaseConocimiento::BaseConocimiento()
{
	predicados=new struct predicado;
	predicados->valor=PRDINICIO;
	predicados->siguiente=NULL;

	iniPredicados=predicados;

	iniElementoCompuesto=new struct elementoCompuesto;
	iniElementoCompuesto->elemento=INICIOCOMPUESTO;
	iniElementoCompuesto->siguiente=NULL;
}
//Funciones Publicas----------------------------------------------------
void BaseConocimiento::formarBaseDeHechos(void * structSemantico)
{
    semanticos=((struct semantico *)structSemantico)->siguiente;

    while(semanticos!=NULL)
    {
        if(semanticos->elemento.length()>0)
        {
            predicadosActual=(struct predicado *)predicadoUnico(semanticos->elemento);

            if(predicadosActual==NULL)
            {
                agregarPredicado(semanticos->elemento);
                predicadosActual=predicados;
            }
        }
        else
        {
            semanticosComplejo=semanticos->elementoComplejo;

            struct elementoCompuesto * compuesto=iniElementoCompuesto;

            while(semanticosComplejo!=NULL)
            {
                struct elementoCompuesto * aux=compuesto;
                compuesto=new struct elementoCompuesto;

                aux->siguiente=compuesto;

                compuesto->elemento=semanticosComplejo->elemento;
                compuesto->siguiente=NULL;

                semanticosComplejo=semanticosComplejo->siguiente;
            }

            sujetos=new struct sujeto;

            if(predicadosActual->inicioSujeto==NULL)
            {
                predicadosActual->inicioSujeto=sujetos;
                predicadosActual->finSujeto=sujetos;
            }
            else
            {
                struct sujeto * aux=predicadosActual->finSujeto;
                aux->siguiente=sujetos;
            }

            sujetos->elemento=iniElementoCompuesto->siguiente;
            sujetos->siguiente=NULL;

            predicadosActual->finSujeto=sujetos;
        }

        semanticos=semanticos->siguiente;
    }
}
void BaseConocimiento::mostrarBaseDeHechos()
{
    struct predicado * recorrer=iniPredicados->siguiente;

    printw("\n");
    while(recorrer!=NULL)
    {
        printw("[%s]\n", recorrer->valor.c_str());

        struct sujeto * recorrerSujeto=recorrer->inicioSujeto;

        while(recorrerSujeto!=NULL)
        {
            printw("   ");
            struct elementoCompuesto * recorrerCompuesto=recorrerSujeto->elemento;

            while(recorrerCompuesto!=NULL)
            {
                printw("<%s>  ", recorrerCompuesto->elemento.c_str());

                recorrerCompuesto=recorrerCompuesto->siguiente;
            }

            printw("\n");
            recorrerSujeto=recorrerSujeto->siguiente;
        }

        recorrer=recorrer->siguiente;
    }
}
void * BaseConocimiento::predicadoUnico(string predicado)
{
	struct predicado * comparacion=iniPredicados;

	while(comparacion!=NULL)
	{
		if(predicado.compare(comparacion->valor)==0)
		{
			return comparacion;
		}

		comparacion=comparacion->siguiente;
	}

	return NULL;
}
//Funciones Privadas----------------------------------------------------
bool BaseConocimiento::agregarPredicado(string elemento)
{
	try
	{
		struct predicado * aux=predicados;
		predicados=new struct predicado;

		aux->siguiente=predicados;

		predicados->valor=elemento;
		predicados->siguiente=NULL;
		predicados->inicioSujeto=NULL;

	}catch(...)
	{
		return false;
	}

	return true;
}

#include "Analizador.h"

Analizador::Analizador()
{
    inicializar();
}
Analizador::Analizador(BaseConocimiento ** conocimiento)
{
    this->conocimiento=*conocimiento;

    inicializar();
}
Analizador::~Analizador()
{
}
//Funciones Publicas----------------------------------------------------
void Analizador::inicializar()
{
    configuracion=new Configuracion();
    configuracion->cargarConfiguracion();
    dirPath=configuracion->obtenerValor("DIR.basecon");
    dirExtension=configuracion->obtenerValor("DIR.extensiones");
    infInicioAnalisis=configuracion->obtenerValor("INF.inicioAnalisis");
    infResultadoOK=configuracion->obtenerValor("INF.resultadoOK");
    infResultadoERROR=configuracion->obtenerValor("INF.resultadoERROR");
    infResultadoFinalOK=configuracion->obtenerValor("INF.resultadoFinalOK");
    infResultadoFinalERROR=configuracion->obtenerValor("INF.resultadoFinalERROR");

    directorio=new Directorio(dirExtension, LSTINICIO);

    analizaLexico=new AnalizadorLexico(LSTINICIO);
    analizaSintactico=new AnalizadorSintactico(LSTINICIO);
    analizaSemantico=new AnalizadorSemantico(LSTINICIO);
}
string Analizador::compilar()
{
    archivos=(struct archivo *)directorio->darArchivos(dirPath);
    return analizar();
}
string Analizador::compilar(string path)
{
    archivos=(struct archivo *)directorio->darArchivos(path);
    return analizar();
}
string Analizador::compilarConsulta(string consulta)
{
    return analisisLexico(consulta);
}
void * Analizador::darSemantico()
{
    return analizaSemantico->darSemantico();
}
//Funciones Privadas----------------------------------------------------
string Analizador::analizar()
{
    string retorno=infResultadoFinalOK;
    archivos=archivos->siguiente;

    while(archivos!=NULL)
    {
        printw(SALTO);
        printw("%s %s %d bytes", infInicioAnalisis.c_str(), archivos->valor.c_str(), archivos->tamanio);

        string aux=analisisLexico(archivos->valor, archivos->tamanio);

        if(aux.compare(infResultadoOK)!=0)
        {
            printw(SALTO);
            printw("   %s", infResultadoERROR.c_str());
            retorno=infResultadoFinalERROR;
        }

        printw(SALTO);
        printw("   %s", aux.c_str());

        archivos=archivos->siguiente;
    }

    printw(SALTO);
    printw(SALTO);
    return retorno;
}
string Analizador::analisisLexico(string consulta)
{
    string retorno=analizaLexico->analizarConsulta(consulta);

    if(retorno.compare(infResultadoOK)==0)
    {
        return analisisSintactico(analizaLexico->darLexico(), 1);
    }

    return retorno;
}
string Analizador::analisisLexico(string archivo, int tamanio)
{
    string retorno=analizaLexico->analizarArchivo(archivo, tamanio);

    if(retorno.compare(infResultadoOK)==0)
    {
        return analisisSintactico(analizaLexico->darLexico(), 0);
    }

    return retorno;
}
string Analizador::analisisSintactico(void * structLexico, int consulta)
{
    string retorno=analizaSintactico->analizar(structLexico);

    if(retorno.compare(infResultadoOK)==0)
    {
        return analisisSemantico(analizaSintactico->darSintactico(), consulta);
    }

    return retorno;
}
string Analizador::analisisSemantico(void * structSintactico, int consulta)
{
    string retorno=analizaSemantico->analizar(structSintactico);

    if(retorno.compare(infResultadoOK)==0)
    {
        if(consulta==0)
        {
            formarConocimiento(analizaSemantico->darSemantico());
        }
    }

    return retorno;
}
void Analizador::formarConocimiento(void * structSemantico)
{
    conocimiento->formarBaseDeHechos(structSemantico);
}

#include "CiLog.h"

CiLog::CiLog()
{
    configuracion=new Configuracion;
	conocimiento=new BaseConocimiento;

    configuracion->cargarConfiguracion();
	opcSalir=configuracion->obtenerValor("OPC.salir");
	opcComando=configuracion->obtenerValor("OPC.comando");
	opcCompilar=configuracion->obtenerValor("OPC.compilar");
	opcAyuda=configuracion->obtenerValor("OPC.ayuda");
	opcFinSentencia=configuracion->obtenerValor("OPC.finsentencia");
	opcOpciones=configuracion->obtenerValor("OPC.opciones");
	opcHistorial=configuracion->obtenerValor("OPC.historial");
	opcLimpiar=configuracion->obtenerValor("OPC.limpiar");
	opcVer=configuracion->obtenerValor("OPC.ver");
	opcHechos=configuracion->obtenerValor("OPC.hechos");
	opcOtroResultado=configuracion->obtenerValor("OPC.otroResultado");

	desVersion=configuracion->obtenerValor("DES.version");
	desInformacion=configuracion->obtenerValor("DES.informacion");
	desAutor=configuracion->obtenerValor("DES.autor");
	desSalir=configuracion->obtenerValor("DES.salir");
	desAyuda=configuracion->obtenerValor("DES.ayuda");
	desCompilar=configuracion->obtenerValor("DES.compilar");
	desHistorial=configuracion->obtenerValor("DES.historial");
	desLimpiar=configuracion->obtenerValor("DES.limpiar");
	desVer=configuracion->obtenerValor("DES.ver");
	desHechos=configuracion->obtenerValor("DES.hechos");
	desOtroResultado=configuracion->obtenerValor("DES.otroResultado");

	infComandoInvalido=configuracion->obtenerValor("INF.comandoInvalido");

	analizador=new Analizador(&conocimiento);
	motor=new MotorInferencia(&conocimiento);
	util=new Util(opcComando, opcFinSentencia, string(AYUDA));
}
CiLog::~CiLog()
{

}
//Funciones Publicas----------------------------------------------------
void CiLog::modoInterprete()
{
    string comando="comando";
	printw(opcAyuda.c_str());
	printw(SALTO);

	//Ciclo que pedira el ingreso de una nuevo comando o sentencia, hasta que se ingrese el comando para salir.
	while(comando.compare(opcSalir)!=0)
	{
		printw("cilog#");
		refresh();

		util->getText(&comando);

        comando=util->aMinuscula(comando);

		interpretarEntrada(&comando);

		printw(SALTO);
	}

	delete util;
}
//Funciones Privadas----------------------------------------------------
void CiLog::interpretarEntrada(string * comando)
{
	if(comando->compare(AYUDA)==0)
	{
		mostrarAyuda();
	}
	else if(opcComando.compare(comando->substr(0,1))==0)
	{
		*comando=comando->substr(1,comando->length());
		ejecutarComando(*comando);
	}
	else
	{
		ejecutarSentencia(*comando);
	}
}
void CiLog::ejecutarComando(string comando)
{
	if(comando.compare(opcCompilar)==0)
	{
	    printw(analizador->compilar().c_str());
	}
	else if(comando.compare(opcHechos)==0)
    {
        conocimiento->mostrarBaseDeHechos();
    }
	else if(comando.compare(opcHistorial)==0)
	{
	    configuracion->cargarHistorial();
	    configuracion->mostrarHistorial();
	}
	else if(comando.compare(opcLimpiar)==0)
	{
	    util->limpiarPantalla();
	}
	else
    {
        printw(SALTO);
        printw(infComandoInvalido.c_str());
    }
}

void CiLog::ejecutarSentencia(string sentencia)
{
    configuracion->agregarHistorial(sentencia);
    motor->ejecutarConsulta(sentencia);
}

void CiLog::mostrarAyuda()
{
    int y, x;
    getyx(stdscr, y, x);

    printw(SALTO);
    move(util->lineaValida(y+1), 0);
    printw("--------------------------------------");
    printw(SALTO);
    move(util->lineaValida(y+2), 0);
	printw(opcOpciones.c_str());

	printw(SALTO);
    printw(SALTO);
	move(util->lineaValida(y+4), 0);
	printw(string(opcComando+opcSalir).c_str());
	move(util->lineaValida(y+4), 15);
	printw(desSalir.c_str());

	printw(SALTO);
	move(util->lineaValida(y+5), 0);
	printw(string(opcComando+opcCompilar).c_str());
	move(util->lineaValida(y+5), 15);
	printw(desCompilar.c_str());

	printw(SALTO);
	move(util->lineaValida(y+6), 0);
	printw(string(opcComando+opcHistorial).c_str());
	move(util->lineaValida(y+6), 15);
	printw(desHistorial.c_str());

	printw(SALTO);
	move(util->lineaValida(y+7), 0);
	printw(string(opcComando+opcLimpiar).c_str());
	move(util->lineaValida(y+7), 15);
	printw(desLimpiar.c_str());

	printw(SALTO);
	move(util->lineaValida(y+8), 0);
	printw(string(opcComando+opcVer).c_str());
	move(util->lineaValida(y+8), 15);
	printw(desVer.c_str());

	printw(SALTO);
	move(util->lineaValida(y+9), 0);
	printw(string(opcComando+opcHechos).c_str());
	move(util->lineaValida(y+9), 15);
	printw(desHechos.c_str());

	printw(SALTO);
	move(util->lineaValida(y+10), 0);
	printw(opcOtroResultado.c_str());
	move(util->lineaValida(y+10), 15);
	printw(desOtroResultado.c_str());

	printw(SALTO);
	move(util->lineaValida(y+11), 0);
	printw(AYUDA);
	move(util->lineaValida(y+11), 15);
	printw(desAyuda.c_str());

	printw(SALTO);
	move(util->lineaValida(y+13), 0);
	printw("--------------------------------------");

	refresh();
}

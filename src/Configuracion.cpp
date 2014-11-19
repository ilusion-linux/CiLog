#include "Configuracion.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	/*for(int i=0; i<argc; i++){cout<<azColName[i]<<(argv[i] ? argv[i] : "NULL")<<endl;}*/
	return 0;
}

Configuracion::Configuracion()
{
	valores=new struct valor;
	valores->clave=LSTNOMBRE;
	valores->siguiente=NULL;
	iniValores=valores;

	comandos=new struct historial;
	comandos->comando=" ";
	comandos->anterior=NULL;
	comandos->siguiente=NULL;

	iniComandos=comandos;
	recorrer=iniComandos;
}
Configuracion::~Configuracion()
{
    valores=iniValores;
    while(valores!=NULL)
    {
        iniValores=valores;

        valores=valores->siguiente;

        delete iniValores;
    }

    comandos=iniComandos;
    while(comandos!=iniComandos->siguiente)
    {
        recorrer=comandos;

        comandos=comandos->anterior;

        delete recorrer;
    }

    comandos=iniComandos->siguiente;
    delete comandos;
}
//Funciones Publicas----------------------------------------------------
string Configuracion::abajo()
{
    return recorrido(1);
}
string Configuracion::arriba()
{
    return recorrido(0);
}
bool Configuracion::cargarTokens()
{
    if(conectar()==true)
	{
	    sqlite3_stmt * ppStmt;
		int ok=sqlite3_prepare_v2(datos, QRYTOKENS, -1, &ppStmt, NULL);

		if(ok!=SQLITE_OK)
		{
			printw(msjError);
			sqlite3_free(msjError);
			return false;
		}
		else
		{
			while(SQLITE_ROW == sqlite3_step(ppStmt))
			{
				agregar(&valores,
                        (const char *)sqlite3_column_text(ppStmt, 0),
						(const char *)sqlite3_column_text(ppStmt, 1));
			}

			sqlite3_finalize(ppStmt);
		}

		sqlite3_close(datos);

		return true;
	}

	return false;
}
bool Configuracion::cargarHistorial()
{
    noHistorial=0;
    if(conectar()==true)
	{
		sqlite3_stmt * ppStmt;
		int ok=sqlite3_prepare_v2(datos, QRYHISTORY, -1, &ppStmt, NULL);

		if(ok!=SQLITE_OK)
		{
			printw(msjError);
			printw(SALTO);
			sqlite3_free(msjError);
			return false;
		}
		else
		{
			while(SQLITE_ROW == sqlite3_step(ppStmt))
			{
				agregarHistorial((const char *)sqlite3_column_text(ppStmt, 0));
			}

			sqlite3_finalize(ppStmt);
		}

		sqlite3_close(datos);

		return true;
	}

	return false;
}
bool Configuracion::cargarConfiguracion()
{
	if(conectar()==true)
	{
		sqlite3_stmt * ppStmt;
		int ok=sqlite3_prepare_v2(datos, QRYCONFIG, -1, &ppStmt, NULL);

		if(ok!=SQLITE_OK)
		{
			printw(msjError);
			sqlite3_free(msjError);
			return false;
		}
		else
		{
			while(SQLITE_ROW == sqlite3_step(ppStmt))
			{
				agregar(&valores,
                        (const char *)sqlite3_column_text(ppStmt, 0),
						(const char *)sqlite3_column_text(ppStmt, 1));
			}

			sqlite3_finalize(ppStmt);
		}

		sqlite3_close(datos);

		return true;
	}

	return false;
}
bool Configuracion::modificarValor(string clave, string valor)
{
	return modificar(clave, valor);
}
string Configuracion::obtenerValor(string clave)
{
	return obtener(clave);
}
bool Configuracion::agregarHistorial(string comando)
{
    try
    {
        struct historial * aux=comandos;

        comandos=new struct historial;
        comandos->comando=comando;
        comandos->siguiente=iniComandos;
        comandos->anterior=aux;

        aux->siguiente=comandos;

        iniComandos->anterior=comandos;

        recorrer=iniComandos;

        ++noHistorial;

    }
    catch(...)
    {
        return false;
    }

    return true;
}
void Configuracion::actualizarConfiguracion()
{

}
void Configuracion::actualizarHistorial()
{
    if(conectar()==true)
	{
        ejecutarSQL(QRYDELHISTORY);

        int indice=1;
        string numero=obtenerValor("HST.cntgrd");
        int maxIndice=stoi(numero);
        int iniciarEn=noHistorial-maxIndice;
        /*printw("indice=%d, maxIndice=%d, iniciarEn=%d, noHistorial=%d", indice, maxIndice, iniciarEn, noHistorial);
        getch();*/
        string sql="insert into historial values";
        string coma="";

        comandos=iniComandos;

        while(comandos!=iniComandos->anterior)
        {
            if(indice>iniciarEn)
            {
                comandos=comandos->siguiente;
                sql+=coma+"("+to_string(indice)+", '"+comandos->comando+"')";
                coma=",";
            }

            ++indice;

            if(indice==maxIndice)
            {
                comandos=iniComandos->anterior;
            }
        }

        sql+=";";
        printw(sql.c_str());
        ejecutarSQL(sql.c_str());
        sqlite3_close(datos);
	}
}
void Configuracion::mostrarHistorial()
{
    int indice=0;
    string numero=obtenerValor("HST.cntmst");
    int maxIndice=stoi(numero);

    comandos=iniComandos;
    while(comandos!=iniComandos->siguiente)
    {
        comandos=comandos->anterior;
        printw(SALTO);
        printw(comandos->comando.c_str());

        indice+=1;
        if(indice==maxIndice)
        {
            comandos=iniComandos->siguiente;
        }
    }
}
void Configuracion::reiniciarHistorial()
{
    recorrer=iniComandos;
}
//Funciones Privadas----------------------------------------------------
bool Configuracion::conectar()
{
	int status=sqlite3_open(DBNOMBRE, &datos);

	if(status)
	{
		printw("Error al abrir la base de datos %s", DBNOMBRE);
		printw(SALTO);
		sqlite3_close(datos);
		return false;
	}

	return true;
}
template<class estructura>
bool Configuracion::agregar(estructura ** lista, string clave, string valor)
{
    try
	{
		estructura * aux=*lista;
		*lista=new estructura;
		aux->siguiente=*lista;

		(*lista)->clave=clave;
		(*lista)->valor=valor;

		(*lista)->siguiente=NULL;
		aux=*lista;
	}catch(...)
	{
		return false;
	}

	return true;
}
bool Configuracion::modificar(string clave, string valor)
{
	valores=iniValores;

	while(valores!=NULL)
	{
		if(clave.compare(valores->clave)==0)
		{
			valores->valor=valor;
			return true;
		}
	}

	return false;
}
string Configuracion::obtener(string clave)
{
	valores=iniValores;

	while(valores!=NULL)
	{
		if(clave.compare(valores->clave)==0)
		{
			return valores->valor;
		}

		valores=valores->siguiente;
	}
	return "NO EXISTE";
}
string Configuracion::recorrido(int movimiento)
{
    if(recorrer->siguiente!=NULL)
    {
        switch(movimiento)
        {
            case 0:
                recorrer=recorrer->anterior;
            break;
            case 1:
                recorrer=recorrer->siguiente;
            break;
        }
    }

    return recorrer->comando;
}
void Configuracion::ejecutarSQL(string sql)
{
    char * error=0;
    int estado=sqlite3_exec(datos, sql.c_str(), callback, 0, &error);

    if(estado!=SQLITE_OK)
    {
        printw(error);
        printw(SALTO);
        sqlite3_free(error);
    }
}

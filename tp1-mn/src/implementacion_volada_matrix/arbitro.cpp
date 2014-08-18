/** \file 

Copyright: 
	Copyright (C)2011 DC-FCEyN-UBA-AR.
License: 
	This file is part of arbitro (a.k.a. theforce).

	arbitro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	arbitro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with arbitro.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#ifdef _MSC_VER
#include <direct.h>
#define chdir _chdir
#else
#include <unistd.h>
#endif

using namespace std;

const double AREA_DE_JUEGO = 100.0;
const size_t DIMENSION_MAXIMA = 100;
const double TOLERANCIA_DISPARO = 0.1;
const double RADIO_EXPLOSION = 1.0;
const size_t ITERACIONES_MAXIMAS = 50;
const bool HACER_PAUSAS = false;
const bool MOSTRAR_LOG = false;
const bool MOSTRAR_TABLA = true;

const string ARCHIVO_POSICION_1 = "posicion1.txt";
const string ARCHIVO_POSICION_2 = "posicion2.txt";
const string ARCHIVO_SALIDA_1 = "salida1.txt";
const string ARCHIVO_SALIDA_2 = "salida2.txt";
const string ARCHIVO_DISPARO_PARA_1 = "disparo1.txt";
const string ARCHIVO_DISPARO_PARA_2 = "disparo2.txt";
const string ARCHIVO_ACUMULADOS_1 = "acumulados1.txt";
const string ARCHIVO_ACUMULADOS_2 = "acumulados2.txt";

// Escribe el primer archivo
bool escribir_archivo_posicion(const string & archivo, const vector<double> & canon);

// Escribe los archivos subsiguientes
bool escribir_archivo_disparo(const string & archivo, int turno, const vector<double> & disparo, const vector< vector<double> > & warp, ios_base::openmode mode);

// Lee el archivo de salida
bool leer_archivo(const string & archivo, int turno, vector<double> & disparo, vector< vector<double> > & warp);

// Chequea que el disparo sea correcto
bool chequear_disparo(const vector<double> & canon, const vector< vector<double> > & warp, const vector<double> & disparo);

// Indica si el cañòn fue alcanzado
double norma_diferencia(const vector<double> & canon, const vector<double> & disparo);

// Cuerpo principal
int main(int argc, char *argv[])
{
	// Presentación
	cout << "La Guerra Lineal, Episodio 5 - Arbitro 2.0\n";

	// Chequea el número de parámetros
	if( argc < 6 )
	{
		cerr << "Uso: " << argv[0] << " directorio1 ejecutable1 directorio2 ejecutable2 dimension [semilla]\n";
		cerr << "Los directorios deben especificarse como caminos absolutos\n";
		cerr << "(por ejemplo: h:\\guerra\\jugador1)\n";
		return 1;
	}

	// Copia a variables locales los parámetros
	const string directorio1(argv[1]);
	const string ejecutable1(argv[2]);
	const string directorio2(argv[3]);
	const string ejecutable2(argv[4]);
	const int nn = atoi(argv[5]);
	if( nn < 1 || nn > DIMENSION_MAXIMA )
	{
		cerr << "Error! El valor de n debe estar entre 2 y " << DIMENSION_MAXIMA << " inclusive.\n";
		return 2;
	}
	const size_t n = nn;

	cout << "\nDim: " << n;

	// Inicializa la semilla aleatoria
	const int semilla = (argc == 6 ? time(NULL) : atoi(argv[6]));
	srand(semilla);
	cout << "\nSemilla: " << semilla;

	// Inicializa las posiciones de ambos cañones
	vector<double> canon1(n);
	vector<double> canon2(n);
	for(size_t i=0; i<n; ++i)
	{
		canon1[i] = (AREA_DE_JUEGO * rand()) / static_cast<double>(RAND_MAX);
		canon2[i] = (AREA_DE_JUEGO * rand()) / static_cast<double>(RAND_MAX);
	}

	cout << "\nCanyon 1: ";
	for(size_t i=0; i<n; ++i)
		cout << canon1[i] << " ";

	cout << "\nCanyon 2: ";
	for(size_t i=0; i<n; ++i)
		cout << canon2[i] << " ";

	cout << "\n\n";

	if( HACER_PAUSAS )
		cin.get();

	// Auxiliares para guardar los disparos y las transformaciones de cada jugador
	vector<double> disparo1(n);
	vector<double> disparo2(n);
	vector< vector<double> > warp1(n,vector<double>(n)); //copia innecesariamente, pero bueh
	vector< vector<double> > warp2(n,vector<double>(n)); //copia innecesariamente, pero bueh

	// Ejecutables y archivos de salida y entrada
	const string posicion1 = directorio1 + "\\" + ARCHIVO_POSICION_1;
	const string posicion2 = directorio2 + "\\" + ARCHIVO_POSICION_2;
	const string salida1 = directorio1 + "\\" + ARCHIVO_SALIDA_1;
	const string salida2 = directorio2 + "\\" + ARCHIVO_SALIDA_2;
	const string entrada1 = directorio1 + "\\" + ARCHIVO_DISPARO_PARA_1;
	const string entrada2 = directorio2 + "\\" + ARCHIVO_DISPARO_PARA_2;
	const string acumulados1 = directorio1 + "\\" + ARCHIVO_ACUMULADOS_1;
	const string acumulados2 = directorio2 + "\\" + ARCHIVO_ACUMULADOS_2;
	string path1 = ejecutable1 + " " + ARCHIVO_POSICION_1 + " " + ARCHIVO_SALIDA_1 + " >> salida1.out";
	string path2 = ejecutable2 + " " + ARCHIVO_POSICION_2 + " " + ARCHIVO_SALIDA_2 + " >> salida2.out";
	const string path1b = ejecutable1 + " " + ARCHIVO_POSICION_1 + " " + ARCHIVO_SALIDA_1 + 
		" " + ARCHIVO_DISPARO_PARA_1 + " " + ARCHIVO_ACUMULADOS_1 + " >> salida1.out";
	const string path2b = ejecutable2 + " " + ARCHIVO_POSICION_2 + " " + ARCHIVO_SALIDA_2 + 
		" " + ARCHIVO_DISPARO_PARA_2 + " " + ARCHIVO_ACUMULADOS_2 + " >> salida2.out";

	// constantes durante todo el juego
	if ( !escribir_archivo_posicion(posicion1, canon1) ) return 3;
	if ( !escribir_archivo_posicion(posicion2, canon2) ) return 3;
	if( HACER_PAUSAS && MOSTRAR_LOG)
	{
		cout << " - Archivos de posicion escritos\n";
		cin.get();
	}


	if( MOSTRAR_TABLA )
	{
		cout << "      Turno       Dist. J1      Dist. J2\n";
		cout << "----------------------------------------------\n";
	}

	// Comienza la acción
	bool eliminado1 = false;
	bool eliminado2 = false;
	for(int i=0; i<ITERACIONES_MAXIMAS && !eliminado1 && !eliminado2; ++i)
	{
		// Ejecuta los programas
		if( MOSTRAR_LOG )
		{
			cout << "Ejecutando turno " << i+1 << "!\n";
			cout << " - Ejecutando primer jugador ...\n";
			cout << "   " << path1 << "\n";
		}
		if( HACER_PAUSAS )
			cin.get();

		chdir(directorio1.c_str());
		system(path1.c_str());

		if( HACER_PAUSAS && MOSTRAR_LOG )
		{
			cout << " - Primer jugador terminado\n";
			cin.get();
		}

		if( MOSTRAR_LOG )
		{
			cout << " - Ejecutando segundo jugador ...\n";
			cout << "   " << path2 << "\n";
		}
		if( HACER_PAUSAS )
			cin.get();

		chdir(directorio2.c_str());
		system(path2.c_str());

		if( HACER_PAUSAS && MOSTRAR_LOG )
		{
			cout << " - Segundo jugador terminado\n";
			cin.get();
		}

		// Prepara para el siguiente turno
		if( i == 0 )
		{
			//los creamos vacios
			ofstream(acumulados1.c_str(), ios_base::trunc);
			ofstream(acumulados2.c_str(), ios_base::trunc);
			path1 = path1b;
			path2 = path2b;
		}
		else
		{
			if( escribir_archivo_disparo(acumulados1, i, disparo2, warp2, ios_base::app) == false ) return 4;
			if( escribir_archivo_disparo(acumulados2, i, disparo1, warp1, ios_base::app) == false ) return 4;
		}

		// Lee los archivos
		if( leer_archivo(salida1, i+1, disparo1, warp1) == false ) eliminado1 = true;
		if( leer_archivo(salida2, i+1, disparo2, warp2) == false ) eliminado2 = true;

		// Revisa que no hayan hecho trampa
		if( chequear_disparo(canon1, warp1, disparo1) == false ) eliminado1 = true;
		if( chequear_disparo(canon2, warp2, disparo2) == false ) eliminado2 = true;

		// Prepara para el siguiente turno
		if( escribir_archivo_disparo(entrada1, i+1, disparo2, warp2, ios_base::trunc) == false ) return 4;
		if( escribir_archivo_disparo(entrada2, i+1, disparo1, warp1, ios_base::trunc) == false ) return 4;
		if( HACER_PAUSAS && MOSTRAR_LOG )
		{
			cout << " - Archivos de disparo escritos - Turno " << i+1 << "\n";
			cin.get();
		}

		// Muestra las distancias de las explosiones
		double dif1 = norma_diferencia(canon1, disparo2);
		double dif2 = norma_diferencia(canon2, disparo1);

		if( MOSTRAR_LOG )
		{
			cout << " - Distancia de la explosion al jugador 1: " << dif1 << "\n";
			cout << " - Distancia de la explosion al jugador 2: " << dif2 << "\n";
		}

		if( MOSTRAR_TABLA )
			cout << "     " << setw(4) << (i+1) 
				<< "       " << setw(10) << setprecision(2) << fixed << dif1
				<< "    " << setw(10) << setprecision(2) << fixed << dif2
				<< " \n";

		if( HACER_PAUSAS )
			cin.get();

		// Determina si algún cañón fue destruido
		if( dif1 <= RADIO_EXPLOSION )
		{
			cout << "El primer jugador fue destruido!\n";
			eliminado1 = true;
		}

		if( dif2 <= RADIO_EXPLOSION )
		{
			cout << "El segundo jugador fue destruido!\n";
			eliminado2 = true;
		}
	}

	// Mensajes finales
	if( eliminado1 == true && eliminado2 == true )
		cout << "Double knock-out! Es un empate, senyores.\n";
	else if( eliminado1 == true )
		cout << "El ganador es el jugador " << directorio2 << "\\" << ejecutable2 << "\n";
	else if( eliminado2 == true )
		cout << "El ganador es el jugador " << directorio1 << "\\" << ejecutable1 << "\n";
	else
		cout << "Empate por aburrimiento! No hay ganadores ...\n";

	return 0;
}

/// Escribe el primer archivo
bool escribir_archivo_posicion(const string & archivo, const vector<double> & canon)
{
	ofstream F(archivo.c_str());
	if( !F.good() )
	{
		cerr << "No se pudo escribir el archivo " << archivo << "!\n";
		return false;
	}
	F.precision(20);
	F.setf(std::ios_base::scientific, std::ios_base::floatfield);

	const size_t n = canon.size();
	F << "0\n";
	F << n << '\n';

	for(size_t i=0; i<n; ++i)
		F << canon[i] << ' ';

	F << '\n';
	F.close(); //esto parece innecesario

	return true;
}

// Escribe los archivos subsiguientes
bool escribir_archivo_disparo(const string & archivo, int turno, const vector<double> & disparo, const vector< vector<double> > & warp, ios_base::openmode mode)
{
	ofstream F(archivo.c_str(), mode);
	if( !F.good() )
	{
		cerr << "No se pudo escribir el archivo " << archivo << "!\n";
		return false;
	}
	F.precision(20);
	F.setf(std::ios_base::scientific, std::ios_base::floatfield);

	const size_t n = disparo.size();
	F << turno << '\n';
	F << n << '\n';

	for(size_t i=0; i<n; ++i)
		F << disparo[i] << ' ';

	F << '\n';

	for(size_t i=0; i<n; ++i)
	{
		for(size_t j=0; j<n; ++j)
			F << warp[i][j] << ' ';

		F << '\n';
	}

	F.close();
	return true;
}

// Lee el archivo de salida
bool leer_archivo(const string & archivo, int turno, vector<double> & disparo, vector< vector<double> > & warp)
{
	ifstream F(archivo.c_str());
	if( !F.good() )
	{
		cerr << "No se pudo leer el archivo " << archivo << "!\n";
		return false;
	}

	int _turno, _n;
	F >> _turno >> _n;

	if( turno != _turno )
	{
		cerr << "El turno del archivo " << archivo << " no es correcto!\n";
		return false;
	}

	const size_t n = disparo.size();
	if( n != _n )
	{
		cerr << "La dimension del archivo " << archivo << " no es correcta!\n";
		return false;
	}

	for(size_t i=0; i<n; ++i)
		F >> disparo[i];

	for(size_t i=0; i<n; ++i)
		for(size_t j=0; j<n; ++j)
			F >> warp[i][j];

	F.close();
	return true;
}

// Chequea que el disparo sea correcto
bool chequear_disparo(const vector<double> & canon, const vector< vector<double> > & warp, const vector<double> & disparo)
{
	const size_t n = canon.size();
	for(size_t i=0; i<n; ++i)
	{
		double x = 0;
		for(size_t j=0; j<n; ++j)
			x += warp[i][j] * canon[j];

		if( fabs( x-disparo[i] ) > TOLERANCIA_DISPARO )
		{
			cout << "Error! La coordenada " << i+1 << " del disparo no esta correctamente calculada!\n";
			return false;
		}
	}

	return true;
}

// Indica si el cañòn fue alcanzado
double norma_diferencia(const vector<double> & canon, const vector<double> & disparo)
{
	const size_t n = canon.size();
	double norma = 0;
	for(size_t i=0; i<n; ++i)
		norma += (canon[i] - disparo[i]) * (canon[i] - disparo[i]);

	return sqrt(norma);
}

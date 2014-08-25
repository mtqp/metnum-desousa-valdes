#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>

#define DEBUG

#define UNDEFINED_TEMPERATURE 2048.0
#define ERROR_TEMPERATURE -2048.0
#define EPS 1.0e-4

#ifdef DEBUG
#define DEBUGMSG(X)  cout << X << endl;
#else
#define DEBUGMSG(X)
#endif

using namespace std;

typedef double Temp;

struct Point {
	double x, y;
	Point();
	Point(const double posx, const double posy) : x(posx), y(posy) {};
	
	friend bool operator==(const Point p1, const Point p2){
		return abs(p1.x - p2.x) <= EPS  && abs(p1.y - p2.y) <= EPS;
	}
};

struct PointDiscr {
	int i, j;
	PointDiscr();
	PointDiscr(const int posx, const int posy) : i(posx), j(posy) {};
	
	friend ostream& operator<<(ostream& stream, const PointDiscr p1){
		stream << "i: " << p1.i << "; j: " << p1.j << endl;
		return stream;
	}
	
	friend bool operator==(const PointDiscr p1, const PointDiscr p2){
		return p1.i == p2.i && p1.j == p2.j;
	}
};

struct PB_Matrix {
	Temp** matrix;
	int discr_width, discr_height;
	double discretization;

	PB_Matrix() : matrix(NULL), discr_width(0), discr_height(0), discretization(0) {};
	PB_Matrix(Temp** m, int d_w, int d_h, double discr) : matrix(m), discr_width(d_w), discr_height(d_h), discretization(discr) {};
	~PB_Matrix(){
		if (matrix != NULL){
			for (int i = 0; i < discr_height; i++)
				delete [] matrix[i];
			delete [] matrix;
		}
	}
	
	void set_borders(){
		// Pongo los bordes en -100 (a revisar)
		for( int j = 0; j < discr_width; j++) {
			matrix[0][j] = -100.0;
			matrix[discr_height-1][j] = -100.0;
		}

		for (int i = 0; i < discr_height; i++){
			matrix[i][0] = -100.0;
			matrix[i][discr_width-1] = -100.0;
		}
	};

	Temp get(const Point p){
		double intpart, d_x = p.x/discretization, d_y = p.y/discretization;
		
		if (modf(d_x, &intpart) <= 0.0 + EPS && modf(d_y, &intpart) <= 0.0 + EPS)	//Si da una cuenta entera. VER ERROR RELATIVO EPS
			return matrix[(int)d_x][(int)d_y];
		else
			return ERROR_TEMPERATURE;
	};
};

double get_norm_2(double a_x, double a_y, double b_x, double b_y){
	return sqrt((a_x - b_x)*(a_x - b_x) + (a_y - b_y)*(a_y - b_y));
}

struct Leech {
	int id;
	Point position;
	double discretization, radius;
	vector<PointDiscr> leeched_points;

	vector<PointDiscr> affected_points(const Point p){
		vector<PointDiscr> affected_points;
		double x_low, x_high, y_low, y_high;
		
		// Defino los intervalos reales
		x_low = p.x - radius;
		x_high = p.x + radius;
		y_low = p.y - radius;
		y_high = p.y + radius;
		int disc_x_low, disc_x_high, disc_y_low, disc_y_high;

		// Defino los intervalos CERRADOS discretos
		disc_x_low = (int)ceil(x_low / discretization);
		disc_x_high = (int)floor( x_high / discretization);
		disc_y_low = (int)ceil( y_low / discretization);
		disc_y_high = (int)floor(y_high / discretization);
		
		for (int i = disc_x_low; i < disc_x_high; i++){
			double real_x = i*discretization;
			for (int j = disc_y_low; j < disc_y_high; j++){
				double real_y = j*discretization;
				if (get_norm_2(p.x, p.y, real_x, real_y) <= radius)
					affected_points.push_back(PointDiscr(i,j));
			}
		}
		
		return affected_points;
	};

	Leech();
	Leech(const int id_leech, const Point p, double d, double r)
		: id(id_leech), position(p), discretization(d), radius(r), leeched_points(affected_points(p)) {};
};

class Parabrisas {
	public:
		Parabrisas();
		~Parabrisas();
		int read_from_input(char* input_file);
		void calculate_temps();
		void kill_leech();
		int write_output(char* output_file);
		double temperatureOnCriticalPoint();
		
	private:

		int rowIndexToWindShield(int i);
		int colIndexToWindShield(int j);
		void gaussianElimination();
		void addLeechInfo();
		void recreateWindShield(vector<double>& vectorX);
		void updateRowJ(double i_j_multiplier, int rowToUse, int rowToUpdate);
		vector<double> resolveTriangularMatrix();
		void create_all_matrices();
		bool is_affected(int i, int j);
		bool is_border(int i, int j);
		
		void imprimir(double** matrix, int i_range, int j_range);
		double width, height, discr_interval, radius, temp;
		int discr_height, discr_width;
		vector<Leech> leeches;
		
		PB_Matrix* pb_matrix;
		double** matrix_A;
		Temp* matrix_B;
};

void Parabrisas :: imprimir(double** matrix, int i_range, int j_range)
{
	/*int dimFi = matrix[0].size();
	int dimCol =matrix[0].;
	cout << "Dimension Filas = " << dimFi << endl;
	cout << "Dimension Columnas = " <<  dimCol<< endl;*/
  
	for(int i=0; i<  i_range; i++){
		cout << endl;
		for(int j=0; j< j_range; j++){
			cout << matrix[i][j] << "\t";
		}
	}
	
	cout << endl;

}

Parabrisas::Parabrisas() { }

Parabrisas::~Parabrisas() { 
	if (pb_matrix != NULL)
		delete pb_matrix;
	if (matrix_A != NULL){
		for (int i = 0; i < discr_height * discr_width; i++)
			delete [] matrix_A[i];
		delete [] matrix_A;
	}
	if (matrix_B != NULL)
		delete [] matrix_B;
	
}

int Parabrisas::read_from_input(char* input_file) {
	ifstream file;
	file.open(input_file);
	
	if (file.is_open()){
		// Inicializo los valores a b h r t k.
		int amount_of_leeches;
		file >> width >> height >> discr_interval >> radius >> temp >> amount_of_leeches;
	
		// Creo el vector de sanguijuelas. 
		// Para cada sanguijuela creada veo los puntos afectados y los guardo en su propia estructura.
	
		for(int i = 0; i < amount_of_leeches; i++) {
			double x, y;
			file >> x >> y;
			leeches.push_back(Leech(i, Point(x,height - y), discr_interval, radius));
		}

		/*for (int i = 0; i < amount_of_leeches; i++){
			for (int j = 0; j < (int)leeches[i].leeched_points.size(); j++)
				cout << "punto afectado: " << leeches[i].leeched_points[j] << endl;
		}*/

		file.close();	

		create_all_matrices();		
	} else {
		cout << "Unable to open input file" << endl;		
		return 1;
	}

	return 0;
}

void Parabrisas::create_all_matrices(){
	// En la creación de la estructura, guardo el n+1 m+1 reales resultantes de dividir el width y height por el intervalo.
	
	discr_width = (width / discr_interval) + 1;	//Techo, piso, round? Se asume correcto entero. N+1 y M+1!! CONSULTAR
	discr_height = (height / discr_interval) + 1;


/****** 	Creación de la matriz del parabrisas final 	*******/
	
	Temp** pb_discr_matrix= new Temp*[discr_width];
	for (int j = 0; j < discr_width; j++){
		Temp* v = new Temp[discr_height];
		for (int i = 0; i < discr_height; i++)
			v[i] = UNDEFINED_TEMPERATURE;	// Temperatura arbitraria para las no-calculadas = UNDEFINED TEMPERATURE
			
		pb_discr_matrix[j] = v;
	}

	
	pb_matrix = new PB_Matrix(pb_discr_matrix, discr_width, discr_height, discr_interval);
	pb_matrix->set_borders();
	

/****** 	Creación de la matriz A 	******/

	int complete_grid_size = discr_width * discr_height;
	
	matrix_A = new double*[complete_grid_size];
	for (int j = 0; j < complete_grid_size; j++){
		double* v = new double[complete_grid_size];
		
		for (int i = 0; i < complete_grid_size; i++)	// Lleno todo con 0 por default
			v[i] = 0;
			
		matrix_A[j] = v;
	}
	
	addLeechInfo();

/******		Creación de la matriz B (pb_matrix aplanada) 	******/

	matrix_B = new Temp[complete_grid_size];
	for (int i = 0; i < discr_height; i++){
		for (int j = 0; j < discr_width; j++){
			int index = (i * (discr_width)) + j;
			
			if (is_border(i,j))
				matrix_B[index] = -100;
			else if (is_affected(i,j))
				matrix_B[index] = temp;
			else
				matrix_B[index] = 0;
		}
	}
}

bool Parabrisas::is_affected(int ai, int aj){
	for (unsigned int i = 0; i < leeches.size(); i++){
		for (unsigned int j = 0; j < leeches[i].leeched_points.size(); j++){
			if (leeches[i].leeched_points[j] == PointDiscr(ai,aj)) return true;
		}
	}

	return false;
}

bool Parabrisas::is_border(int i, int j){
	return i == 0 || j == 0 || i == (discr_height-1) || j == (discr_width-1);
}

vector<double> Parabrisas::resolveTriangularMatrix(){
	vector<double> vectorX; 
	//double sum = 0.0;
	for (int i = (discr_height * discr_width) - 1; i >= 0; i--){
		//Calculate Xi
		double b_i = matrix_B[i];
		double a_ii = matrix_A[i][i];
		double sum = 0.0;
		
		int xs = (int)(vectorX.size()-1);
		for (int j = (discr_height * discr_width) - 1;  j >= i+1 && i != 0; j--){
			sum += matrix_A[i][j] * vectorX[xs];
			xs--;
		}
		double x_i = (b_i - sum) / a_ii;
	
		//Update vector
		vectorX.insert(vectorX.begin(), x_i);		
	}
	return vectorX;
}

int Parabrisas :: rowIndexToWindShield(int i){
	return i / discr_height;
}

int Parabrisas :: colIndexToWindShield(int j){
	return j % discr_width;
}

void Parabrisas :: recreateWindShield(vector<double>& vectorX){
	for (int i = 0; i < discr_height * discr_width; i++)
	{
		pb_matrix->matrix[rowIndexToWindShield(i)][colIndexToWindShield(i)] = vectorX[i];
	}
	
}

void Parabrisas::addLeechInfo(){
	for (int i = 0; i < discr_height * discr_width; i++){	// Voy fila por fila y me fijo la diagonal (el triangulo inferior queda 0)
		int j = i;											// Para seguir nuestro modelo
		
		if (is_border(rowIndexToWindShield(i),colIndexToWindShield(j)))
			matrix_A[i][j] = 1;
		else if (is_affected(i,j))
			matrix_A[i][j] = 1;
		else {
			matrix_A[i][j] = -4;
			matrix_A[i][j + discr_width]  = 1; // i-1 en la fila
			matrix_A[i][j - discr_width] = 1; // i+1 en la fila
			matrix_A[i][j-1] = 1;
			matrix_A[i][j+1] = 1;
		}
	}	
}

void Parabrisas::calculate_temps() {
	//cout << "ANTES A: " << endl;
	/*int size = discr_height * discr_width;
	
	imprimir(matrix_A, size, size);*/
	cout << endl << "ANTES B: " << endl;
	for (int i = 0; i < discr_height*discr_width; i++)
		cout << matrix_B[i] << endl;
	
	gaussianElimination();
	/*cout << "DESPUES A: " << endl;
	imprimir(matrix_A, size, size);
	
	cout << endl << "DESPUES B: " << endl;
	for (int i = 0; i < discr_height*discr_width; i++)
		cout << matrix_B[i] << endl;*/
	
	vector<double> temperatureVector = resolveTriangularMatrix();
	
	recreateWindShield(temperatureVector);
}

void Parabrisas::kill_leech() {
	//killCriticalPointNearestLeech();
	leeches.pop_back();
}

void Parabrisas::updateRowJ(double i_j_multiplier, int rowToUse, int rowToUpdate){
	for(int j = 0; j < discr_width * discr_height; j++){ 
		//double valorAnterior = matrix_A[rowToUpdate][j];
		matrix_A[rowToUpdate][j] = matrix_A[rowToUpdate][j] - i_j_multiplier * matrix_A[rowToUse][j];
		/*if(valorAnterior != 0)
		{		
			cout << "rowToUse: " << rowToUse << "; i_j_multiplier: " << i_j_multiplier << endl;
			cout << "i:" << rowToUpdate << " j:" << j << "; antes:" << valorAnterior << " ahora:" << matrix_A[rowToUpdate][j] << endl << endl;
		}*/
	}
	matrix_B[rowToUpdate] = matrix_B[rowToUpdate] - i_j_multiplier * matrix_B[rowToUse];
} 

void Parabrisas::gaussianElimination() {
	for (int i = 0; i < (discr_height * discr_width) - 1; i++){
		for (int j = i+1; j < discr_height * discr_width; j++){			// Ver si los bordes estan bien
			if(abs(matrix_A[i][i])  < EPS){
				cout << "0 en la diagonal en iteracion:" << i << endl;
				return; 
			}
			double i_j_multiplier = matrix_A[j][i] / matrix_A[i][i];			// Chequear por error
			updateRowJ(i_j_multiplier, i, j);
		}
	}
}

double Parabrisas :: temperatureOnCriticalPoint(){
	double middle_x = height / 2;
	double middle_y = width / 2;
	Point criticalPoint = Point(middle_x, middle_y);
	
	return pb_matrix->get(criticalPoint);
}

int Parabrisas::write_output(char* output_file) {
	ofstream file;
	
	file.open(output_file);
	
	if (file.is_open()){				//chequea que este abierto x las dudas
		for(int i = 0; i < pb_matrix->discr_width; i++){
			for(int j = 0; j < pb_matrix->discr_height; j++){
				file << i << "\t" << j << "\t" ;
				file << fixed;
				file << setprecision(5) << pb_matrix->matrix[i][j] << endl; 
			}
		}

		file.close();
		return 0;
	 }
	 else { 
		cout << "Unable to open output file" << endl;		// si no pudo da esto... SE VE SI SE PONE REALMENTE
		return 1;
	}
}

int main(int argc, char *argv[]) {
	Parabrisas pb;
	
	char* input_file = argv[1];
	char* output_file = argv[2];

	if(pb.read_from_input(input_file)) exit(1);
	//if(pb.write_output(output_file)) exit(1);
	pb.calculate_temps();
	cout << "temperatura en punto critico: " << fixed << setprecision(5) << pb.temperatureOnCriticalPoint() << endl;
	while (pb.temperatureOnCriticalPoint() >= 235.0){
		pb.kill_leech();
		pb.calculate_temps();
	}
	
	if(pb.write_output(output_file)) exit(1);
	
	return 0;
}
 

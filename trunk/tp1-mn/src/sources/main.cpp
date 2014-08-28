#include "structs.h"

using namespace std;

double get_norm_2(double a_x, double a_y, double b_x, double b_y){
	return sqrt((a_x - b_x)*(a_x - b_x) + (a_y - b_y)*(a_y - b_y));
}

class Parabrisas {
	public:
		Parabrisas();
		~Parabrisas();
		int read_from_input(char* input_file);
		void calculate_temps();
		void kill_leech();
		bool freeOfLeeches();
		int write_output(char* output_file);
		double temperatureOnCriticalPoint();
		
	private:

		void create_all_matrices();
		void createPBMatrix();
		void createMatrixA();
		void createMatrixB();
		
		void addLeechInfo();
		void gaussianElimination();
		void updateRowJ(double i_j_multiplier, int rowToUse, int rowToUpdate);
		vector<double> resolveTriangularMatrix();
		void recreateWindShield(vector<double>& vectorX);
		
		bool is_affected(int i, int j);
		bool is_border(int i, int j);
		int rowIndexToWindShield(int i);
		int colIndexToWindShield(int j);
		
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
	
	
		// En la creación de la estructura, guardo el n+1 m+1 reales resultantes de dividir el width y height por el intervalo.
		discr_width = (width / discr_interval) + 1;
		discr_height = (height / discr_interval) + 1;
		
		// Creo el vector de sanguijuelas. 
		// Para cada sanguijuela creada veo los puntos afectados y los guardo en su propia estructura.
	
		for(int i = 0; i < amount_of_leeches; i++) {
			double x, y;
			file >> x >> y;
			Point pointLeech = Point(x,y);
			Leech leech = Leech(i, pointLeech, discr_width, discr_height, discr_interval, radius); 
			leech.leeched_points = leech.affected_points(pointLeech);
			
			/*for (int j = 0; j < (int)leech.leeched_points.size(); j++)
				cout << leech.leeched_points[i];*/
				
			if (!leech.leeched_points.empty())
				leeches.push_back(leech);
		}

		file.close();	

		create_all_matrices();		
	} else {
		cout << "Unable to open input file" << endl;		
		return 1;
	}

	return 0;
}

void Parabrisas::createPBMatrix(){
	Temp** pb_discr_matrix= new Temp*[discr_height];
	for (int j = 0; j < discr_height; j++){
		Temp* v = new Temp[discr_width];
		for (int i = 0; i < discr_width; i++)
			v[i] = UNDEFINED_TEMPERATURE;	// Temperatura arbitraria para las no-calculadas = UNDEFINED TEMPERATURE
			
		pb_discr_matrix[j] = v;
	}
	
	pb_matrix = new PB_Matrix(pb_discr_matrix, discr_width, discr_height, discr_interval);
}

void Parabrisas:: createMatrixA(){
	int complete_grid_size = discr_width * discr_height;
	
	matrix_A = new double*[complete_grid_size];
	for (int j = 0; j < complete_grid_size; j++){
		double* v = new double[complete_grid_size];
		
		for (int i = 0; i < complete_grid_size; i++)	// Lleno todo con 0 por default
			v[i] = 0;
			
		matrix_A[j] = v;
	}
	
	addLeechInfo();
}

void Parabrisas::addLeechInfo(){
	int index_i = 0;
	int index_j = 0;
	
	for (int i = 0; i < discr_height * discr_width; i++){	// Voy fila por fila y me fijo la diagonal (el triangulo inferior queda 0)
		int j = i;											// Para seguir nuestro modelo

		//cout << "es borde " << i << ", " << j << ": " << is_border(index_i,index_j) << " index_i: " << index_i << "; index_j: " << index_j << endl;
		if (is_border(index_i,index_j))
			matrix_A[i][j] = 1;
		else if (is_affected(index_i,index_j))
			matrix_A[i][j] = 1;
		else {
			matrix_A[i][j] = -4;
			matrix_A[i][j + discr_width]  = 1; // i-1 en la fila
			matrix_A[i][j - discr_width] = 1; // i+1 en la fila
			matrix_A[i][j-1] = 1;
			matrix_A[i][j+1] = 1;
		}
		index_j++;
		if(index_j == discr_width) {
			index_i++;
			index_j = 0;
		}
	}	
}

void Parabrisas::createMatrixB(){
	matrix_B = new Temp[discr_height * discr_width];
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

void Parabrisas::create_all_matrices(){
	createPBMatrix();
	createMatrixA();
	createMatrixB();
}

bool Parabrisas::is_affected(int ai, int aj){
	for (unsigned int i = 0; i < leeches.size(); i++){
		for (unsigned int j = 0; j < leeches[i].leeched_points.size(); j++){
			//cout << leeches[i].leeched_points[j];
			/*	cout << PointDiscr(ai,aj);*/
			if (leeches[i].leeched_points[j] == PointDiscr(ai,aj)) return true;
		}
	}

	return false;
}

bool Parabrisas::is_border(int i, int j){
	return i == 0 || j == 0 || i == (discr_height-1) || j == (discr_width-1);
}

vector<double> Parabrisas::resolveTriangularMatrix(){
	int n = discr_height * discr_width;
	vector<double> vectorX = vector<double>(n); 

	for (int i = n - 1; i >= 0; i--){
		vectorX[i] = matrix_B[i];
		for (int j = i+1; j < n; j++){
			vectorX[i] -= matrix_A[i][j] * vectorX[j];
		}
		
		vectorX[i] /= matrix_A[i][i];	
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
	//cout << "discr_height: " << discr_height << endl;
	//cout << "discr_width: " << discr_width << endl;
	//cout << "rowIndex: " << rowIndexToWindShield(i) << endl;
	//cout << "colIndex: " << colIndexToWindShield(i) << endl;
	//cout << "i: " << i << endl; //361
	for (int i = 0; i < discr_height; i++)
	{
		for (int j = 0; j < discr_width; j++)
		{
			pb_matrix->matrix[i][j] = vectorX[i*discr_width + j];
		}
	}
}

void Parabrisas::calculate_temps() {
	/*cout << "ANTES A: " << endl;
	int size = discr_height * discr_width;
	
	imprimir(matrix_A, size, size);

	cout << endl << "ANTES B: " << endl;
	for (int i = 0; i < discr_height*discr_width; i++)
		cout << matrix_B[i] << endl;*/
	
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
	// Busco el punto afectado mas cercano al centro y quito su sanguijuela asociada
	double middle_x = height / 2;
	double middle_y = width / 2;
	
	double closestDistance = width * height;	//Esta distancia es mas grande que cualquier punto afectado al centro
	int leechToKillId = 0;
	
	
	for (int i = 0 ; i < (int) leeches.size(); i++){
		for (int j = 0; j < (int) leeches[i].leeched_points.size(); j++){
			PointDiscr affectedPoint = leeches[i].leeched_points[j];
			double real_affP_x = affectedPoint.i * discr_interval;
			double real_affP_y = affectedPoint.j * discr_interval;
			double distanceToMiddle = get_norm_2(real_affP_x, real_affP_y, middle_x, middle_y);
			
			if ( distanceToMiddle < closestDistance ){
				closestDistance = distanceToMiddle;
				leechToKillId = i;
			} else if ( abs(distanceToMiddle - closestDistance) < EPS){				// Si la distancia es la misma pero una tiene mas puntos afectados que la otra
				unsigned int leechToKillAmountOfPoints = leeches[leechToKillId].leeched_points.size();
				unsigned int currentLeechAmountOfPoints = leeches[i].leeched_points.size();
				if (currentLeechAmountOfPoints > leechToKillAmountOfPoints){
					leechToKillId = i;
				}
			}
		}
	}
	
	// Hago los cambios necesarios en la matriz A y B
	Leech& leechToKill = leeches[leechToKillId];
	
	for (int i = 0; i < (int) leechToKill.leeched_points.size(); i++){
		PointDiscr affectedPoint = leechToKill.leeched_points[i];
		int affP_i = (affectedPoint.i * discr_width) + affectedPoint.j;
		int affP_j = affP_i;
		
		matrix_A[affP_i][affP_j] = -4;
		matrix_A[affP_i][affP_j + discr_width]  = 1; // i-1 en la fila
		matrix_A[affP_i][affP_j - discr_width] = 1; // i+1 en la fila
		matrix_A[affP_i][affP_j-1] = 1;
		matrix_A[affP_i][affP_j+1] = 1;
		
		matrix_B[affP_i] = 0;
	}
	
	// Mato a la sanguijuela indicada en leechToKillId
	leeches.erase(leeches.begin() + leechToKillId);	
}

void Parabrisas::updateRowJ(double i_j_multiplier, int rowToUse, int rowToUpdate){
	for(int j = 0; j < discr_width * discr_height; j++){ 
		matrix_A[rowToUpdate][j] = matrix_A[rowToUpdate][j] - i_j_multiplier * matrix_A[rowToUse][j];
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

bool Parabrisas :: freeOfLeeches(){
	return leeches.empty();
}

int Parabrisas::write_output(char* output_file) {
	ofstream file;
	
	file.open(output_file);
	
	if (file.is_open()){				//chequea que este abierto x las dudas
	for(int i = 0; i < discr_height; i++){
			for(int j = 0; j < discr_width; j++){
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
	
	pb.calculate_temps();
	
	/*cout << "temperatura en punto critico: " << fixed << setprecision(5) << pb.temperatureOnCriticalPoint() << endl;
	while (pb.temperatureOnCriticalPoint() >= 235.0 && !pb.freeOfLeeches()){
	
		pb.kill_leech();
		pb.calculate_temps();
		cout << "temperatura en punto critico: " << fixed << setprecision(5) << pb.temperatureOnCriticalPoint() << endl;
	}*/
	
	if(pb.write_output(output_file)) exit(1);
	
	return 0;
}

#include "structs.h"
#include "rdtsc.h"

using namespace std;

double get_norm_2(double a_x, double a_y, double b_x, double b_y){
	return sqrt((a_x - b_x)*(a_x - b_x) + (a_y - b_y)*(a_y - b_y));
}

void imprimir(double** matrix, int i_range, int j_range){  
	for(int i=0; i<  i_range; i++){
		cout << endl;
		for(int j=0; j< j_range; j++){
			cout << matrix[i][j] << "\t";
		}
	}
	
	cout << endl;
}

class Parabrisas {
	public:
		Parabrisas();
		Parabrisas(int bandImpl);
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
		void createVectorB();
		
		void addLeechAndBorderInfo();
		void gaussianElimination();
		void updateRowJ(double i_j_multiplier, int rowToUse, int rowToUpdate);
		vector<double> resolveTriangularMatrix();
		void recreateWindShield(vector<double>& vectorX);
		void clearMatrixA();
		
		bool is_affected(int i, int j);
		bool is_border(int i, int j);

		
		double width, height, discr_interval, radius, temp;
		int discr_height, discr_width;
		int bandImplementation;
		vector<Leech> leeches;
		
		PB_Matrix* pb_matrix;
		double** matrix_A;
		Temp* vector_B;
};

void Parabrisas :: clearMatrixA(){
	int n = discr_height*discr_width;
	int j_range = n;
	if (bandImplementation)
		j_range = 2*discr_width + 1;
		
	for (int i = 0; i < n; i++){
		for (int j = 0; j < j_range; j++){
			matrix_A[i][j] = 0;
		}
	}
	
}

Parabrisas::Parabrisas() { 
	bandImplementation = false;
	matrix_A = NULL;
	vector_B = NULL;
	pb_matrix = NULL;	
}

Parabrisas :: Parabrisas(int bandImpl){
	bandImplementation = bandImpl;
	matrix_A = NULL;
	vector_B = NULL;
	pb_matrix = NULL;	
}

Parabrisas::~Parabrisas() { 
	if (pb_matrix != NULL)
		delete pb_matrix;
	if (matrix_A != NULL){
		for (int i = 0; i < discr_height * discr_width; i++)
			delete [] matrix_A[i];
		delete [] matrix_A;
	}
	if (vector_B != NULL)
		delete [] vector_B;
	
}

int Parabrisas::read_from_input(char* input_file) {
	ifstream file;
	file.open(input_file);
	
	if (file.is_open()){
		// Inicializo los valores a b h r t k.
		int amount_of_leeches;
		file >> width >> height >> discr_interval >> radius >> temp >> amount_of_leeches;
	
        cout << "Discretizacion: " << discr_interval;
    
		// En la creación de la estructura, guardo el n+1 m+1 reales resultantes de dividir el width y height por el intervalo.
		discr_width = (width / discr_interval) + 1;
		discr_height = (height / discr_interval) + 1;
		
		// Creo el vector de sanguijuelas. 
	
		for(int i = 0; i < amount_of_leeches; i++) {
			double x, y;
			file >> x >> y;
			Point pointLeech = Point(x,y);
			Leech leech = Leech(i, pointLeech, discr_width, discr_height, discr_interval, radius); 
			leech.leeched_points = leech.affected_points(pointLeech);
			
			// Al fijarme los puntos afectados: si no toqué un borde y si afecté algún punto, incluyo la sanguijuela en el vector

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
	/// Creación de la matriz que representa el parabrisas final
	
	Temp** pb_discr_matrix= new Temp*[discr_height];
	for (int j = 0; j < discr_height; j++){
		Temp* v = new Temp[discr_width];
		for (int i = 0; i < discr_width; i++)
			v[i] = UNDEFINED_TEMPERATURE;	// Temperatura arbitraria para las no-calculadas = UNDEFINED TEMPERATURE = -2048.0
			
		pb_discr_matrix[j] = v;
	}
	
	pb_matrix = new PB_Matrix(pb_discr_matrix, discr_width, discr_height, discr_interval);
}

void Parabrisas:: createMatrixA(){
	/// Creación de la matriz A 
	
	int complete_grid_size = discr_width * discr_height;
	int row_width = complete_grid_size;
	
	if (bandImplementation)
		row_width = 2*discr_width +1;			// Si usamos matriz banda, el ancho será 2n + 1 por fila para cubrir toda la banda. Se mantiene la cantidad de filas
		
		matrix_A = new double*[complete_grid_size];
		for (int j = 0; j < complete_grid_size; j++){
			double* v = new double[row_width];
		 
			for (int i = 0; i < row_width; i++)	// Lleno todo con 0 por default --> necesario inicializar así en C++
				v[i] = 0;
			
			matrix_A[j] = v;
		}
		
	addLeechAndBorderInfo();
}

void Parabrisas::addLeechAndBorderInfo(){
	/** Aquí agrego la información de la función partida para la matriz A:
	 *  - si es borde: 1
	 *  - si está afectado por sanguijuela: 1
	 *  - si no es ninguno de ambas, no conocemos la temperatura: se utiliza la simplificación de la derivada segunda
	 */
	
	// index_i e index_j nos servirán para determinar cuáles son los indices correspondientes 
	// en la matriz del parabrisas para cada i del bloque for
	int index_i = 0;
	int index_j = 0;
	
	for (int i = 0; i < discr_height * discr_width; i++){	// Voy fila por fila y me fijo la diagonal (el triangulo inferior queda 0)
		int j = i;											
		if (bandImplementation)
			j = discr_width;			// Para matriz banda, el elemento de la mitad (indice discr_width) representa al elemento de la diagonal (de la matriz cuadrada) en la fila i 

		if (is_border(index_i,index_j) || is_affected(index_i, index_j))
			matrix_A[i][j] = 1;
		else {
			matrix_A[i][j] = -4;				// t(i,j)
			matrix_A[i][j + discr_width]  = 1; 	// t(i-1,j)
			matrix_A[i][j - discr_width] = 1; 	// t(i+1,j)
			matrix_A[i][j-1] = 1;				// t(i,j+1)
			matrix_A[i][j+1] = 1;				// t(i,j-1)
		}
		
		index_j++;
		if(index_j == discr_width) {
			index_i++;
			index_j = 0;
		}
	}	
}

void Parabrisas::createVectorB(){
	/** Aquí agrego la información de la función partida para la matriz B:
	 *  - si es borde: -100
	 *  - si está afectado por sanguijuela: temp_sanguijuela
	 *  - si no es ninguno de ambas, no conocemos la temperatura: 0
	 */
	 
	if (vector_B == NULL) vector_B = new Temp[discr_height * discr_width];
	for (int i = 0; i < discr_height; i++){
		for (int j = 0; j < discr_width; j++){
			int index = (i * (discr_width)) + j;
		
			if (is_border(i,j))
				vector_B[index] = -100;
			else if (is_affected(i,j))
				vector_B[index] = temp;
			else
				vector_B[index] = 0;
		}
	}
}

void Parabrisas::create_all_matrices(){
	createPBMatrix();
	createMatrixA();
	createVectorB();
}

bool Parabrisas::is_affected(int ai, int aj){
	/** Miro los puntos afectados para cada sanguijuela y los comparo
	 * con ai y aj para ver si la posicion (ai,aj) está afectada
	 * por alguna sanguijuela */
	 
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
	int n = discr_height * discr_width;
	vector<double> vectorX = vector<double>(n); 

	
	if (!bandImplementation){
		// Para la implementación clásica, se utiliza un backward sustitution sin modificaciones
		
		for (int i = n - 1; i >= 0; i--){
			vectorX[i] = vector_B[i];
			for (int j = i+1; j < n; j++){
				vectorX[i] -= matrix_A[i][j] * vectorX[j];
			}
		
			vectorX[i] /= matrix_A[i][i];	
		}
	} else {
		
		// Para la implementación con matriz banda, defino el rango para las j. Las i quedan igual
		// La idea es utilizar solo los elementos de la banda, ya que esos son los relevantes
		int j_range = 0;
		for (int i = n - 1; i >= 0; i--){
			vectorX[i] = vector_B[i];
			
			// Si me encuentro en el principio de la matriz, hay columnas de la banda que no existen
			// ya que la banda en ese punto de la matriz no es completa (solo la mitad hacia la derecha existe allí)
			int j_start = 0;
			if (i < discr_width)
				j_start = discr_width-i;
				
			for (int j = j_start; j < j_range; j++)
				// j+discr_width+1 --> representa al i+1 de la implementación normal (desde la mitad+1 en adelante para la nueva matriz A)
				// i+j+1 --> representa a la j en el otro caso. El reemplazo es directo (j = i+1 en el otro caso y voy sumando de a 1)
				vectorX[i] -= matrix_A[i][j+discr_width+1] * vectorX[i+j+1];
			
			if (j_range < discr_width) j_range++;

			// matrix_A[i][discr_width] representa las diagonales de la matriz A completa
			vectorX[i] /= matrix_A[i][discr_width];	
		}
	}
	
	
	return vectorX;
}

void Parabrisas :: recreateWindShield(vector<double>& vectorX){
	/** Aquí se rellena la matriz del parabrisas con los valores
	 * nuevos de las temperaturas calculados justo antes.*/
	 
	for (int i = 0; i < discr_height; i++)
		for (int j = 0; j < discr_width; j++)
			pb_matrix->matrix[i][j] = vectorX[i*discr_width + j];
}

void Parabrisas::calculate_temps() {
	/** Aquí se calculan las temperaturas, primero aplicando eliminación gaussiana
     * y luego resolviendo la matriz triangular superior.
	 * Después se rellena la matriz que representa el parabrisas */
	 
	gaussianElimination();
	vector<double> temperatureVector = resolveTriangularMatrix();
	recreateWindShield(temperatureVector);
}

void Parabrisas::kill_leech() {
	// Busco el punto afectado mas cercano al centro y quito su sanguijuela asociada
	double middle_x = height / 2;
	double middle_y = width / 2;
	
	// Elijo una distancia mayor a todas las distancias desde el punto crítico hacia un punto afectado.
	double closestDistance = width * height;	
	int leechToKillId = 0;
	
	for (int i = 0 ; i < (int) leeches.size(); i++){
		for (int j = 0; j < (int) leeches[i].leeched_points.size(); j++){
			
			// Tomo un punto afectado
			
			PointDiscr affectedPoint = leeches[i].leeched_points[j];
			double real_affP_x = affectedPoint.i * discr_interval;
			double real_affP_y = affectedPoint.j * discr_interval;
			
			// Calculo la distancia de este punto al punto crítico
			double distanceToMiddle = get_norm_2(real_affP_x, real_affP_y, middle_x, middle_y);
			
			// Si encontré una distancia más chica al medio, indico a la sanguijuela i para ser asesinada
			if ( distanceToMiddle < closestDistance ){
				closestDistance = distanceToMiddle;
				leechToKillId = i;
			
			/* Criterio de desempate por distancias:
			 * Si la distancia es la misma pero la sanguijuela actual tiene más puntos afectados que la anterior indicada a asesinar
			 * entonces elijo la sanguijuela con más puntos afectados entre las dos*/
			 
			} else if ( abs(distanceToMiddle - closestDistance) < EPS){				
				unsigned int leechToKillAmountOfPoints = leeches[leechToKillId].leeched_points.size();
				unsigned int currentLeechAmountOfPoints = leeches[i].leeched_points.size();
				if (currentLeechAmountOfPoints > leechToKillAmountOfPoints)
					leechToKillId = i;
			}
		}
	}
	
	
	// Mato a la sanguijuela indicada en leechToKillId
	leeches.erase(leeches.begin() + leechToKillId);	
	
	// Hago los cambios necesarios en la matriz A y B
	clearMatrixA();
	createVectorB();
	addLeechAndBorderInfo();
}

void Parabrisas::updateRowJ(double i_j_multiplier, int rowToUse, int rowToUpdate){
	if (!bandImplementation){
		// Fj = Fj - Fi sin modificaciones 
		
		for(int j = 0; j < discr_width * discr_height; j++)
			matrix_A[rowToUpdate][j] = matrix_A[rowToUpdate][j] - i_j_multiplier * matrix_A[rowToUse][j];
		
	} else {
		// Fj = Fj - Fi CON modificaciones
		
		int n = discr_width;
		
		// El offset está y es necesario porque en la matriz A en esta implementación, 
		// si uno toma dos filas consecutivas, estas van a estar desfasadas en una columna
		// en la matriz grande original A
		int offset = rowToUpdate - rowToUse;
	
		// Defino rangos: j_range es "2n+1 - offset" ya que quiero actualizar solo los elementos
		// que involucran la banda: si el offset entre la fila a updatear y la que se usa es muy grande,
		// entonces los elementos que voy a poder actualizar en la fila de update serán pocos.
		int j_range = 2*n + 1 - offset;
		
		// Utilizo toda la banda, salvo que esté al principio de la matriz original, en donde falta parte de
		// la banda (por ejemplo, solo tengo la mitad de la banda hacia la derecha en la primera fila)
		int j_start = 0;
		if (rowToUpdate < n)
			j_start = n-rowToUpdate;
		
		for (int j = j_start; j < j_range; j++)
			matrix_A[rowToUpdate][j] = matrix_A[rowToUpdate][j] - i_j_multiplier * matrix_A[rowToUse][j+offset];

	}
	
	// Hacer el update de la matriz B es independiente de la implementación
	vector_B[rowToUpdate] = vector_B[rowToUpdate] - i_j_multiplier * vector_B[rowToUse];
} 

void Parabrisas::gaussianElimination() {
	if (!bandImplementation){
		
		// Para la implementación clásica, se utiliza una EG sin modificaciones
		
		for (int i = 0; i < (discr_height * discr_width) - 1; i++){
			for (int j = i+1; j < discr_height * discr_width; j++){
				// Checkeamos si hay un 0 en la diagonal (no debería suceder)
				if(abs(matrix_A[i][i])  < EPS){
					cout << "0 en la diagonal en la posición [" << i << "," << i << "]" << endl;
					exit(1); 
				}
				
				double i_j_multiplier = matrix_A[j][i] / matrix_A[i][i];
				updateRowJ(i_j_multiplier, i, j);
			}
		}
	} else {	
		
		// Para la implementación con matriz banda, modificamos la EG
		
		int n = discr_width;
		int i_range = (discr_height * discr_width) - 1;
		
		for (int i = 0; i < i_range; i++){
			
			// Se define el rango para j: se recorre hacia abajo discr_width (n) posiciones en el peor caso
			// ---> luego j = i+1 hasta (i+1) + n. 
			int j_range = (i+1) + n;
			
			// Al llegar al final de la banda hay menos de discr_width filas por recorrer
			// ---> luego va a hasta el final (discr_width * discr_height)
			if ( i >= (i_range + 1) - n )
				j_range = i_range + 1;
			
			for (int j = i+1; j < j_range; j++){
				if(abs(matrix_A[i][n])  < EPS){
					cout << "0 en la diagonal en la posición [" << i << "," << i << "]" << endl;
					exit(1); 
				}
				
				// Utilizo de indice de las columnas n-j+i para tener el elemento correspondiente bajo la diagonal
				double elemBajoDiagonal = matrix_A[j][n-j+i];
				
				// La columna para los elementos de la diagonal en la matriz_A en esta implementacion
				// está siempre en la columna n (ya que hay 2n+1 elementos)
				double elemDiagonal = matrix_A[i][n];
				
				double i_j_multiplier = elemBajoDiagonal / elemDiagonal;
				updateRowJ(i_j_multiplier, i, j);
			}
		}
	}
}

double Parabrisas :: temperatureOnCriticalPoint(){
	/** Tomo las coordenadas reales del punto crítico y busco la
	 * temperatura allí. Si el punto existe en la discretización, lo
	 * devuelvo, si no, devuelvo el promedio de los 4 puntos que lo
	 * rodean */
	 
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
	
	if (file.is_open()){			
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
		cout << "Unable to open output file" << endl;
		return 1;
	}
}

int main(int argc, char *argv[]) {	
	char* input_file = argv[1];
	char* output_file = argv[2];
	int bandImplementation = atoi(argv[3]);

	Parabrisas pb(bandImplementation);
	if(pb.read_from_input(input_file)) exit(1);
	
	pb.calculate_temps();
	
    cout << " - Temp(PuntoCritico): " << fixed << setprecision(5) << pb.temperatureOnCriticalPoint() << endl;
    
    if(argc >= 5)
    {
        int killLeechState = atoi(argv[4]);
        bool shouldKillLeeches = killLeechState == 1;
        
        if(shouldKillLeeches)
        {
            while (pb.temperatureOnCriticalPoint() >= 235.0 && !pb.freeOfLeeches()){
            
                pb.kill_leech();
                pb.calculate_temps();
            }
        }
    }    
	
	if(pb.write_output(output_file)) exit(1);
	
	return 0;
}

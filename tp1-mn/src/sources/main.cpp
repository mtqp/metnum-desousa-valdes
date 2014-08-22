#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>

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
		return p1.x == p2.x and p1.y == p2.y;
	}
};

class Parabrisas {
	public:
		Parabrisas();
		~Parabrisas();
		int read_from_input(char* input_file);
		double get_width() const;
		double get_height() const;
		double get_radius() const;
		double get_temp() const;
		double get_discretization() const;
		void calculate_temps();
		void kill_leech();
		int write_output(char* output_file);
		
		
	private:
		struct PB_Matrix {
			Temp** matrix;
			const Parabrisas* pb;
			int discr_width, discr_height;

			PB_Matrix() : matrix(NULL), pb(NULL), discr_width(0), discr_height(0) {};
			PB_Matrix(Temp** m, const Parabrisas* parab, int d_w, int d_h) : matrix(m), pb(parab), discr_width(d_w), discr_height(d_h) {};
			
			void set_borders(){
				// Pongo los bordes en -100 (a revisar)
				for( int j = 0; j < discr_width; j++) {
					matrix[0][j] = -100.0;
					matrix[discr_height][j] = -100.0;
				}

				for (int i = 0; i < discr_height; i++){
					matrix[i][0] = -100.0;
					matrix[i][discr_width] = -100.0;
				}
			};

			Temp get(const Point p){
				double intpart, d_x = p.x/pb->get_discretization(), d_y = p.y/pb->get_discretization();
				
				if (modf(d_x, &intpart) <= 0.0 + EPS and modf(d_y, &intpart) <= 0.0 + EPS)									//Si da una cuenta entera. VER ERROR RELATIVO EPS
					return matrix[(int)d_x][(int)d_y];
				else
					return ERROR_TEMPERATURE;
			};
		};

		struct Leech {
			int id;
			Point position;
			vector<Point> leeched_points;
			Parabrisas* pb; 
	
			vector<Point> affected_points(const Point p){
				vector<Point> res;
				double x_low, x_high, y_low, y_high, radius = pb->get_radius();
				
				// Defino los intervalos reales
				x_low = p.x - radius;
				x_high = p.x + radius;
				y_low = p.y - radius;
				y_high = p.y + radius;
				
				int disc_x_low, disc_x_high, disc_y_low, disc_y_high;
				double discretization = pb->get_discretization();

				// Defino los intervalos CERRADOS discretos
				disc_x_low = (int)ceil(x_low/discretization);
				disc_x_high = (int)floor(x_high/discretization);
				disc_y_low = (int)ceil(y_low/discretization);
				disc_y_high = (int)floor(y_high/discretization);
				
				for (int i = disc_x_low; i < disc_x_high; i++){
					double real_x = i*discretization;
					for (int j = disc_y_low; j < disc_y_high; j++){
						double real_y = j*discretization;
						if (sqrt((p.x - real_x)*(p.x - real_x) + (p.y - real_y)*(p.y - real_y)) <= radius)
							res.push_back(Point(i,j));
					}
				}
				
				return res;
			};
	
			Leech();
			Leech(const int id_leech, const Point p, Parabrisas* pba) : id(id_leech), position(p), leeched_points(affected_points(p)), pb(pba) {};
		};

		void gaussian_elimination();
		bool is_affected(int i, int j);
		bool is_border(int i, int j);
		
		double width, height, discr_interval, radius, temp;
		int discr_height, discr_width;
		vector<Leech> leeches;
		
		PB_Matrix* pb_matrix;
		int** matrix_A;
		int* matrix_B;
};

Parabrisas::Parabrisas() { }

Parabrisas::~Parabrisas() { 
	if (pb_matrix != NULL)
		delete pb_matrix;
	if (matrix_A != NULL)
		delete matrix_A;
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
		
			leeches.push_back(Leech(i, Point(x,y), this));
		}

		file.close();	

		
		// En la creación de la estructura, guardo el n+1 m+1 reales resultantes de dividir el width y height por el intervalo.
		discr_width = (width / discr_interval) + 1;	//Techo, piso, round? Se asume correcto entero. N+1 y M+1!! CONSULTAR
		discr_height = (height / discr_interval) + 1;

		// Temperatura arbitraria para las no-calculadas = UNDEFINED TEMPERATURE
		//vector< vector<Temp> > pb_discr_matrix(discr_width, vector<Temp>(discr_height, UNDEFINED_TEMPERATURE));
		
		Temp** pb_discr_matrix= new Temp*[discr_width];
		for (int j = 0; j < discr_width; j++){
			Temp* v = new Temp[discr_height];
			for (int i = 0; i < discr_height; i++)
				v[i] = UNDEFINED_TEMPERATURE;
				
			pb_discr_matrix[j] = v;
		}

		// Creo el parabrisas discreto
		pb_matrix = new PB_Matrix(pb_discr_matrix, this, discr_width, discr_height);
		pb_matrix->set_borders();
		

		int complete_grid_size = discr_width*discr_height;

		// Creo la matriz genérica A
		matrix_A = new int*[complete_grid_size];
		for (int j = 0; j < complete_grid_size; j++){
			int* v = new int[complete_grid_size];
			
			for (int i = 0; i < complete_grid_size; i++)	// Lleno todo con 0 por default
				v[i] = 0;
				
			matrix_A[j] = v;
		}
		
		
		// Relleno con toda la info (bordes + sanguijuela)
		for (int j = 0; j < complete_grid_size; j++){
			for (int i = 0; i < complete_grid_size; i++){
				if (is_border(i,j))
					matrix_A[i][j] = 1;
				else if (is_affected(i,j))
					matrix_A[i][j] = temp;
				else {
					matrix_A[i][j] = -4;
					matrix_A[i-1][j] = 1;
					matrix_A[i+1][j] = 1;
					matrix_A[i][j-1] = 1;
					matrix_A[i][j+1] = 1;
				}
				
			}
		}
		
		matrix_B = new int[complete_grid_size];
		for (int i = 0; i < discr_height; i++){
			for (int j = 0; i < discr_width; j++){
				int index = (i*discr_width) + j;
				
				if (is_affected(i,j))
					matrix_B[index] = temp;
				else if (is_border(i,j))
					matrix_B[index] = -100;
				else
					matrix_B[index] = 0;
			}
		}
		
	} else {
		cout << "Unable to open input file" << endl;		
		return 1;
	}

	return 0;
}

bool Parabrisas::is_affected(int ai, int aj){
	for (unsigned int i = 0; i < leeches.size(); i++){
		for (unsigned int j = 0; j < leeches[i].leeched_points.size(); j++){
			if (leeches[i].leeched_points[j] == Point(ai,aj)) return true;
		}
	}

	return false;
}

bool Parabrisas::is_border(int i, int j){
	return i == 0 or j == 0 or i == discr_height or j == discr_width;
}

double Parabrisas::get_width() const{ return width; }
double Parabrisas::get_height() const{ return height; }
double Parabrisas::get_discretization() const{ return discr_interval; }
double Parabrisas::get_radius() const{ return radius; }
double Parabrisas::get_temp() const{ return temp; }

void Parabrisas::calculate_temps() {

}

void Parabrisas::kill_leech() {

}

void Parabrisas::gaussian_elimination() {

}

int Parabrisas::write_output(char* output_file) {
	ofstream file;
	
	file.open(output_file);
	
	if (file.is_open()){				//chequea que este abierto x las dudas
		for(int i = 0; i < pb_matrix->discr_width; i++){
			for(int j = 0; j < pb_matrix->discr_height; j++){
				file << i << " " << j << " " << pb_matrix->matrix[i][j] << endl; 
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
	if(pb.write_output(output_file)) exit(1);
	
	return 0;
}
 

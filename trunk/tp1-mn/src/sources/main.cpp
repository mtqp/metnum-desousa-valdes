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
			vector< vector<Temp> > matrix;
			int real_width, real_height;
			const Parabrisas* pb;

			PB_Matrix() : matrix(vector<vector<Temp> >(0, vector<Temp>(0, 0))), real_width(0), real_height(0), pb(0) {};
			PB_Matrix(const Parabrisas* parab){
				pb = parab;				

				// En la creación de la estructura, guardo el n+1 m+1 reales resultantes de dividir el width y height por el intervalo.
				real_width = (pb->get_width() / pb->get_discretization()) + 1;	//Techo, piso, round? Se asume correcto entero. N+1 y M+1!! CONSULTAR
				real_height = (pb->get_height() / pb->get_discretization()) + 1;

				matrix = vector<vector<Temp> >(real_width, vector<Temp>(real_height, UNDEFINED_TEMPERATURE) );	//temperatura arbitraria para las no-calculadas.

				// Pongo los bordes en -100 (a revisar)
				for( int i = 0; i < real_width; i++) {
					matrix[0][i] = -100.0;
					matrix[real_height][i] = -100.0;
				}

				for (int j = 0; j < real_height; j++){
					matrix[j][0] = -100.0;
					matrix[j][real_width] = -100.0;
				}
			};

			Temp get(const Point p){
				double intpart, d_x = p.x/pb->get_discretization(), d_y = p.y/pb->get_discretization();
				
				if (modf(d_x, &intpart) <= 0.0 + EPS and modf(d_y, &intpart) <= 0.0 + EPS)									//Si da una cuenta entera. VER ERROR RELATIVO EPS
					return matrix[d_x][d_y];
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
		
		double width, height, discr_interval, radius, temp;
		vector<Leech> leeches;
		PB_Matrix pb_matrix;
};

Parabrisas::Parabrisas() { }

Parabrisas::~Parabrisas() { }

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

		// Creo el parabrisas discreto
		pb_matrix = PB_Matrix(this);
	} else {
		cout << "Unable to open input file" << endl;		
		return 1;
	}

	return 0;
}

double Parabrisas::get_width() const{
	return width;
}

double Parabrisas::get_height() const{
	return height;
}

double Parabrisas::get_discretization() const{
	return discr_interval;
}

double Parabrisas::get_radius() const{
	return radius;
}

double Parabrisas::get_temp() const{
	return temp;
}

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
		for(int i = 0; i < pb_matrix.real_width; i++){
			for(int j = 0; j < pb_matrix.real_height; j++){
				file << i << " " << j << " " << pb_matrix.matrix[i][j] << endl; 
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
 

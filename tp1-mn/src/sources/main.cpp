#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>

#define DEBUG

#define UNDEFINED_TEMPERATURE 2048.0

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
		int read_from_input();
		void calculate_temps();
		void kill_leech();
		void write_output(char* file);
	private:
		struct Leech {
			int id;
			Point position;
			vector<Point> leeched_points;
	
			vector<Point> affected_points(const Point p){
					/// QUERIA HACER ESTO
					vector<Point> res;
					return res;
			};
	
			Leech();
			Leech(const int id_leech, const Point p) : id(id_leech), position(p), leeched_points(affected_points(p)) {};
		};

		struct PB_Matrix {
			vector< vector<Temp> > matrix;
			int real_width, real_height;
			const Parabrisas* pb;

			PB_Matrix() : matrix(vector<vector<Temp> >(0, vector<Temp>(0, 0))), real_width(0), real_height(0), pb(0) {};
			PB_Matrix(const Parabrisas* parab){
				pb = parab;				

				// En la creación de la estructura, guardo el n+1 m+1 reales resultantes de dividir el width y height por el intervalo.
				real_width = pb->width / pb->discr_interval;	//Techo, piso, round?
				real_height = pb->height / pb->discr_interval;

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
				return matrix[p.x/pb->discr_interval][p.y/pb->discr_interval];
			};
		};

		void gaussian_elimination();
		
		double width, height, discr_interval, radius, temp;
		vector<Leech> leeches;
		PB_Matrix pb_matrix;
};

Parabrisas::Parabrisas() { }

Parabrisas::~Parabrisas() { }

int Parabrisas::read_from_input() {
	// Inicializo los valores a b h r t k.
	int amount_of_leeches;
	cin >> width >> height >> discr_interval >> radius >> temp >> amount_of_leeches;
	
	// Creo el vector de sanguijuelas. 
	// Para cada sanguijuela creada veo los puntos afectados y los guardo en su propia estructura.
	
	for(int i = 0; i < amount_of_leeches; i++) {
		double x, y;
		cin >> x >> y;
		
		leeches.push_back(Leech(i, Point(x,y)));
	}
	

	// Creo el parabrisas discreto
	pb_matrix = PB_Matrix(this);

	return 0;
}

void Parabrisas::calculate_temps() {

}

void Parabrisas::kill_leech() {

}

void Parabrisas::gaussian_elimination() {

}

void Parabrisas::write_output(char* output_file) {
	ofstream file;
	
	file.open(output_file);
	
	if (file.is_open()){				//chequea que este abierto x las dudas
		for(int i = 0; i < pb_matrix.real_width; i++){
			for(int j = 0; j < pb_matrix.real_height; j++){
				file << i << " " << j << " " << pb_matrix.matrix[i][j] << endl; 
			}
		}
		file.close();
	 }
	 else cout << "Unable to open file";		// si no pudo da esto... SE VE SI SE PONE REALMENTE
}

int main(int argc, char *argv[]) {
	Parabrisas pb;
	
	pb.read_from_input();
	pb.calculate_temps();
	char* output_file = argv[2];
	pb.write_output(output_file);
	
	return 0;
}
 

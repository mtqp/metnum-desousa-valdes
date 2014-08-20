#include <iostream>
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
		void show();
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
			
			PB_Matrix(){
				// En la creación de la estructura, guardo el n+1 m+1 reales resultantes de dividir el width y height por el intervalo.
				real_width = Parabrisas::width / Parabrisas::discr_interval;	//Techo, piso, round?
				real_height = Parabrisas::height / Parabrisas::discr_interval;
				
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
				return matrix[p.x/Parabrisas::discr_interval][p.y/Parabrisas::discr_interval];
			};
		};
		
		void gaussian_elimination();
		
		static double width, height, discr_interval, radius, temp;
		//int amount_of_leeches;	// No lo guardo porque se puede pedir el length de leeches, encima esto va a ir variando al matar sanguijuelas.
		
		vector<Leech> leeches;
};

// Static variables para compilar 

double Parabrisas::width = 0.0;
double Parabrisas::height = 0.0;
double Parabrisas::discr_interval = 0.0;
double Parabrisas::radius = 0.0;
double Parabrisas::temp = 0.0;

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
	
	return 0;
}

void Parabrisas::calculate_temps() {

}

void Parabrisas::kill_leech() {

}

void Parabrisas::gaussian_elimination() {

}

void Parabrisas::show() {
	/*if (best_path.size() == 0){
		cout << "no" << endl;
		return;
	}
	
	// Pesos desde 0 se le habian sumado 1.
	cout << best_weight_1 << " " << best_weight_2 << " " << best_path.size();
	for(int i = 0; i < (int) best_path.size(); i++) {
		// Nosotros numeramos desde 0 y ellos desde 1.
		cout << " " << best_path[i] + 1;
	}
	cout << endl;*/
}

int main(int argc, char *argv[]) {
	Parabrisas pb;
	
	pb.read_from_input();
	pb.calculate_temps();
	pb.show();
	
	return 0;
}
 

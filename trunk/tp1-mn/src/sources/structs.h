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

double get_norm_2(double a_x, double a_y, double b_x, double b_y);

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
	PB_Matrix(Temp** m, int d_w, int d_h, double discr) : matrix(m), discr_width(d_w), discr_height(d_h), discretization(discr) {
		set_borders();
	};
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

struct Leech {
	int id;
	Point position;
	int discr_width, discr_height;
	double discretization, radius;
	vector<PointDiscr> leeched_points;

	bool is_border(int i, int j){
		return i == 0 || j == 0 || i == (discr_height-1) || j == (discr_width-1);
	};

	vector<PointDiscr> affected_points(const Point p){
		vector<PointDiscr> affected_points;
		double x_low, x_high, y_low, y_high;
		
		// Defino los intervalos reales
		x_low = p.x - radius;
		x_high = p.x + radius;
		y_low = p.y - radius;
		y_high = p.y + radius;
		int disc_x_low, disc_x_high, disc_y_low, disc_y_high;
		
		/*cout << "x_low: " << x_low << endl;
		cout << "x_high: " << x_high << endl;
		cout << "y_low: " << y_low << endl;
		cout << "y_high: " << y_high << endl;*/
		
		// Defino los intervalos CERRADOS discretos
		disc_x_low = (int)ceil(x_low / discretization);
		disc_x_high = (int)floor( x_high / discretization);
		disc_y_low = (int)ceil( y_low / discretization);
		disc_y_high = (int)floor(y_high / discretization);
		
		/*cout << "disc_x_low: " << disc_x_low << endl;
		cout << "disc_x_high: " << disc_x_high << endl;
		cout << "disc_y_low: " << disc_y_low << endl;
		cout << "disc_y_high: " << disc_y_high << endl;*/
		
		for (int i = disc_x_low; i <= disc_x_high; i++){
			double real_x = i*discretization;
			
			for (int j = disc_y_low; j <= disc_y_high; j++){
				if (is_border(i,j)){
					vector<PointDiscr> vacio;
					return vacio;
				}
				double real_y = j*discretization;
			//	cout << "radius: " << radius << "; norma2: " << get_norm_2(p.x, p.y, real_x, real_y) << endl;
				if (get_norm_2(p.x, p.y, real_x, real_y) <= radius) //corregir borde mata sanguijuela
					affected_points.push_back(PointDiscr(i,j));
			}
			
		}
		
		return affected_points;
	};

	Leech();
	Leech(const int id_leech, const Point p, const int d_w, const int d_h, double d, double r)
		: id(id_leech), position(p), discr_width(d_w), discr_height(d_h), discretization(d), radius(r) {};
};

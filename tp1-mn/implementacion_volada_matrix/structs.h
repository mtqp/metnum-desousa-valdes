#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <utility>
#include "includes.h"
#include "matrix.h"
#include "vector.h"

struct warpData{
	uInt turn;
	Vector<double> position;
	
	/* Si es el primer turno quedan con ceros */
	Matrix<double> A;
	Vector<double> d;
	pair<Vector<double>*,double>* position_enemy;
	warpData(uInt dim) : position(dim), A(dim), d(dim){}
};

struct attackData{
	Matrix<double> A;
	Vector<double> d;
	attackData(uInt dim) : A(dim), d(dim){}
};

#endif

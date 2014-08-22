#include "warp_cannon.h"

WarpCannon :: WarpCannon(warpData wd, uInt dim) : _position(dim), _A(dim), _d(dim){
	if((wd.position).dimension() != dim) throw MatrixException((char*)"No se puede construir WarpCannon por inconsistencia en las dimensiones de los datos y lo que se pretende crear", Default);
	_dim = dim;
	_turn = wd.turn;
	_position = wd.position;

	/* Si es el primer turno quedan con ceros */
	_A = wd.A;
	_d = wd.d;
	_position_enemy = wd.position_enemy;
}

WarpCannon :: ~WarpCannon(){}

attackData WarpCannon :: attack() const{
	Matrix<double> attack_A(_dim);
	Vector<double> attack_point(_dim);
	if(_turn==1){
		/* Empiezo con una matriz mal condicionada */
		Matrix<double> Bad(_dim,BadK);
		attack_A = Bad;
		attack_point = (attack_A*_position.traspuesta()).traspuesta();
	}
	else
	{
		attack_point = getAimPosition();
		attack_A = getMatrixAttack(attack_point);
	}
	
	attackData ad(_dim);
	ad.A=attack_A;
	ad.d=attack_point;
	
	return ad;
}

Vector<double> WarpCannon :: getAimPosition() const{
	Vector<double> average(_dim);
	uInt data_amount = _turn-1;
	double average_coordinate;
	
	for(uInt i=0; i<=data_amount; i++)
		for(uInt j=1; j<=_dim; j++){
			if(i!=data_amount)
				average_coordinate = (*_position_enemy[i].first).getValue(j) + average.getValue(j);
			else
				average_coordinate = average.getValue(j)/data_amount;
			average.setValue(average_coordinate,j);
		}

	return average;
}

Matrix<double> WarpCannon :: getMatrixAttack(const Vector<double> attack_point) const{
	/* Quiero A tal que A*_position=attack_point
	 * Tengo _dim ecuaciones y _dim*_dim incognitas por lo que dim*dim-dim coeficientes de A estan libres*/

	/* Empiezo con una matriz mal condicionada */
	Matrix<double> attack_A(_dim,BadK);
	
	/* Seteo una columna (elegida tal que en esa posicion el vector _position sea distinto de cero) con los valores tal que al multiplicarla por _position de attack_point */
	uInt col = 1;
	bool zero = abs(_position.getValue(col))<EPSILON_ERROR;
	for(uInt i=2; i<=_dim && zero; i++){
		col = i;
		zero = abs(_position.getValue(col))<EPSILON_ERROR;
	}
	if(!zero){
		double acum;
		double value;
		for(uInt i=1; i<=_dim; i++){
			acum = 0;
			for(uInt j=1; j<=_dim; j++){
				if(j!=col){
					acum += attack_A.getValue(i,j)*_position.getValue(j);
				}
			}
			value = attack_point.getValue(i);
			value -= acum;
			value /= _position.getValue(col);
			attack_A.setValue(value,i,col);
		}
	}

	return attack_A;
}
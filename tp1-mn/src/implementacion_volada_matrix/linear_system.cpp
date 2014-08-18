#include "linear_system.h"

linearSystem::linearSystem(const Matrix<double> A, const Vector<double> d): _A(A), _d(d){
	_dim = _A.getFiDimension();
	if(_dim!=d.dimension()) throw MatrixException((char*)"El sistema no se puede resolver, las dimensiones no coinciden",Default);
}
linearSystem::~linearSystem(){}

Vector<double> linearSystem::usingInverse() const{
	Matrix<double> A_inverse(_A.inverse());
	Vector<double> x(_dim);
	x = (A_inverse*_d.traspuesta()).traspuesta();
	return x;
}

Vector<double> linearSystem::usingLU() const{
	PLU<double> plu(_dim);
	plu = _A.LU();
	
	/* Creo la matriz de permutacion */
	Matrix<double> perm(_dim);
	for(uInt i=1; i<=_dim; i++){
		for(uInt j=1; j<=_dim; j++){
			if(j==plu.P.getValue(i))
				perm.setValue(1,i,j);
			else 
				perm.setValue(0,i,j);
		}
	}
	
	/* Permuto el d */
	Vector<double> d_perm(_dim);
	d_perm = (perm*_d.traspuesta()).traspuesta();
	
	linearSystem ls(plu.LU,d_perm);							//LUx=d
	Vector<double> z(ls.forwardSub());						//Ux=z
	ls._d=z;
	Vector<double> x(ls.backSub());							//Lz=d
	return x;
}

Vector<double> linearSystem::backSub() const{
	Vector<double> res(_d.dimension());
	double acum;
	for(uInt i=_dim;i>=1;i--){
		acum = 0;
		for(uInt j=_dim;j>i;j--){
			acum+=(_A.getValue(i,j)*res.getValue(j));
		}
		res.setValue((_d.getValue(i)-acum)/_A.getValue(i,i),i);
	}
	return res;
	cout << res;
}

Vector<double> linearSystem::forwardSub() const{
	Vector<double> res(_d.dimension());
	double acum;
	for(uInt i=1;i<=_dim;i++){
		acum = 0;
		for(uInt j=1;j<i;j++){
			acum+=(_A.getValue(i,j)*res.getValue(j));
		}
		res.setValue((_d.getValue(i)-acum),i);							//la diagonal de L esta toda en uno por eso no divido
	}
	return res;
}

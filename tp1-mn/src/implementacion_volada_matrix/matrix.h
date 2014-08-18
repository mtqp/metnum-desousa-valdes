#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "includes.h"
#include "vector.h"
#include "matrix_base.h"
#include "matrix_exceptions.h"

enum MatrixType{
	ID,
	TSup,
	TInf,
	Nula,
	Diag,
	Rand,
	BadK,
	Hilbert
};

template <class T>
class Matrix;

template <class T>
struct PLU{
	Matrix<T> LU;
	Vector<uInt> P;
	PLU<T>(uInt dim) : LU(dim), P(dim){}
};

template <class T>
class Matrix : public MatrixBase<T>{
	public:
		Matrix(uInt dim);
		Matrix(const Matrix<T>& mCopy);
		Matrix(T** data, uInt dim);
		Matrix(const T* data, uInt dim);
		Matrix(uInt dim, MatrixType type);
		~Matrix();

		Matrix<T> gaussianElim() const;
		PLU<T> LU() const;
		Matrix<T> inverse() const;
		T K() const;

		Matrix<T>& operator= (const MatrixBase<T> &mb);
	
	private:
		bool isTriang(bool superior) const;
		bool isId() const;

		T det() const;													//determinante en valor absouto, porque usa permutaciones que pueden cambiar el signo
		void Gauss_LU(bool L);
		T coefficient(uInt i, uInt j);
		void putZero(uInt i, uInt j, T coefficient, bool from_j);		//pone el cero en esa posicion
		uInt maxUnderDiag(uInt j) const;								//estrategia de pivoteo parcial
		T  	 normF() const;
		
		void createId(uInt dim);
		bool createBadK(uInt dim);
		void createHilbertMatrix(uInt dim);
		
		uInt _dim;
		Vector<uInt> _P;
};

template <typename T>
Matrix<T> :: Matrix(uInt dim) : MatrixBase<T>(dim,dim), _P(dim){
	_dim = dim;
	for(uInt k=1;k<=_dim;k++) _P.setValue(k,k);
}

template <typename T>
Matrix<T> :: Matrix(const Matrix<T>& mCopy) : MatrixBase<T>(mCopy), _P(mCopy.getFiDimension()){
	_dim = mCopy.getFiDimension();
	for(uInt k=1;k<=_dim;k++) _P.setValue(k,k);
}

template <typename T>
Matrix<T> :: Matrix(T** data, uInt dim) : MatrixBase<T>(data,dim,dim), _P(dim){
	_dim = dim;
	for(uInt k=1;k<=_dim;k++) _P.setValue(k,k);
}

template <typename T>
Matrix<T> :: Matrix(const T* data, uInt dim) : MatrixBase<T>(dim,dim), _P(dim){
	_dim = dim;
	for(uInt k=1;k<=_dim;k++) _P.setValue(k,k);
	uInt h = 0;

	for(uInt i=1;i<=dim;i++)
		for(uInt j=1;i<=dim;j++){
			this->setValue(data[h],i,j);
			h++;
		}
}

template <typename T>
Matrix<T> :: Matrix(uInt dim, MatrixType type) : MatrixBase<T>(dim, dim), _P(dim){
	_dim = dim;
	for(uInt k=1;k<=_dim;k++) _P.setValue(k,k);
	uInt attempts;
	bool invertible;
	switch(type){
		case(ID):
			createId(dim);
			break;
		case(BadK):
			attempts = 0;
			invertible = this->createBadK(dim);
			while(!invertible && attempts<tolerance){
				invertible = this->createBadK(dim);
				attempts++;
			}
			if(!invertible) this->createHilbertMatrix(dim);
			break;
		case(Hilbert):
			createHilbertMatrix(dim);
			break;
		default:
			throw MatrixException((char*)"Generador no implementado.", Default);
	}
}

template <typename T>
Matrix<T> :: ~Matrix(){}

template <typename T>
Matrix<T> Matrix<T> :: gaussianElim() const {
	Matrix<T> copy(*this);
	copy.Gauss_LU(false);
	return copy;
}

/*	Devuelve una matriz la cual se interpreta: L abajo de la diagonal, U arriba de la diagonal y la diagonal */
template <typename T>
PLU<T> Matrix<T> :: LU() const{
	PLU<T> plu(_dim);
	Matrix<T> copy(*this);
	copy.Gauss_LU(true);
	plu.LU = copy;
	plu.P = copy._P;
	return plu;
}

template <typename T>
Matrix<T> Matrix<T> :: inverse() const{
	if(!this->isInversible()) throw MatrixException((char*)"No existe la inversa", NotInversible);

	Matrix<T> copy(*this);
	Matrix<T> I(_dim,ID);

	uInt maxCol;
	T coefficient;
	
	/* Triangulo inferiormente */
	for(uInt j=1; j<_dim; j++){
		maxCol = copy.maxUnderDiag(j);
		if(abs(copy.getValue(maxCol,j))>EPSILON_ERROR){			//si es cero se pasa a la otra columna, ya esta lo que queremos
			copy.swapFi(j,maxCol);
			copy._P.swap(j,maxCol);
			I.swapFi(j,maxCol);
			I._P.swap(j,maxCol);
			for(uInt i=j+1; i<=_dim; i++){
				coefficient = copy.coefficient(i,j);
				copy.putZero(i,j,coefficient,false);
				I.putZero(i,j,coefficient,false	);
			}
		}
	}
	
	/* Triangulo superiormente -- Aca no hay ceros en la diagonal, sino la matriz no seria inversible. No uso pivoteo parcial para no arruinar los ceros que ya consegui abajo de la diagonal */
	for(uInt j=_dim; j>1; j--){
		if(abs(copy.getValue(j,j))<EPSILON_ERROR) throw MatrixException((char*)"Error no deberia haber ceros en la diagonal", Default);
		for(uInt i=1; i<j; i++){
			coefficient = copy.coefficient(i,j);
			copy.putZero(i,j,coefficient,false);
			I.putZero(i,j,coefficient,false);
		}
	}
	
	T elemDiag;
	T elem;

	/*Pone unos en la diagonal para llegar a la identidad, modifica solo la matriz resultante*/
	for(uInt i=1; i<=_dim; i++){
		elemDiag = copy.getValue(i,i);
		for(uInt j=1; j<=_dim; j++){
			elem = I.getValue(i,j);
			elem /= elemDiag;
			I.setValue(elem,i,j);
		}
	}
	
	return I;
}

template <typename T>
bool Matrix<T> :: isTriang(bool superior) const {
	bool res = true;
	
	if(superior){
		for(uInt i=2; i<=_dim; i++)
			for(uInt j=1; j<i; j++)
				res &= (abs(this->getValue(i,j))<EPSILON_ERROR);
	}
	else{
		for(uInt i=1; i<_dim; i++)
			for(uInt j=i+1; j<=_dim; j++)
				res &= (abs(this->getValue(i,j))<EPSILON_ERROR);
	}

	return res;
}

template <typename T>
bool Matrix<T> :: isId() const {
	bool res = true;

	for(uInt i=1; i<=_dim; i++)
		for(uInt j=1; j<=_dim; j++)
			res &= (i==j && (abs(this->getValue(i,j)-1)<EPSILON_ERROR)) || (i!=j && (abs(this->getValue(i,j))<EPSILON_ERROR));

	return res;
}


template <typename T>
T Matrix<T> :: K() const{
	Matrix<T> Inverse(_dim);
	Inverse = this->inverse();
	T k = Inverse.normF();
	k *= this->normF();
	return k;
}

template <typename T>
Matrix<T>& Matrix<T> :: operator= (const MatrixBase<T> &mb){
	if(!this->matchExactDimesions(mb))
		throw MatrixException((char*)"Asignacion de matrices de diferente dimension)", Default);

	for(uInt i=1; i<=_dim; i++)
		for(uInt j=1;j<=_dim;j++)
			this->setValue(mb.getValue(i,j),i,j);

	return *this;
}

template <typename T>
T Matrix<T> :: det() const{
	Matrix<T> copy(*this);

	if(!this->isTriang(true) && !this->isTriang(false)){
		copy = this->gaussianElim();
	}

	T aux;
	T det = 1;
	for(uInt i=1; i<=_dim; i++){
		aux = copy.getValue(i,i);
		if(abs(aux) < EPSILON_ERROR) return 0;							//si es menor al epsilon lo considero cero
		det *= aux;
	}

	return abs(det);
}

template <typename T>
void Matrix<T> :: Gauss_LU(bool L){		
	if(!this->isTriang(true)){
		uInt maxCol=0;
		T coefficient;
		for(uInt j=1; j<=_dim; j++){
			maxCol = this->maxUnderDiag(j);
			if(abs(this->getValue(maxCol,j))>EPSILON_ERROR){			//si es cero se pasa a la otra columna, ya esta lo que queremos
				this->swapFi(j,maxCol);
				_P.swap(j,maxCol);
				for(uInt i=j+1; i<=_dim; i++){
					coefficient = this->coefficient(i,j);
					this->putZero(i,j,coefficient,true);
					if(L) this->setValue(coefficient,i,j);
				}
			}
		}
	}
}

template <typename T>
T Matrix<T> :: coefficient(uInt i, uInt j){
	T pivot = this->getValue(j,j);
	if(abs(pivot)<EPSILON_ERROR)
		throw MatrixException((char*)"El pivot es cero.", Default);

	pivot = this->getValue(i,j)/pivot;

	return pivot;
}

/* Esta funcion afecta toda la fila de la matriz donde se quiere poner el cero para que sirva tanto para triangular abajo como arriba de la diagonal, funciona siempre y cuando los ceros se vayan poniendo en orden
 * Se agrega el bool porque para LU se guarda los coeficientes de L en la matriz y si se afecta toda la fila cambian */
template <typename T>
void Matrix<T> :: putZero(uInt i, uInt j, T coefficient, bool from_j){
	T elem;
	for(uInt k=1 + (j-1)*from_j; k<=_dim; k++){
		elem = coefficient*this->getValue(j,k);
		elem = this->getValue(i,k) - elem;
		this->setValue(elem,i,k);
	}
}

template <typename T>
uInt Matrix<T> :: maxUnderDiag(uInt j) const{
	if(j==0 || j>_dim)
		throw MatrixException((char*)"El indice no pertenece a la diagonal, no esta en rango.", Default);

	T pivot = abs(this->getValue(j,j));
	uInt pivot_pos = j;

	for(uInt i=j+1; i<=_dim; i++){
		T elem = abs(this->getValue(i,j));
		if(elem>pivot){
			pivot = elem;
			pivot_pos = i;
		}
	}

	return pivot_pos;
}

template <typename T>
T Matrix<T> :: normF() const {
	T normF = this->getValue(1,1);

	if(_dim==1) return normF;

	for(uInt i=1; i<=_dim; i++)
		for(uInt j=1; j<=_dim; j++)
			normF += this->getValue(i,j)*this->getValue(i,j);

	normF -= this->getValue(1,1);
	return sqrt(normF);
}

template <typename T>
void Matrix<T> :: createId(uInt dim){
	for(uInt i=1;i<=dim;i++)
		this->setValue((T) 1, i,i);
}

template <typename T>
bool Matrix<T> :: createBadK(uInt dim) {
	uInt mode = rand()%10;
	if(mode==0 && dim>4){							//para matrices chicas el num de cond de Hilbert no es tan malo
		//matrix de hilbert por un coef
		T randomCoef = rand()%100;
		Matrix<double> bad_conditioned(dim, Hilbert);
		*this = randomCoef * bad_conditioned;
	}
	else
	{
		//matriz con filas casi ld
		T epsilon = 1.0/1000000.0;

		Vector<T> randomV(dim);
		randomV.createRandomVector();
		
		for(uInt i=1;i<=dim;i++)
			for(uInt j=1;j<=dim;j++){
				this->setValue(randomV.getValue(j),i,j);
				if(i==j)
					this->setValue(randomV.getValue(j)+epsilon,i,j);
			}
	}
	return this->isInversible();
}

/* Matriz de Hilbert de orden dim */
template <typename T>
void Matrix<T> :: createHilbertMatrix(uInt dim){
	T elem;
	for(uInt i=1; i<=dim; i++)
		for(uInt j=1; j<=dim; j++){
			elem = 1;
			elem /= (i+j-1);
			this->setValue(elem,i,j);
		}
}

#endif

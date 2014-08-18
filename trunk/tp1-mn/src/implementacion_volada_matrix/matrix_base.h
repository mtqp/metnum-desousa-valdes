#ifndef __MATRIX_BASE_H__
#define __MATRIX_BASE_H__

#include "includes.h"
#include "matrix_exceptions.h"

extern const double EPSILON_ERROR;

/*
	Quien herede de MatrixBase deberá atrapar las excepciones lanzadas
*/

template <class T>
class MatrixBase;
template <typename T>
ostream &operator<< (ostream &stream,const MatrixBase<T>& mb);

template <class T>
class MatrixBase{
	public:
		MatrixBase(uInt dimFi, uInt dimCol);
		MatrixBase(T** data, uInt dimFi, uInt dimCol); 
		MatrixBase(const MatrixBase<T>& mb);
		~MatrixBase();

		MatrixBase<T> operator+ (const MatrixBase<T> &mb) const;
		MatrixBase<T> operator- (const MatrixBase<T> &mb) const;
		MatrixBase<T> operator* (const MatrixBase<T> &mb) const;
		MatrixBase<T>& operator= (const MatrixBase<T> &mb);
		bool operator==(const MatrixBase<T> &mb) const;
		bool operator!=(const MatrixBase<T> &mb) const;

		MatrixBase<T> traspuesta() const;

		void setValue(T value, uInt i, uInt j);
		T    getValue(uInt i, uInt j) const;

		bool isInversible() const;
		bool isSquare() const;

		virtual T det() const;	///Se calcula recursivamente -- NO triangula
	
		friend ostream &operator<< <>(ostream &stream, const MatrixBase<T>& mb);
		
		static MatrixBase<T> scalarMult(const T& value, const MatrixBase<T> &mb);
		
		uInt  getFiDimension()  const;
		uInt  getColDimension() const;
		
	protected:
		bool matchExactDimesions(const MatrixBase<T> &mb) const;
		bool matchMultDimesions(const MatrixBase<T> &mb)  const;
		MatrixBase<T> deleteFi(uInt i) const;
		MatrixBase<T> deleteCol(uInt j)const;
		MatrixBase<T> deleteFiCol(uInt i, uInt j) const;
		MatrixBase<T>& swapFi(uInt i, uInt i2);
		MatrixBase<T>& swapCol(uInt j, uInt j2);
			
	private:
		void setMatrix(uInt dimFi, uInt dimCol);
		void multiplyFiCol(uInt fiI, uInt colJ, const MatrixBase<T>& A, const MatrixBase<T>& B);
		
		uInt _dimFi;
		uInt _dimCol;
		T** _matrix;
};

template <typename T>
MatrixBase<T> :: MatrixBase(uInt dimFi, uInt dimCol){
	setMatrix(dimFi,dimCol);
}

template <typename T>
MatrixBase<T> :: MatrixBase(const MatrixBase<T>& mb){
	setMatrix(mb._dimFi,mb._dimCol);

	for(uInt i=0; i<this->_dimFi; i++)
		for(uInt j=0;j<this->_dimCol;j++)
			this->_matrix[i][j] = mb._matrix[i][j];	
}

template <typename T>
MatrixBase<T> :: MatrixBase(T** data, uInt dimFi, uInt dimCol){
	setMatrix(dimFi,dimCol);
	
	///Copia la estructura en vez de hacer alias <==> T no genera alias en la asignacion
	for(uInt i=0; i<_dimFi; i++)
		for(uInt j=0;j<_dimCol;j++)
			_matrix[i][j] = data[i][j];	
}

template <typename T>
MatrixBase<T> :: ~MatrixBase(){
	for(uInt j=0;j<_dimFi; j++)
		delete [] _matrix[j];
		
	delete [] _matrix;
}

template <typename T>
MatrixBase<T> MatrixBase<T> :: operator+ (const MatrixBase<T> &mb) const {
	if(!matchExactDimesions(mb))
		throw MatrixException((char*)"Suma de matrices de distinta dimension.", Default);
	
	MatrixBase<T> resultSum(this->_dimFi,this->_dimCol);

	for(uInt i=0;i<_dimFi;i++)
		for(uInt j=0;j<_dimCol;j++)
			resultSum._matrix[i][j] = this->_matrix[i][j] + mb._matrix[i][j];

	return resultSum;
}

template <typename T>
MatrixBase<T> MatrixBase<T> :: operator- (const MatrixBase<T> &mb) const {
	if(!matchExactDimesions(mb))
		throw MatrixException((char*)"Resta de matrices de distinta dimension.", Default);
	
	MatrixBase<T> resultSub(this->_dimFi,this->_dimCol);
	
	for(uInt i=0;i<_dimFi;i++)
		for(uInt j=0;j<_dimCol;j++)
			resultSub._matrix[i][j] = this->_matrix[i][j] - mb._matrix[i][j];

	return resultSub;
}

template <typename T>
MatrixBase<T> MatrixBase<T> :: operator* (const MatrixBase<T> &mb) const {
	if(!matchMultDimesions(mb))
		throw MatrixException((char*)"Multiplicación con incorrecta dimensiones.", Default);
	
	MatrixBase<T> resultMult(this->_dimFi,mb._dimCol);

	for(uInt i=0;i<this->_dimFi;i++){
		for(uInt j=0;j<mb._dimCol;j++){
			resultMult.multiplyFiCol(i,j,*this, mb);
		}
	}
	
	return resultMult;
}

template <typename T>
MatrixBase<T> operator* (const T& value, MatrixBase<T> &mb){
	return MatrixBase<T> :: scalarMult(value,mb);
}

template <typename T>
MatrixBase<T> operator* (MatrixBase<T> &mb, const T& value){
	return MatrixBase<T> :: scalarMult(value,mb);
}

template <typename T>
bool MatrixBase<T> :: operator==(const MatrixBase<T> &mb) const {
	if(!matchExactDimesions(mb))
		return false;
	
	bool eq = true;

	for(uInt i=0; i<this->_dimFi && eq; i++)
		for(uInt j=0; j<this->_dimCol && eq; j++)
			eq = abs(this->_matrix[i][j] - mb._matrix[i][j]) < ::EPSILON_ERROR;
	
	return eq;
}

template <typename T>
bool MatrixBase<T> :: operator!=(const MatrixBase<T> &mb) const {
	return !(*this==mb);
}


template <typename T>
MatrixBase<T>& MatrixBase<T> :: operator= (const MatrixBase<T> &mb){
	if(this!=&mb){
		if(!matchExactDimesions(mb))
			throw MatrixException((char*)"Asignacion de matrices de distinta dimension", Default);
	
		for(uInt i=0; i<mb._dimFi; i++)
			for(uInt j=0;j<mb._dimCol;j++)
				this->_matrix[i][j] = mb._matrix[i][j];	
	}
	
	return *this;
}

template <typename T>
MatrixBase<T> MatrixBase<T> :: traspuesta() const {
	MatrixBase<T> mt(this->_dimCol,this->_dimFi);
	for(uInt i=0; i<mt._dimFi; i++)
		for(uInt j=0;j<mt._dimCol;j++)
			mt._matrix[i][j] = this->_matrix[j][i];
	
	return mt;
}


template <typename T>
MatrixBase<T> MatrixBase<T> :: scalarMult(const T& value, const MatrixBase<T> &mb){
	MatrixBase<T> scalarMultMatrix(mb._dimFi,mb._dimCol);

	for(uInt i=0; i<mb._dimFi; i++)
		for(uInt j=0;j<mb._dimCol;j++)
			scalarMultMatrix._matrix[i][j] = value * mb._matrix[i][j];

	return scalarMultMatrix;	
}

template <typename T>
void MatrixBase<T> :: setValue(T value, uInt i, uInt j){
	if(i==0 || i>_dimFi || j==0 || j>_dimCol)
		throw MatrixException((char*)"SetValue -->Asignacion de valor en posicion inexistente", Default);

	_matrix[i-1][j-1] = value;
}

template <typename T>
T MatrixBase<T> :: getValue(uInt i, uInt j) const {
	if(i==0 || i>_dimFi)
		throw MatrixException((char*)"GetValue --> fila cero o inexistente", Default);
	if(j==0 || j>_dimCol)
		throw MatrixException((char*)"GetValue --> columna cero o inexistente", Default);
		
	return this->_matrix[i-1][j-1];
}

template <typename T>
bool MatrixBase<T> :: isInversible() const {
	return det()!=0;	//T tiene q tener definido la desigualdad contra int!
}

template <typename T>
bool MatrixBase<T> :: isSquare() const {
	return _dimFi == _dimCol;
}

template <typename T>
T MatrixBase<T> :: det() const {
	if(!isSquare())
		throw MatrixException((char*)"Calculo de determinante en matriz cuadrada.", Default);
	
	if(_dimFi==1)					//Caso base
		return this->_matrix[0][0];
	
	if(_dimFi==2 && _dimCol==2){	//Caso base
		return (_matrix[0][0]*_matrix[1][1]) - (_matrix[0][1]*_matrix[1][0]);
	}

	int determinante = 0;
	uInt i = 1;

	for(uInt j=1;j<=_dimCol;j++){	//Caso Recursivo
		MatrixBase<T> subMatrix(this->_dimFi-1,this->_dimCol-1);
		subMatrix = this->deleteFiCol(i,j);

		T subDet = this->_matrix[i-1][j-1] * subMatrix.det();

		if((i+j)%2 == 0)	//1^(i+j)
			determinante = determinante + subDet;
		else
			determinante = determinante - subDet;
	}
	
	return determinante;
}


template <typename T>
ostream &operator<< (ostream &stream, const MatrixBase<T>& mb){
  stream << "Dimension Filas = " << mb._dimFi << endl;
  stream << "Dimension Columnas = " << mb._dimCol << endl;
  
  for(uInt i=0; i<mb._dimFi; i++){
  	stream << endl;
  	for(uInt j=0; j<mb._dimCol; j++){
  		stream << mb._matrix[i][j] << "\t";
  	}
  }
  
  stream << endl;
  return stream; 
}

template <typename T>
uInt  MatrixBase<T> :: getFiDimension() const {
	return _dimFi;
}

template <typename T>
uInt  MatrixBase<T> :: getColDimension() const {
	return _dimCol;
}


template <typename T>
bool MatrixBase<T> :: matchExactDimesions(const MatrixBase<T> &mb) const{
	return (this->_dimFi==mb._dimFi) && (this->_dimCol==mb._dimCol);
}

template <typename T>
bool MatrixBase<T> :: matchMultDimesions(const MatrixBase<T> &mb)  const{
	return this->_dimCol == mb._dimFi;
}

template <typename T>
MatrixBase<T> MatrixBase<T> :: deleteFi(uInt fiElim) const {
	if(fiElim==0 || fiElim>_dimFi || this->_dimFi==1)
		throw MatrixException((char*)"NO se puede realizar la eliminacion de la fila", Default);
	
	fiElim--;	//Recordar que los arreglos son [0,...n-1], nosotros pasamos [1,...,n]
	
	MatrixBase<T> deletedFi(_dimFi-1,_dimCol);
	
	for(uInt i=0; i<deletedFi._dimFi; i++){
		if(i<fiElim){
			for(uInt j=0;j<_dimCol;j++){
				deletedFi._matrix[i][j] = this->_matrix[i][j];
			}
		}
		else 
		{
			for(uInt j=0;j<_dimCol;j++){
				deletedFi._matrix[i][j] = this->_matrix[i+1][j];
			}
		}
	}
	
	return deletedFi;
}

template <typename T>
MatrixBase<T> MatrixBase<T> :: deleteCol(uInt colElim) const {
	if(colElim==0 || colElim>_dimCol || _dimCol==1)
		throw MatrixException((char*)"NO se puede realizar la eliminacion de la columna", Default);
		
	MatrixBase<T> deletedCol(_dimFi,_dimCol-1);
	
	colElim--;	//Recordar que los arreglos son [0,...n-1], nosotros pasamos [1,...,n]
	
	for(uInt j=0;j<deletedCol._dimCol;j++){
		if(j<colElim){
			for(uInt i=0; i<this->_dimFi; i++){
				deletedCol._matrix[i][j] = this->_matrix[i][j];
			}
		}
		else 
		{
			for(uInt i=0; i<this->_dimFi; i++){
				deletedCol._matrix[i][j] = this->_matrix[i][j+1];
			}
		}
	}
	
	return deletedCol;
}

template <typename T>
MatrixBase<T> MatrixBase<T> :: deleteFiCol(uInt i, uInt j) const {
	if(i==0 || i>_dimFi)
		throw MatrixException((char*)"Se desea eliminar fila NO existente", Default);
	if(j==0 || j>_dimCol)
		throw MatrixException((char*)"Se desea eliminar columna NO existente", Default);

	MatrixBase<T> delFi(_dimFi-1,_dimCol);
	delFi = this->deleteFi(i);
	
	MatrixBase<T> delCol(delFi._dimFi,_dimCol-1); 
	delCol = delFi.deleteCol(j);
	
	return delCol;
}

template <typename T>	//genera aliasing
MatrixBase<T>& MatrixBase<T> :: swapFi(uInt i, uInt i2){
	if(i==0 || i>_dimFi || i2==0 || i2>_dimFi)
		throw MatrixException((char*)"Swap filas con posiciones no existentes", Default);	
		
	if(i==i2)	//pequeña optimizacion
		return *this;
	
	T swapElem;
	
	for(uInt h=0;h<_dimCol;h++){
		swapElem = _matrix[i2-1][h];
		_matrix[i2-1][h] = _matrix[i-1][h];
		_matrix[i-1][h] = swapElem;	
	}
	
	return *this;
}

template <typename T>	//genera aliasing
MatrixBase<T>& MatrixBase<T> :: swapCol(uInt j, uInt j2){
	if(j==0 || j>_dimCol || j2==0 || j2>_dimCol)
		throw MatrixException((char*)"Swap columnas con posiciones no existentes", Default);	

	if(j==j2)
		return *this;
	
	T swapElem;

	for(uInt h=0;h<_dimFi;h++){
		swapElem = _matrix[h][j2-1];
		_matrix[h][j2-1] = _matrix[h][j-1];
		_matrix[h][j-1] = swapElem;
	}

	return *this;
}


template <typename T>
void MatrixBase<T> :: setMatrix(uInt dimFi, uInt dimCol){
	_dimFi = dimFi;
	_dimCol= dimCol;
	
	if(_dimFi==0 || _dimCol==0)
		throw MatrixException((char*)"Las dimensiones de la matriz deben ser mayores a cero", Default);
	
	_matrix = new T* [_dimFi];

    for (uInt i=0; i<_dimFi; i++){
      	_matrix[i] = new T [_dimCol];
     	for(uInt j=0; j<_dimCol; j++){
     		memset((void*) &_matrix[i][j],0,sizeof(T));
     	}
     }
}

template <typename T>
void MatrixBase<T> :: multiplyFiCol(uInt fiI, uInt colJ, const MatrixBase<T>& A, const MatrixBase<T>& B){
	for(uInt i=0; i<A._dimCol; i++){
		this->_matrix[fiI][colJ] = this->_matrix[fiI][colJ] + (A._matrix[fiI][i] * B._matrix[i][colJ]);
	}
}

#endif

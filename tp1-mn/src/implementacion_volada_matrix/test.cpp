#include "includes.h"
#include "structs.h"
#include "matrix.h"
#include "vector.h"
#include "warp_cannon.h"
#include "linear_system.h"


/*	arreglar lo q qdo feo del determinante
	constructor por copia de vector
	constructor y destructor q warpcannon
	generador de matrices mal condicionadas (matrix.h)
*/


int main(int argc, char** argv){
	cout.precision(20);
	cout.setf(ios::scientific,ios::floatfield);

	try
	{
		/*Matrix<double> A(10,Hilbert);
		cout << "A --->" << endl;
		cout << "A" << endl;
		cout << A;
		Vector<double> pos(10);
		pos.setValue(1,1);
		pos.setValue(1,2);
		pos.setValue(1,3);
		pos.setValue(1,4);
		pos.setValue(1,5);
		pos.setValue(1,6);
		pos.setValue(1,7);
		pos.setValue(1,8);
		pos.setValue(1,9);
		pos.setValue(1,10);
		Vector<double> d(10);
		d = (A*pos.traspuesta()).traspuesta();
		cout << "d --->" << endl;
		cout << d << endl;*/
		
		uInt dim=5;
		Matrix<double> id(dim,ID);
		cout << "ID" << endl;
		cout << id;
		Matrix<double> id_inverse(dim);
		id_inverse = id.inverse();
		cout << "Inversa de ID" << endl;
		cout << id_inverse;
		Matrix<double> res(dim);
		res = id*id_inverse;
		cout << "es id?" << endl;
		cout << res;
		
/*		srand(time(NULL));
		cout.precision(6);
		/*Matrix<int>A(1,1);
		Matrix<int>B(A);
		
		int dim = 10;
		
		warpData wd(dim);
		wd.d.setValue(1,1);
		wd.d.setValue(2,2);
		wd.d.setValue(3,3);
		wd.d.setValue(4,4);		
		wd.position.setValue(10,1);
		wd.position.setValue(5,2);
		wd.position.setValue(5,3);
		wd.position.setValue(1,4);
	
		Matrix<double> C(dim,BadK);
		cout << C.K() << endl;
	
		WarpCannon wp(wd,dim);
		Matrix<double> B(wp.getMatrixAttack(wd.d));
		cout << "resultado: " << endl;
		cout << B.K() << endl;
*/		
/*		for(int i=0;i<20;i++)
			cout << wp.createRandomVector();
		cout << "mal condicionada hilberiana" << endl;
		//for(int i=0;i<2;i++){
		Matrix<double> A(wp.getBadKMatrix());
			cout << "isInversible? = " << A.isInversible() << endl;
			cout << "det value = "  << A.det() << endl;
			cout << "nro condicion = " << A.K() << endl;
			cout << A;
			cout << "inversa de A" << endl;
			cout << A.inverse();
			cout << "id???" << endl;
			cout << A * A.inverse();
			cout << "---------------------------" << endl;
			
		Matrix<double> B(wp.getBadKMatrix());
			cout << "isInversible? = " << B.isInversible() << endl;
			cout << "det value = "  << B.det() << endl;
			cout << "nro condicion = " << B.K() << endl;
			cout << B;
			cout << "inversa de B" << endl;
			cout << B.inverse();
			cout << "id???" << endl;
			cout << B * B.inverse();
			cout << "---------------------------" << endl;
		//}
	
/*		Vector<double> v(10);
		for(int i=1;i<=10;i++)
			v.setValue(i,i);
		
		cout << "v" << endl;
		cout << v;
		
/*		cout << "v tras" << endl;
		cout << v.traspuesta().traspuesta();
	*//*
		Vector<double> x(v.traspuesta());
		cout << x;
		
/*		Vector<double> z(10,true);
		z = v.traspuesta();
		cout << "freaking z" << endl;
		cout << z;
//		cout << v.traspuesta();	
/*		Vector<double> copy(v.traspuesta());
		cout << "v tras copy" << endl << copy;
		
/*		MatrixBase<double> A(3,3);
		A.setValue(2,1,1);
		A.setValue(-2,1,2);
		A.setValue(1,1,3);
		A.setValue(-1,2,1);
		A.setValue(1,2,2);
		A.setValue(1,2,3);
		A.setValue(-1,3,1);
		A.setValue(3,3,2);
		A.setValue(5,3,3);
		
		cout << "A" << endl;
		cout << A << endl;
		cout << "det A"  << A.det() << endl;
/*			
		Matrix<double> B(3);
		B=A.inverse();
		
		cout << "Inversa de A" << endl;
		cout << B << endl;
		
		Matrix<double> C(3);
		C=A*B;
		
		cout << "Id?" << endl;
		cout << C << endl;
//*/		
		/*
		Matrix<double> C(2);
		C.setValue(4,1,1);
		C.setValue(4,1,2);
		C.setValue(0,2,1);
		C.setValue(5,2,2);

		
		cout << "C" << endl;
		cout << C << endl;
		cout << C.det() << endl;*/

	}
	catch (MatrixException& e) //HACER ANDAR BIEN!
	{
		cout << "MatrixException: " << e.what() << endl;
		/*
			Las excepciones se utilizan para no cortar con la ejecucion del programa,
			se puede hacer un manejo mas inteligente de ser necesario, basta con agregar
			funcionalidad al objeto MatrixException
		*/
	}

	return 0;
}


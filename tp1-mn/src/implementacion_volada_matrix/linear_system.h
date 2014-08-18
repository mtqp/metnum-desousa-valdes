#ifndef __LINEAR_SYSTEM_H__
#define __LINEAR_SYSTEM_H__

#include "includes.h"
#include "vector.h"
#include "matrix.h"

class linearSystem{
	public:
		linearSystem(const Matrix<double> A, const Vector<double> d);
		~linearSystem();
		
		Vector<double> usingInverse() const;
		Vector<double> usingLU() const;

	private:
		Vector<double> backSub() const;
		Vector<double> forwardSub() const;
	
		uInt _dim;
		Matrix<double> _A;
		Vector<double> _d;
};

#endif


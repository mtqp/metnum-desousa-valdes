#ifndef __MATRIX_EXCEPTIONS_H__
#define __MATRIX_EXCEPTIONS_H__

#include "includes.h"

enum MatrixExceptionType {
	NotInversible,
	Default
	/*ETC*/
};

class MatrixException : public exception{
	public:
		MatrixException(char*,MatrixExceptionType);

		const char* what() const throw();
		const MatrixExceptionType type() const;
		void show();
	private:
		char* message;
		MatrixExceptionType _type;
};

#endif

#include "matrix_exceptions.h"

MatrixException :: MatrixException(char* exception, MatrixExceptionType type){
	_type = type;
	message = exception;
}

const char* MatrixException :: what() const throw(){
	return message;
}

const MatrixExceptionType MatrixException :: type() const {
	return _type;
}

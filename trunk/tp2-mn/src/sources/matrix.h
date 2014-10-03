#ifndef __MATRIX__
#define __MATRIX__

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#define EPS 0.0001
#define NO_ROW -1

using namespace std;

class CRSMatrix {//: public RealMatrix{ //Compressed sparse row
    public:
        CRSMatrix(int n, int m, vector<int> colIndexes, vector<int> rowPointers, vector<double> values) : _n(n), _m(m), _colIndexes(colIndexes), _rowPointers(rowPointers), _values(values){}
        int amountOfRows();
		int amountOfColumns();
        ~CRSMatrix();

        vector<double> Multiply(vector<double> aVector);
        void PrintItSelf();
        double ElementAt(int i, int j);
        
    private:
        int _n;
        int _m;
        vector<int> _colIndexes;
        vector<int> _rowPointers;
        vector<double> _values;
        
        void CheckRanges(int i, int j);
        int UpperBoundFromRow(int row);
};

class CRSBuilder{
    public:
        void AddElementAt(int i, int j, double value);
        CRSMatrix Build(int n, int m);

    private:
        class MatrixElement{
            public:
            MatrixElement(int i, int j, double value){
                this->row = i;
                this->column = j;
                this->value = value;
            }
            
            void PrintItSelf(){
                cout << "i: " << row << " j: " << column << " value: " << value << endl;
            }
            
            int row;
            int column;
            double value;
        };
    
        vector<MatrixElement> _elements;

        struct Sorter {
            bool operator() (MatrixElement left, MatrixElement right) {
                if(left.row != right.row)
                    return left.row < right.row;
                
                //Rows are equal, we must untie by column
                return left.column < right.column;
            }
        } SortingFunction;
        
        vector<int> CollectIndexes();        
        vector<int> CollectPointers(int amountOfRows);
        vector<double> CollectValues();
};


#endif

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

double EPS = 0.0001;
int NO_ROW = -1;

class CRSMatrix {//: public RealMatrix{ //Compressed sparse column
    public:
        CRSMatrix(int n, int m, vector<int> colIndexes, vector<int> rowPointers, vector<double> values){
            _n = n;
            _m = m;

            _colIndexes = colIndexes;
            _rowPointers = rowPointers;
            _values = values;
        }
        
        int amountOfRows(){
			return _m;
		}
		
		int amountOfColumns(){
			return _m;
		}
        
        ~CRSMatrix(){ //todo: complete!
            
        }

        vector<double> Multiply(vector<double> aVector){ 
			//c*A +  (1-c/N) remember to do this on he crazy page rank algorithm
   			//multiply knowing that if element i.j is zero, we should user (1-c)/n
            vector<double> multiplication;

            for(int row=0; row<_n; row++){
                bool isRowEmpty = _rowPointers[row] == NO_ROW;
                if(isRowEmpty){ //any vector multiplying a zero vector equals zero.
                    multiplication.push_back(0.0);
                    continue;
                }
                
                int colLowerBound = _rowPointers[row];
                int colUpperBound = UpperBoundFromRow(row);
                
                double rowValue = 0;
                
                for(int index=colLowerBound; index<colUpperBound; index++){
                    int column = _colIndexes[index];
                    double matrixValue = _values[index];
                    double vectorValue = aVector[column];
                    
                    bool vectorValueIsZero = abs(vectorValue) < EPS;
                
                    if(!vectorValueIsZero){
                        double i_jTimesVector = vectorValue * matrixValue;
                        rowValue += i_jTimesVector;
                    }
                }
                
                multiplication.push_back(rowValue);
            }

            return multiplication;
        }

        void PrintItSelf(){ //esto es malisimo!, solo para debuggear, borrar antes de entregar
            cout << "colIndex: " << _colIndexes.size() << endl;
            for(int i=0; i < (int)_colIndexes.size(); i++)
            {
                cout << _colIndexes[i] << ", ";
            }
            cout << endl;
            
            cout << "rowPointers: " << _rowPointers.size() << endl;
            for(int i=0; i < (int)_rowPointers.size(); i++)
            {
                cout << _rowPointers[i] << ", ";
            }
            cout << endl;
            
            cout << "values: " << _values.size() << endl;
            for(int i=0; i < (int)_values.size(); i++)
            {
                cout << _values[i] << ", ";
            }
            cout << endl;
            
            
            cout << "Whole matrix: " << endl;
            
            for(int i=0; i<_n; i++)
            {
                for(int j=0; j<_m; j++)
                {
                    cout << "(" << i << "," << j << ") = " << ElementAt(i,j) << endl;
                }
            }
        }
        
        double ElementAt(int i, int j) { 
            CheckRanges(i, j);
            
            int colIndex = _rowPointers[i];
            
            if(colIndex != NO_ROW){
                int lowerSearchBound = colIndex;
                int upperSearchBound = UpperBoundFromRow(i);
                
                for(int index=lowerSearchBound; index<upperSearchBound; index++)
                {
                    bool rowWasFound = _colIndexes[index] == j;
                    if(rowWasFound)
                        return _values[index];
                }
            }
            return 0.0; //when element is not found
        }
        
    private:
        int _n;
        int _m;
        vector<int> _colIndexes;
        vector<int> _rowPointers;
        vector<double> _values;
        
        void CheckRanges(int i, int j){
            if(i>= _n)
                cout << "ElementAt - i: " << i << " is out of range" << endl;
            if(j>=_m)
                cout << "ElementAt - j: " << j << " is out of range" << endl;
        }
         
        int UpperBoundFromRow(int row)
        {
            bool isLastColumn = row == (_n-1);
            int upperSearchBound;
            
            if(!isLastColumn){
                int firstNonZeroRow = row;
                bool nextPointerIsNoRow;
                do
                {
                    firstNonZeroRow++;
                    nextPointerIsNoRow = _rowPointers[firstNonZeroRow] == NO_ROW;
                }
                while(nextPointerIsNoRow && firstNonZeroRow < (int)_rowPointers.size());
                
                upperSearchBound = _rowPointers[firstNonZeroRow];
            }
            else
                upperSearchBound = _colIndexes.size();

            return upperSearchBound;
        }
};

class CRSBuilder{
    public:
        void AddElementAt(int i, int j, double value)
        {
            //insert element as transpose matrix cause we actually need CSRMatrix
            MatrixElement element(i, j, value);
            
            _elements.push_back(element);
        }
        
        CRSMatrix Build(int n, int m){
            sort(_elements.begin(), _elements.end(), SortingFunction);
        
            vector<int> colIndexes = CollectIndexes();
            vector<int> rowPointers = CollectPointers();
            vector<double> values = CollectValues();
            
            CRSMatrix matrix(n, m, colIndexes, rowPointers, values);
            return matrix;
        }

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
        
        vector<int> CollectIndexes(){
            vector<int> indexes;
            for(int i=0; i < (int)_elements.size(); i++){
                indexes.push_back(_elements[i].column);
            }
            return indexes;
        }
        
        vector<int> CollectPointers(){
            vector<int> pointers;
            int actualRow = -1;
            
            for(int nextPointer=0; nextPointer < (int)_elements.size(); nextPointer++){
                int elementRow = _elements[nextPointer].row;
                
                int mustMoveToNextRow = (elementRow - actualRow) >= 1;
                int rowLeap = elementRow - actualRow;
                if(mustMoveToNextRow)
                {
                    while(rowLeap > 1){
                        pointers.push_back(NO_ROW);    
                        rowLeap--;
                    }
                    pointers.push_back(nextPointer);    
                    actualRow = elementRow;
                }
            }
            return pointers;
        }
        
        vector<double> CollectValues(){
            vector<double> values;
            for(int i=0; i < (int)_elements.size(); i++){
                values.push_back(_elements[i].value);
            }
            return values;
        }
};


//~ int main(int argc, char *argv[]) {	
    //~ CRSBuilder builder;
    //~ builder.AddElementAt(0, 0, 1.01);
    //~ builder.AddElementAt(0, 2, 2.0);
    //~ builder.AddElementAt(1, 0, 10.0);
    //~ builder.AddElementAt(1, 1, 11.0);
    //~ builder.AddElementAt(2, 2, 22.0);
    //~ builder.AddElementAt(2, 1, 21.0);
    //~ 
    //~ CRSMatrix matrix = builder.Build(3,3);
    //~ 
    //~ matrix.PrintItSelf();
    //~ 
    //~ vector<double> multiplyingVector;
    //~ multiplyingVector.push_back(-1.0);
    //~ multiplyingVector.push_back(1.0);
    //~ multiplyingVector.push_back(0.0);
    //~ vector<double> multiplication = matrix.Multiply(multiplyingVector);
//~ 
    //~ cout << "Multiplication result: "; 
    //~ for(int i=0;i<multiplication.size();i++)
        //~ cout << multiplication[i] << ", " ;
    //~ cout << endl;
    //~ 
	//~ return 0;
//~ }

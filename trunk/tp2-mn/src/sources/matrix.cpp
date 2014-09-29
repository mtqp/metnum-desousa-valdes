#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;


class CSCMatrix {//: public RealMatrix{ //Compressed sparse column
    public:
        CSCMatrix(int n, int m, vector<int> colIndexes, vector<int> rowPointers, vector<double> values){
            _n = n;
            _m = m;

            _colIndexes = colIndexes;
            _rowPointers = rowPointers;
            _values = values;
        }
        
        ~CSCMatrix(){ //todo: complete!
            
        }
        
        vector<double>& Multiply(vector<double> aVector){ 
			//c*A +  (1-c/N)
   			//multiply knowing that if element i.j is zero, we should user (1-c)/n
            vector<double> dummy;
            return dummy;
        }

        void PrintItSelf(){ //esto es malisimo!, solo para debuggear, borrar antes de entregar
            cout << "colIndex: " << _colIndexes.size() << endl;
            for(int i=0; i<_colIndexes.size(); i++)
            {
                cout << _colIndexes[i] << ", ";
            }
            cout << endl;
            
            cout << "rowPointers: " << _rowPointers.size() << endl;
            for(int i=0; i<_rowPointers.size(); i++)
            {
                cout << _rowPointers[i] << ", ";
            }
            cout << endl;
            
            cout << "values: " << _values.size() << endl;
            for(int i=0; i<_values.size(); i++)
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
            
            int NO_ROW = -1;
            int colIndex = _rowPointers[j];
            
            if(colIndex != NO_ROW){
                bool isLastColumn = j == (_m-1);
                
                int lowerSearchBound = colIndex;
                int upperSearchBound;
                
                if(!isLastColumn)
                    upperSearchBound = _rowPointers[j+1];
                else
                    upperSearchBound = _colIndexes.size();
                
                for(int index=lowerSearchBound; index<upperSearchBound; index++)
                {
                    bool rowWasFound = _colIndexes[index] == i;
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

};

class CSCBuilder{
    public:
        void AddElementAt(int i, int j, double value)
        {
            MatrixElement element(i, j, value);
            _elements.push_back(element);
        }
        
        CSCMatrix Build(int n, int m){
            sort(_elements.begin(), _elements.end(), SortingFunction);
        
            vector<int> colIndexes = CollectIndexes();
            vector<int> rowPointers = CollectPointers();
            vector<double> values = CollectValues();
            
            CSCMatrix matrix(n, m, colIndexes, rowPointers, values);
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
                if(left.column != right.column)
                    return left.column < right.column;
                
                //Columns are equal, we must untie by row
                return left.row < right.row;
            }
        } SortingFunction;
        
        vector<int> CollectIndexes(){
            vector<int> indexes;
            for(int i=0; i<_elements.size(); i++){
                indexes.push_back(_elements[i].row);
            }
            return indexes;
        }
        
        vector<int> CollectPointers(){
            vector<int> pointers;
            int NO_ROW = -1;
            int actualRow = -1;
            
            for(int nextPointer=0; nextPointer<_elements.size(); nextPointer++){
                int elementRow = _elements[nextPointer].column;
                
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
            for(int i=0; i<_elements.size(); i++){
                values.push_back(_elements[i].value);
            }
            return values;
        }
};


int main(int argc, char *argv[]) {	
    CSCBuilder builder;
    builder.AddElementAt(0, 0, 1.01);
    builder.AddElementAt(0, 2, 2.0);
    builder.AddElementAt(1, 0, 10.0);
    builder.AddElementAt(1, 1, 11.0);
    builder.AddElementAt(2, 2, 22.0);
    builder.AddElementAt(2, 1, 21.0);
    
    CSCMatrix matrix = builder.Build(3,3);
    
    matrix.PrintItSelf();

	return 0;
}
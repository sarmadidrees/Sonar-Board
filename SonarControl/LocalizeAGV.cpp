
#include "LocalizeAGV.h"

char colorArray[11][5] = {{'W','W','R','R','R'},
                     	  {'W','W','R','R','R'},
                     	  {'W','W','R','R','R'},
                     	  {'W','W','W','W','W'},
                     	  {'Y','Y','Y','W','W'},
                     	  {'Y','Y','Y','W','G'},
                     	  {'Y','Y','Y','W','W'},
                     	  {'W','W','W','W','W'},
                     	  {'W','W','B','B','B'},
                     	  {'W','W','B','B','B'},
                     	  {'W','W','B','B','B'}
                     	  };


char frontArray[11][5] = {{'D','D','C','C','D'},
                          {'D','C','B','B','D'},
                          {'D','B','A','A','D'},
                          {'D','A','A','D','D'},
                          {'C','D','C','D','D'},
                          {'B','D','B','C','D'},
                          {'A','D','A','B','D'},
                          {'D','D','A','A','D'},
                          {'C','C','C','C','C'},
                          {'B','B','B','B','B'},
                          {'A','A','A','A','A'}
                          };

char rightArray[11][5] = {{'B','A','C','B','A'},
                          {'D','D','C','B','A'},
                          {'B','A','C','B','A'},
                          {'D','D','C','B','A'},
                          {'C','B','A','B','A'},
                          {'C','B','A','B','A'},
                          {'C','B','A','B','A'},
                          {'D','D','C','B','A'},
                          {'B','A','C','B','A'},
                          {'D','D','C','B','A'},
                          {'B','A','C','B','A'}
                          }; 

char backArray[11][5] = {{'A','A','A','A','A'},
                         {'B','B','B','B','B'},
                         {'C','C','C','C','C'},
                         {'D','D','A','A','D'},
                         {'D','A','A','B','D'},
                         {'D','B','B','C','D'},
                         {'D','C','C','D','D'},
                         {'A','D','A','D','D'},
                         {'B','D','A','A','D'},
                         {'C','D','B','B','D'},
                         {'D','D','C','C','D'}
                         };

char leftArray[11][5] =  {{'A','B','A','B','C'},
	                      {'A','B','C','D','D'},
	                      {'A','B','A','B','C'},
	                      {'A','B','C','D','D'},
	                      {'A','B','C','A','B'},
	                      {'A','B','C','A','B'},
	                      {'A','B','C','A','B'},
	                      {'A','B','C','D','D'},
	                      {'A','B','A','B','C'},
	                      {'A','B','C','D','D'},
	                      {'A','B','A','B','C'}
	                      };

//THE constructor :)                          
LocalizeAGV::LocalizeAGV(){

  memcpy(_colorArray,colorArray, sizeof(_colorArray));					//Copy global Array to private Array 
  
  _colLocalize = 0;
  _rowLocalize = 0;
}

void LocalizeAGV::setOrientation(char facing){
	_facing = facing;
}

unsigned int LocalizeAGV::getX(){
	return _colLocalize;
}

unsigned int LocalizeAGV::getY(){
	return _rowLocalize;
}

void LocalizeAGV::pleaseLocalize(char inputString[31]){

	_inputString = inputString;			//Store input char array to private char Array
	_rowLocalize = 0;
	_colLocalize = 0;

  //Copy each global array to private array wrt given orientation of ROBOT
	switch (_facing){
		case 'N':
	         memcpy(_frontArray,frontArray,sizeof(_frontArray));
	         memcpy(_rightArray,rightArray,sizeof(_rightArray));
	         memcpy(_backArray,backArray,sizeof(_backArray));
	         memcpy(_leftArray,leftArray,sizeof(_leftArray));
			 break;

		case 'E':
	         memcpy(_frontArray,rightArray,sizeof(_rightArray));
	         memcpy(_rightArray,backArray,sizeof(_backArray));
	         memcpy(_backArray,leftArray,sizeof(_leftArray));
	         memcpy(_leftArray,frontArray,sizeof(_frontArray));
			 break;

		case 'S':
	         memcpy(_frontArray,backArray,sizeof(_backArray));
	         memcpy(_rightArray,leftArray,sizeof(_leftArray));
	         memcpy(_backArray,frontArray,sizeof(_frontArray));
	         memcpy(_leftArray,rightArray,sizeof(_rightArray));
			 break;

		case 'W':
	         memcpy(_frontArray,leftArray,sizeof(_leftArray));
	         memcpy(_rightArray,frontArray,sizeof(_frontArray));
	         memcpy(_backArray,rightArray,sizeof(_rightArray));
	         memcpy(_leftArray,backArray,sizeof(_backArray));
			 break;
	}

	//Start searching through private arrays wrt given inputString
	for (_row = 0; _row <= 10; _row++) 
    {
        for (_col = 0; _col <= 4; _col++)
        {
            if ( _colorArray[_row][_col] == _inputString[colorIndex])
            {
                if ((_frontArray[_row][_col] == _inputString[frontIndex]) && 
                	(_rightArray[_row][_col] == _inputString[rightIndex]) && 
                	(_backArray [_row][_col] == _inputString[backIndex])  && 
                	(_leftArray [_row][_col] == _inputString[leftIndex]))
                {
                	_rowLocalize = _row;
                	_colLocalize = _col;
                  break;
                }
            }
        } 
    }
}

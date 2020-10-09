//JEFFREY ZHAO
#include<iostream>
#include<fstream>
#include <stdlib.h>  

using namespace std;

class image
{
	private:
		int numRows, numCols, minValue, maxValue,thresholdValue;
		int* histAry;
	public:
		image(int numRows, int numCols, int minValue, int maxValue,int thresholdValue)
		{
			this->numRows=numRows;
			this->numCols=numCols;
			this->minValue=minValue;
			this->maxValue=maxValue;
			this->thresholdValue=thresholdValue;
			histAry = new int[maxValue+1]{0};
		}
		
		//STEP 4
		void computeHist(ifstream & inFile)
		{
			int count=0;	
 			for(int i=0;i<numRows*numCols;i++)
 			{
 				int temp;
				inFile>>temp;
				histAry[temp]++;			
			}

		}	

		//STEP 5
		void printHist(ofstream & outFile1)
		{
			for(int i=0;i<maxValue+1;i++)
			{
		
				outFile1<<i<<"  "<<"("<<histAry[i]<<")"<<":";
		
				int count=0;
				for(int j=0;j<histAry[i];j++)
				{
					if(count==60)
						break;
					outFile1<<"+";
					count++;
				}
				outFile1<<"\n";
			}
		}

		//STEP 8
		void thresholdOp(ifstream & inFile, ofstream & outFile2)
		{
			int row=-1,col=-1,min=-1,max=-1,jRow=0,jCol=0;
			inFile>>row;
			inFile>>col;
			inFile>>min;
			inFile>>max;
	
			while(jRow!=numRows)
			{
				while(jCol!=numCols)
				{
					int pixel_val=-1;
					inFile>>pixel_val;
					if(pixel_val>=thresholdValue)
						outFile2<<"1";
					else
						outFile2<<"0";
					outFile2<<" ";
					jCol++;
				}
				jRow++;
				jCol=0;
				outFile2<<"\n";
			}
	
	
		}

		//STEP 9
		void prettyPrint(ifstream & outFile2, ofstream & outFile3)
		{
			int row=-1,col=-1,min=-1,max=-1,iRow=0,jCol=0;
			outFile2>>row;
			outFile2>>col;
			outFile2>>min;
			outFile2>>max;
	
			while(iRow!=numRows)
			{
				while(jCol!=numCols)
				{
					int pixel_val=-1;
					outFile2>>pixel_val;
					if(pixel_val>0)
						outFile3<<pixel_val<<" ";
					else
						outFile3<<"  ";
			
					jCol++;
				}
				iRow++;
				jCol=0;
				outFile3<<"\n";
			}
	
		}
		
		void setnumRows(int numRows)
		{
			this->numRows=numRows;	
		}
		
		void setnumCols(int numCols)
		{
			this->numCols=numCols;	
		}
		
		void setminValue(int minValue)
		{
			this->minValue=minValue;	
		}
		
		void setmaxValue(int maxValue)
		{
			this->maxValue=maxValue;	
		}
		
		void setthresholdValue(int thresholdValue)
		{
			this->thresholdValue=thresholdValue;	
		}
		
		void sethistAry(int* histAry)
		{
			this->histAry=histAry;
		}
		
		int getnumRows()
		{
			return numRows;	
		}
		
		int getnumCols()
		{
			return numCols;	
		}
		
		int getminValue()
		{
			return minValue;	
		}
		
		int getmaxValue()
		{
			return maxValue;	
		}
		
		int getthresholdValue()
		{
			return thresholdValue;	
		}
		
		int* gethistAry()
		{
			return histAry;	
		}
		
		//EXTRA CREDIT
		void printExCredit(ofstream & outFile)
		{
			for(int i=0;i<maxValue+1;i++)
			{
				outFile<<i<<" "<<histAry[i]<<"\n\n";
			}
			
			outFile.close();
		}		
};

int main(int argc, char **argv)
{
	//STEP 0
	ifstream inFile;
	ofstream outFile1, outFile2, outFile3,exCredit;
	inFile.open(argv[1]);
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	outFile3.open(argv[4]);
	exCredit.open("exCredit.txt");
	int thresholdValue = atoi(argv[5]);
	
	
	//STEP 1
	int rows=-1,cols=-1,min=-1,max=-1;
	inFile>>rows;
	inFile>>cols;
	inFile>>min;
	inFile>>max;
	
	//STEP 2
	outFile1<<rows<<" "<<cols<<" "<<min<<" "<<max<<"\n";
	outFile2<<rows<<" "<<cols<<" "<<"0"<<" "<<"1"<<"\n";
	
	//EXTRA CREDIT
	exCredit<<rows<<" "<<cols<<" "<<min<<" "<<max<<"\n\n";
	
	//STEP 3
	image ourImage(rows,cols,min,max,thresholdValue);
	
	//STEP 4
	ourImage.computeHist(inFile);
	
	//STEP 5
	ourImage.printHist(outFile1);
	ourImage.printExCredit(exCredit);
	
	//STEP 6
	inFile.close();
	
 	//STEP 7
 	inFile.open(argv[1]);
 	
 	//STEP 8
 	ourImage.thresholdOp(inFile,outFile2);
 	
 	//STEP 9
 	outFile2.close();
 	
 	//STEP 10
 	ifstream inFile2;
 	inFile2.open(argv[3]);
 	
 	//STEP 11
 	ourImage.prettyPrint(inFile2,outFile3);
 	
 	//STEP 12
 	inFile2.close();
 	outFile3.close();
}

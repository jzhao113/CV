#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

class CCLabel
{
	private:
	//struct property
	//{
		//int label, numpixels,upperLftR,upperLftC,lowerRgtR,lowerRgtC;
	//};
	int numRows, numCols, minVal, maxVal;
	int newMin, newMax;
	int newLabel = 0;
	int trueNumCC;
	int numNb = 5;
	int connectness;
	
	int** zeroFramedAry;
	int nonZeroNeighborAry [5];
	int* EQAry;
	int** CCProperty;
	
	bool diffFlag=false;
	int nonZeroCount=0;
	
	void fillin(int**x,int rows, int cols)
	{
		for(int i=0;i<rows;i++)
		{
			x[i] = new int[cols]{0};
		}
	}
	
	void loadImage(ifstream & iff)
	{
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				iff>>zeroFramedAry[i][j];
			}
		}
	}
	
	int connectPass1(int connectness, int**ary, int nonZeroAry [] )
	{
		newLabel=0;
		
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				if(ary[i][j]>0)
				{
					int minLabel = loadNoneZeroPass1(ary,connectness,i,j,nonZeroAry);
					
					if(minLabel==0)
					{
						newLabel++;
						ary[i][j]=newLabel;
					}
					else if(minLabel>0)
					{
						ary[i][j]=minLabel;
					}
					
					if(diffFlag==true)
						updateEQ(minLabel);
				}
			}
		}
		
		return newLabel;	
	}
	
	void connectPass2(int connectness, int**ary, int nonZeroAry[])
	{
		for(int i=numRows;i>0;i--)
		{
			for(int j=numCols;j>0;j--)
			{
				if(ary[i][j]>0)
				{
					int minLabel = loadNonZeroPass2(ary,connectness,i,j,nonZeroAry);
					
					if(minLabel!=ary[i][j])
						ary[i][j] = minLabel;
					 
					if(diffFlag==true)
						updateEQ(minLabel);
					
				}
			}
		}
	}
	
	void connectPass3()
	{
		newMin = 9999;
		newMax=0;
		
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				zeroFramedAry[i][j] = EQAry[zeroFramedAry[i][j]];
				if(zeroFramedAry[i][j]>0)
				{
					CCProperty[zeroFramedAry[i][j]][1] = CCProperty[zeroFramedAry[i][j]][1]+1;
					
					if(CCProperty[zeroFramedAry[i][j]][2]==-1)
					{
						CCProperty[zeroFramedAry[i][j]][2]=i;
						CCProperty[zeroFramedAry[i][j]][3]=j;
						CCProperty[zeroFramedAry[i][j]][4]=i;
						CCProperty[zeroFramedAry[i][j]][5]=j;
						CCProperty[zeroFramedAry[i][j]][0]=zeroFramedAry[i][j];
					}
					else
					{
						if(i<CCProperty[zeroFramedAry[i][j]][2])
						{
							CCProperty[zeroFramedAry[i][j]][2]=i;
						}
						if(j<CCProperty[zeroFramedAry[i][j]][3])
						{
							CCProperty[zeroFramedAry[i][j]][3]=j;
						}
						if(i>CCProperty[zeroFramedAry[i][j]][4])
						{
							CCProperty[zeroFramedAry[i][j]][4]=i;
						}
						if(j>CCProperty[zeroFramedAry[i][j]][5])
						{
							CCProperty[zeroFramedAry[i][j]][5]=j;
						}
					}
					
					if(zeroFramedAry[i][j]<newMin)
						newMin = zeroFramedAry[i][j];
					if(zeroFramedAry[i][j]>newMax)
						newMax = zeroFramedAry[i][j];
				}
			}
		}
	}	
	void setupCC()
	{
		CCProperty = new int*[EQAry[newLabel+1]];
		fillin(CCProperty,EQAry[newLabel+1],6);
		for(int i=0;i<newLabel+1;i++)
		{
			CCProperty[i][0] = -1;
			CCProperty[i][1] = 0;
			CCProperty[i][2] = -1;
			CCProperty[i][3] = -1;
			CCProperty[i][4] = -1;
			CCProperty[i][5] = -1;
			
		}
	}
	
	void drawBoxes(int count)
	{
		for(int i=1;i<=count;i++)
		{
			int minRow = CCProperty [i][2];
			int minCol = CCProperty[i][3];
			int maxRow = CCProperty [i][4];
			int maxCol = CCProperty[i][5];
			int label = CCProperty[i][0];
			
			int height = maxRow-minRow;
			int width =  maxCol-minCol;
			
			for(int j=0; j<=width;j++)
			{
				zeroFramedAry[minRow][minCol+j] = label;
				zeroFramedAry[height+minRow][minCol+j] = label;	
		 	}

			for(int j=0; j<=height;j++)
			{
				zeroFramedAry[minRow+j][minCol] = label;
				zeroFramedAry[minRow+j][width+minCol] = label;	
			}
		}
	}
	
	int loadNoneZeroPass1(int** ary, int connectness, int i, int j, int nonZeroAry[])
	{
		minus1d(nonZeroAry, numNb);
		nonZeroCount=0;
		
		if(ary[i-1][j]>0)
		{
			nonZeroAry[nonZeroCount] = ary[i-1][j];
			nonZeroCount++;
		}
		if(ary[i][j-1]>0)
		{
			nonZeroAry[nonZeroCount] = ary[i][j-1];
			nonZeroCount++;
		}
		
		if(connectness==8)
		{
			if(ary[i-1][j-1]>0)
			{
				nonZeroAry[nonZeroCount] = ary[i-1][j-1];
				nonZeroCount++;
			}
			if(ary[i-1][j+1]>0)
			{
				nonZeroAry[nonZeroCount] = ary[i-1][j+1];
				nonZeroCount++;
			}
		}
		
		if(nonZeroCount<=0)
			return 0;
		
		int minLabel = nonZeroAry[0];
		diffFlag=false;
		
		int index=1;
		while(index < nonZeroCount)
		{
			if(minLabel!=nonZeroAry[index])
			{
				diffFlag=true;
			}
			if(minLabel>nonZeroAry[index])
			{
				minLabel = nonZeroAry[index];
			}
			index++;
		}
		
		return minLabel;
	}
	
	int loadNonZeroPass2(int** ary, int connectness, int i , int j, int nonZeroAry[])
	{
		minus1d(nonZeroAry, numNb);
		nonZeroCount=0;
		
		nonZeroAry[nonZeroCount] = ary[i][j];
		nonZeroCount++;
		
		if(ary[i+1][j]>0)
		{
			nonZeroAry[nonZeroCount] = ary[i+1][j];
			nonZeroCount++;
		}
		if(ary[i][j+1]>0)
		{
			nonZeroAry[nonZeroCount] = ary[i][j+1];
			nonZeroCount++;
		}
		
		if(connectness==8)
		{
			if(ary[i+1][j-1]>0)
			{
				nonZeroAry[nonZeroCount] = ary[i+1][j-1];
				nonZeroCount++;
			}
			if(ary[i+1][j+1]>0)
			{
				nonZeroAry[nonZeroCount] = ary[i+1][j+1];
				nonZeroCount++;
			}
		}
		
		int minLabel = nonZeroAry[0];
		diffFlag=false;
		
		int index=1;
		while(index < nonZeroCount)
		{
			if(minLabel!=nonZeroAry[index])
			{
				diffFlag=true;
			}
			if(minLabel>nonZeroAry[index])
			{
				minLabel = nonZeroAry[index];
			}
			index++;
		}
		return minLabel;
		
	}
	
	void minus1d(int x[], int size)
	{
		for(int i=0;i<size;i++)
		{
			x[i]=-1;
		}
	}
	
	void updateEQ(int minLabel)
	{
		int index =0;
		while(index<nonZeroCount && nonZeroNeighborAry[index]!=-1)
		{
			EQAry[nonZeroNeighborAry[index]] = minLabel;
			index++;
		}
	}
	
	int manageEQAry()
	{
		int realLabel=0;
		int index=1;
		while(index <= newLabel)
		{
			if(index != EQAry[index])
				EQAry[index] = EQAry[EQAry[index]];
			else
			{
				realLabel++;
				EQAry[index]=realLabel;
			}
			index++;
		}
		
		return realLabel;
	}
	
	void prettyPrint(string caption, ofstream & outFile)
	{
		outFile<<caption<<endl;
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				if(zeroFramedAry[i][j]>0)
				{
					outFile<<zeroFramedAry[i][j];
					if(zeroFramedAry[i][j]<10)
						outFile<<" ";
				}
				else
					outFile<<"  ";
			}
			outFile<<endl;
		}
		outFile<<endl;
	}
	
	void printEQAry(int newLabel, ofstream & of)
	{
		of<<"Label: "<<newLabel<<endl;
		for(int i=0;i<=newLabel;i++)
		{
			of<<"Index "<<i<<": "<<EQAry[i]<<endl;
		}
		
		of<<endl;
	}
	void printImg(ofstream & outFile)
	{
		outFile<<numRows<<" "<<numCols<<" "<<0<<" "<<trueNumCC<<endl;
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				outFile<<zeroFramedAry[i][j]<<" ";
			}
			outFile<<endl;
		}
		outFile<<endl;
	}
	void printCC(ofstream & outFile)
	{
		outFile<<numRows<<" "<<numCols<<" "<<newMin<<" "<<newMax<<endl;
		outFile<<trueNumCC<<endl<<endl;
		
		for(int i=1;i<=trueNumCC;i++)
		{
			outFile<<CCProperty[i][0]<<endl;
			outFile<<CCProperty[i][1]<<endl;
			outFile<<CCProperty[i][2]<<" "<<CCProperty[i][3]<<endl;
			outFile<<CCProperty[i][4]<<" "<<CCProperty[i][5]<<endl<<endl;	
		}
	}
		
	public:
	CCLabel(string inputFile, int connectness,string pretty, string label, string property)
	{
		this->connectness=connectness;
		ifstream input;
		input.open(inputFile.c_str());
		input>>numRows;
		input>>numCols;
		input>>minVal;
		input>>maxVal;
		
		ofstream out3;
		out3.open(pretty.c_str());
		ofstream out4;
		out4.open(label.c_str());
		ofstream out5;
		out5.open(property.c_str());
		
		zeroFramedAry = new int*[numRows+2];
		fillin(zeroFramedAry,numRows+2,numCols+2);
		
		EQAry = new int[(numRows*numCols)/2];
		
		for(int i=0;i<(numRows*numCols)/2;i++)
		{
			EQAry[i] = i;
		}
		
		loadImage(input);
		input.close();
		
		
		newLabel = connectPass1(connectness, zeroFramedAry, nonZeroNeighborAry);
		prettyPrint("connectPass1",out3);
		printEQAry(newLabel,out3);
		
		connectPass2(connectness,zeroFramedAry, nonZeroNeighborAry);
		prettyPrint("connectPass2",out3);
		printEQAry(newLabel,out3);
		
		trueNumCC = manageEQAry();
		out3<<"EQAry after manageEQAry"<<endl;
		printEQAry(newLabel,out3);
		setupCC();
		
		connectPass3();
		prettyPrint("connectPass3",out3);
		printEQAry(newLabel,out3);
		
		printImg(out4);
		fillin(zeroFramedAry,numRows+2,numCols+2);

		drawBoxes(trueNumCC);
		
		printCC(out5);
		prettyPrint("boxes",out3);
		
		out3.close();
		out4.close();
		out5.close();

	}	
};

int main(int argc, char** argv)
{
	CCLabel x(argv[1],atoi(argv[2]),argv[3],argv[4],argv[5]);
}

#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

class imageProcessing
{
	private:
		int numRows,numCols,minVal,maxVal;
		int maskRows, maskCols, maskMin, maskMax;
		int totalWeight=0;
		int** mirrorFramedAry;
		int** avgAry;
		int** medianAry;
		int** GaussAry;
		int** maskAry;
		int* neighbors = new int[9];
		int* min = new int[3];
		int* max = new int[3];
		
		void sort3x3()
		{
			sort(neighbors,neighbors+9);	
		}
		
		void loadNeighbors(int i, int j)
		{
			neighbors[0] = mirrorFramedAry[i-1][j-1];
			neighbors[1] =  mirrorFramedAry[i-1][j];
			neighbors[2] =  mirrorFramedAry[i-1][j+1];
			neighbors[3] =  mirrorFramedAry[i][j-1];
			neighbors[4] =  mirrorFramedAry[i][j];
			neighbors[5] =  mirrorFramedAry[i][j+1];
			neighbors[6] =  mirrorFramedAry[i+1][j-1];
			neighbors[7] =  mirrorFramedAry[i+1][j];
			neighbors[8] =  mirrorFramedAry[i+1][j+1];
		}
		
		int avg3x3(int i, int j)
		{
			return (mirrorFramedAry[i-1][j-1]+
				  mirrorFramedAry[i-1][j]+
				  mirrorFramedAry[i-1][j+1]+
				  mirrorFramedAry[i][j-1]+
				  mirrorFramedAry[i][j]+
				  mirrorFramedAry[i][j+1]+
				  mirrorFramedAry[i+1][j-1]+
				  mirrorFramedAry[i+1][j]+
				  mirrorFramedAry[i+1][j+1])/9;
		}
		
		void fillin(int**x,int rows, int cols)
		{
			for(int i=0;i<rows;i++)
			{
				x[i] = new int[cols]{0};
			}
		}
		
		void loadMask(ifstream & inFile)
		{
			for(int i=0;i<maskRows;i++)
			{
				for(int j=0;j<maskCols;j++)
				{
					inFile>>maskAry[i][j];
					totalWeight+=maskAry[i][j];
				}
			}
		}
		
		void loadImage(ifstream & inFile)
		{
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					inFile>>mirrorFramedAry[i][j];
				}
			}
			
		}
		
		void mirrorFraming()
		{
			for(int i=0;i<numRows+2;i++)
			{
				mirrorFramedAry[i][0] =	mirrorFramedAry[i][1];
				mirrorFramedAry[i][numCols+1] = mirrorFramedAry[i][numCols];
			}
			
			for(int i=0;i<numCols+2;i++)
			{
				mirrorFramedAry[0][i] =	mirrorFramedAry[1][i];
				mirrorFramedAry[numRows+1][i] = mirrorFramedAry[numRows][i];
			}
		}
		
				
		int covolution(int i, int j)
		{
			return ((mirrorFramedAry[i-1][j-1]*maskAry[0][0])+
				  (mirrorFramedAry[i-1][j]*maskAry[0][1])+
				  (mirrorFramedAry[i-1][j+1]*maskAry[0][2])+
				   (mirrorFramedAry[i][j-1]*maskAry[1][0])+
				     (mirrorFramedAry[i][j]*maskAry[1][1])+
				   (mirrorFramedAry[i][j+1]*maskAry[1][2])+
				   (mirrorFramedAry[i+1][j-1]*maskAry[2][0])+
				   (mirrorFramedAry[i+1][j]*maskAry[2][1])+
				   (mirrorFramedAry[i+1][j+1]*maskAry[2][2]))/totalWeight;
		}
		
	public:
		imageProcessing(string file1, string file2)
		{
			ifstream inFile1;
			inFile1.open(file1.c_str());
			inFile1>>numRows;
			inFile1>>numCols;
			inFile1>>minVal;
			inFile1>>maxVal;
			
			ifstream inFile2;
			inFile2.open(file2.c_str());
			inFile2>>maskRows;
			inFile2>>maskCols;
			inFile2>>maskMin;
			inFile2>>maskMax;
			
			mirrorFramedAry = new int*[numRows+2];
			fillin(mirrorFramedAry, numRows+2, numCols+2);
			
			avgAry = new int*[numRows+2];
			fillin(avgAry, numRows+2, numCols+2);
			
			medianAry = new int*[numRows+2];
			fillin(medianAry, numRows+2, numCols+2);
			
			GaussAry = new int*[numRows+2];
			fillin(GaussAry, numRows+2, numCols+2);
			
			maskAry = new int*[maskRows];
			fillin(maskAry, maskRows, maskCols);
			
			loadImage(inFile1);
			loadMask(inFile2);
			mirrorFraming();
			
			inFile1.close();
			inFile2.close();
			
		}
		
		void ComputeAvgImg()
		{
			int newMin=9999;
			int newMax=0;
			int count=0;
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					avgAry[i][j] = avg3x3(i,j);

					if(newMin > avgAry[i][j])
						newMin = avgAry[i][j];
					if(newMax < avgAry[i][j])
						newMax = avgAry[i][j];
				}
				
			}
			
			min[0]=newMin;
			max[0]=newMax;
		}
		
		void ComputeMedianImg()
		{
			int newMin = 9999;
			int newMax = 0;
			
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					loadNeighbors(i,j);
					sort3x3();
					medianAry[i][j] = neighbors[4];
					if(newMin > medianAry[i][j])
						newMin = medianAry[i][j];
					if(newMax < medianAry[i][j])
						newMax = medianAry[i][j];
				}
			}
			
			min[1]=newMin;
			max[1]=newMax;
		}
		
		void ComputeGaussImg()
		{
			int newMin = 9999;
			int newMax = 0;
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					GaussAry[i][j] = covolution(i,j);
					if(newMin>GaussAry[i][j])
						newMin = GaussAry[i][j];
					if(newMax < GaussAry[i][j])
						newMax = GaussAry[i][j];
				}
			}
			
			min[2]=newMin;
			max[2]=newMax;
		}
		
		void print(ofstream & outfile, int option)
		{
			int** temp;
			int x,y,z,oMin,oMax;
			if(option==0)
			{
				temp=avgAry;
				oMin=min[0];
				oMax=max[0];
			}
			else if(option==1)
			{
				temp=medianAry;
				oMin=min[1];
				oMax=max[1];
			}
			else
			{
				temp=GaussAry;
				oMin=min[2];
				oMax=max[2];
			}
			
			outfile<<numRows<<" "<<numCols<<" "<<oMin<<" "<<oMax<<"\n";
			
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					outfile<<temp[i][j]<<" ";
				}
				outfile<<"\n";
			}
			
			outfile.close();
				
		}
		
		
};

int main(int argc, char **argv)
{
	imageProcessing ip (argv[1],argv[2]);
	
	ip.ComputeAvgImg();
	ip.ComputeMedianImg();
	ip.ComputeGaussImg();
	
	ofstream outfile1;
	outfile1.open(argv[3]);
	
	ofstream outfile2;
	outfile2.open(argv[4]);
	
	ofstream outfile3;
	outfile3.open(argv[5]);
	
	ip.print(outfile1,0);
	ip.print(outfile2,1);
	ip.print(outfile3,2);
}

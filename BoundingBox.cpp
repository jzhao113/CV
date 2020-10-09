#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

class boxNode
{
	public:
		int boxType;
		boxNode* next;
		int minR, minC, maxR, maxC;
		
		boxNode(int minR, int minC, int maxR, int maxC, int boxType)
		{
			this->boxType = boxType;
			this->minR = minR;
			this->minC = minC;
			this->maxR = maxR;
			this->maxC = maxC;
			
			next = NULL;	
		}	
};

class boxQ
{
	public:
		boxNode* Qfront;
		boxNode* Qback;
		bool flag;
		
		boxQ()
		{
		}
		
		void insert(boxNode * inserting)
		{
			if(flag)
			{
				flag=false;
				Qfront = inserting;
				Qback = inserting;
			}
			else
			{
				Qback->next =  inserting;
				Qback = inserting;
			}
		}
	
	
};

class imagePP
{
	private:
		int numRows, numCols, minVal, maxVal;
		int thrVal;
		int HPPruns,VPPruns;
		int dir=-1;
		
		int** imgAry;
		
		int* HPP;
		int* VPP;
		int* HPPbin;
		int* VPPbin;
		
		int* HPPMorph;
		int* VPPMorph;
		int* middleAryH;
		int* middleAryV;
		
		boxQ bq;
		
		void loadImage(ifstream & input)
		{
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					input>>imgAry[i][j];
				}
			}
		}
		
		void fillin(int**x,int rows, int cols)
		{
			for(int i=0;i<rows;i++)
			{
				x[i] = new int[cols]{0};
			}
		}
		
		void setHPP()
		{
			int counter=0;
			for(int i=0;i<numRows+2;i++)
			{
				for(int j=0;j<numCols+2;j++)
				{
					if(imgAry[i][j]>0)
						counter++;
				}
				HPP[i] = counter;
				counter=0;
			}
		}
		
		void setVPP()
		{
			int counter=0;
			for(int j=0;j<numCols+2;j++)
			{
				for(int i=0;i<numRows+2;i++)
				{
					if(imgAry[i][j]>0)
						counter++;
				}
				VPP[j] = counter;
				counter=0;
			}
		}
		
		void biIt(int* input, int* output, int size)
		{
			for(int i=0;i<size;i++)
			{
				if(input[i] >= thrVal)
					output[i]=1;
			}
		}
		
		void erosion(int *fromAry,int *toAry, int x)
		{
			//only works with 111 at position 1
			if(fromAry[x-1] == 1 && fromAry[x+1]==1)
				toAry[x] = 1;
		}
		
		void dilation(int *fromAry,int *toAry, int x)
		{
			//only works with 111 at position 1
			toAry[x+1] = 1;
			toAry[x-1] = 1;
		}
		
		void ComputeDilation(int *fromAry,int *toAry, int size)
		{
			for(int i=1;i<size-1;i++)
			{
				if(fromAry[i]>0)
					dilation(fromAry,toAry, i);
			}
		}
		
		void ComputeErosion(int *fromAry,int *toAry, int size)
		{
			for(int i=1;i<size-1;i++)
			{
				if(fromAry[i]>0)
					erosion(fromAry,toAry, i);
			}
		}
		
		void ComputeClosing(int* input, int* morph, int* middle, int size)
		{
			ComputeDilation(input,middle,size);	
			ComputeErosion(middle,morph,size);
		}
		
		void countRuns(int* input, int & output, int size)
		{
			bool flag = false;
			int counter=0;
			for(int i=0;i<size;i++)
			{
				if(input[i]>0 && flag==false)
				{
					flag=true;
					counter++;
				}
				else if(input[i] == 0)
				{
					flag=false;
				}
			}
			
			output=counter;
		}
		
		int direction()
		{
			int factorHPP = HPPruns * 3;
			int factorVPP = VPPruns * 3;
			
			if(HPPruns >= factorVPP)
				return 0;
			else if(VPPruns >= factorHPP)
				return 1;
			
			return -1;
		}
		
		void HPPdom (boxQ & bq)
		{
			bool flag =false;
			int x1,x2,y1,y2;
			
			for(int i=0;i<numRows+2;i++)
			{
				if(HPPMorph[i]>0 && flag==false)
				{
					flag=true;
					x1 = i;
				}
				else if(HPPMorph[i] == 0 && flag==true)
				{
					flag=false;
					x2 = i-1;
					
					bool flag2 = false;
					for(int j=0;j<numCols+2;j++)
					{
						if(VPPMorph[j]>0 && flag2==false)
						{
							flag2=true;
							y1 = j;
						}
						else if(VPPMorph[j] == 0 && flag2==true)
						{
							flag2=false;
							y2 = j-1;
							
							boxNode* temp = new boxNode(x1,y1,x2,y2,3);
							bq.insert(temp);		
						}		
					}
				}
			}
		}
		
		void VPPdom(boxQ & bq)
		{
			bool flag =false;
			int x1,x2,y1,y2;
			
			for(int i=0;i<numCols+2;i++)
			{
				if(VPPMorph[i]>0 && flag==false)
				{
					flag=true;
					y1 = i;
				}
				else if(VPPMorph[i] == 0 && flag==true)
				{
					flag=false;
					y2 = i-1;
					
					bool flag2 = false;
					for(int j=0;j<numRows+2;j++)
					{
						if(HPPMorph[j]>0 && flag2==false)
						{
							flag2=true;
							x1 = j;
						}
						else if(HPPMorph[j] == 0 && flag2==true)
						{
							flag2=false;
							x2 = j-1;
							
							boxNode* temp = new boxNode(x1,y1,x2,y2,3);
							bq.insert(temp);		
						}		
					}
				}
			}
		}
		
		void drawBoxes(boxQ & bq)
		{
			boxNode* currentPointer = bq.Qfront;
			while(currentPointer!=NULL)
			{
				int minRow = currentPointer->minR;
				int minCol = currentPointer->minC;
				int maxRow = currentPointer->maxR;
				int maxCol = currentPointer->maxC;
				
				int height = maxRow-minRow;
				int width =  maxCol-minCol;
			
				for(int j=0; j<=width;j++)
				{
					imgAry[minRow][minCol+j] = currentPointer->boxType;
					imgAry[height+minRow][minCol+j] = currentPointer->boxType;	
		 		}

				for(int j=0; j<=height;j++)
				{
					imgAry[minRow+j][minCol] = currentPointer->boxType;
					imgAry[minRow+j][width+minCol] = currentPointer->boxType;	
				}
				
				currentPointer = currentPointer->next;
			}
		}
		
		void resetMaxMin()
		{
			minVal=9999;
			maxVal=0;
			for(int i=0;i<numRows+2;i++)
			{
				for(int j=0;j<numCols+2;j++)
				{
					if(imgAry[i][j]>maxVal)
						maxVal=imgAry[i][j];
					if(imgAry[i][j]<minVal && imgAry[i][j]!=0)
						minVal = imgAry[i][j];
				}
			}
		}
	
	public:
		imagePP(string input, int thrVal)
		{
			ifstream inputFile;
			inputFile.open(input.c_str());
			inputFile>>numRows;
			inputFile>>numCols;
			inputFile>>minVal;
			inputFile>>maxVal;
			
			this->thrVal = thrVal;
			
			//setting up image array
			imgAry = new int*[numRows+2];
			fillin(imgAry,numRows+2,numCols+2);
			loadImage(inputFile);
			
			//setting up HPP and VPP
			HPP = new int[numRows+2]{0};
			VPP = new int[numCols+2]{0};
			HPPbin = new int[numRows+2]{0};
			VPPbin =  new int[numCols+2]{0};
			
			//doing erosion, these arrays take into account the zeroframed
			middleAryH = new int[numRows+2]{0};
			middleAryV = new int[numCols+2]{0};
					
		}
		
		void detection()
		{
			setHPP();
			setVPP();
			biIt(HPP,HPPbin,numRows+2);
			biIt(VPP,VPPbin,numCols+2);
			
			HPPMorph = new int[numRows+2]{0};
			VPPMorph = new int[numCols+2]{0};
			
			ComputeClosing(HPPbin,HPPMorph,middleAryH,numRows+2);
			ComputeClosing(VPPbin,VPPMorph,middleAryV,numCols+2);
			
			countRuns(HPPMorph,HPPruns,numRows+2);
			countRuns(VPPMorph,VPPruns,numCols+2);
			
			dir = direction();
			
			if(dir==0)
				HPPdom(bq);
			else if(dir==1)
				VPPdom(bq);
			else
			{
				cout<<"No Direction"<<endl;
				exit (0);
			}
			
			drawBoxes(bq);			
		}
		
		void printInformation(ofstream & out)
		{
			out<<"HPP"<<endl;
			for(int i=0;i<numRows+2;i++)
			{
				out<<i<<" "<<HPP[i]<<endl;
			}
			out<<endl;
			
			out<<"VPP"<<endl;
			for(int i=0;i<numCols+2;i++)
			{
				out<<i<<" "<<VPP[i]<<endl;
			}
			out<<endl;
			
			out<<"HPPBinary"<<endl;
			for(int i=0;i<numRows+2;i++)
			{
				out<<i<<" "<<HPPbin[i]<<endl;
			}
			out<<endl;
			
			out<<"VPPBinary"<<endl;
			for(int i=0;i<numCols+2;i++)
			{
				out<<i<<" "<<VPPbin[i]<<endl;
			}
			out<<endl;
			
			out<<"Direction"<<endl;
			if(dir==0)
				out<<"Horizontal"<<endl;
			else
				out<<"Vertical"<<endl;
				
			out<<endl;
			
			out<<"Queue"<<endl;
			
			boxNode* currentPointer = bq.Qfront;
			while(currentPointer!=NULL)
			{
				out<<currentPointer->boxType<<endl;
				out<<currentPointer->minR<<" "<<currentPointer->minC<<" ";
				out<<currentPointer->maxR<<" "<<currentPointer->maxC<<endl;
				currentPointer = currentPointer->next;
			}
			
			
		}
		
		void printTextLine(ofstream & out)
		{	
			resetMaxMin();
			out<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal;
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					if(imgAry[i][j]>0)
						out<<imgAry[i][j]<<" ";
					else
						out<<"  ";
				}
				out<<endl;
			}
		}
};

int main(int argc, char** argv)
{
	imagePP ig (argv[1],atoi(argv[2]));
	
	ofstream out1;
	ofstream out2;
	string temp = argv[3];
	out1.open(temp.c_str());
	temp = argv[4];
	out2.open(temp.c_str());
	
	ig.detection();
	ig.printInformation(out2);
	ig.printTextLine(out1);
	
	out1.close();
	out2.close();	
}

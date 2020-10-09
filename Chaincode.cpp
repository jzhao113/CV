#include<iostream>
#include<fstream>

using namespace std;

class connectCC
{
	public:
		int label,numPixels;
		int boundbox [4];
		
		connectCC()
		{
		}
		connectCC(int label, int numPixels, int sX, int sY, int eX, int eY)
		{
			this->label = label;
			this->numPixels = numPixels;
			boundbox[0] = sX;
			boundbox[1] = sY;
			boundbox[2] = eX;
			boundbox[3] = eY;
		}
};

class image
{
	private:
		
		void fillin(int**x,int rows, int cols)
		{
			for(int i=0;i<rows;i++)
			{
				x[i] = new int[cols]{0};
			}
		}
		
		void loadImage(ifstream & input)
		{
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					input>>imageAry[i][j];
				}
			}
			
		}
		
	public:
		int numRows, numCols, minVal, maxVal;
		int** imageAry;
		int** CCAry;
		
		image()
		{
		}
		image(ifstream & input, ifstream & input2)
		{
			
			input>>numRows;
			input>>numCols;
			input>>minVal;
			input>>maxVal;
			
			input2>>numRows;
			input2>>numCols;
			input2>>minVal;
			input2>>maxVal;
			
			imageAry = new int*[numRows+2];
			fillin(imageAry,numRows+2,numCols+2);
			
			CCAry = new int*[numRows+2];
			fillin(CCAry,numRows+2,numCols+2);
			
			loadImage(input);
		}
				
		void clearCC()
		{
			for(int i=0;i<numRows+2;i++)
			{
				for(int j=0;j<numCols+2;j++)
				{
					CCAry[i][j]=0;
				}
			}
		}
};

class point
{
	public:
		int x, y;
		
		point()
		{
		}
		point(int x, int y)
		{
			this->x=x;
			this->y=y;
		}	
};

class chainCode
{
	public:
		connectCC CCC;
		image ig;
		
		point startP,currentP,nextP;
		int lastQ, nextQ,PchainDir,label;
		point neighbors[8];
		int zeroTable[8] = {6,0,0,2,2,4,4,6};
		
		chainCode(connectCC CCC,image ig)
		{
			this->CCC = CCC;
			this->ig = ig;	
		}
		
		void loadCC()
		{
			for(int i=CCC.boundbox[0]+1;i<=CCC.boundbox[2]+1;i++)
			{
				for(int j=CCC.boundbox[1]+1;j<=CCC.boundbox[3]+1;j++)
				{
					if(ig.imageAry[i][j]!=0)
					{
						ig.CCAry[i][j] = ig.imageAry[i][j];
					}
				}
			}
		}
		
		void loadNeighbors()
		{
			point x1 (currentP.x-1,currentP.y-1);
			point x2 (currentP.x-1,currentP.y);
			point x3 (currentP.x-1,currentP.y+1);
			point x4 (currentP.x,currentP.y-1);
			point x5 (currentP.x,currentP.y+1);
			point x6 (currentP.x+1,currentP.y-1);
			point x7 (currentP.x+1,currentP.y);
			point x8 (currentP.x+1,currentP.y+1);
			
			neighbors[0] = x5;
			neighbors[1] = x3;
			neighbors[2] = x2;
			neighbors[3] = x1;
			neighbors[4] = x4;
			neighbors[5] = x6;
			neighbors[6] = x7;
			neighbors[7] = x8;
		}
		
		int findNextP()
		{
			int output=-1;
			loadNeighbors();
			
			int direction = lastQ%8;
			
			for(int i=0;i<8;i++)
			{ 
				if(ig.CCAry[neighbors[direction].x][neighbors[direction].y] == label)
				{
					output=direction;
					point temp(neighbors[direction].x,neighbors[direction].y);
					nextP = temp;
					break;
				}
				
				direction = (++direction)%8;
			}
			
			return output;
		}
		
		void getChainCode(ofstream & out1, ofstream & out2)
		{
			label = CCC.label;
			bool flag =false;
			int counter=0;
			for(int i=CCC.boundbox[0]+1;i<=CCC.boundbox[2]+1;i++)
			{
				for(int j=CCC.boundbox[1]+1;j<=CCC.boundbox[3]+1;j++)
				{
					if(ig.CCAry[i][j]==label)
					{
						startP.x=i;
						startP.y=j;
						currentP.x=i;
						currentP.y=j;
						lastQ=4;
						flag=true;
						break;
					}
				}
				if(flag)
					break;
			}
			
			out1<<label<<" "<<startP.x<<" "<<startP.y<<" ";
			out2<<label<<" "<<startP.x<<" "<<startP.y<<" ";
			do
			{
				counter++;
				nextQ = (lastQ+1)%8;
				PchainDir = findNextP();
				out1<<PchainDir;
				if(counter%20==1)
					out2<<endl<<PchainDir<<" ";
				else
					out2<<PchainDir<<" ";
				lastQ = zeroTable[(PchainDir+7)%8];
				currentP = nextP;

			}while(currentP.x != startP.x || currentP.y != startP.y);
			
			out1<<endl<<"###"<<endl;
			out2<<endl<<"###"<<endl;
		}	
};

int main(int argc, char** argv)
{
	//creating streams
	ifstream input,input2;
	string temp1 = argv[1], temp2=argv[2];
	input.open(temp1.c_str());
	input2.open(temp2.c_str());
	
	ofstream out1,out2;
	string temp3 = argv[3];
	string temp4 = argv[3];
	temp3 = temp3+"ChainCodeFile.txt";
	temp4 = temp4+"debugFile.txt";
	out1.open(temp3.c_str());
	out2.open(temp4.c_str());
		
	//creating image,prepping input and input2
	image ig(input,input2);
	connectCC CCC (0,0,0,0,0,0);
	chainCode chain(CCC,ig);
	
	int totalCC;
	input2>>totalCC;
	
	out1<<ig.numRows<<" "<<ig.numCols<<" "<<ig.minVal<<" "<<ig.maxVal<<endl;
	out2<<ig.numRows<<" "<<ig.numCols<<" "<<ig.minVal<<" "<<ig.maxVal<<endl;
	
	for(int i=0;i<totalCC;i++)
	{
		
		input2>>CCC.label;
		input2>>CCC.numPixels;
		input2>>CCC.boundbox[0];
		input2>>CCC.boundbox[1];
		input2>>CCC.boundbox[2];
		input2>>CCC.boundbox[3];
		
		chain.CCC = CCC;
		chain.ig = ig;
		
		chain.ig.clearCC();
		chain.loadCC();
		chain.getChainCode(out1,out2);
	}	
}

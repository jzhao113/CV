#include <iostream>
#include <fstream>
using namespace std;

class image
{
	private:
		int numImgRows,numImgCols,imgMin,imgMax;
		int numStructRows,numStructCols,structMin,structMax,rowOrigin,colOrigin;
		int rowFrameSize,colFrameSize,extraRows,extraCols;
		int** zeroFramedAry;
		int** morphAry;
		int** structAry;
		int** middleAry;
		
		
		int north, south, start;
		
		void fillin(int**x,int rows, int cols)
		{
			for(int i=0;i<rows;i++)
			{
				x[i] = new int[cols]{0};
			}
		}
		
		void loadImage(ifstream & input,int rows, int cols)
		{
			int rO = rowOrigin, cO = colOrigin;
			for(int i=0;i<rows;i++)
			{
				for(int j=0;j<cols;j++)
				{
					input>>zeroFramedAry[rO][cO];
					cO++;
				}
				rO++;
				cO=colOrigin;
			}
		}
		
		void loadStruct(ifstream & input, int rows, int cols)
		{
			for(int i=0;i<rows;i++)
			{
				for(int j=0;j<cols;j++)
				{
					input>>structAry[i][j];
				}
			}
			
			input.close();
		}
		
	public:
		image(string igFile,string structFile)
		{
			ifstream imageFile;
			imageFile.open(igFile.c_str());
			ifstream sFile;
			sFile.open(structFile.c_str());
			
			if(!sFile.is_open() || !imageFile.is_open())
			{
				cout<<"FILE ERROR, OBJECT NOT CREATED CORRECT"<<endl;
			}
			else
			{
				imageFile>>numImgRows;
				imageFile>>numImgCols;
				imageFile>>imgMin;
				imageFile>>imgMax;
			
				sFile>>numStructRows;
				sFile>>numStructCols;
				sFile>>structMin;
				sFile>>structMax;
				sFile>>rowOrigin;
				sFile>>colOrigin;
			
				rowFrameSize = numStructRows/2;
				colFrameSize = numStructCols/2;
			
				extraRows = rowFrameSize * 2;
				extraCols = colFrameSize * 2;
			
				zeroFramedAry = new int*[numImgRows+extraRows];
				fillin(zeroFramedAry,numImgRows+extraRows,numImgCols+extraCols);
			
				morphAry = new int*[numImgRows+extraRows];
				fillin(morphAry,numImgRows+extraRows,numImgCols+extraCols);
			
				middleAry = new int*[numImgRows+extraRows];
				fillin(middleAry,numImgRows+extraRows,numImgCols+extraCols);
			
				structAry = new int*[numStructRows];
				fillin(structAry,numStructRows,numStructCols);
			
				north = rowOrigin;
				south = numStructRows-north;
				if(south<=0) 
					south=1;
				start = colOrigin;
				loadImage(imageFile,numImgRows,numImgCols);
				loadStruct(sFile,numStructRows,numStructCols);	
			}
		}
		
		
		void reset()
		{
			fillin(morphAry,numImgRows+extraRows,numImgCols+extraCols);
			fillin(middleAry,numImgRows+extraRows,numImgCols+extraCols);
		}
		
		void prettyPrintStruct(ofstream & output)
		{
			output<<numStructRows<<" "<<numStructCols<<" "<<structMin<<" "<<structMax<<" "<<"\n";
			for(int i=0;i<numStructRows;i++)
			{
				for(int j=0;j<numStructCols;j++)
				{
					if(structAry[i][j]==0)
							output<<"."<<" ";
					else
						output<<structAry[i][j]<<" ";
				}
				output<<endl;
			}
			output<<endl;
		}
		
		void prettyPrintZero(ofstream & output)
		{
			output<<numImgRows<<" "<<numImgCols<<" "<<imgMin<<" "<<imgMax<<"\n";
			for(int i=rowOrigin;i<numImgRows+rowOrigin;i++)
			{
				for(int j=colOrigin;j<numImgCols+colOrigin;j++)
				{
					if(zeroFramedAry[i][j]==0)
						output<<"."<<" ";
					else
						output<<zeroFramedAry[i][j]<<" ";
				}
				
				output<<endl;
			}
			output<<endl;
		}
		
		void prettyPrintMorph(ofstream & output)
		{
			output<<numImgRows<<" "<<numImgCols<<" "<<imgMin<<" "<<imgMax<<"\n";
			for(int i=rowOrigin;i<numImgRows+rowOrigin;i++)
			{
				for(int j=colOrigin;j<numImgCols+colOrigin;j++)
				{
					if(morphAry[i][j]==0)
						output<<"."<<" ";
					else
						output<<morphAry[i][j]<<" ";
				}
				output<<endl<<endl;
			}
			output<<endl;
		}
		
		void aryPrint(string file)
		{
			ofstream output;
			output.open(file.c_str());
			if(!output.is_open())
			{
				cout<<"FILE ERROR"<<endl;
			}
			else
			{
				output<<numImgRows<<" "<<numImgCols<<" "<<imgMin<<" "<<imgMax<<"\n";
			
				for(int i=rowOrigin;i<numImgRows+rowOrigin;i++)
				{
					for(int j=colOrigin;j<numImgCols+colOrigin;j++)
					{
						output<<morphAry[i][j]<<" ";
					}
					output<<endl<<endl;
				}
			
				output.close();	
			}	
		}
		
		void ComputeDilation()
		{
			int counter=0;
			for(int i=rowOrigin;i<numImgRows+rowOrigin;i++)
			{
				for(int j=colOrigin;j<numImgCols+colOrigin;j++)
				{
					if(zeroFramedAry[i][j]>0)
					{
						dilation(zeroFramedAry,morphAry, i,j);
					}
				}
			}
		}
				
		void ComputeDilation(int **fromAry,int **toAry)
		{
			int counter=0;
			for(int i=rowOrigin;i<numImgRows+rowOrigin;i++)
			{
				for(int j=colOrigin;j<numImgCols+colOrigin;j++)
				{
					if(fromAry[i][j]>0)
					{
						dilation(fromAry,toAry, i,j);
					}
				}
			}
		}
		
		void ComputeErosion()
		{
			for(int i=rowOrigin;i<numImgRows+rowOrigin;i++)
			{
				for(int j=colOrigin;j<numImgCols+colOrigin;j++)
				{
					if(zeroFramedAry[i][j]>0)
					{
						erosion(zeroFramedAry,morphAry, i,j);
					}
				}
			}
		}

		void ComputeErosion(int **fromAry,int **toAry)
		{
			for(int i=rowOrigin;i<numImgRows+rowOrigin;i++)
			{
				for(int j=colOrigin;j<numImgCols+colOrigin;j++)
				{
					if(fromAry[i][j]>0)
					{
						erosion(fromAry,toAry, i,j);
					}
				}
			}
		}
		
		void erosion(int **fromAry,int **toAry, int x, int y)
		{
			int helper = 0,count=0;
			//north
			for(int i=north;i>0;i--)
			{
				for(int j=0;j<numStructCols;j++)
				{
					if(structAry[helper][j]==0 || fromAry[x-i][y-start+j]==structAry[helper][j])
						count++;
					
				}
				helper++;
			}
			//south
			for(int i=0;i<south;i++)
			{
				for(int j=0;j<numStructCols;j++)
				{
					if(structAry[helper][j]==0 ||fromAry[x+i][y-start+j] == structAry[helper][j])
						count++;
				}
				helper++;
			}

			if(count==numStructCols*numStructRows)
				toAry[x][y] = fromAry[x][y];
		}
		
		void dilation(int **fromAry,int **toAry, int x, int y)
		{
			int helper = 0;
			//north
			for(int i=north;i>0;i--)
			{
				for(int j=0;j<numStructCols;j++)
				{
					if(toAry[x-i][y-start+j]!=1)
						toAry[x-i][y-start+j] = structAry[helper][j];
				}
				helper++;
			}
			//south
			for(int i=0;i<south;i++)
			{
				for(int j=0;j<numStructCols;j++)
				{
					if(toAry[x+i][y-start+j]!=1)
						toAry[x+i][y-start+j] = structAry[helper][j];
				}
				helper++;
			}
		}
		
		void ComputeOpening()
		{
			ComputeErosion(zeroFramedAry,middleAry);
			ComputeDilation(middleAry,morphAry);
		}
		
		void ComputeClosing()
		{
			ComputeDilation(zeroFramedAry,middleAry);
			ComputeErosion(middleAry,morphAry);
		}
};

int main(int argc, char** argv)
{
	image ig (argv[1],argv[2]);
	
	//starting prettyPrint
	ofstream of;
	string x = argv[7];
	of.open(x.c_str());
	
	if(!of.is_open())
	{
		cout<<"FILE ERROR"<<endl;
	}
	else
	{
		of<<"zeroFramedAry"<<endl<<endl;
		ig.prettyPrintZero(of);
		of<<"structAry"<<endl<<endl;
		ig.prettyPrintStruct(of);
	
		//dilation	
		ig.ComputeDilation();
		ig.aryPrint(argv[3]);
		of<<"morphAry for dilation"<<endl;
		ig.prettyPrintMorph(of);

		ig.reset();
	
		//erosion
		ig.ComputeErosion();
		ig.aryPrint(argv[4]);
		of<<"morphAry for erosion"<<endl;
		ig.prettyPrintMorph(of);
	
		ig.reset();
	
		//opening
		ig.ComputeOpening();
		ig.aryPrint(argv[6]);
		of<<"morphAry for opening"<<endl;
		ig.prettyPrintMorph(of);
	
		ig.reset();
	
		//closing
		ig.ComputeClosing();
		ig.aryPrint(argv[5]);
		of<<"morphAry for closing"<<endl;
		ig.prettyPrintMorph(of);
	
	
		of.close();
	}
		
	
}

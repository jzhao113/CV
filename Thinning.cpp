#include <iostream>
#include <fstream>
using namespace std;

class ThinningSkeleton
{
	private:
		int numRows,numCols,minVal,maxVal,changeFlag,cycleCount;
		ofstream out1, out2;
		int** ary1;
		int** ary2;
		
		void fillin(int**x,int rows, int cols)
		{
			for(int i=0;i<rows;i++)
			{
				x[i] = new int[cols]{0};
			}
		}
		
		void loadImage(string inputFile)
		{
			ifstream input;
			input.open(inputFile.c_str());
			input>>numRows;
			input>>numCols;
			input>>minVal;
			input>>maxVal;
			
			ary1 = new int*[numRows+2];
			fillin(ary1,numRows+2,numCols+2);
			
			ary2 = new int*[numRows+2];
			fillin(ary2,numRows+2,numCols+2);
			
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					input>>ary1[i][j];
				}
			}
		}
		
		bool helper(int i, int j)
		{
			int x[5][5] = {{0,0,0,0,0},
			   		   {0,ary1[i-1][j-1],ary1[i-1][j],ary1[i-1][j+1],0},
			               {0,ary1[i][j-1],0,ary1[i][j+1],0},
			               {0,ary1[i+1][j-1],ary1[i+1][j],ary1[i+1][j+1],0},
			               {0,0,0,0,0}};
			bool flag1=false,flag2=false,flag3=false;
			int count0=0,count1=0;
	
			//GATHERING INFORMATION	
			//crosses
			if(x[2][1]==0 && x[2][2]==0 && x[2][3]==0)
				flag1=true;
			if(x[1][2]==0 && x[2][2]==0 && x[3][2]==0)
				flag2=true;

			for(int i=1;i<4;i++)
			{
				for(int j=1;j<4;j++)
				{
					if(x[i][j]==1)
					{
						count1++;
						if(x[i][j]!=0 &&
			   	   		x[i-1][j-1]!=1 && 
			   	   		x[i-1][j]!=1 && 
			         		x[i-1][j+1]!=1 && 
			         		x[i][j-1]!=1 && 
			        		x[i][j+1]!=1 && 
			         		x[i+1][j-1]!=1 && 
			         		x[i+1][j]!=1 && 
		  	         		x[i+1][j+1]!=1)
				    		 flag3=true;
					}
					else
						count0++;
				}
			}
	
			//CHECKING CASES
			//case where one whole row or col is filled and rest of grid are zeroes
			if((flag1||flag2) && count0<6)
				return false;
			//case where the grid is crossed by zeroes and there are more than 1 one between 4 corners
			if(flag1 && flag2 && flag3 && count1>1)
				return false;
			//case where something is found not connected to anything else 
			if(flag3 && count1>1)
				return false;
	
			return true;
		}
		
		bool check3n4Conditions(int i, int j,int check)
		{
			int count=0;
			if(ary1[i-1][j-1]==1)
				count++;
			if(ary1[i-1][j]==1)
				count++;
			if(ary1[i-1][j+1]==1)
				count++;
			if(ary1[i][j-1]==1)
				count++;
			if(ary1[i][j+1]==1)
				count++;
			if(ary1[i+1][j-1]==1)
				count++;
			if(ary1[i+1][j]==1)
				count++;
			if(ary1[i+1][j+1]==1)
				count++;
			
			bool x = helper(i,j);
			if(count >=check && x)
				return true;
			
			return false;	
		}
		
		void northThinning()
		{
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					ary2[i][j] = ary1[i][j];
					
					if(ary1[i][j] > 0 && ary1[i-1][j] <=0)
					{
						if(check3n4Conditions(i,j,4))
						{
							ary2[i][j] = 0;
							changeFlag++;
						}
					}
				}
			}
		}
		
		void southThinning()
		{
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					ary2[i][j] = ary1[i][j];
					
					if(ary1[i][j] > 0 && ary1[i+1][j] <=0)
					{
						if(check3n4Conditions(i,j,4))
						{
							ary2[i][j] = 0;
							changeFlag++;
						}
					}
				}
			}
		}
		
		void westThinning()
		{
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					ary2[i][j] = ary1[i][j];
					
					if(ary1[i][j] > 0 && ary1[i][j-1] <=0)
					{
						if(check3n4Conditions(i,j,3))
						{
							ary2[i][j] = 0;
							changeFlag++;
						}
					}
				}
			}
		}
		
		void eastThinning()
		{
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					ary2[i][j] = ary1[i][j];
					
					if(ary1[i][j] > 0 && ary1[i][j+1] <=0)
					{
						if(check3n4Conditions(i,j,3))
						{
							ary2[i][j] = 0;
							changeFlag++;
						}
					}
				}
			}
		}
		
		void copyArys()
		{
			for(int i=0;i<numRows+2;i++)
			{
				for(int j=0;j<numCols+2;j++)
				{
					ary1[i][j] = ary2[i][j];
				}
			}
		}
		
		void prettyPrint(int** ary,ofstream & out)
		{
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					if(ary1[i][j] == 1)
						out<<ary[i][j]<<" ";
					else
						out<<"  ";
				}
				out<<"\n";
			}
		}
		
		void prettyPrintThin(int** ary,ofstream & out)
		{
			out<<"cycle: "<<cycleCount<<endl;
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					if(ary1[i][j] == 1)
						out<<ary[i][j]<<" ";
					else
						out<<"  ";
				}
				out<<"\n";
			}
		}
	public:
		ThinningSkeleton(string inputFile,string outputFile1, string outputFile2)
		{
			loadImage(inputFile);
			out1.open(outputFile1.c_str());
			out1<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
			out2.open(outputFile2.c_str());
			
			out2<<"Original Image"<<endl;
			prettyPrint(ary1,out2);
		}
		void start()
		{
			changeFlag=1;
			while(changeFlag>0)
			{
				changeFlag=0;
				doThinning();
				cycleCount++;
				prettyPrintThin(ary1,out2);
			}
			
			for(int i=1;i<numRows+1;i++)
			{
				for(int j=1;j<numCols+1;j++)
				{
					out1<<ary1[i][j]<<" ";
				}
				out1<<endl;
			}
			
			out1.close();
			out2.close();
		}
		void doThinning()
		{
			northThinning();
			copyArys();
			southThinning();
			copyArys();
			westThinning();
			copyArys();
			eastThinning();
			copyArys();
		}
		
};


int main(int argc, char** argv)
{
	string input = argv[1],output1 = argv[2], output2= argv[3]; 
	ThinningSkeleton x(input,output1,output2);
	x.start();
}

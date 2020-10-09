import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;

public class image {
	
	private int numRows, numCols, minVal, maxVal;
	private int newMax, newMin=9999, gnewMax=0, gnewMin;
	private int [][] mirrorFramedAry;
	private int [][]maskRobertRightDiag = {{1,-1},{-1,1}};
	
	private int [][]maskRobertLeftDiag = {{-1,1},{1,-1}};
	
	private int [][]maskSobelRightDiag={{0,1,2},
								{-1,0,1},
								{-2,-1,0}};
						
	private int [][]maskSobelLeftDiag ={{2,1,0},
								{1,0,-1},
								{0,-1,-2}};
	
	private int [][] RobertRightDiag;
	private int [][] RobertLeftDiag;
	private int [][] SobelRightDiag;
	private int [][] SobelLeftDiag;
	private int [][] GradiantEdge;
	private int [][] edgeSum;
	
	public image(String file) throws NumberFormatException, IOException
	{
		BufferedReader br = new BufferedReader(new FileReader(file));
		String line;
		int x=1,y=1;
		while ((line = br.readLine()) != null) 
		{
            String elements []= line.split("\\s+");
            if(elements.length==4)
            {
            	numRows = Integer.parseInt(elements[0]);
            	numCols = Integer.parseInt(elements[1]);
            	minVal = Integer.parseInt(elements[2]);
            	maxVal = Integer.parseInt(elements[3]);
            	
            	mirrorFramedAry = new int [numRows+2][numCols+2];
            	mirrorFramedAry = set2DZero(mirrorFramedAry);
            	
            	RobertRightDiag = new int [numRows+2][numCols+2];
            	RobertRightDiag = set2DZero(RobertRightDiag);
            	
            	RobertLeftDiag = new int [numRows+2][numCols+2];
            	RobertLeftDiag = set2DZero(RobertLeftDiag);
            	
            	SobelRightDiag = new int [numRows+2][numCols+2];
            	SobelRightDiag = set2DZero(SobelRightDiag);
            	
            	SobelLeftDiag = new int [numRows+2][numCols+2];
            	SobelLeftDiag = set2DZero(SobelLeftDiag);
            	
            	GradiantEdge = new int [numRows+2][numCols+2];
            	GradiantEdge = set2DZero(GradiantEdge);
            	
            	edgeSum = new int [numRows+2][numCols+2];
            	edgeSum = set2DZero(edgeSum);
            	
            }
            else
            {
            	if(elements.length==numCols)
            	{
            		for(int i=0;i<elements.length;i++)
                	{
                		if(!elements[i].equals(""))
                		{
                			mirrorFramedAry[x][y] = Integer.parseInt(elements[i]);
                			y++;
                		}
                	}
                	y=1;
                    x++;
            	}
            }
        }
		
		mirrorFramed();
	}
	
	public void process(String robert, String sobel, String gradiant, String prettyPrint, String debug) throws IOException
	{
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
			    RobertRightDiag[i][j] = Math.abs(convoluteRobert(i,j,maskRobertRightDiag));
				RobertLeftDiag[i][j] = Math.abs(convoluteRobert(i,j,maskRobertLeftDiag));
				
				SobelRightDiag[i][j] = Math.abs(convoluteSobel(i,j,maskSobelRightDiag));
				SobelLeftDiag[i][j] = Math.abs(convoluteSobel(i,j,maskSobelLeftDiag));
				
				GradiantEdge[i][j] = Math.abs(gEdge(i,j));
			}
		}
		
		BufferedWriter robertW = new BufferedWriter(new FileWriter(robert));
		addTwoArys(RobertRightDiag,RobertLeftDiag);
		printOut(robertW,edgeSum);
		
		BufferedWriter sobelW = new BufferedWriter(new FileWriter(sobel));
		addTwoArys(SobelRightDiag,SobelLeftDiag);
		printOut(sobelW,edgeSum);
		
		BufferedWriter gradiantW = new BufferedWriter(new FileWriter(gradiant));
		newMax = gnewMax;
		newMin = gnewMin;
		printOut(gradiantW,GradiantEdge);
		
		
		BufferedWriter prettyW = new BufferedWriter(new FileWriter(prettyPrint));
		BufferedWriter debugW = new BufferedWriter(new FileWriter(debug));
		robertW.close();
		sobelW.close();
		gradiantW.close();
		prettyW.close();
		debugW.close();
		
		
	}
	
	private void printOut(BufferedWriter bw, int x[][]) throws IOException
	{
		bw.write(numRows+" "+numCols+" "+newMin+" "+newMax+"\n");
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				bw.write(x[i][j]+" ");
			}
			bw.write("\n");
		}
	}
	
	private int convoluteRobert(int i,int j, int mask[][])
	{
		int sum=0;
		sum+= mirrorFramedAry[i][j] * mask[0][0];
		sum+= mirrorFramedAry[i][j+1] * mask[0][1];
		sum+= mirrorFramedAry[i+1][j] * mask[1][0];
		sum+= mirrorFramedAry[i+1][j+1] * mask[1][1];
		
		return sum;
	}
	
	private int convoluteSobel(int i, int j,int mask[][])
	{
		int sum=0;
		sum+= mirrorFramedAry[i-1][j-1] * mask[0][0];
		sum+= mirrorFramedAry[i-1][j] * mask[0][1];
		sum+= mirrorFramedAry[i-1][j+1] * mask[0][2];
		
		sum+= mirrorFramedAry[i][j-1] * mask[1][0];
		sum+= mirrorFramedAry[i][j] * mask[1][1];
		sum+= mirrorFramedAry[i][j+1] * mask[1][2];
		
		sum+= mirrorFramedAry[i+1][j-1] * mask[2][0];
		sum+= mirrorFramedAry[i+1][j] * mask[2][1];
		sum+= mirrorFramedAry[i+1][j+1] * mask[2][2];
		
		return sum;
	}
	
	private int gEdge(int i, int j)
	{
		int a = mirrorFramedAry[i][j]-mirrorFramedAry[i+1][j];
		int b = mirrorFramedAry[i][j]-mirrorFramedAry[i][j+1];
		int output = (int)Math.sqrt((a*a)+(b*b));
		
		if(gnewMax<output)
			gnewMax=output;
		if(gnewMin>output)
			gnewMin=output;
		
		return output;
	}
	
	private void addTwoArys(int[][] x, int[][] y)
	{
		newMax=0;
		newMin=9999;
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				edgeSum[i][j] = x[i][j]+y[i][j];
				
				if(newMax<edgeSum[i][j])
					newMax=edgeSum[i][j];
				
				if(edgeSum[i][j]<newMin)
					newMin=edgeSum[i][j];
			}
		}
	}
	
	private void mirrorFramed()
	{
		for(int i=0;i<numRows+2;i++)
		{
			mirrorFramedAry[i][0] = mirrorFramedAry[i][1];
			mirrorFramedAry[i][numCols+1] = mirrorFramedAry[i][numCols];
		}
		
		for(int i=0;i<numCols+2;i++)
		{
			mirrorFramedAry[0][i] = mirrorFramedAry[1][i];
			mirrorFramedAry[numRows+1][i] = mirrorFramedAry[numRows][i];
		}
	}
	
	private int[][] set2DZero(int [][] Ary2D)
	{
		for(int i=0;i<Ary2D.length;i++)
		{
			Arrays.fill(Ary2D[i],0);
		}
		return Ary2D;
	}

}

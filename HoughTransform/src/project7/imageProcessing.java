package project7;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;

public class imageProcessing {
	
	private int numRows, numCols, minVal, maxVal;
	private int newMinVal, newMaxVal;
	private int [][]zeroFramedAry;
	private int [][]skeletonAry;
	private ArrayList<int[]> triplet = new ArrayList<>();
	
	public imageProcessing(String input) throws IOException
	{
		BufferedReader br = new BufferedReader(new FileReader(input));
		String line;
		boolean firstRound=true;
		int x=1,y=1;
		while ((line = br.readLine()) != null) 
		{
			String elements []= line.split("\\s+");
			if(firstRound)
			{
				numRows = Integer.parseInt(elements[0]);
				numCols = Integer.parseInt(elements[1]);
				minVal = Integer.parseInt(elements[2]);
				maxVal = Integer.parseInt(elements[3]);
				
				zeroFramedAry = new int [numRows+2][numCols+2];
				setAllZero(zeroFramedAry,numRows+2,numCols+2);
				
				skeletonAry = new int [numRows+2][numCols+2];
				setAllZero(zeroFramedAry,numRows+2,numCols+2);
				
				firstRound=false;
			}
			else
			{	
				if(elements.length==numCols)
				{
					for(int i=0;i<elements.length;i++)
					{
						if(!elements[i].equals(""))
						{
							zeroFramedAry[x][y] = Integer.parseInt(elements[i]);
							y++;
						}
					}
					
					x++;
					y=1;
				}
				
			}
			
        }
		
		br.close();
	}
	
	private void setAllZero(int [][] ary, int row, int col)
	{
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				ary[i][j] = 0;
	}
	
	private void firstPass_8Distance(int [][]ary)
	{
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				if(ary[i][j]!=0)
				{
					ary[i][j] = Math.min(ary[i-1][j-1],Math.min(ary[i-1][j], Math.min(ary[i-1][j+1],ary[i][j-1])))+1;
				}
			}
		}
	}
	
	private void secondPass_8Distance(int [][]ary)
	{
		for(int i=numRows;i>0;i--)
		{
			for(int j=numCols;j>0;j--)
			{
				if(ary[i][j]!=0)
				{
					ary[i][j] = Math.min(ary[i+1][j-1]+1,Math.min(ary[i+1][j]+1, Math.min(ary[i+1][j+1]+1,Math.min(ary[i][j+1]+1,ary[i][j]))));
				}
			}
		}
	}
	
	private void computeLocalMaxima(int ary[][], int skel[][]) throws IOException
	{
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				if(ary[i][j]>=ary[i-1][j-1]&&
				   ary[i][j]>=ary[i-1][j]&&
				   ary[i][j]>=ary[i-1][j+1]&&
				   ary[i][j]>=ary[i][j-1]&&
				   ary[i][j]>=ary[i][j+1]&&
				   ary[i][j]>=ary[i+1][j-1]&&
				   ary[i][j]>=ary[i+1][j]&&
				   ary[i][j]>=ary[i+1][j+1])
					skel[i][j] = ary[i][j];
				else
					skel[i][j] = 0;
			}
		}
	}
	
	private void extractLocalMaxima(int skel[][]) throws IOException
	{
		newMinVal = 9999;
		newMaxVal = 0;
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				if(skel[i][j]>0)
				{
					int temp [] = {i,j,skel[i][j]};
					triplet.add(temp);
					
					if(newMinVal > skel[i][j])
						newMinVal = skel[i][j];
					
					if(newMaxVal < skel[i][j])
						newMaxVal = skel[i][j];
				}
			}
		}
	}
	
	private void firstpassexpansion(int ary[][])
	{
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				if(ary[i][j]==0)
				{
					ary[i][j] = Math.max(0,Math.max(ary[i-1][j-1]-1, 
											Math.max(ary[i-1][j]-1,
											Math.max(ary[i-1][j+1]-1,
											Math.max(ary[i][j-1]-1,
											Math.max(ary[i][j+1]-1,
											Math.max(ary[i+1][j-1]-1,
											Math.max(ary[i+1][j]-1,
											ary[i+1][j+1]-1))))))));
				}
			}
		}
	}
	
	private void secondpassexpansion(int ary[][])
	{
		for(int i=numRows;i>0;i--)
		{
			for(int j=numCols;j>0;j--)
			{
				ary[i][j] = Math.max(ary[i][j],Math.max(ary[i-1][j-1], 
						Math.max(ary[i-1][j],
						Math.max(ary[i-1][j+1],
						Math.max(ary[i][j-1],
						Math.max(ary[i][j+1],
						Math.max(ary[i+1][j-1],
						Math.max(ary[i+1][j],
						(ary[i+1][j+1]))))))))-1);
			}
		}
	}
	
	private void prettyPrint(int ary[][], BufferedWriter out) throws IOException
	{
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				if(ary[i][j]>0)
					out.write(ary[i][j]+" ");
				else
					out.write("  ");
			}
			out.write("\n");
		}
		out.write("\n");
	}
	
	private void prettyPrintMax(BufferedWriter out) throws IOException
	{
		out.write(numRows+" "+numCols+" "+newMinVal+" "+newMaxVal+"\n");
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				if(skeletonAry[i][j]>0)
				{
					out.write(i+" "+j+" "+skeletonAry[i][j]+"\n");
				}
			}
		}
	}
	
	private void decompression(BufferedWriter bDec) throws IOException
	{
		bDec.write(numRows+" "+numCols+" "+"0"+" "+"1"+"\n");
		for(int i=1;i<numRows+1;i++)
		{
			for(int j=1;j<numCols+1;j++)
			{
				if(zeroFramedAry[i][j]>0)
					bDec.write("1"+" ");
				else
					bDec.write("0"+" ");
			}
			bDec.write("\n");
		}
		
	}
	
	public void compute8Distance(BufferedWriter out) throws IOException
	{
		firstPass_8Distance(zeroFramedAry);
		
		out.write("first pass 8 distance\n");
		prettyPrint(zeroFramedAry, out);
		
		out.write("second pass 8 distance\n");
		secondPass_8Distance(zeroFramedAry);
		
		prettyPrint(zeroFramedAry, out);
		
	}
	
	public void skeletonExtraction(BufferedWriter out, BufferedWriter skelF) throws IOException
	{
		computeLocalMaxima(zeroFramedAry,skeletonAry);
		out.write("compute local maxima"+"\n");
		prettyPrint(skeletonAry,out);
		
		extractLocalMaxima(skeletonAry);
		prettyPrintMax(skelF);
	}
	
	public void skeletonExpansion(BufferedWriter skelF, BufferedWriter out, BufferedWriter bDec) throws NumberFormatException, IOException
	{
		setAllZero(zeroFramedAry,numRows+2,numCols+2);
		
		for(int i=0;i<triplet.size();i++)
		{
			zeroFramedAry[triplet.get(i)[0]][triplet.get(i)[1]] = triplet.get(i)[2]; 
		}
		
		out.write("skeleton first expansion");
		firstpassexpansion(zeroFramedAry);
		prettyPrint(zeroFramedAry,out);
		
		out.write("skeleton second expansion");
		secondpassexpansion(zeroFramedAry);
		prettyPrint(zeroFramedAry,out);
		
		decompression(bDec);
		
	}
	

}

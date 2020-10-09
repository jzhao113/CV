package project9;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class image 
{
	private int numRows, numCols, minVal, maxVal;
	private int imgAry[][];
	private HoughTransform ht;
	
	private void setAllZero(int [][] ary, int row, int col)
	{
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				ary[i][j] = 0;
	}
	
	private void loadImage(String input) throws NumberFormatException, IOException
	{
		BufferedReader br = new BufferedReader(new FileReader(input));
		String line;
		boolean firstRound=true;
		int x=0, y=0;
		while ((line = br.readLine()) != null) 
		{
			String elements []= line.split("\\s+");
			if(firstRound)
			{
				numRows = Integer.parseInt(elements[0]);
				numCols = Integer.parseInt(elements[1]);
				minVal = Integer.parseInt(elements[2]);
				maxVal = Integer.parseInt(elements[3]);
				
				imgAry = new int [numRows][numCols];
				setAllZero(imgAry,numRows,numCols);
				
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
							imgAry[x][y] = Integer.parseInt(elements[i]);
							y++;
						}
					}
					
					x++;
					y=0;
	
				}
				
			}
			
        }
	}
	
	public void prettyPrintHough(BufferedWriter bw) throws IOException
	{
		ht.prettyPrintHough(bw);
	}
	
	public void ary2File(BufferedWriter bw) throws IOException
	{
		ht.ary2File(bw);
	}
	
	public image(String input) throws NumberFormatException, IOException
	{
		loadImage(input);
		ht = new HoughTransform(imgAry, numRows, numCols);
	}
	

}

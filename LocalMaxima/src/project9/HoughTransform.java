package project9;

import java.io.BufferedWriter;
import java.io.IOException;

public class HoughTransform {
	
	private int angleInDegrees, houghDist, houghAngle, houghMinVal=9999, houghMaxVal=0;
	private double angleInRadians, offSet;
	private int houghAry [][];
	private int imgAry[][];
	private int rows, cols;
	
	public HoughTransform(int imgAry[][], int rows, int cols)
	{
		this.rows = rows;
		this.cols = cols;
		this.imgAry = imgAry;
		
		houghDist = (int)(2 * (Math.sqrt(Math.pow(rows, 2)+Math.pow(cols,2))));
		offSet =  houghDist/2.0;
		System.out.println(offSet);
		houghAngle = 180;
		
		houghAry = new int [houghDist][houghAngle];
		setAllZero(houghAry,houghDist,houghAngle);
		
		buildHoughSpace();
		determineMinMax();
	}
	
	public void prettyPrintHough(BufferedWriter bw) throws IOException
	{
		for(int i=0;i<houghDist;i++)
		{
			for(int j=0;j<houghAngle;j++)
			{
				if(houghAry[i][j]>0)
					bw.write("1 ");
				else
					bw.write("  ");
			}
			bw.write("\n");
		}
		
		bw.write("\n");
	}
	
	public void ary2File(BufferedWriter bw) throws IOException
	{
		bw.write(houghDist+" "+houghAngle+" "+houghMinVal+" "+houghMaxVal+"\n");
		for(int i=0;i<houghDist;i++)
		{
			for(int j=0;j<houghAngle;j++)
			{
				bw.write(houghAry[i][j]+" ");
			}
			bw.write("\n");
		}
	}
	
	private void setAllZero(int [][] ary, int row, int col)
	{
		for(int i=0;i<row;i++)
			for(int j=0;j<col;j++)
				ary[i][j] = 0;
	}
	
	private int computeDistance(int x, int y, double a)
	{
		double t = a - Math.atan((double)y/(double)x) - (Math.PI/2.0);
		double distance = ((Math.sqrt(Math.pow((double)x, 2) + Math.pow((double)y,2))) * Math.cos(t) ) + offSet;
		return (int) distance;
	}
	
	private void buildHoughSpace()
	{
		for(int i=0;i<rows;i++)
		{
			for(int j=0;j<cols;j++)
			{
				if(imgAry[i][j] > 0)
				{
					angleInDegrees = 0;
					while(angleInDegrees <=179)
					{
						angleInRadians = (Math.PI / 180)*angleInDegrees;
						int distInt = computeDistance(j,i,angleInRadians);
						houghAry[distInt][angleInDegrees]++;
						angleInDegrees++;
					}
				}
			}
		}
		
	}
	
	private void determineMinMax()
	{
		for(int i=0;i<houghDist;i++)
		{
			for(int j=0;j<houghAngle;j++)
			{
				if(houghAry[i][j]>houghMaxVal)
					houghMaxVal = houghAry[i][j];
				if(houghAry[i][j]<houghMinVal)
					houghMinVal = houghAry[i][j];
			}
		}
	}

}

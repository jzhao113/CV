import java.util.*;
import java.math.*;
import java.io.*;
public class image {
	
	private int numRows,numCols,minVal,maxVal,maxHight;
	private int offSet, thrVal;
	private int histAry[];
	private int histGraph[][];
	private int GaussAry[];
	private int GaussGraph[][];
	private int gapGraph[][];
	
	public image(String inFile) throws IOException
	{
		loadHist(inFile);
		
	}
	
	private void loadHist(String inFile) throws IOException
	{
		BufferedReader br = new BufferedReader(new FileReader(inFile));
		String line;
		int largestValue = 0;
		while ((line = br.readLine()) != null) 
		{
            String elements []= line.split("\\s+");
            if(elements.length==4)
            {
            	numRows = Integer.parseInt(elements[0]);
            	numCols = Integer.parseInt(elements[1]);
            	minVal = Integer.parseInt(elements[2]);
            	maxVal = Integer.parseInt(elements[3]);

            	histAry = new int [maxVal+1];
            	Arrays.fill(histAry,0);
        		GaussAry = new int [maxVal+1];
        		Arrays.fill(GaussAry,0);
        		
            	offSet = (maxVal-minVal)/10;
            	thrVal=offSet;
            	
            }
            else if(elements.length==2)
            	histAry[Integer.parseInt(elements[0])] = Integer.parseInt(elements[1]);
            
            if(elements.length==2 && histAry[Integer.parseInt(elements[0])]>largestValue)
                largestValue=histAry[Integer.parseInt(elements[0])];
        }
		
		maxHight=largestValue;
		
		histGraph = new int [maxVal+1][maxHight+1];
		histGraph = set2DZero(histGraph);
		
		GaussGraph = new int [maxVal+1][maxHight+1];
		GaussGraph = set2DZero(histGraph);
		
		gapGraph = new int [maxVal+1][maxHight+1];
		gapGraph = set2DZero(gapGraph);

	}
	
	public void plotHistGraph()
	{
		for(int i=0;i<histAry.length;i++)
		{
			histGraph[i][histAry[i]]=1;
		}
	}
	
	public void prettyPrintHistGraph(BufferedWriter bw) throws IOException
	{
		bw.write("histGraph:"+" "+numRows+" "+numCols+" "+minVal+" "+maxVal);
		bw.write("\n");
		for(int i=0;i<histGraph.length;i++)
		{
			for(int j=0;j<histGraph[i].length;j++)
			{
				if(histGraph[i][j]<=0)
				{
					bw.write(" ");
				}
				else
				{
					bw.write("*");
				}
			}
			
			bw.write("\n");
		}
		
	}
	
	public void prettyPrintGaussGraph(BufferedWriter bw) throws IOException
	{
		bw.write("GaussGraph:"+" "+numRows+" "+numCols+" "+minVal+" "+maxVal);
		bw.write("\n");
		for(int i=0;i<GaussGraph.length;i++)
		{
			for(int j=0;j<GaussGraph[i].length;j++)
			{
				if(GaussGraph[i][j]==0)
				{
					bw.write(" ");
				}
				else
				{
					bw.write("*");
				}
			}
			
			bw.write("\n");
		}

	}
	
	public void prettyPrintGapGraph(BufferedWriter bw) throws IOException
	{
		bw.write("GapGraph:"+" "+numRows+" "+numCols+" "+minVal+" "+maxVal);
		bw.write("\n");
		for(int i=0;i<gapGraph.length;i++)
		{
			for(int j=0;j<gapGraph[i].length;j++)
			{
				if(gapGraph[i][j]==0)
				{
					bw.write(" ");
				}
				else
				{
					bw.write("*");
				}
			}
			
			bw.write("\n");
		}
		

	}
	
	public int[] set1DZero(int [] Ary1D)
	{
		Arrays.fill(Ary1D,0);
		return Ary1D;
	}
	
	public int[][] set2DZero(int [][] Ary2D)
	{
		for(int i=0;i<Ary2D.length;i++)
		{
			Arrays.fill(Ary2D[i],0);
		}
		return Ary2D;
	}
	
	public int biMeanGauss(BufferedWriter bw2,BufferedWriter bw3) throws IOException
	{
		double sum1,sum2,total;
		int bestThr = thrVal;
		double minSumDiff=999999.0;
		
		while(thrVal < (maxVal-offSet))
		{	
			GaussAry = set1DZero(GaussAry);
			GaussGraph = set2DZero(GaussGraph);
			gapGraph = set2DZero(gapGraph);
			
			sum1 = fitGauss(0,thrVal);
			sum2 = fitGauss(thrVal,maxVal);
			total = sum1+sum2;
			
			if(total<minSumDiff)
			{
				minSumDiff = total;
				bestThr = thrVal;
			}
			
			thrVal++;
			prettyPrintGaussGraph(bw2);
			plotGaps();
			prettyPrintGapGraph(bw3);
		}
		
		return bestThr;
	}
	
	public double fitGauss(int leftIndex, int rightIndex)
	{
		double mean,var,sum=0.0,Gval,maxGval;
		
		mean = computeMean(leftIndex,rightIndex);
		var = computeVar(leftIndex,rightIndex,mean);
		
		for(int index=leftIndex; index<=rightIndex;index++)
		{
			Gval=modifiedGauss(index,mean,var);
			sum+=Math.abs(Gval - (double)histAry[index]);
			GaussAry[index] = (int)Gval;
			GaussGraph[index][(int)Gval] = 1;
		}
		
		return sum;
	}
	
	public double computeMean(int leftIndex, int rightIndex)
	{
		int sum=0,numPixels=0;
		int height=0;
		for(int index=leftIndex; index<rightIndex;index++)
		{
			sum+=(histAry[index]*index);
			numPixels+=histAry[index];
			
			if(histAry[index]>height)
				height=histAry[index];
		}
		
		return (double)sum/(double)numPixels;
	}
	
	public double computeVar(int leftIndex, int rightIndex, double mean)
	{
		double sum=0.0,numPixels=0.0;
		for(int index=leftIndex;index<rightIndex;index++)
		{
			sum+=(double)histAry[index] * Math.pow(((double)index-mean), 2);
			numPixels+=(double)histAry[index];
		}
		
		return sum/numPixels;
	}
	
	public double modifiedGauss(int x, double mean, double var)
	{
		return (double) (maxHight*Math.exp(-1 * Math.pow((double)(x-mean),2)/(2*var)));
	}
	
	public void plotGaps()
	{
		for(int index=minVal;index<maxVal;index++)
		{
			int first = Math.min(histAry[index],GaussAry[index]);
			int last = Math.max(histAry[index],GaussAry[index]);
			for(int index2=first;index2<last;index2++)
			{
				gapGraph[index][index2]=1;
			}
		}
	}
	
	public void bestThrPlot(int bestThrVal)
	{
		double sum1,sum2;
		GaussAry=set1DZero(GaussAry);
		GaussGraph=set2DZero(GaussGraph);
		gapGraph=set2DZero(gapGraph);
		
		sum1 = fitGauss(0,bestThrVal);
		sum2 = fitGauss(bestThrVal,maxVal);
		plotGaps();
	}

}

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

//BiGaussAutoThrSelection_data1.txt
public class main {
	
	public static void main(String[]args) throws IOException
	{
		BufferedWriter bw1 = new BufferedWriter(new FileWriter(args[1]));
		BufferedWriter bw2 = new BufferedWriter(new FileWriter(args[2]));
		BufferedWriter bw3 = new BufferedWriter(new FileWriter(args[3]));
		
		String inFile = args[0];
		
		image ourImage = new image(inFile);
		ourImage.plotHistGraph();
		ourImage.prettyPrintHistGraph(bw1);

		int bestThrVal = ourImage.biMeanGauss(bw2, bw3);
		bw1.write("bestThrVal: "+bestThrVal+"\n");
		ourImage.bestThrPlot(bestThrVal);
		ourImage.prettyPrintGaussGraph(bw1);
		ourImage.prettyPrintGapGraph(bw1);
		
		bw1.close();
		bw2.close();
		bw3.close();
		
	}

}

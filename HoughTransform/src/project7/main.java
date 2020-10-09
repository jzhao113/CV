package project7;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class main {
	
	public static void main(String[]args) throws IOException
	{
		imageProcessing x = new imageProcessing(args[0]);
		
		
		BufferedWriter bOut1 = new BufferedWriter(new FileWriter(args[1]+".txt"));
		BufferedWriter bOut2 = new BufferedWriter(new FileWriter(args[2]+".txt"));
		BufferedWriter bSkelF = new BufferedWriter(new FileWriter(args[1]+"_skeleton.txt"));
		BufferedWriter bDec = new BufferedWriter(new FileWriter(args[1]+"_decompressed.txt"));
		
		x.compute8Distance(bOut1);
		x.skeletonExtraction(bOut1,bSkelF);
		x.skeletonExpansion(bSkelF, bOut2,bDec);
		
		bOut1.close();
		bOut2.close();
		bSkelF.close();
		bDec.close();
	}

}

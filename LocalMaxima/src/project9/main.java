package project9;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class main {
	
	public static void main (String[]args) throws NumberFormatException, IOException
	{
		image ig = new image(args[0]);
		BufferedWriter bOut1 = new BufferedWriter(new FileWriter(args[1]));
		BufferedWriter bOut2 = new BufferedWriter(new FileWriter(args[2]));
		
		ig.prettyPrintHough(bOut1);
		ig.ary2File(bOut2);
		
		bOut1.close();
		bOut2.close();
	}

}

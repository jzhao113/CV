import java.io.IOException;

public class main {
	
	public static void main(String[]args) throws NumberFormatException, IOException
	{
		image ig = new image(args[0]);
		ig.process(args[1], args[2], args[3], args[4],args[5]);
	}

}

import org.img.processing.BailingJni;
import org.img.processing.BailingDemo;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;

public class testDemo
{
	public static void main(String[] args)
	{
		String modelpath="/root/1xiu/BailingJniPro/model/";
		BailingDemo demo = new BailingDemo(modelpath);

		File file2 = new File("/root/1xiu/BailingJniPro/yy.jpg");
		FileInputStream fis = null;
		try {
			fis = new FileInputStream(file2);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		byte[] buf = new byte[(int) file2.length()];
		try {
			fis.read(buf);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		//System.out.println(file2.length());

		String b = demo.processing(buf);


		try {
			fis.close();
			System.out.println("\n"+b);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}

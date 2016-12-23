package org.img.processing;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;

public class BailingJni 
{
	static
	{
		//System.loadLibrary("BailingOCRgJni");
		System.loadLibrary("BailingOCRJni");
	}
	public native String processing(int ptrNative,byte[] img);
	public native int initPath(String path);

	

	public static void main(String[] args)
	{
		//String modelpath=JarDllJava.rootPath(AppTest.class)+"model";
		String modelpath="/root/1xiu/BailingJniPro/model/";
		System.out.println(modelpath);

		BailingJni bl = new BailingJni();
		int hand = bl.initPath(modelpath);


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

		System.out.println(file2.length());

		String b = bl.processing(hand,buf);
		try {
			fis.close();
			System.out.println("\n"+b+"\n"+"end");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return;

	}
}

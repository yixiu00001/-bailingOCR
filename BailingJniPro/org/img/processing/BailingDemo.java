package org.img.processing;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;

public class BailingDemo
{
	private BailingJni bailingJni;

	public BailingDemo(String path)
	{
		bailingJni = new BailingJni(path);
	}

	public String processing(byte[] img)
	{
		return bailingJni.processing(bailingJni.ptrNative,img);
	}

	public void realeseBailingJni()
	{
		bailingJni.delete(bailingJni.ptrNative);
		bailingJni.ptrNative=0;
	}
	public int getPtr()
	{
		return bailingJni.ptrNative;
	}

}

#define _USE_MATH_DEFINES 
#include <cstdio>
#include <stdlib.h>
#include <math.h>

int main()
{
	FILE *filein;
	FILE *fileout;
	/*********************Rectify the image of omnnidirectional camera to a panoramic image*********************/
	unsigned char *omni = new unsigned char[1024 * 768 * 3];
	unsigned char *omni_out = new unsigned char[720 * 384 * 3];
	/*------------------------讀檔---------------------------*/
	filein = fopen("omni.raw", "rb");
	fread(omni, 1, 1024 * 768 * 3, filein);
	fclose(filein);
	
	for (int r = 0; r < 384; r++)
	{
		for (int theta = 0; theta < 720; theta++)
		{
			double t = ((double)theta / 720) * 2 * M_PI;
			int x = (r*cos(-t) + 512);
			int y = (r*sin(-t) + 384);

			for (int z = 0; z < 3; z++)
			{
				omni_out[(383 - r) * 720 * 3 + theta * 3 + z] = omni[y * 1024 * 3 + x * 3 + z];
			}
		}
	}
	/*------------------------寫檔---------------------------*/
	fileout = fopen("HW3_omni_out.raw", "wb");
	fwrite(omni_out, 720 * 384 * 3, 1, fileout);
	fclose(fileout);
	/***********************************************************************************************************/

	/*******************************Image stitching with the projective transform*******************************/
	unsigned char *left = new unsigned char[510 * 370 * 3];
	unsigned char *right = new unsigned char[510 * 370 * 3];
	unsigned char *image_warping = new unsigned char[900 * 480 * 3];
	/*------------------------讀檔---------------------------*/
	filein = fopen("left.raw", "rb");
	fread(left, 1, 510 * 370 * 3, filein);
	fclose(filein);
	filein = fopen("right.raw", "rb");
	fread(right, 1, 510 * 370 * 3, filein);
	fclose(filein);
	/*-------------------初始化輸出矩陣-----------------------*/
	for (int i = 0; i < 700 * 480 * 3; i++)
	{
		image_warping[i] = 0;
	}
	///*-------------------將left.raw放置於900*480陣列中-----------------------*/
	//for (int y = 0; y < 480; y++)
	//{
	//	for (int x = 0; x < 900; x++)
	//	{
	//		for (int z = 0; z < 3; z++)
	//		{
	//			if (x >= 0 && x <= 509 && y >= 80 && y <= 449)
	//			{
	//				image_warping[y * 900 * 3 + x * 3 + z] = left[(y - 80) * 510 * 3 + x * 3 + z];
	//			}
	//		}
	//	}
	//}

	/*-----------------------輸出影像的四個頂點-----------------------*/
	double x0 = 250, x3 = 310, x2 = 900, x1 = 770;
	double y0 = 130, y3 = 470, y2 = 475, y1 = 0;

	double delta_x1 = x1 - x2, delta_x2 = x3 - x2, sigma_x = x0 - x1 + x2 - x3;
	double delta_y1 = y1 - y2, delta_y2 = y3 - y2, sigma_y = y0 - y1 + y2 - y3;

	double g = (sigma_x*delta_y2 - sigma_y*delta_x2) / (delta_x1*delta_y2 - delta_y1*delta_x2);
	double h = (sigma_y*delta_x1 - sigma_x*delta_y1) / (delta_x1*delta_y2 - delta_y1*delta_x2);
	double ii = 1;
	double a = x1 - x0 + g*x1;
	double b = x3 - x0 + h*x3;
	double c = x0;
	double d = y1 - y0 + g*y1;
	double e = y3 - y0 + h*y3;
	double f = y0;

	double A = e*ii - f*h;
	double B = c*h - b*ii;
	double C = b*f - c*e;
	double D = f*g - d*ii;
	double E = a*ii - c*g;
	double F = c*d - a*f;
	double G = d*h - e*g;
	double H = b*g - a*h;
	double I = a*e - b*d;
	
	double u, v;

	for (int y = 0; y < 480; y++)
	{
		for (int x = 0; x < 900; x++)
		{
			for (int z = 0; z < 3; z++)
			{
				/*-------------------將left.raw放置於900*480陣列中-----------------------*/
				if (x >= 0 && x <= 509 && y >= 80 && y <= 449)
				{
					image_warping[y * 900 * 3 + x * 3 + z] = left[(y - 80) * 510 * 3 + x * 3 + z];
				}
				u = (double)(A*x + B*y + C) / (double)(G*x + H*y + I);
				v = (double)(D*x + E*y + F) / (double)(G*x + H*y + I);
				/*-------------------將right.raw放置於900*480陣列中-----------------------*/
				if (u >= 0 && u <= 1 && v >= 0 && v <= 1)
				{
					image_warping[y * 900 * 3 + x * 3 + z] = right[(int)(v * 370) * 510 * 3 + (int)(u * 510) * 3 + z];
				}
			}
		}
	}
	/*------------------------寫檔---------------------------*/
	fileout = fopen("HW3_left_out.raw", "wb");
	fwrite(left, 510 * 370 * 3, 1, fileout);
	fclose(fileout);
	fileout = fopen("HW3_right_out.raw", "wb");
	fwrite(right, 510 * 370 * 3, 1, fileout);
	fclose(fileout);
	fileout = fopen("HW3_image_warping.raw", "wb");
	fwrite(image_warping, 900 * 480 * 3, 1, fileout);
	fclose(fileout);
	/***********************************************************************************************************/
}
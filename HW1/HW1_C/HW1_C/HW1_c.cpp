/************************************************************/
/*  �{���W�١GImage Read/Write/Rotation/Channel_Interchange   */
/*           (Not using OpenCV function )          */
/*  �@�̩m�W�G����                                            */
/*	�Ǹ�:106318025                                           */
/*  ���ɱб¡GDr.������                                       */
/*  �q�l�l��Gt106318025@ntut.org.tw                         */
/************************************************************/
#include <cstdio>

void image1();
void image2();
void image3();
void image4();

int main()
{
	image1();
	image2();
	image3();
	image4();
}
/************************Image Read/Write**********************/
void image1()
{
	unsigned char arr[786486];
	/*------------------------Ū��---------------------------*/
	FILE *filein = fopen("InputImage1.bmp", "rb");
	fread(arr, 1, sizeof(arr), filein);
	/*------------------------�g��---------------------------*/
	FILE *fileout = fopen("C_code_output_image.bmp", "wb");
	fwrite(arr, 1, sizeof(arr), fileout);

	fclose(filein);
	fclose(fileout);
}
/**************************************************************/
/*************************Image Rotation***********************/
void image2()
{
	unsigned char head[54];
	unsigned char *arr = new unsigned char[786432];
	unsigned char *img = new unsigned char[786432];
	/*------------------------Ū��---------------------------*/
	FILE *filein = fopen("InputImage1.bmp", "rb");
	fread(head, 1, 54, filein);
	fread(arr, 1, 786432, filein);

	//fseek(filein, 54, 0);
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				img[512 * 3 * (511 - j) + 3 * i + k] = arr[(3 * j + k) + i * 3 * 512];
			}
		}
	}
	/*------------------------�g��---------------------------*/
	FILE *fileout = fopen("C_code_rotation.bmp", "wb");
	fwrite(head, 1, 54, fileout);
	fwrite(img, 1, 786432, fileout);

	fclose(filein);
	fclose(fileout);

}
/**************************************************************/
/******************Image Channel Interchange*******************/
void image3()//
{
	/*------------------�ŧi���Y�B���e��T---------------------*/
	unsigned char head[54];
	unsigned char *arr = new unsigned char[786432];
	unsigned char *img = new unsigned char[786432];
	/*------------------------Ū��---------------------------*/
	FILE *filein = fopen("C_code_rotation.bmp", "rb");
	fread(head, 1, 54, filein);
	fread(arr, 1, 786432, filein);

	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (k == 0)
				{
					img[(3 * j + k + 2) + i * 3 * 512] = arr[(3 * j + k) + i * 3 * 512];
				}
				else
				{
					img[(3 * j + k - 1) + i * 3 * 512] = arr[(3 * j + k) + i * 3 * 512];
				}
			}
		}
	}
	/*------------------------�g��---------------------------*/
	FILE *fileout = fopen("C_code_Channel_Interchange.bmp", "wb");
	fwrite(head, 1, 54, fileout);
	fwrite(img, 1, 786432, fileout);

	fclose(filein);
	fclose(fileout);
}
/**************************************************************/
/****************************Bonus*****************************/
void image4()//
{
	unsigned char arr_1[442422];
	/*------------------------Ū��---------------------------*/
	FILE *filein = fopen("lena_cropped.bmp", "rb");
	fread(arr_1, 1, sizeof(arr_1), filein);
	/*------------------------�g��---------------------------*/
	FILE *fileout = fopen("C_code_lena_cropped.bmp", "wb");
	fwrite(arr_1, 1, sizeof(arr_1), fileout);

	fclose(filein);
	fclose(fileout);

	/*------------------�ŧi���Y�B���e��T---------------------*/
	unsigned char head[54];
	unsigned char *arr = new unsigned char[442368];
	unsigned char *img = new unsigned char[442368];

	/*------------------------Ū��---------------------------*/
	filein = fopen("lena_cropped.bmp", "rb");
	fread(head, 1, 54, filein);
	fread(arr, 1, 442368, filein);

	/*-------------------���Y�ɪ��e����-----------------------*/
	head[17] = head[17] ^ head[21];
	head[21] = head[17] ^ head[21];
	head[17] = head[17] ^ head[21];

	head[18] = head[18] ^ head[22];
	head[22] = head[18] ^ head[22];
	head[18] = head[18] ^ head[22];

	head[19] = head[19] ^ head[23];
	head[23] = head[19] ^ head[23];
	head[19] = head[19] ^ head[23];

	head[20] = head[20] ^ head[24];
	head[24] = head[20] ^ head[24];
	head[20] = head[20] ^ head[24];

	/*------------------------����---------------------------*/
	for (int i = 0; i < 288; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				img[288 * 3 * (511 - j) + 3 * i + k] = arr[(3 * j + k) + i * 3 * 512];
			}
		}
	}
	/*------------------------�g��---------------------------*/
	fileout = fopen("C_code_lena_cropped_rotation.bmp", "wb");
	fwrite(head, 1, 54, fileout);
	fwrite(img, 1, 442368, fileout);

	fclose(filein);
	fclose(fileout);
}
/**************************************************************/
#include <cstdio>
#include <stdlib.h>
#include <iostream> 
#include <time.h>  
#include <vector>

using namespace std;

int main()
{
	FILE *filein;
	FILE *fileout;
	/************************Binarized image(threshold 128)**********************/
	unsigned char head[54];
	unsigned char *arr = new unsigned char[786432];
	unsigned char *img = new unsigned char[786432];
	/*------------------------Ū��---------------------------*/
	filein = fopen("lena512.bmp", "rb");
	fread(head, 1, 54, filein);
	fread(arr, 1, 786432, filein);
	float B, G, R;
	//Gray = 0.299*R + 0.587*G + 0.114*B
	/*------------------------RGB��Ƕ�---------------------------*/
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (k == 0)//B
				{
					B = 0.114*arr[(3 * j + k) + i * 3 * 512];
				}
				if (k == 1)//G
				{
					G = 0.587*arr[(3 * j + k) + i * 3 * 512];
				}
				if (k == 2)//R
				{
					R = 0.299*arr[(3 * j + k) + i * 3 * 512];
				}
			}
			for (int k = 0; k < 3; k++)
			{
				img[(3 * j + k) + i * 3 * 512] = R + G + B;
			}
		}
	}
	/*------------------------2�Ȥ�---------------------------*/
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (img[(3 * j + k) + i * 3 * 512] <= 128)
				{
					img[(3 * j + k) + i * 3 * 512] = 0;
				}
				else
				{
					img[(3 * j + k) + i * 3 * 512] = 255;
				}
			}
		}
	}
	/*------------------------�g��---------------------------*/
	fileout = fopen("lena512_binarized.bmp", "wb");
	fwrite(head, 1, 54, fileout);
	fwrite(img, 1, 786432, fileout);

	fclose(filein);
	fclose(fileout);
	/**************************************************************/
	/************************Connected Component**********************/
	unsigned char head_test[54];
	unsigned char *arr_test = new unsigned char[786432];
	unsigned char *img_test = new unsigned char[786432];
	/*------------------------Ū��---------------------------*/
	filein = fopen("test2.bmp", "rb");
	fread(head_test, 1, 54, filein);
	fread(arr_test, 1, 786432, filein);
	
	/*-----------------------------------------------------------*/
	/*------------------------2�Ȥ�---------------------------*/
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (arr_test[(3 * j + k) + i * 3 * 512] <= 128)
				{
					arr_test[(3 * j + k) + i * 3 * 512] = 0;
				}
				else
				{
					arr_test[(3 * j + k) + i * 3 * 512] = 255;
				}
			}
		}
	}
	/*-----------------------------------------------------------*/
	int Label = 0;
	int equ_pair[500][2];
	int count_equ = 0;
	/*--------------------��l�ƦsLabel���x�}---------------------*/
	for (int i = 0; i < 786432; i++)
	{
		img_test[i] = 0;
	}
	/*--------------------�sLabel���x�}---------------------*/
	for (int y = 0; y < 512; y++)
	{
		for (int x = 0; x < 512; x++)
		{
			for (int z = 0; z < 3; z++)
			{
				if (arr_test[y * 512 * 3 + x * 3 + z] == 255)
				{
					if (y == 0 && x != 0)//�W����ɰ��D
					{
						///////////////////////////////�Y"��"������//////////////////////////////
						if (img_test[y * 512 * 3 + x * 3 + (z - 1)] != 0)
						{
							img_test[y * 512 * 3 + x * 3 + z] = img_test[y * 512 * 3 + x * 3 + (z - 1)];//�ƻs"��"��Label
						}
						///////////////////////////////�Y"��"�L����//////////////////////////////
						if (img_test[y * 512 * 3 + x * 3 + (z - 1)] == 0)
						{
							Label = Label + 1;
							img_test[y * 512 * 3 + x * 3 + z] = Label;//�s�W�@�ӷsLabel
						}
					}
					if (y != 0 && x == 0 && z == 0)//������ɰ��D
					{
						///////////////////////////////�Y"�W"������//////////////////////////////
						if (img_test[(y - 1) * 512 * 3 + x * 3 + z] != 0)
						{
							img_test[y * 512 * 3 + x * 3 + z] = img_test[(y - 1) * 512 * 3 + x * 3 + z];//�ƻs"�W"��Label
						}
						///////////////////////////////�Y"�W"�L����//////////////////////////////
						if (img_test[(y - 1) * 512 * 3 + x * 3 + z] == 0)
						{
							Label = Label + 1;
							img_test[y * 512 * 3 + x * 3 + z] = Label;//�s�W�@�ӷsLabel
						}
					}
					if (y == 0 && x == 0 && z == 0)//���W������ɰ��D
					{
						///////////////////////////////"�W"&"��"���S����//////////////////////////////
						Label = Label + 1;
						img_test[y * 512 * 3 + x * 3 + z] = Label;//�s�W�@�ӷsLabel
							
					}
					if (y != 0 && x != 0)
					{
						///////////////////////////////�Y"�W"&"��"���S����//////////////////////////////
						if (img_test[(y - 1) * 512 * 3 + x * 3 + z] == 0 && img_test[y * 512 * 3 + x * 3 + (z - 1)] == 0)
						{
							Label = Label + 1;
							img_test[y * 512 * 3 + x * 3 + z] = Label;//�s�W�@�ӷsLabel
						}
						///////////////////////////////�Y�u��"�W"������//////////////////////////////
						if (img_test[(y - 1) * 512 * 3 + x * 3 + z] != 0 && img_test[y * 512 * 3 + x * 3 + (z - 1)] == 0)
						{
							img_test[y * 512 * 3 + x * 3 + z] = img_test[(y - 1) * 512 * 3 + x * 3 + z];//�ƻs"�W"��Label
						}
						///////////////////////////////�Y�u��"��"������//////////////////////////////
						if (img_test[(y - 1) * 512 * 3 + x * 3 + z] == 0 && img_test[y * 512 * 3 + x * 3 + (z - 1)] != 0)
						{
							img_test[y * 512 * 3 + x * 3 + z] = img_test[y * 512 * 3 + x * 3 + (z - 1)];//�ƻs"��"��Label
						}
						///////////////////////////////�Y"�W"&"��"��������//////////////////////////////
						if (img_test[(y - 1) * 512 * 3 + x * 3 + z] != 0 && img_test[y * 512 * 3 + x * 3 + (z - 1)] != 0)
						{
							if (img_test[(y - 1) * 512 * 3 + x * 3 + z] > img_test[y * 512 * 3 + x * 3 + (z - 1)])
							{
								img_test[y * 512 * 3 + x * 3 + z] = img_test[y * 512 * 3 + x * 3 + (z - 1)];//�J��Ĭ�A�ƻs�p��Label
								equ_pair[count_equ][0] = img_test[y * 512 * 3 + x * 3 + (z - 1)];//�N���Ĭ�Label�s�Jequivalence pair array
								equ_pair[count_equ][1] = img_test[(y - 1) * 512 * 3 + x * 3 + z];//�N���Ĭ�Label�s�Jequivalence pair array
								count_equ = count_equ + 1;
							}
							if (img_test[(y - 1) * 512 * 3 + x * 3 + z] < img_test[y * 512 * 3 + x * 3 + (z - 1)])
							{
								img_test[y * 512 * 3 + x * 3 + z] = img_test[(y - 1) * 512 * 3 + x * 3 + z];//�J��Ĭ�A�ƻs�p��Label
								equ_pair[count_equ][0] = img_test[(y - 1) * 512 * 3 + x * 3 + z];//�N���Ĭ�Label�s�Jequivalence pair array
								equ_pair[count_equ][1] = img_test[y * 512 * 3 + x * 3 + (z - 1)];//�N���Ĭ�Label�s�Jequivalence pair array
								count_equ = count_equ + 1;
							}
							if (img_test[(y - 1) * 512 * 3 + x * 3 + z] == img_test[y * 512 * 3 + x * 3 + (z - 1)])
							{
								img_test[y * 512 * 3 + x * 3 + z] = img_test[(y - 1) * 512 * 3 + x * 3 + z];//�J��Ĭ�A�ƻs�p��Label
							}
						}
					}
					//img_test[y * 512 * 3 + x * 3 + z] = arr_test[(3 * x + z) + y * 3 * 512];
					//printf("x=%d y=%d Label=%d\n", x, y, img_test[y * 512 * 3 + x * 3 + z]);
					//system("pause");
				}
				
			}
		}
	}
	//system("pause");
	/*----------------------------------�Nequivalence pair ���� equivalence table----------------------------------*/
	int count_tab1, count_tab2;
	int reg;
	unsigned int equ_tab[500][500];//���]��15�Ӥ��P�ϰ�
	//vector <vector<unsigned int>> equ_tab(count_tab1, vector<unsigned int>(count_tab2));

	count_tab1 = 0;

	for (int i = 0; i < count_equ; i++)
	{
		if (equ_pair[i][0] != 0)
		{
			count_tab2 = 0;
			equ_tab[count_tab1][count_tab2] = equ_pair[i][0];
			equ_tab[count_tab1][count_tab2 + 1] = equ_pair[i][1];
			reg = count_tab2 + 1;
			equ_pair[i][0] = 0;
			equ_pair[i][1] = 0;
			for (int j = 0; j < count_equ; j++)
			{
				if (equ_pair[j][0] == equ_tab[count_tab1][count_tab2])
				{
					count_tab2 = reg + 1;
					reg = reg + 1;
					equ_tab[count_tab1][count_tab2] = equ_pair[j][1];
					equ_pair[j][0] = 0;
					equ_pair[j][1] = 0;
				}
			}
			for (int k = 0; k < count_equ; k++)
			{
				if (equ_pair[k][1] == equ_tab[count_tab1][count_tab2])
				{
					count_tab2 = reg + 1;
					reg = reg + 1;
					equ_tab[count_tab1][count_tab2] = equ_pair[k][0];
					equ_pair[k][0] = 0;
					equ_pair[k][1] = 0;
				}
			}
			count_tab1 = count_tab1 + 1;
		}
	}
		/*----------------------------------��sLabel���x�}----------------------------------*/
		for (int y = 0; y < 512; y++)
		{
			for (int x = 0; x < 512; x++)
			{
				for (int z = 0; z < 3; z++)
				{
					if (img_test[y * 512 * 3 + x * 3 + z] != 0)//�Y����줧�e�]�w��Label�h�h�P�_��Label���Q��s����?
					{
						for (int i = 0; i < count_tab1; i++)
						{
							for (int j = 0; j < count_tab2; j++)
							{
								if (img_test[y * 512 * 3 + x * 3 + z] == equ_tab[count_tab1][count_tab2])
									img_test[y * 512 * 3 + x * 3 + z] = count_tab1 + 1;
							}
						}
					}
				}
			}
		}
		/*----------------------------------�ϰ�W��----------------------------------*/
		int *color = new int[count_tab1];
		srand(time(NULL));
		for (int i = 0; i < count_tab1; i++)
		{
			color[i] = rand() % 256;
			/*printf("color[%d]=%d\n", i, color[i]);
			system("pause");*/
		}
		for (int y = 0; y < 512; y++)
		{
			for (int x = 0; x < 512; x++)
			{
				for (int z = 0; z < 3; z++)
				{
					if (img_test[y * 512 * 3 + x * 3 + z] != 0)//�N��O�ϰ�
					{
						img_test[y * 512 * 3 + x * 3 + z] = color[img_test[y * 512 * 3 + x * 3 + z] - 1];
					}
				}
			}
		}
	
	/*------------------------�g��---------------------------*/
   	fileout = fopen("Connected_Component_test2.bmp", "wb");
	fwrite(head_test, 1, 54, fileout);
	fwrite(img_test, 1, 786432, fileout);

	fclose(filein);
	fclose(fileout);
}

#include "Matting.h"


#include<algorithm>
using namespace std;


Matting::Matting(ImpressionistDoc* p)
{
	pDoc = p;
	width = pDoc->m_nWidth;
	height = pDoc->m_nHeight;

	newAlphaBitmap = new int[width * height];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			newAlphaBitmap[i * width + j] = 0.0;
		}
	}
}

void Matting::solveAlpha()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{

			if (*(pDoc->m_ucPainting + (j + i * width) * 3) != 255 &&
				*(pDoc->m_ucPainting + (j + i * width) * 3) != 0)
			{
				//std::cout <<(int)*(pDoc->m_mattingAlphaBitmap + (j + i * width) * 3) << '\n';
				double* k = new double[10];
				int* alphas = new int[10];
				for (int a = 0; a < 10; a++)
				{
					k[a] = 0.99;
					alphas[a] = 1;
				}

				bool flag = 0;



				for (int  y= -5; y < 5; y++)
				{
					for (int x = -5; x < 5; x++)
					{
						if (j + x <0 || j + x >width || i + y<0 || i + y>height - 1)
							continue;
						if(sqrt(pow(y*1.0,2.0)+pow(x*1.0,2.0))>5)
							continue;
						if (*(pDoc->m_ucPainting + (j+x + (i+y) * width) * 3) == 255 ||
							*(pDoc->m_ucPainting + (j+x + (i+y) * width) * 3) == 0)
						{
							double distance = findingDistance(j, i, x+j,y+i);


							if (distance < sqrt(3 * pow(5 / 255, 2.0)))
							{
								flag = 1;
								newAlphaBitmap[j + i * width] = *(pDoc->m_ucPainting + (j + x + (i + y) * width) * 3);
								for (int a = 0; a < 3; a++)
									*(pDoc->m_ucPainting + (j + i * width) * 3 + a) = newAlphaBitmap[j + i * width];
								break;
							}
								
							//double weight = 1 - distance / 4;
							int p = 0;
							for (int i = 1; i < 10; i++)
							{
								if (k[p] < k[i])
									p = i;
							}

							if (distance < k[p])
							{
								k[p] = distance;
								alphas[p] = *(pDoc->m_ucPainting + (j+x + (i+y) * width) * 3);
							}
						}


					}
				}

				if (!flag)
				{
					double sum = 0.0;
					double new_alpha = 0.0;
					for (int a = 0; a < 10; a++)
						sum += (1 - k[a]);
					for (int a = 0; a < 10; a++)
						 new_alpha += (1 - k[a]) / sum * alphas[a] /255;
					newAlphaBitmap[j + i * width] = 255 * new_alpha;
				}

				for (int a = 0; a < 3; a++)
					*(pDoc->m_ucPainting + (j + i * width) * 3 + a) = newAlphaBitmap[j + i * width] ;


				std::cout << newAlphaBitmap[j + i * width] << '\n';

			}

		}
	}
}





double Matting::findingDistance(int x1,int y1,int x2,int y2)
{
	double color1;
	double color2;
	double d = 0.0;

	for (int a = 0; a < 3; a++)
	{
		color1 = *(pDoc->m_ucBitmap + (x1 + y1 * width) * 3 + a) / 255;
		color2 = *(pDoc->m_ucBitmap + (x2 + y2 * width) * 3 + a) / 255;
		d += pow(color1 - color2, 2.0);
	}


	//d += (pow(x1 - x2, 2.0) / (pow(width, 2.0) + pow(height, 2.0)));
	//d += (pow(y1 - y2, 2.0) / (pow(width, 2.0) + pow(height, 2.0)));
	
	return sqrt(d);

}




void Matting::generateNewAlphaBitmap()
{
	//for (int i = 0; i < height; i++)
	//{
	//	for (int j = 0; j < width; j++)
	//	{

	//		for (int a = 0; a < 3; a++)
	//		{
	//			*(pDoc->m_mattingAlphaBitmap + (j + i * width) * 3+a) += newAlphaBitmap[j + i * width] * 255;
	//			*(pDoc->m_ucPainting + (j + i * width) * 3+a) = *(pDoc->m_mattingAlphaBitmap + (j + i * width) * 3+a);
	//		}
	//			

	//	}
	//}
}
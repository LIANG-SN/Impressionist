

#include "Bayesian.h"


Bayesian::Bayesian(ImpressionistDoc* p)
{
	pDoc = p;
	pDoc = p;
	width = pDoc->m_nWidth;
	height = pDoc->m_nHeight;

	newAlphaBitmap = new int[width * height];
	pixels = new Vector3d [width * height];
	frontground=new Cluster[clusterSize];
	background = new Cluster[clusterSize];
	unknown = new Cluster;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int a = 0; a < 3; a++)
				pixels[i*width + height](a) = *(pDoc->m_ucBitmap + 3 * (i * width + j) + a);


			if (*(pDoc->m_mattingAlphaBitmap + 3 * (j + height * i)) == 255)
			{
				newAlphaBitmap[i * width + j] = 1;
				frontground->add_pair(Point(j, i), pixels + i * width + j);
			}
			else if (*(pDoc->m_mattingAlphaBitmap + 3 * (j + height * i)) == 0)
			{
				newAlphaBitmap[i * width + j] = 0;
				background->add_pair(Point(j, i), pixels + i * width + j);
			}
			else
			{
				newAlphaBitmap[i * width + j] = 0;
				unknown->add_pair(Point(j, i), pixels + i * width + j);
			}
				
		}
	}
}

void Bayesian::solve()
{
	for (int i = 1; i < clusterSize; i++)
	{
		int p = 0;
		double max_eigen = 0.0;
		for (int j = 0; j < i; j++)
		{
			frontground[j].set_mean();
			frontground[j].set_split_cov();
			frontground[j].set_eigen();
			if (frontground[i].eigenvalue >= max_eigen)
			{
				p = i;
				max_eigen = frontground[i].eigenvalue;
			}
		}

		frontground[p].split(frontground + i);
	}


	for (int i = 1; i < clusterSize; i++)
	{
		int p = 0;
		double max_eigen = 0.0;
		for (int j = 0; j < i; j++)
		{
			background[j].set_mean();
			background[j].set_split_cov();
			background[j].set_eigen();
			if (background[i].eigenvalue >= max_eigen)
			{
				p = i;
				max_eigen = background[i].eigenvalue;
			}
		}

		background[p].split(background + i);
	}


	int filterSize = 20;
	double** Gaussian_filter = new double* [filterSize];
	double sigma = 8.0;
	// sum is for normalization 
	double sum = 0.0;
	int i, j;
	for (i = 0; i < filterSize; i++) {
		Gaussian_filter[i] = new double[filterSize];
		for (j = 0; j < filterSize; j++) {
			// using gaussian function to generate gaussian filter 

			Gaussian_filter[i][j] = exp(-(pow((i - filterSize / 2) * 1.0, 2.0) + pow((j - filterSize / 2) * 1.0, 2.0)) / (2 * sigma * sigma)) / (M_PI * sigma * sigma * 2);
		}
	}



	int nAllF = 0;
	for (int i = 1; i < clusterSize; i++)
	{
		int size = frontground[i].members.size();
		nAllF += size;


		double allWeight = 0;
		for (int j = 0; j < size; j++)
		{
			double curWeight = 0;
			int x = frontground[i].members[j].first.x;
			int y = frontground[i].members[j].first.y;
			for(int a=-filterSize/2;a<filterSize/2;a++)
				for (int b = -filterSize / 2; b < filterSize / 2; b++)
				{
					int cx = x + a;
					int cy = y + b;
					Vector3d
					if (cx > 0 && cx < width - 1 && y>0 && y < height - 1)
						if (newAlphaBitmap[cx + cy * width] == 1)
						{
							curWeight += Gaussian_filter[x + filterSize / 2][y + filterSize / 2];
							frontground[i].w_mean
						}
				}

			allWeight += curWeight;
		}

	}


}
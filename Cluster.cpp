//
//#include "Cluster.h"
//
//
//Cluster::Cluster()
//{
//	mean << 0, 0, 0;
//	split_cov.setZero();
//}
//
//void Cluster::split(Cluster* c1, Cluster* c2)
//{
//	JacobiSVD<MatrixXd> svd(split_cov, ComputeThinU | ComputeThinV);
//
//	Matrix3d U = svd.matrixU();
//
//	Matrix3d V = svd.matrixV();
//
//	Matrix3d S = U.inverse() * split_cov * V.transpose().inverse();
//
//	eigenvalue = S(0, 0);
//	eigenvector(0) = U(0, 0); 
//	eigenvector(1) = U(1, 0); 
//	eigenvector(2) = U(2, 0);
//
//	int size = members.size();
//	for (int i = 0; i < size; i++)
//	{
//		if (eigenvector.transpose() * (*members[i].second) <= eigenvector.transpose() * mean)
//			c1->add_pair(members[i]);
//		else
//			c2->add_pair(members[i]);
//	}
//
//
//	//std::cout << S << '\n';
//}
//
//void Cluster::add_pair(Point f, Vector3d* s)
//{
//	std::pair<Point, Vector3d*> a(f, s);
//	members.push_back(a);
//}
//
//void Cluster::add_pair(std::pair<Point, Vector3d*> a)
//{
//	members.push_back(a);
//}
//
//void Cluster::set_mean()
//{
//	int size = members.size();
//	for (int i = 0; i < size; i++)
//	{
//		mean = mean + *(members[i].second);
//	}
//
//	mean = mean / size;
//}
//
//void Cluster::set_split_cov()
//{
//	int size = members.size();
//	for (int i = 0; i < size; i++)
//	{
//		split_cov +=  (*(members[i].second) - mean) * (*(members[i].second) - mean).transpose();
//	}
//	split_cov /= size;
//}
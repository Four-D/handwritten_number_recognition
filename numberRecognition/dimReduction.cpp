
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "opencv2/opencv.hpp"
#include "readUbyte.h"
#include "dimReduction.h"

using namespace std;
using namespace cv;


Mat dimReduction::toGrayscale(InputArray _src) 
{
	Mat src = _src.getMat();
	// only allow one channel
	if (src.channels() != 1) {
		CV_Error(CV_StsBadArg, "Only Matrices with one channel are supported");
	}
	// create and return normalized image
	Mat dst;
	cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
	return dst;
}

void dimReduction::showMessage(const string method, int inputLength, int outputLength)
{
	cout << "==================================================================" << endl;
	cout << " Dimension reduction method:" << setw(37) << method << endl;
	cout << " Length of input vector:" << setw(41) << inputLength << endl;
	cout << " Length of output vector:" << setw(40) << outputLength << endl;
	cout << "------------------------------------------------------------------" << endl;
}


//�����н�ά���������ݽ��й�һ��
void onlynormalize::Init(const Mat &trainData, double retainedVariance, bool readfromfile)
{
	if (trainData.cols != featureLen)
	{
		cout << "onlynormalize �������ݳ��Ȳ�����Ҫ��" << endl;
		exit(-1);
	}

	return;
}

void onlynormalize::protect(const Mat &alldata, Mat &result)
{
	result = alldata.clone();
	Mat result_i;
	for (unsigned int i = 0; i < alldata.rows; i++)
	{
		result_i = result.row(i);							//������������һ����������
		normalize(result_i, result_i);						//��һ��
	}

	showMessage("onlynormalize", featureLen, featureLen);	//�����ά�����Ϣ

	return;
}

void onlynormalize::backProtect(const Mat &data, Mat &result)
{
	if (data.cols != featureLen)
	{
		cout << "onlynormalize ����ͶӰ�������ݳ��Ȳ�����Ҫ��" << endl;
		exit(-1);
	}

	result = toGrayscale(data);

	return;
}

//PCA��ά                          
const string PCAdimReduction::PCA_MEAN = "mean";
const string PCAdimReduction::PCA_EIGEN_VECTOR = "eigen_vector";

void PCAdimReduction::Init(const Mat &trainData, double retainedVariance, bool readfromfile)
{
	if (readfromfile == true)
	{
		FileStorage fs_r("PCA.xml", FileStorage::READ);
		fs_r[PCA_MEAN] >> pca.mean;
		fs_r[PCA_EIGEN_VECTOR] >> pca.eigenvectors;
		fs_r.release();
	}
	else
	{
		if (trainData.cols != featureLen)
		{
			cout << "PCA�������ݳ��Ȳ�����Ҫ��" << endl;
			exit(-1);
		}

		pca.computeVar(trainData, cv::Mat(), CV_PCA_DATA_AS_ROW, retainedVariance);

		FileStorage fs_w("PCA.xml", FileStorage::WRITE); 
		fs_w << PCA_MEAN << pca.mean;
		fs_w << PCA_EIGEN_VECTOR << pca.eigenvectors;
		fs_w.release();
	}

	showMessage("PCA", featureLen, pca.eigenvalues.rows);			//�����ά�����Ϣ

	return;
}

//����ȫ�����ݹ��ɵľ��󣬵õ���ά����һ���ľ���
void PCAdimReduction::protect(const Mat &alldata, Mat &result)
{
	if (alldata.cols != featureLen)
	{
		cout << "PCA�����������ݳ��Ȳ�����Ҫ��" << endl;
		exit(-1);
	}

	result.create(alldata.rows, pca.eigenvalues.rows, CV_32FC1);
	Mat result_i;
	for (unsigned int i = 0; i < alldata.rows; i++)
	{
		result_i = result.row(i);					//������������һ����������
		pca.project(alldata.row(i), result_i);		//PCA��ά
		normalize(result_i, result_i);				//��һ��
	}

	return;
}

void PCAdimReduction::backProtect(const Mat &data, Mat &result)
{
	if (data.cols != pca.eigenvalues.cols)
	{
		cout << "PCA����ͶӰ�������ݳ��Ȳ�����Ҫ��" << endl;
		exit(-1);
	}

	pca.backProject(data, result);
	result = toGrayscale(result);

	return;
}

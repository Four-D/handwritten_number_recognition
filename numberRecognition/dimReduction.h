

#ifndef __DIMREDUCTION_
#define __DIMREDUCTION_

#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


class dimReduction
{
public:
	virtual void Init(const Mat &trainData, double retainedVariance = 0.95, bool readfromfile = true) = 0;	//��ά���߳�ʼ����������������־��ά���߾�����Ϣ�Ӵ洢�ļ��л��
	virtual void protect(const Mat &data, Mat &result) = 0;													//��һ������ͶӰ�����ɷֿռ���
	virtual void backProtect(const Mat &data, Mat &result) = 0;												//�����ɷֿռ��е�����
protected:
	void showMessage(const string method, int inputLength, int outputLength);								//��ʾ��ά��Ϣ
	Mat toGrayscale(InputArray _src);
};

class onlynormalize : public dimReduction
{
public:
	void Init(const Mat &trainData, double retainedVariance = 0, bool readfromfile = 0);
	void protect(const Mat &data, Mat &result);
	void backProtect(const Mat &data, Mat &result);
};

class PCAdimReduction : public dimReduction
{
public:
	void Init(const Mat &trainData, double retainedVariance = 0.95, bool readfromfile = 0);
	void protect(const Mat &data, Mat &result);
	void backProtect(const Mat &data, Mat &result);
private:
	cv::PCA pca;
	static const string PCA_MEAN;
	static const string PCA_EIGEN_VECTOR;
};

#endif
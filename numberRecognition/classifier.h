
#ifndef __CLASSIFIER_
#define __CLASSIFIER_

#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"

#include "readUbyte.h"

using namespace std;
using namespace cv;


class classifier
{
public:
	enum trainWay { HAND, AUTO };
	virtual void read(const string &XMLfilename) = 0;
	virtual void train(const vector<NumTrainData>& trainData, trainWay trainway = HAND) = 0;//ѵ��������,Ĭ�ϲ�ʹ���Զ����Σ�̫�ķ�ʱ�䣩
	virtual void predict(const vector<NumTrainData>& predictData) = 0;	//��ѵ���õķ��������м��
protected:
	void show(string classifierName,int total, int right, int error);
};

class randomForest: public classifier
{
public:
	void read(const string &XMLfilename);
	void train(const vector<NumTrainData>& trainData, trainWay trainway = HAND);
	void predict(const vector<NumTrainData>& predictData);
private:
	CvRTrees forest;
};

class SVMclassifier: public classifier
{
public:
	void read(const string &XMLfilename);
	void train(const vector<NumTrainData>& trainData, trainWay trainway = HAND);
	void predict(const vector<NumTrainData>& predictData);
private:
	CvSVM svm;
	CvSVMParams param;
	CvTermCriteria criteria;
};

#endif
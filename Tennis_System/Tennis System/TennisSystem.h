#include <stdio.h>
#include <math.h>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define MAXSIZE 1000


//������Ƶ����
const char* videoName = "MVI_8834.AVI";


//��ʾ����Ƶ����
const char*  video = "video    ";

//�����ַ��ĳ���
int s1;
int s2;


//����IplImageָ��
IplImage* pFrame = NULL;	//frame       
IplImage* pFrImg = NULL;	//forground   
IplImage* pBkImg = NULL;	//background  
IplImage* pEqlImg = NULL;	//equalized
IplImage* pBinImg = NULL;	//binary
IplImage* pTrpImg = NULL;   //point tracking
IplImage* pTrjImg = NULL;   //trajectory tracking
IplImage* pHough  = NULL;   //the image after hough transformation
IplImage* pCanny = NULL;    //the image after canny 
IplImage* pJudge = NULL;    //the image used for display judging
IplImage* pOper = NULL;    //the image after human operation


//����CvMat����cvRunningAvg��Ҫʹ�þ���
CvMat* pFrameMat = NULL;
CvMat* pFrMat = NULL;
CvMat* pBkMat = NULL;
//����ʵ��Surendra�㷨�������������
CvMat* temp = NULL;
CvMat* Dxy = NULL;

//������ȡ��Ƶ��Ҫ�õ�ָ��
CvCapture* pCapture = NULL;


//��ʼ������
CvSeq* contour = 0;
CvSeq* lines = 0;


//��������
CvRect r;


//��̬ѧ�õĲ���
IplConvKernel* element;


//���������ж�����Ҫ�õ�������
CvFont font;


//��������Ĵ�С
CvSize text_size1;
CvSize text_size2;

//���������ݽṹ
CvPoint pt1;
CvPoint pt2;



//�����л��õ������ͱ���
int nFrmNum = 0;
int i = 0;
int k = 0;
int m = 0;
int ymin = 0;
int down = 0;


//����ʶ����������Ҫ�õĵ�
int Xa;
int Ya;
int Xb;
int Yb;


//��¼������֡����
int bounceFrmNum[MAXSIZE];


//��������
int Xo;
int Yo;

//������������������
int centerX[MAXSIZE];
int centerY[MAXSIZE];

//����Ƶ��ʵ����ͣ��ťֵ
int g_switch_value = 1;


//��ά�������ڴ洢hough�任���ֵĿ��ܵ�ֱ���ϵĵ�
int point[MAXSIZE][2];


//���������ж�����ָ���ĳ��������ڻ�������
bool judging;


//������������ָ���ĳ��������ڻ�������
int fx;


//������¼������������������
int Xg[MAXSIZE]; //g means ground
int Yg[MAXSIZE];


//������¼ֱ�߷��̽���ı���
int Xs;    //s means intersect
int Ys;


//��������״̬��һ��bool����
bool lastGrounded;//������ʾ��һ���Ƿ���������


//������������������������
int xi0;
int yi0;
int xi1;
int yi1;
int xi2;
int yi2;
int xi3;
int yi3;




#include "stdafx.h"
#include <stdio.h>
#include <math.h>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
 
int main( int argc, char** argv )
{
    	IplImage* img;
		IplImage* mask2; //����IplImageָ��

		CvRect r;
 
        //����ͼ��
        img = cvLoadImage( "tennis_input.jpg",1);

		cvNamedWindow( "originalImage", 1 );//��������
        cvShowImage( "originalImage", img );//��ʾͼ��
   
		CvSize size = cvGetSize(img);
		IplImage *hsv = cvCreateImage(size, IPL_DEPTH_8U, 3);
		cvCvtColor(img, hsv, CV_BGR2HSV);  


		CvMat *mask = cvCreateMat(size.height, size.width, CV_8UC1);
		mask2	 = cvCreateImage(size, IPL_DEPTH_8U,3);
		
		cvInRangeS(hsv, cvScalar(0.11*256, 0.60*256, 0.20*256, 0), cvScalar(0.14*256, 1.00*256, 1.00*256, 0), mask);
		cvReleaseImage(&hsv);

		IplConvKernel *se21 = cvCreateStructuringElementEx(21, 21, 10, 10, CV_SHAPE_RECT, NULL);
		IplConvKernel *se11 = cvCreateStructuringElementEx(11, 11, 5,  5,  CV_SHAPE_RECT, NULL);

		cvNamedWindow( "Mask before", 1 );//��������
        cvShowImage( "Mask before", mask );//��ʾͼ��
		//cvClose(mask, mask, se21);  See completed example for cvClose definition
		cvDilate(mask, mask, se21,1);
		cvErode(mask, mask, se21,1);	
		//cvOpen(mask, mask, se11);  See completed example for cvOpen  definition
		cvErode(mask, mask, se11,1);
		cvDilate(mask, mask, se11,1);	
		cvReleaseStructuringElement(&se21);
		cvReleaseStructuringElement(&se11);

		/* Copy mask into a grayscale image */
		IplImage *hough_in = cvCreateImage(size, 8, 1);
		cvCopy(mask, hough_in, NULL);
		cvCvtColor( mask, mask2, CV_GRAY2BGR );
		
        cvSmooth(hough_in, hough_in, CV_GAUSSIAN, 15, 15, 0, 0);

		/* Run the Hough function */
		CvMemStorage *storage = cvCreateMemStorage(0);
		//CvSeq *circles = cvHoughCircles(hough_in, storage, CV_HOUGH_GRADIENT, 4, size.height/10, 50, 150, 0, 0);
		CvSeq *contour;
		cvFindContours( hough_in, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );


		for(;contour;contour=contour->h_next)
		{
			    r=((CvContour*)contour)->rect;
	
				//��ͼ�л������ο�
				cvRectangle(img,cvPoint(r.x,r.y),cvPoint(r.x+r.width,r.y+r.height),CV_RGB(255,0,0),3,CV_AA,0);
				cvRectangle(mask2,cvPoint(r.x,r.y),cvPoint(r.x+r.width,r.y+r.height),CV_RGB(255,0,0),3,CV_AA,0);

		}

		cvReleaseMemStorage(&storage);
	


		cvNamedWindow( "Image", 1 );//��������
        cvShowImage( "Image", img );//��ʾͼ��
		cvNamedWindow( "Mask", 1 );//��������
        cvShowImage( "Mask", mask );//��ʾͼ��

		cvNamedWindow( "Mask2", 1 );//��������
        cvShowImage( "Mask2", mask2 );//��ʾͼ��



	    cvWaitKey(0); //�ȴ�����
        cvDestroyWindow( "Image" );//���ٴ���
        cvReleaseImage( &img ); //�ͷ�ͼ��
		cvDestroyWindow( "Mask" );//���ٴ���
		cvDestroyWindow( "Mask before" );//���ٴ���
		cvDestroyWindow( "Original" );//���ٴ���
		cvDestroyWindow( "Mask2" );//���ٴ���
        return 0;


}
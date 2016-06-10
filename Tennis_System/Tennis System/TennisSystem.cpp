#include "stdafx.h"
#include "TennisSystem.h"
#include "Interaction.h"
#include "Calculation.h"


// main�������

int main( int argc, char** argv )
{
	//�ϲ���Ƶ�ļ���video�ַ���������ʾ�ڴ��ڵ����Ͻ�
	s1 = strlen(video); 
	s2 = strlen(videoName); 
	char*  displayName= new char[s1+s2];

	memset(displayName,0,s1+s2);
	memcpy(displayName,video,s1);
	memcpy(displayName+s1,videoName,s2); 

	//��������ڴ�
	CvMemStorage* storageContour = cvCreateMemStorage(0);
	CvMemStorage* storageHough = cvCreateMemStorage(0);

	//��������
	cvNamedWindow("background",1);
	cvNamedWindow("binary",1);
	cvNamedWindow("background model",1);
	cvNamedWindow("point tracking",1);
	cvNamedWindow(displayName, 1);
	cvNamedWindow("trajectory tracking",1);
	cvNamedWindow("judging display",1);

	//���ڴ��ڴ�С
	cvResizeWindow("judging display",250,200);

	//����trackbar������ʵ�ֿ���
	cvCreateTrackbar("Pause/Run",displayName, &g_switch_value, 1, switch_callback );

	//�������Ļص�����
	cvSetMouseCallback("trajectory tracking",mouse_callback1 ,0);
	cvSetMouseCallback("point tracking",mouse_callback2 ,0);
	cvSetMouseCallback("judging display",mouse_callback3 ,0);

	//ʹ������������
	cvMoveWindow(displayName, 0, 0);
	cvMoveWindow("background", 0, 0);
	cvMoveWindow("binary", 0, 0);
	cvMoveWindow("background model", 0, 0);
	cvMoveWindow("point tracking", 0, 0);
	cvMoveWindow("trajectory tracking", 620, 250);
	cvMoveWindow("judging display", 800, 0);

	//��ʼ������
	cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX, 3, 3, 0.0, 5, CV_AA );

	//�趨����Ĵ�С    
	cvGetTextSize( "IN", &font, &text_size1, &ymin );
	cvGetTextSize( "OUT", &font, &text_size2, &ymin );	


	//���������λ��
	pt1.x = (250 - text_size1.width)/2;
	pt1.y = (200 + text_size1.height)/2;

	pt2.x = (250 - text_size2.width)/2;
	pt2.y = (200 + text_size2.height)/2;


	//���Ҫʹ������ͷ���뽫 if 0 ��Ϊ if 1
#if 0 
  //������ͷ
    if( !(pCapture = cvCaptureFromCAM(-1)))
    {
		fprintf(stderr, "Can not open camera.\n");
		return -1;
    }

#else
  //����Ƶ�ļ�
    if( !(pCapture = cvCaptureFromFile(videoName)))
    {
		fprintf(stderr, "Can not open video file.\n");
		return -1;
    }
#endif
	
	//��ʼ������������
	contour = cvCreateSeq(CV_SEQ_ELTYPE_POINT,sizeof(CvSeq),sizeof(CvPoint),storageContour);

	//��ʾ�ȼ��Ͱ���
	manual();

	//���ѭ�������ڿ��ص�ʵ��
	printf("case1 g_switch_value %d\n",g_switch_value);
      
    while(1)
	{	   
		//��֡��ȡ��Ƶ
		if(g_switch_value)
		{
			pFrame = cvQueryFrame( pCapture);
			//�����Ƶ�Ѿ����꣬������ѭ��
			if( !pFrame) 
				break;
			nFrmNum++;
			printf("Frame Number: %d \n",nFrmNum);
		}
		else
		{
			//��Ҫ�ȴ��û�����,�����������
			cvWaitKey(0);
			//ѭ�������
			continue;
		}
		
		//����ǵ�һ֡����Ҫ�����ڴ棬����������ʼ��
		if(nFrmNum == 1)
		{
			//��ͼ��ĳ�ʼ��
			pBkImg	 = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			pFrImg	 = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			pEqlImg	 = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			pBinImg	 = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			pTrpImg	 = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,3);
			cvZero( pTrpImg );
			pTrjImg	 = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,3);
			pHough	 = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,3);
			pCanny	 = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
			pJudge   = cvCreateImage(cvSize(250, 200),  IPL_DEPTH_8U,3);
			cvZero( pJudge );	
			pOper    = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,3);

			//�Ծ���ĳ�ʼ��������ʵ�ֱ������µľ���
			pBkMat	 = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
			pFrMat	 = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
			pFrameMat= cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
			//����ʵ��Surendra�㷨
			temp     = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
			Dxy      = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);

			//ת���ɵ�ͨ��ͼ���ٴ���
			cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
			cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);

			//��ͼ��ת��Ϊ����
			cvConvert(pFrImg, pFrameMat);
			cvConvert(pFrImg, pFrMat);
			cvConvert(pFrImg, pBkMat);

			//�Խ�ģ��Ҫʹ�õı�����canny ����
			cvCanny( pBkImg, pCanny, 50, 200, 3 );

			//ת��ΪGRB��ͨ��
			cvCvtColor( pCanny, pHough, CV_GRAY2BGR );

			//����ͼ��
			cvCopy(pHough,pOper);	

			//ʹ��Hough�߱任�ҳ����������ڳ�����ģ���ҳ����п��ܵ�ֱ��
			lines = cvHoughLines2( pCanny, storageHough, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 25 );

			//��ӡ���ҳ����ߵ���Ŀ
			printf("total lines : %d\n",lines->total);

			//�ڳ�������ʾ����������
			for(int j = 0; j < lines->total; j++ )
			{
				CvPoint* linePoint = (CvPoint*)cvGetSeqElem(lines,j);
				//ˮƽ����ĳ����߲�����ʾ������ֻ������ֱ�����ϵĵ���
				if( abs(linePoint[0].y - linePoint[1].y) <25 || abs(linePoint[0].x - linePoint[1].x) >25)
				{
					continue;
				}
				else
				{
					// һ����˵�����߶���������
					if ((linePoint[0].x > 270)&& (linePoint[0].x < 370))
					{
						cvCircle(pHough,linePoint[0],1,CV_RGB(0,0,255),3,8,0);
						//��Ҫ�Գ����ߵı�Ե�����ж�������ȡ���ұ�Ե�����ж���Ȼ��ѡ���������������ֱ��
						point[m][0] =  linePoint[0].x;
						point[m][1] =  linePoint[0].y;
						//��ӡ����ĳ��ص�
						printf("point[%d][0]=%d  point[%d][1]=%d\n",m,point[m][0],m,point[m][1]);
						//��m���±��������
						m++;
					}
					// һ����˵�����߶���������
					if ((linePoint[1].x > 270) && (linePoint[1].x < 370))
					{
						cvCircle(pHough,linePoint[1],1,CV_RGB(0,0,255),3,8,0);	
						//��Ҫ�Գ����ߵı�Ե�����ж�������ȡ���ұ�Ե�����ж���Ȼ��ѡ���������������ֱ��
						point[m][0] =  linePoint[1].x;
						point[m][1] =  linePoint[1].y;
						//��ӡ����ĳ��ص�
						printf("point[%d][0]=%d  point[%d][1]=%d\n",m,point[m][0],m,point[m][1]);
						//��m���±��������
						m++;
					}	
					//��ֱ�ߵ��ӡ����
					printf("%d   linePoint[0] (%d,%d) linePoint[1] (%d,%d) \n",j,linePoint[0].x,linePoint[0].y,linePoint[1].x,linePoint[1].y);	
				}//end else 
			} //end for

			//�Կ��ܵ�ֱ�߽�������
			sort(point,m);
    		//�����������������ӡ
    		for(int d = 0; d < m; d++)
			{
				printf("���������� point[%d][0]=%d  point[%d][1]=%d \n",d,point[d][0],d,point[d][1]);
			}
			//���ҵ���ֱ�ߵ���и�ֵ
			Xa = point[0][0];
			Ya = point[0][1];		
			
			//�м�����������
			Xb = point[3][0];
			Yb = point[3][1];

			//�ڳ��ر����е���cvLine��������������
			cvLine( pHough, cvPoint(Xa,Ya),cvPoint(Xb,Yb),CV_RGB(255,0,0), 5, CV_AA, 0 );		

			//���Ƴ���ͼ�����ڽ�ģ
			cvCopy(pHough,pTrjImg);	
	
		}// end if ����˳�����ģ�Ĳ��������濪ʼ�������ʶ��ʹ���

		//��ʼ�����ʶ��ʹ����Լ��켣׷�٣�������ж�
		else
		{
			cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
			cvConvert(pFrImg, pFrameMat);

			//��˹�˲��ȣ���ƽ��ͼ��
			cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);

			//��ǰ֡������ͼ���
			cvAbsDiff(pFrameMat, pBkMat, pFrMat);

			cvConvert(pFrMat, pFrImg);

			//ֱ��ͼ����
			cvEqualizeHist(pFrImg,pEqlImg );

			//��ֵ��ǰ��ͼ
			cvThreshold(pFrImg, pFrImg, 50, 255.0, CV_THRESH_BINARY);
	 
			//������̬ѧ�˲���ȥ������  

			element = cvCreateStructuringElementEx( 9, 9, 3, 3, CV_SHAPE_RECT, NULL);
			cvMorphologyEx( pFrImg, pFrImg, NULL, element, CV_MOP_OPEN, 1);
			cvDilate(pFrImg,pFrImg,0,2);
		  
			//���ƶ�ֵͼ��������ʾ
			cvCopy(pFrImg,pBinImg);


			//���Ҳ��һ����ſ�����,�ҳ�������֮����б���
			cvFindContours( pFrImg, storageContour, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

			//��������Ŀ�����С����
			for(;contour;contour=contour->h_next)
			{
				r=((CvContour*)contour)->rect;

				if(r.height>20&&r.width>20)
				{	
					//��ͼ�л������ο�
					cvRectangle(pFrame,cvPoint(r.x,r.y),cvPoint(r.x+r.width,r.y+r.height),CV_RGB(255,0,0),1,CV_AA,0);

					//������������
					Xo = r.x+(int)(r.width/2);
					Yo = r.y+(int)(r.height/2);

					//��ӡ��������
					printf("Center Point:(%d,%d)\n",Xo,Yo);

					//��¼��������
					centerX[i] = Xo;
					centerY[i] = Yo;
					printf("Center Point x[%d]y[%d]:(%d,%d)\n",i,i,centerX[i],centerY[i]);
					
					//��Դͼ������ʾ�ҳ������ĵ�λ��
					cvCircle(pFrame,cvPoint(Xo,Yo),1,CV_RGB(0,255,0),3,8,0);

					//�����ĸ���ͼ�л�����ĵ�ǰλ��
					cvCircle(pTrpImg,cvPoint(Xo,Yo),5,CV_RGB(255,255,255),12,8,0);

					//�ڹ켣����ͼ�л�����ĵ�ǰλ��,��Ҫ���б߽��ж�
					if(i>0 && centerX[i-1]<610 && centerY[i-1]>20)
					{	
						//������Ĺ켣����ģ��
						cvLine(pTrjImg,cvPoint(centerX[i-1],centerY[i-1]),cvPoint(centerX[i],centerY[i]),CV_RGB(255,255,255),14,8,0);
					}
					else 
					{
						//�����������±��������,��������ѭ��һ��
						i++;
						continue;
					}
			
					//�������Ƶ����ȡ��������������ȡ
					if(i>1&&(centerY[i] - centerY[i-1]<0) && (centerY[i-1] - centerY[i-2]>0)&&(centerY[i-1]>100))
					{
						
						//��¼�µ�ǰ֡����
						bounceFrmNum[k] = nFrmNum;
						printf("��Ƶ���������� ��x[%d]y[%d]:(%d,%d) FrmNum = %d \n",i-1,i-1,centerX[i-1],centerY[i-1],bounceFrmNum[k]);
						//���е�һ��ģʽ���ж�
						//��ֹһ���������ж�����,һ����˵���������ļ��ʱ����1�����ϣ���֤��ص���ȡ�ľ�ȷ��
						if(((k>0 && (bounceFrmNum[k]-bounceFrmNum[k-1]>30)) || k ==0)
							&& (abs(centerY[i-1] - centerY[i-2])>50) && (abs(centerY[i-1] - centerY[i])>50))

						{

							//�ڱ���ͼ�б�ʾ����������
							cvCircle(pTrpImg,cvPoint(centerX[i-1],centerY[i-1]),5,CV_RGB(0,255,0),12,8,0);

							//�ڳ���ͼ�б�ʾ����������
							cvCircle(pTrjImg,cvPoint(centerX[i-1],centerY[i-1]),2,CV_RGB(0,255,0),12,8,0);

							//��ӡ�����
							printf("�����жϵ��������� ��x[%d]y[%d]:(%d,%d) FrmNum = %d \n",i-1,i-1,centerX[i-1],centerY[i-1],bounceFrmNum[k]);
							
							//�����Ƿ�������ĺ��������жϣ�������õ������߶εļ��㷽ʽ
							judge( Xa, Ya, Xb, Yb, centerX[i-1], centerY[i-1]);


							//��judging display��������ʾ�о�
							if( judging == true )
							{
								//�������ԭ��������
								cvZero( pJudge );
								//��ʾ����
								cvPutText( pJudge, "IN", pt1, &font, CV_RGB(255,255,255));	
							}
							else
							{    
								//�������ԭ��������
								cvZero( pJudge );	
								//��ʾ����
								cvPutText( pJudge, "OUT", pt2, &font, CV_RGB(255,255,255));							
							}
						}//end if ��� ��һ�����ģʽ���ж�

						//���еڶ���ģʽ���ж�
						//��ֹһ���������ж�����,һ����˵���������ļ��ʱ����1�����ϣ���֤��ص���ȡ�ľ�ȷ��
						if(((k>0 && (bounceFrmNum[k]-bounceFrmNum[k-1]>30)) || k ==0)
							&& ((abs(centerY[i-1] - centerY[i-2])<50) || (abs(centerY[i-1] - centerY[i])<50))
							&&((abs(centerY[i] - centerY[i-1])-abs(centerY[i-1] - centerY[i-2]))>0)
							&& (i>2))

						{
						
							//���ֱ�߷��̵Ľ⣬���Xs��Ys
							intersect(centerX[i],centerY[i],centerX[i-1],centerY[i-1],centerX[i-2],centerY[i-2],centerX[i-3],centerY[i-3]);
							
							//����������
							cvLine(pTrjImg,cvPoint(centerX[i-1],centerY[i-1]),cvPoint(Xs,Ys),CV_RGB(255,255,0),14,8,0);
							cvLine(pTrjImg,cvPoint(centerX[i-2],centerY[i-2]),cvPoint(Xs,Ys),CV_RGB(255,255,0),14,8,0);

							//�ڱ���ͼ�б�ʾ����Ƶ�е���������
							cvCircle(pTrpImg,cvPoint(centerX[i-1],centerY[i-1]),5,CV_RGB(0,0,255),12,8,0);
							//�ڱ���ͼ�б�ʾ����������������㣬��ɫ��ʾ
							cvCircle(pTrpImg,cvPoint(Xs,Ys),5,CV_RGB(0,255,0),12,8,0);

							//�ڳ���ͼ�б�ʾ����������
							cvCircle(pTrjImg,cvPoint(centerX[i-1],centerY[i-1]),2,CV_RGB(0,0,255),8,8,0);
							//�ڱ���ͼ�б�ʾ����������������㣬��ɫ��ʾ
							cvCircle(pTrjImg,cvPoint(Xs,Ys),2,CV_RGB(0,255,0),12,8,0);

							//��ӡ�����
							printf("��Ƶ�е��������� ��x[%d]y[%d]:(%d,%d) FrmNum = %d \n",i-1,i-1,centerX[i-1],centerY[i-1],bounceFrmNum[k]);
							printf("��������������� ��Xs Ys:(%d,%d) FrmNum = %d \n",Xs,Ys,bounceFrmNum[k]);

							//�����Ƿ�������ĺ��������жϣ�������õ������߶εļ��㷽ʽ
							judge( Xa, Ya, Xb, Yb, Xs, Ys);


							//��judging display��������ʾ�о�
							if( judging == true )
							{							
								//�������ԭ��������
								cvZero( pJudge );
								//��ʾ����
								cvPutText( pJudge, "IN", pt1, &font, CV_RGB(255,255,255));	
							}
							else
							{						    
								//�������ԭ��������
								cvZero( pJudge );	
								//��ʾ����
								cvPutText( pJudge, "OUT", pt2, &font, CV_RGB(255,255,255));							
							}

						}//end if ��� �ڶ������ģʽ���ж�

						//���е�����ģʽ���ж�
						//��ֹһ���������ж�����,һ����˵���������ļ��ʱ����1�����ϣ���֤��ص���ȡ�ľ�ȷ��
						if(((k>0 && (bounceFrmNum[k]-bounceFrmNum[k-1]>30)) || k ==0)
							&& ((abs(centerY[i-1] - centerY[i-2])<50) || (abs(centerY[i-1] - centerY[i])<50))
							&&((abs(centerY[i] - centerY[i-1])-abs(centerY[i-1] - centerY[i-2]))<0))

						{ 					
							//�������жϵı�Ǳ�ʾΪ��
							lastGrounded = true;
							//����������¼����
							xi1 = centerX[i];
							yi1 = centerY[i];
							xi2 = centerX[i-1];
							yi2 = centerY[i-1];
							xi3 = centerX[i-2];
							yi3 = centerY[i-2];
							printf("xi1 =%d yi1 =%d xi2 =%d yi2 =%d xi3 =%d yi3 =%d \n", xi1,yi1,xi2,yi2,xi3,yi3);	
							//��bounceFrmNum���±��������
							k++;
							//�����������±��������
							i++;
							continue;		  
						}  //end if �������жϵĵ�һ������

						//��bounceFrmNum���±��������
						k++;				
					}// end if ��ɶ�����ص����ȡ

					//�������жϵĵڶ�����ʼ
					if(lastGrounded == true)
					{
						printf("�������ж�֮��ĵ�ǰ֡%d\n",nFrmNum);
						
						//��������и�ֵ
						xi0 = centerX[i];
						yi0 = centerY[i];
						printf("xi0 =%d yi0 =%d \n", xi0,yi0);	

						//������ֱ�ߵĽ���
						intersect(xi0,yi0,xi1,yi1,xi2,yi2,xi3,yi3);
						
						//����������
						cvLine(pTrjImg,cvPoint(centerX[i-1],centerY[i-1]),cvPoint(Xs,Ys),CV_RGB(255,255,0),14,8,0);
						cvLine(pTrjImg,cvPoint(centerX[i-2],centerY[i-2]),cvPoint(Xs,Ys),CV_RGB(255,255,0),14,8,0);

						//�ڱ���ͼ�б�ʾ����Ƶ�е���������
						cvCircle(pTrpImg,cvPoint(centerX[i-2],centerY[i-2]),5,CV_RGB(0,0,255),12,8,0);
						//�ڱ���ͼ�б�ʾ����������������㣬��ɫ��ʾ
						cvCircle(pTrpImg,cvPoint(Xs,Ys),5,CV_RGB(0,255,0),12,8,0);

						//�ڳ���ͼ�б�ʾ����������
						cvCircle(pTrjImg,cvPoint(centerX[i-2],centerY[i-2]),2,CV_RGB(0,0,255),8,8,0);
						//�ڱ���ͼ�б�ʾ����������������㣬��ɫ��ʾ
						cvCircle(pTrjImg,cvPoint(Xs,Ys),2,CV_RGB(0,255,0),12,8,0);

						//��ӡ�����
						printf("��Ƶ�е��������� ��x[%d]y[%d]:(%d,%d) FrmNum = %d \n",i-1,i-1,centerX[i-2],centerY[i-2],bounceFrmNum[k]);
						printf("��������������� ��Xs Ys:(%d,%d) FrmNum = %d \n",Xs,Ys,bounceFrmNum[k]);

						//�����Ƿ�������ĺ��������жϣ�������õ������߶εļ��㷽ʽ
						judge( Xa, Ya, Xb, Yb, Xs, Ys);

						//��judging display��������ʾ�о�
						if( judging == true )
						{
							//�������ԭ��������
							cvZero( pJudge );
							//��ʾ����
							cvPutText( pJudge, "IN", pt1, &font, CV_RGB(255,255,255));	

						}
						else
						{
							//�������ԭ��������
							cvZero( pJudge );	
							//��ʾ����
							cvPutText( pJudge, "OUT", pt2, &font, CV_RGB(255,255,255));						
						}

						//���жϱ�־��Ϊ��
						lastGrounded = false;
					}//end if ��ɶԵ���������ķ���
		    			
					//�����������±��������
					i++;

				}//end if Ŀ���ʶ��

			}//end for
		
			//��������ģʽѡ�������Ҫʹ��Surendra����ģʽ���뽫if 0��Ϊ if 1
	  
#if 0		
			//��ͼ���еı���ת��Ϊ����
			cvConvert(pFrImg, Dxy);
			
			//ʹ��Surendra����ģʽ�����������ٶȻ����������ȡ���˶��������Ӱ��
			Surendra(temp,pBkMat,pFrameMat,Dxy );
		    	  
			//���±�����Ȩֵȡ����0.3
			cvRunningAvg(pFrameMat, temp, 0.3, 0);

#else
			//���±�����Ȩֵȡ����0.01�������Ǳ����е��˶����壬�ٶȽϿ�
			cvRunningAvg(pFrameMat, pBkMat, 0.01, 0);

#endif

			//������ת��Ϊͼ���ʽ��������ʾ
			cvConvert(pBkMat, pBkImg);
	 
			//��ʾͼ��
			cvShowImage(displayName, pFrame);
			cvShowImage("background", pBkImg);
			cvShowImage("background model", pHough);
			cvShowImage("binary", pBinImg);
			cvShowImage("point tracking", pTrpImg);
			cvShowImage("trajectory tracking", pTrjImg);
			cvShowImage("judging display", pJudge);
	 
			//����а����¼���������ѭ��,�˳�����
			char c = cvWaitKey(10);
			if (c == 27 )
			return 0;
 
		}//end else
	
	}// end  while

	cvWaitKey(0);
	 
    //���ٴ���
	cvDestroyWindow(displayName);
    cvDestroyWindow("background");
	cvDestroyWindow("background model");
	cvDestroyWindow("binary");
	cvDestroyWindow("point tracking");
	cvDestroyWindow("trajectory tracking");
	cvDestroyWindow("judging display");
 
	//�ͷ�ͼ��
	cvReleaseImage(&pFrame);
	cvReleaseImage(&pFrImg);
	cvReleaseImage(&pBkImg);
	cvReleaseImage(&pEqlImg);
	cvReleaseImage(&pBinImg);
	cvReleaseImage(&pTrpImg);
	cvReleaseImage(&pTrjImg);	
	cvReleaseImage(&pHough);
	cvReleaseImage(&pCanny);
	cvReleaseImage(&pJudge);
	cvReleaseImage(&pOper);

    //�ͷž���
	cvReleaseMat(&pFrameMat);
	cvReleaseMat(&pFrMat);
	cvReleaseMat(&pBkMat);
	cvReleaseMat(&temp);
	cvReleaseMat(&Dxy);

	//�ͷ��ڴ�
	cvReleaseMemStorage( &storageContour );
	cvReleaseMemStorage( &storageHough );
	cvReleaseCapture(&pCapture);

	return 0;
}





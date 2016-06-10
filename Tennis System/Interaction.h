//��ͷ�ļ�"Interaction.h"���ڶ�����Ƶ�е�һЩ��������


//����Ƶ��ʵ����ͣ��ť�ڿ���̨����ʾ
void switch_off_function() 
{
	printf("Pause g_switch_value = %d \n",g_switch_value);
}

void switch_on_function() 
{
	printf("Run g_switch_value = %d \n",g_switch_value);

}

//����Ƶ��ʵ����ͣ��ť�ĺ���
void switch_callback( int position )
{
	if( position == 0 ) 
	{
		switch_off_function();
	} 
	else 
	{
		switch_on_function();
	}
}


//����ʵ�����Ĳ���,ʵ�ֶ�trajectory tracking���ڵĲ���
void mouse_callback1(int event, int x, int y, int flags, void* param)
{
    if( !pTrjImg )    
	return; 

	//����ʶ���ڵ����������ͬʱ��Ҫ��סSHIFT���Ϳ���ִ�в���
	if( event == CV_EVENT_LBUTTONDOWN &&(flags & CV_EVENT_FLAG_SHIFTKEY) )
	{
		//�������ʶ��׼�������������жԳ����ߵĵ�������г����ߵı궨
		if( down == 0)
		{
			printf("���������Ϊ��(%d,%d)\n",x,y);
			printf("��һ���޸ĵ�����Ϊ��(%d,%d)\n",Xa,Ya);
			Xa = x;
			Ya = y;		
			//��down���������������
			down++;
		}
		else if( down == 1)
		{
			
			printf("���������Ϊ��(%d,%d)\n",x,y);
			printf("�ڶ����޸ĵ�����Ϊ��(%d,%d)\n",Xb,Yb);
			Xb = x;
		    Yb = y;
			//��down���������������
		    down++;
		
			//�����˹�ʶ���ֱ��
			cvLine( pOper, cvPoint(Xa,Ya),cvPoint(Xb,Yb),CV_RGB(255,0,255), 5, CV_AA, 0 );

			//����ͼ��
			cvCopy(pOper,pTrjImg);	
		}
	}

    if( event == CV_EVENT_RBUTTONDOWN )
	{
		//�ָ��켣ͼΪû�й켣�Ŀճ���״̬
		if(down ==2)
		{
			cvCopy(pOper,pTrjImg);
		}
		else
		{
			printf("down = %d",down);
			cvCopy(pHough,pTrjImg);	
		}
	}
}


//����ʵ�����Ĳ���,ʵ�ֶ�point tracking���ڵĲ���
void mouse_callback2(int event, int x, int y, int flags, void* param)
{
    if( !pTrpImg )
		return; 
    if( event == CV_EVENT_RBUTTONDOWN )
	//�ָ��켣��ͼΪ��
	cvZero(pTrpImg);	
}


//����˵�� 
void manual()
{
	printf( "�ȼ�ָ��: \n"
            "\tESC - �˳�����\n"
            "\t�� trajectory tracking �����е���Ҽ����������һ����Ĺ켣\n" 
			"\t�� trajectory tracking ���ڰ�סshift��Ȼ���������������˹�ʶ�������ߣ�"
			"�ر�ע����Ϊ���������߶ε��б�ʽ��������Ӱ�˳��ͼ��ײ����������е��ѡ��\n" 
			"\t�� point tracking �����е���Ҽ����������ͼ���еı�ʾ��\n" 
			"\t�� judging display  �����е���Ҽ����������ͼ���еĶ���һ����о�\n"
			"\t�� video ������Pause/Run ��0Ϊ��ͣ��1Ϊ��ͣ��������ţ����1֮�󣬻���Ҫ��һ�¼����ϵ����ⰴ���ſɼ�������\n" );
}



//����ʵ�����Ĳ���,ʵ�ֶ�judging display ���ڵĲ���
void mouse_callback3(int event, int x, int y, int flags, void* param)
{
    if( !pJudge )
		return; 
    if( event == CV_EVENT_RBUTTONDOWN )
	//�ָ��о�ͼƬͼΪ��
	cvZero(pJudge);	
}
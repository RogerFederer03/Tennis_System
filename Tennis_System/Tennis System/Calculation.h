
//�������е�����Ԫ����ð�����򽫵�һ��Ԫ�شӴ�С���У�ͬʱ�ƶ��ڶ���Ԫ��

void sort(int a[][2],int n) /*�������������������׵�ַ������������С*/ 
{ 
	int i,j,temp; 
	for(i=0;i<n-1;i++) 
	for(j=i+1;j<n;j++)
	//��Ҫ�Ӵ�С�Ե�һԪ��ͬʱ�󶨵ڶ���Ԫ�ؽ�������
	if(a[i][0]<a[j][0])
	{ 
		temp = a[i][0]; 
		a[i][0] = a[j][0]; 
		a[j][0] = temp;
		temp = a[i][1]; 
		a[i][1] = a[j][1]; 
		a[j][1] = temp;
	} 
} 



//�����жϵ��Ƿ���ֱ����ĺ���

void judge(int x0, int y0, int x1, int y1, int x, int y)
{
	fx = (y1 - y0)*(x - x0)-(y - y0)*(x1 - x0);
	printf("fx = %d\n",fx);
	//ͨ���������fx��ֵ���ж����Ƿ��ڽ���
	if ( fx >0 || fx == 0)
	{	
		printf("���ٽ���\n");
		//�ı�judging��ֵ
		judging = true;
	}
	else
	{
		printf("���ٽ���\n");	
		//�ı�judging��ֵ
		judging = false;
	}
}



//������ֱ�߷��̵Ĺ�����
/*
����Ҫ�ҵ���Ԫһ�η������ͨ�⣬���磺
ax+by=e
cx+dy=f
�������
x=(ed-bf)/(ad-bc)
y=(ec-af)/(bc-ad) 
������󽫽��ת��Ϊ����

ͬʱ���ǿ���֪��ͨ�������ֱ�߷���Ϊ��
(y1-y0)*x + (x0-x1)*y = x0*y1 - x1*y0
*/

void intersect(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{
	//����ֱ�߷���ϵ������ı���
	int a = y1-y0; 
	int b = x0-x1;
	int e = x0*y1 - x1*y0; 
	int c = y3-y2; 
	int d = x2-x3; 
	int f = x2*y3 - x3*y2;

	//������ֱ�ߵĽ���
	Xs=(e*d-b*f)/(a*d-b*c);
	Ys=(e*c-a*f)/(b*c-a*d);

	printf("���̵Ľ�Ϊ Xs=%d,Ys=%d\n",Xs,Ys); 
}



//Surendra���ڱ�������
void Surendra(CvMat* t,CvMat* B,CvMat* f,CvMat* D )
{
	int i,j;
	//ȡ�����������е�Ԫ���ñ���
	for(i = 0;i<D->rows;i++)
		for(j = 0;j<D->cols;j++)
		{   //�������ʱ�������õ�ǰ֡���£��������˶�������ñ�������
			if((int)cvGetReal2D(D,i,j) == 0)
			{	
				cvSetReal2D(t,i,j,cvGetReal2D(f,i,j));
			}
			else
			{
				cvSetReal2D(t,i,j,cvGetReal2D(B,i,j));
			}
		}
}


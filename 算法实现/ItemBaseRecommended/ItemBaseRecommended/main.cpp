#define _CRT_SECURE_NO_DEPRECATE
#include "math.h"
#include "string.h"
#include "stdlib.h" 
#include "stdio.h"
#include "time.h"  
#include <iostream>
#include<iomanip>
using namespace std;



FILE *fp;
const int userSum = 6040;          //���ݼ����û�����
const int itemSum = 3952;          //���ݼ��Ĳ�Ʒ����
const int recommendNum = 10;                    //Ϊÿ���û��Ƽ���Ʒ������recommendNum
char datasetFile[100] = "E:\\mytest\\ratings.dat";
int ArrTrain[itemSum][userSum] = { 0 };	       //ѵ����
int test[itemSum][userSum] = { 0 };           //���Լ�
double trainuseritem[userSum][itemSum] = { 0.0 };//��Ȥ�̶� ����
int recommend[userSum][recommendNum] = { 0 };     //Ϊÿ���û��Ƽ�recommendNum����Ʒ
struct simi
{
	double value; //����ֵ 
	int num;	 //������Ʒ�� 
};
simi simiItem[itemSum][itemSum]; //�����������Ծ���

//��һ����������ݼ�Ϊ���Լ�test��ѵ����train������1/mΪ���Լ�
int SplitData(int m, int k);
//Ѱ�����ݼ�ÿ����¼��Ӧ���û��ź���Ʒ��
int Buf_UIR(char* buf, int* user, int* item);
double Simility(int* ItemA, int* ItemB);    //�ڶ�����������Ʒ֮��������ԣ������Ʒ�����Ծ���
int sort(double* simArr, simi* simStruct); //����������Ʒ�����Ծ������򣨸����������ɸߵ�������
double getUserLikeItem(int i, int j, int k); //���Ĳ����õ��û�i����ƷjԤ����Ȥ�̶ȣ�����k�������������
int getRecommend();                       //���岽��ͨ����Ʒ��Ȥ�̶ȣ��Ƽ�ǰrecommendNum��
double Recall();                           //�������������ٻ��ʡ�׼ȷ�ʺ͸�����
double Precision();
double Coverage();
double Diversity(double **a);//���������

void main()
{
	int i, j;
	double recall, precision, coverage, diversity;
	int k;        //ȥ�û���k������ھ��������Ƽ���Ʒ
	k = 10;
	/*printf("������ȡ����ھ�������ĸ�����k=");
	scanf("%d", &k);*/

	


	//cout << "�ֱ���ǰ10���û��Ƽ�ǰ10����Ʒ��\n";

	//1.��ʼ�����ݼ�
	SplitData(8, 1); //�漴����1/8Ϊ���Լ�������Ϊѵ����
	//�����ʼ���ľ���	
	/*
	for (i=0;i<5;i++)
	{
	cout<<"Item"<<i<<":   "<<endl;
	for (j=0;j<100;j++)
	{
	cout<<train[i][j]<<"  ";
	}
	cout<<endl;
	}
	*/

	//��̬�����ڴ�ռ������Ʒ�����Ծ���
	double **itemsim;
	itemsim = (double**)malloc(sizeof(double*)*itemSum);
	for (i = 0; i<itemSum; i++)
	{
		itemsim[i] = (double*)malloc(sizeof(double)*itemSum);
	}
	//2.������Ʒ֮�������ԣ��õ������Ծ���
	for (i = 0; i<itemSum; i++)
	{
		for (j = 0; j<itemSum; j++)
		{
			itemsim[i][j] = Simility(ArrTrain[i], ArrTrain[j]);
		}
	}
	//�����Ʒ�����Ծ���	
	/*
	for (i=0;i<5;i++)
	{
	cout<<"Item"<<i<<":   "<<endl;
	for (j=0;j<100;j++)
	{
	cout<<itemsim[i][j]<<"  ";
	}
	cout<<endl;
	}
	*/

	//3.��Ʒ���ƶ��ɸߵ�������
	for (i = 0; i<itemSum; i++)
	{
		//cout<<"Item"<<i<<":   "<<endl;
		sort(itemsim[i], simiItem[i]);
		//cout<<endl;
	}
	//�����������Ʒ�����Ծ���
	/*
	for(i=0;i<5;i++)
	{
	cout<<"item"<<i<<":  "<<endl;
	for(j=0;j<100;j++)
	{
	cout<<simiItem[i][j].num<<","<<simiItem[i][j].value<<"  ";
	}
	cout<<endl;
	}
	*/
	//4.�õ��û�����Ʒ��Ȥ�̶ȵľ���
	for (i = 0; i<userSum; i++)
	{
		for (j = 0; j<itemSum; j++)
		{
			if (ArrTrain[j][i] == 0)            //����û�i����Ʒjû�й���Ϊ���ż���i��j��Ԥ����Ȥ�̶�
				trainuseritem[i][j] = getUserLikeItem(i, j, k);
		}
	}
	//����û�����Ʒ��Ȥ�ľ���
	/*
	for (i=0;i<5;i++)
	{
	cout<<"User"<<i<<":   "<<endl;
	for (j=0;j<100;j++)
	{
	cout<<trainuseritem[i][j]<<"  ";
	}
	cout<<endl;
	}
	*/

	//5.ͨ����Ʒ��Ȥ�̶ȣ��Ƽ�ǰrecommendNum��
	getRecommend();
	//����Ƽ�����-------------------------------------------------
	/*for (i = 0; i<10; i++)
	{
		cout << "���û� " << i << " �Ƽ�:   " << endl;
		for (j = 0; j<recommendNum; j++)
		{
			cout << recommend[i][j] << "  ";
		}
		cout << endl;
	}
	//---------------------------------------------
*/
	recall = Recall();	//�����ٻ���
	//printf("�ٻ���:recall=%lf ", recall);
	//cout << "\n";
	precision = Precision();//����׼ȷ��
	//printf("׼ȷ�� :precision=%lf ", precision);
	//cout << "\n";
	coverage = Coverage();//���㸲����
	//printf("������:coverage=%lf ", precision);
	cout << "�㷨������£�\n";

	cout << setw(10) << "�ٻ���" << setw(10) << "׼ȷ��" << setw(10) << "������" << endl;
	cout << "\n";
	cout << setw(10) << recall << setw(10) << precision << setw(10) << precision << endl;
	cout << "\n";
	//diversity = Diversity(itemsim);//���������
	//printf("������diversity=%lf ", diversity);

	//�ͷ��ڴ� 
	for (i = 0; i<itemSum; i++)
	{
		free(itemsim[i]);
	}
	free(itemsim);

	cout << "\n ---�������---\n";
	cout << "��������Ҫ�Ƽ���Ʒ���û�ID (0<ID<6040)��\n";
	int userID;
	cin >> userID;
	cout << "����û� "<<userID<<" �Ƽ��Ĳ�ƷID���£�\n";
	for (int i = 0; i < 10; i++)
	{
		cout << recommend[userID][i] << " ";

	}

	int hehe;
	cin >> hehe;
	//return 1;
}

//������ݼ�Ϊ���Լ�test��ѵ����trainuser������1/mΪ���Լ�,ȡ��ͬ��k<=m-1ֵ ����ͬ���漴�����¿ɵõ���ͬ�Ĳ�/ѵ����
int SplitData(int m, int k)
{
	fp = fopen(datasetFile, "r");
	char tmpbuf[100];		//�ݴ��ļ�һ�м�¼
	int usernum;
	int itemnum;

	if (!fp)
	{
		cout << "open datasetfile failed";
		return 1;
	}
	else
	{
		srand((unsigned)time(NULL));                  //���õ�ǰʱ��Ϊ�������
		while (!feof(fp))
		{
			fgets(tmpbuf, 100, fp);                     //��fpָ��ĵ�ǰ��¼�浽tmpbuf[100]
			Buf_UIR(tmpbuf, &usernum, &itemnum);        //Ѱ�����ݼ�ÿ����¼��Ӧ���û��ź���Ʒ��
			if (usernum <= userSum&&itemnum <= itemSum)
			{
				if (rand() % (m - 1) == k)                       //�ж��������0-7֮���������Ƿ����k
					test[itemnum - 1][usernum - 1] = 1;        //rateΪ���֣��ٴ�ʵ����ֻ��ͳ���������ֵģ��������۾�������
				else
					ArrTrain[itemnum - 1][usernum - 1] = 1;  //�û��ŵ���Ʒ�ž���0��ʼ����
			}
		}
		fclose(fp);
	}
	return 1;
}
//Ѱ�����ݼ�ÿ����¼��Ӧ���û��ź���Ʒ��
int Buf_UIR(char* buf, int* user, int* item)
{
	char U[20] = { 0 };
	char I[20] = { 0 };
	char R[20] = { 0 };
	char *Temp;
	int curpos;

	//�����û���
	Temp = strchr(buf, ':');
	curpos = Temp - buf;
	strncpy(U, buf, curpos);
	*user = atoi(U);
	//����item��
	Temp = strchr(buf + curpos + 2, ':');
	//curpos = Temp - buf;
	strncpy(I, buf + curpos + 2, Temp - buf - curpos - 2);
	*item = atoi(I);

	return 1;
}

//����ѵ���������û�֮�����ƶ�
/* ��������ItemA��ItemB�������ԣ�����ֵΪItemA��ItemB�����ƶ� */
double Simility(int* ItemA, int* ItemB)
{
	int comUser = 0;                   //ItemA��ItemB�Ķ����û����۵��û�����
	double simility = 0.0;
	int countIa = 0;
	int countIb = 0;

	int i;
	for (i = 0; i<itemSum; i++)
	{
		if (ItemA[i]>0 && ItemB[i]>0)
		{
			comUser++;//����ItemA��ItemB�Ķ����û����۵��û�����
		}
		if (ItemA[i]>0){
			countIa++;//����ItemA���û�����
		}
		if (ItemB[i]>0){
			countIb++;//����ItemB���û�����
		}
	}
	double tem = sqrt(countIa*countIb);
	if (tem == 0)
	{
		return 0;
	}
	else
	{
		simility = comUser / tem;
		return simility;
	}
}


/*��Ʒ�����Ծ������򣨸����������ɸߵ������򣩣�ÿ�е�һ�����Լ�*/
#include <algorithm>
struct SimiLessComp {
	bool operator () (const simi& a, const simi& b) const {
		return a.value > b.value;
	}
};


int sort(double* simArr, simi* simStruct)
{
#if 1
	for (int i = 0; i < itemSum; ++i)
		(simStruct[i].num = i), (simStruct[i].value = simArr[i]);
	std::sort(simStruct, simStruct + itemSum, SimiLessComp());

	return 1;

#else
	int finflag[itemSum] = { 0 };//�Ƿ����ź����ʾ
	int maxnum;//��ǰ�����
	int m = 0, t;

	for (int i = 0; i<itemSum; i++)
	{
		t = 0;                  //������Ѱ�ҵ�һ�� û�б�����õ��û���
		while (finflag[t] != 0)  //����ѱ�����
			t++;
		maxnum = t;                    //����������ƶ���Ʒ�š�
		for (int j = 0; j<itemSum; j++)
		{
			if (simArr[j]>simArr[maxnum] && finflag[j] == 0)
			{
				maxnum = j;
			}
		}
		simStruct[m].num = maxnum;
		simStruct[m].value = simArr[maxnum];
		finflag[maxnum] = 1;             //����ѭ��������ƶ���Ʒ����Ѿ�����
		m++;
		//cout<<i<<":"<<maxnum<<":"<<simArr[maxnum]<<"  ";

	}

	return 1;
#endif
}

//�õ��û�i����ƷjԤ����Ȥ�̶ȣ������Ƽ�
double getUserLikeItem(int i, int j, int k)
{
	for (int x = 1; x <= k; x++)//����Ʒj�����Ƶ�k����Ʒ�У��ҳ��û�i�й���Ϊ����Ʒ,��Ϊ��һ��simiItem[][]��ÿ�е�һ����ŵ����Լ������Դӵڶ�����ʼ�㣬x=1
	{
		if (ArrTrain[simiItem[j][x].num][i]>0)//������û�ͬ����������ƷҲ�й���Ϊ
		{
			trainuseritem[i][j] += simiItem[j][x].value;
		}
	}
	return trainuseritem[i][j];
}
/*ͨ����Ʒ��Ȥ�̶ȣ��Ƽ�ǰrecommendNum��*/
int getRecommend()
{
	int maxnum;//��ǰ�����Ȥ��Ʒ��
	for (int i = 0; i<userSum; i++)
	{
		int finflag[itemSum] = { 0 };//�Ƿ����ź����ʾ
		for (int x = 0; x<recommendNum; x++)//�Ƽ�recommendNum��
		{
			maxnum = 0;
			while (finflag[maxnum] != 0)  //�жϴ˴�ѭ���ĵ�һ��δ���������Ʒ
				maxnum++;
			for (int j = 0; j<itemSum; j++)  //ÿѭ��һ�ξ�Ѱ�Ҵ˴θ���Ȥ������Ʒ
			{
				if (trainuseritem[i][maxnum]<trainuseritem[i][j] && finflag[j] == 0)
				{
					maxnum = j;
				}
			}
			finflag[maxnum] = 1;
			recommend[i][x] = maxnum;//recommend�����0��ʼʹ��
		}
	}
	return 1;
}


//�����ٻ���
double Recall()   //test[usersum][itemSum], recommend[usersum][recommendNum],recommendNumΪ�Ƽ���
{
	int i, j, k, tnum = 0, rnum = 0, count[userSum] = { 0 };  //tnumΪ���Լ���ϲ������Ʒ����;count[i]Ϊ��i���û����Ƽ��������Լ������е���Ʒ����������rnumΪ�ܹ����е���Ʒ����
	for (i = 0; i<userSum; i++)
	{
		for (j = 0; j<itemSum; j++)
		{
			if (test[j][i] != 0)          //����û�i����Ʒj����Ȥ
			{
				tnum++;
				for (k = 0; k<recommendNum; k++)
				{
					if (recommend[i][k] == j)    //�û�i����Ʒj����Ȥ��ͬʱ����Ʒj���Ǳ��Ƽ�����Ʒ
						count[i]++;
				}
			}
		}
		rnum = rnum + count[i];
	}
	return rnum / (tnum*1.0);
}
//����׼ȷ��
double Precision()   //test[usersum][itemSum], recommend[usersum][recommendNum],recommendNumΪ�Ƽ���
{
	int i, j, k, rnum = 0, count[userSum] = { 0 };  //count[i]Ϊ��i���û����Ƽ��������Լ������е���Ʒ������rnumΪ�ܹ����е���Ʒ����
	for (i = 0; i<userSum; i++)
	{
		for (j = 0; j<itemSum; j++)
		{
			if (test[j][i] != 0)          //����û�i����Ʒj����Ȥ
			{
				for (k = 0; k<recommendNum; k++)
				{
					if (recommend[i][k] == j)    //�û�i����Ʒj����Ȥ��ͬʱ����Ʒj���Ǳ��Ƽ�����Ʒ
						count[i]++;
				}
			}
		}
		rnum = rnum + count[i];
	}
	return rnum / (recommendNum*userSum*1.0);
}
//���㸲����
double Coverage()
{
	int i, j, k, sum = 0, count[itemSum] = { NULL };  ////count[i]��=NULL��ʾ��i����Ʒ��������,sumͳ�Ʊ����ǵ���Ʒ����
	for (i = 0; i<userSum; i++)
	{
		for (j = 0; j<recommendNum; j++)
		{
			k = recommend[i][j];      // k=�Ƽ����û�i�ĵ�j����Ʒ����Ʒ��
			if (count[k] == NULL)      //�жϵ�k����Ʒ�Ƿ��Ѿ�����
			{
				count[k] = 1;
				sum++;
			}
		}
	}
	return sum / (itemSum*1.0);
}
//���������
double Diversity(double **a)
{
	double diversity, count = 0, sum = 0;   //count��Ŷ�ÿһ���û��Ƽ�����Ʒ֮���������ֵ��sum��������Ƽ���Ʒ��������ֵ�ܺ�
	for (int i = 0; i<userSum; i++)
	{
		for (int j = 0; j<recommendNum; j++)
		{
			for (int k = j + 1; k<recommendNum; k++)
			{
				count = count + a[recommend[i][j]][recommend[i][k]];
			}
		}
		sum = sum + 1 - 2 * count / (recommendNum*(recommendNum - 1));  //��ÿ���û��Ķ�����ֵ�ۼӵ�sum
		count = 0;
	}
	diversity = sum / userSum;
	return diversity;
}



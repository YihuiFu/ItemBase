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
char datasetFile[100] = "E:\\mytest\\ratings.dat";    //UserID::MovieID::Rating::Time
int ArrTrain[itemSum][userSum] = { 0 };	       //ѵ����
int ArrTest[itemSum][userSum] = { 0 };           //���Լ�
double trainuseritem[userSum][itemSum] = { 0.0 };//��Ȥ�̶� ����
int ArrRecommended[userSum][recommendNum] = { 0 };     //�û��Ƽ�������ÿ�û��Ƽ�recommendNum��
struct Simi
{
	double value; //����ֵ 
	int num;	 //������Ʒ�� 
};
Simi ArrOrderedItemSimi[itemSum][itemSum]; //�����������Ծ���

//��һ����������ݼ�Ϊ���Լ�ArrTest��ѵ����ArrTrain������1/mΪ���Լ�
int SplitData(int m, int k);
//Ѱ�����ݼ�ÿ����¼��Ӧ���û��ź���Ʒ��
int Buf_UIR(char* buf, int* user, int* item);
double Simility(int* ItemA, int* ItemB);    //�ڶ�����������Ʒ֮��������ԣ������Ʒ�����Ծ���
int sort(double* simArr, Simi* simStruct); //����������Ʒ�����Ծ������򣨸����������ɸߵ�������
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
	int k=10;        //ȡ�û��� 10 ������ھ�
	
	//1.�����ݼ���Ϊѵ���� �� ���Լ�
	SplitData(8, 1); 


	//���ѵ�����ĵ�ǰ
	for (int i = 0; i < 10; i++)
	{
		cout <<ArrTrain[0][i]<<" " ;
	}
	
	//��̬Ϊ��Ʒ�����Ծ������ռ�
	double **ArrItemSimi;
	ArrItemSimi = (double**)malloc(sizeof(double*)*itemSum);
	for (i = 0; i<itemSum; i++)
	{
		ArrItemSimi[i] = (double*)malloc(sizeof(double)*itemSum);
	}
	//2.������Ʒ֮�������ԣ��õ������Ծ���
	for (i = 0; i<itemSum; i++)
	{
		for (j = 0; j<itemSum; j++)
		{
			ArrItemSimi[i][j] = Simility(ArrTrain[i], ArrTrain[j]);
		}
	}

#pragma region --�����Ʒ�����Ծ���
	//�����Ʒ�����Ծ���	
	/*
	cout<<"��Ʒ�����Ծ���:\n";
	for (i=0;i<5;i++)
	{
	cout<<"Item"<<i<<":   "<<endl;
	for (j=0;j<100;j++)
	{
	cout<<ArrItemSimi[i][j]<<"  ";
	}
	cout<<endl;
	}
	*/
#pragma endregion


	//3.��Ʒ���ƶ��ɸߵ�������
	for (i = 0; i<itemSum; i++)
	{
		sort(ArrItemSimi[i], ArrOrderedItemSimi[i]);
	}
	//�����������Ʒ�����Ծ���
	/*
	for(i=0;i<5;i++)
	{
	cout<<"item"<<i<<":  "<<endl;
	for(j=0;j<100;j++)
	{
	cout<<ArrOrderedItemSimi[i][j].num<<","<<ArrOrderedItemSimi[i][j].value<<"  ";
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

	precision = Precision();//����׼ȷ��

	coverage = Coverage();//���㸲����

	cout << "�㷨������£�\n";

	cout << setw(10) << "�ٻ���" << setw(10) << "׼ȷ��" << setw(10) << "������" << endl;
	cout << "\n";
	cout << setw(10) << recall << setw(10) << precision << setw(10) << precision << endl;
	cout << "\n";

	//�ͷ��ڴ� 
	for (i = 0; i<itemSum; i++)
	{
		free(ArrItemSimi[i]);
	}
	free(ArrItemSimi);

	cout << "\n ---�������---\n";
	cout << "��������Ҫ�Ƽ���Ʒ���û�ID (0<ID<6040)��\n";
	int userID;
	cin >> userID;
	cout << "����û� "<<userID<<" �Ƽ��Ĳ�ƷID���£�\n";
	for (int i = 0; i < 10; i++)
	{
		cout << ArrRecommended[userID][i] << " ";

	}

	int hehe;
	cin >> hehe;
}

//������ݼ�Ϊ���Լ�ArrTest��ѵ����trainuser������1/mΪ���Լ�,ȡ��ͬ��k<=m-1ֵ ����ͬ���漴�����¿ɵõ���ͬ�Ĳ�/ѵ����
int SplitData(int m, int k)
{
	fp = fopen(datasetFile, "r");
	char tmpbuf[100];		//�ݴ��ļ�һ�м�¼
	int userNum;            //�û����
	int itemNum;            //��Ʒ���

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
			Buf_UIR(tmpbuf, &userNum, &itemNum);        //Ѱ�����ݼ�ÿ����¼��Ӧ���û��ź���Ʒ��
			if (userNum <= userSum&&itemNum <= itemSum)
			{
				if (rand() % (m - 1) == k)                       //�ж��������0-7֮���������Ƿ����k
					ArrTest[itemNum - 1][userNum - 1] = 1;        //rateΪ���֣��ٴ�ʵ����ֻ��ͳ���������ֵģ��������۾�������
				else
					ArrTrain[itemNum - 1][userNum - 1] = 1;  //�û��ŵ���Ʒ�ž���0��ʼ����
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
	//--------------------------------------------
	for (i = 0; i<userSum; i++)
	{
		//int aa = ItemA[4000];
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
	bool operator () (const Simi& a, const Simi& b) const {
		return a.value > b.value;
	}
};


int sort(double* simArr, Simi* simStruct)
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
	for (int x = 1; x <= k; x++)//����Ʒj�����Ƶ�k����Ʒ�У��ҳ��û�i�й���Ϊ����Ʒ,��Ϊ��һ��ArrOrderedItemSimi[][]��ÿ�е�һ����ŵ����Լ������Դӵڶ�����ʼ�㣬x=1
	{
		if (ArrTrain[ArrOrderedItemSimi[j][x].num][i]>0)//������û�ͬ����������ƷҲ�й���Ϊ
		{
			trainuseritem[i][j] += ArrOrderedItemSimi[j][x].value;
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
			ArrRecommended[i][x] = maxnum;//recommend�����0��ʼʹ��
		}
	}
	return 1;
}


//�����ٻ���
double Recall()   //ArrTest[usersum][itemSum], recommend[usersum][recommendNum],recommendNumΪ�Ƽ���
{
	int i, j, k, tnum = 0, rnum = 0, count[userSum] = { 0 };  //tnumΪ���Լ���ϲ������Ʒ����;count[i]Ϊ��i���û����Ƽ��������Լ������е���Ʒ����������rnumΪ�ܹ����е���Ʒ����
	for (i = 0; i<userSum; i++)
	{
		for (j = 0; j<itemSum; j++)
		{
			if (ArrTest[j][i] != 0)          //����û�i����Ʒj����Ȥ
			{
				tnum++;
				for (k = 0; k<recommendNum; k++)
				{
					if (ArrRecommended[i][k] == j)    //�û�i����Ʒj����Ȥ��ͬʱ����Ʒj���Ǳ��Ƽ�����Ʒ
						count[i]++;
				}
			}
		}
		rnum = rnum + count[i];
	}
	return rnum / (tnum*1.0);
}
//����׼ȷ��
double Precision()   //ArrTest[usersum][itemSum], recommend[usersum][recommendNum],recommendNumΪ�Ƽ���
{
	int i, j, k, rnum = 0, count[userSum] = { 0 };  //count[i]Ϊ��i���û����Ƽ��������Լ������е���Ʒ������rnumΪ�ܹ����е���Ʒ����
	for (i = 0; i<userSum; i++)
	{
		for (j = 0; j<itemSum; j++)
		{
			if (ArrTest[j][i] != 0)          //����û�i����Ʒj����Ȥ
			{
				for (k = 0; k<recommendNum; k++)
				{
					if (ArrRecommended[i][k] == j)    //�û�i����Ʒj����Ȥ��ͬʱ����Ʒj���Ǳ��Ƽ�����Ʒ
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
			k = ArrRecommended[i][j];      // k=�Ƽ����û�i�ĵ�j����Ʒ����Ʒ��
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
				count = count + a[ArrRecommended[i][j]][ArrRecommended[i][k]];
			}
		}
		sum = sum + 1 - 2 * count / (recommendNum*(recommendNum - 1));  //��ÿ���û��Ķ�����ֵ�ۼӵ�sum
		count = 0;
	}
	diversity = sum / userSum;
	return diversity;
}



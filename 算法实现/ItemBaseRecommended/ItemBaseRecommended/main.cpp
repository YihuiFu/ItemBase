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
const int userSum = 6040;          //数据集的用户总数
const int itemSum = 3952;          //数据集的产品总数
const int recommendNum = 10;                    //为每个用户推荐产品的数量recommendNum
char datasetFile[100] = "E:\\mytest\\ratings.dat";    //UserID::MovieID::Rating::Time
int ArrTrain[itemSum][userSum] = { 0 };	       //训练集
int ArrTest[itemSum][userSum] = { 0 };           //测试集
double trainuseritem[userSum][itemSum] = { 0.0 };//兴趣程度 矩阵
int ArrRecommended[userSum][recommendNum] = { 0 };     //用户推荐矩阵，向每用户推荐recommendNum个
struct Simi
{
	double value; //相似值 
	int num;	 //相似物品号 
};
Simi ArrOrderedItemSimi[itemSum][itemSum]; //排序后的相似性矩阵

//第一步，拆分数据集为测试集ArrTest和训练集ArrTrain，其中1/m为测试集
int SplitData(int m, int k);
//寻找数据集每条记录对应的用户号和物品号
int Buf_UIR(char* buf, int* user, int* item);
double Simility(int* ItemA, int* ItemB);    //第二步，计算物品之间的相似性，获得物品相似性矩阵
int sort(double* simArr, Simi* simStruct); //第三步，物品相似性矩阵排序（根据相似性由高到低排序）
double getUserLikeItem(int i, int j, int k); //第四步，得到用户i对物品j预测兴趣程度，利用k个最近邻来计算
int getRecommend();                       //第五步，通过物品兴趣程度，推荐前recommendNum个
double Recall();                           //第六步，计算召回率、准确率和覆盖率
double Precision();
double Coverage();
double Diversity(double **a);//计算多样性

void main()
{
	int i, j;
	double recall, precision, coverage, diversity;
	int k = 10;        //取用户的 10 个最近邻居

	//1.将数据集分为训练集 和 测试集
	SplitData(8, 1);


	//输出训练集的的前
	/*for (int i = 0; i < 10; i++)
	{
	cout <<ArrTrain[0][i]<<" " ;
	}*/

	//动态为物品相似性矩阵分配空间
	double **ArrItemSimi;
	ArrItemSimi = (double**)malloc(sizeof(double*)*itemSum);
	for (i = 0; i < itemSum; i++)
	{
		ArrItemSimi[i] = (double*)malloc(sizeof(double)*itemSum);
	}
	//2.计算物品之间相似性，得到相似性矩阵
	for (i = 0; i < itemSum; i++)
	{
		for (j = 0; j < itemSum; j++)
		{
			ArrItemSimi[i][j] = Simility(ArrTrain[i], ArrTrain[j]);
		}
	}

#pragma region --输出物品相似性矩阵
	//输出物品相似性矩阵	

	cout << "物品相似性矩阵:\n";
	for (i = 0; i < 5; i++)
	{
		cout << "Item" << i + 1 << ":   " << endl;
		for (j = 0; j<100; j++)
		{
			cout << setw(10) << ArrItemSimi[i][j];
			if (j>0 && j % 8 == 0)
			{
				cout << "\n";
			}
		}
		cout << endl;
	}

#pragma endregion


	//3.物品相似度由高到低排序
	for (i = 0; i < itemSum; i++)
	{
		sort(ArrItemSimi[i], ArrOrderedItemSimi[i]);
	}
	//输出排序后的物品相似性矩阵
	cout << "排序后的物品相似性矩阵：";
	for (i = 0; i < 5; i++)
	{
		cout << "item" << i + 1 << ":  " << endl;
		for (j = 0; j<100; j++)
		{
			cout << setw(15) << ArrOrderedItemSimi[i][j].num << "," << ArrOrderedItemSimi[i][j].value;
			if (j>0 && j % 6 == 0)
			{
				cout << "\n";
			}
		}
		cout << endl;
	}

	//4.得到用户对物品兴趣程度的矩阵
	for (i = 0; i < userSum; i++)
	{
		for (j = 0; j < itemSum; j++)
		{
			if (ArrTrain[j][i] == 0)            //如果用户i对物品j没有过行为，才计算i对j的预测兴趣程度
				trainuseritem[i][j] = getUserLikeItem(i, j, k);
		}
	}
	//输出用户对物品兴趣的矩阵
	cout << "用户对物品兴趣度矩阵：";
	for (i = 0; i < 5; i++)
	{
		cout << "User" << i + 1 << ":   " << endl;
		for (j = 0; j<100; j++)
		{
			cout << setw(10) << trainuseritem[i][j];
			if (j>0 && j % 8 == 0)
			{
				cout << "\n";
			}
		}
		cout << endl;
	}


	//5.通过物品兴趣程度，推荐前recommendNum个
	getRecommend();
	//输出推荐矩阵-------------------------------------------------
	/*for (i = 0; i<10; i++)
	{
	cout << "向用户 " << i << " 推荐:   " << endl;
	for (j = 0; j<recommendNum; j++)
	{
	cout << recommend[i][j] << "  ";
	}
	cout << endl;
	}
	//---------------------------------------------
	*/
	recall = Recall();	//计算召回率

	precision = Precision();//计算准确率

	//coverage = Coverage();//计算覆盖率

	cout << "算法结果如下：\n";

	cout << setw(10) << "召回率" << setw(10) << "准确率" << endl;
	cout << "\n";
	cout << setw(10) << recall << setw(10) << precision << endl;
	cout << "\n";

	//释放内存 
	for (i = 0; i < itemSum; i++)
	{
		free(ArrItemSimi[i]);
	}
	free(ArrItemSimi);

	cout << "\n ---计算完成---\n";
	cout << "请输入需要推荐产品的用户ID (0<ID<6040)：\n";
	int userID;
	cin >> userID;
	cout << "向该用户 " << userID << " 推荐的产品ID如下：\n";
	for (int i = 0; i < 10; i++)
	{
		cout << ArrRecommended[userID][i] << " ";

	}

	int hehe;
	cin >> hehe;
}

//拆分数据集为测试集ArrTest和训练集trainuser，其中1/m为测试集,取不同的k<=m-1值 在相同的随即种子下可得到不同的测/训集合
int SplitData(int m, int k)
{
	fp = fopen(datasetFile, "r");
	char tmpbuf[100];		//暂存文件一行记录
	int userNum;            //用户编号
	int itemNum;            //物品编号

	if (!fp)
	{
		cout << "open datasetfile failed";
		return 1;
	}
	else
	{
		srand((unsigned)time(NULL));                  //设置当前时间为随机种子
		while (!feof(fp))
		{
			fgets(tmpbuf, 100, fp);                     //将fp指向的当前记录存到tmpbuf[100]
			Buf_UIR(tmpbuf, &userNum, &itemNum);        //寻找数据集每条记录对应的用户号和物品号
			if (userNum <= userSum&&itemNum <= itemSum)
			{
				if (rand() % (m - 1) == k)                       //判断随机产生0-7之间的随机数是否等于k
					ArrTest[itemNum - 1][userNum - 1] = 1;        //rate为评分，再此实验中只需统计有无评分的，无需讨论具体评分
				else
					ArrTrain[itemNum - 1][userNum - 1] = 1;  //用户号的物品号均从0开始算起，
			}
		}
		fclose(fp);
	}
	return 1;
}
//寻找数据集每条记录对应的用户号和物品号
int Buf_UIR(char* buf, int* user, int* item)
{
	char U[20] = { 0 };
	char I[20] = { 0 };
	char R[20] = { 0 };
	char *Temp;
	int curpos;

	//查找用户号
	Temp = strchr(buf, ':');
	curpos = Temp - buf;
	strncpy(U, buf, curpos);
	*user = atoi(U);
	//查找item号
	Temp = strchr(buf + curpos + 2, ':');
	//curpos = Temp - buf;
	strncpy(I, buf + curpos + 2, Temp - buf - curpos - 2);
	*item = atoi(I);

	return 1;
}

//利用训练集计算用户之间相似度
/* 计算向量ItemA和ItemB的相似性，返回值为ItemA和ItemB的相似度 */
double Simility(int* ItemA, int* ItemB)
{
	int comUser = 0;                   //ItemA与ItemB的都被用户评论的用户个数
	double simility = 0.0;
	int countIa = 0;
	int countIb = 0;

	int i;
	//--------------------------------------------
	for (i = 0; i<userSum; i++)
	{
		//int aa = ItemA[4000];
		if (ItemA[i]>0 && ItemB[i] > 0)
		{
			comUser++;//查找ItemA与ItemB的都被用户评论的用户个数
		}
		if (ItemA[i] > 0){
			countIa++;//评论ItemA的用户数量
		}
		if (ItemB[i] > 0){
			countIb++;//评论ItemB的用户数量
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


/*物品相似性矩阵排序（根据相似性由高到低排序），每行第一个是自己*/
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
	int finflag[itemSum] = { 0 };//是否已排好序标示
	int maxnum;//当前最大标号
	int m = 0, t;

	for (int i = 0; i<itemSum; i++)
	{
		t = 0;                  //从左到右寻找第一项 没有被排序好的用户号
		while (finflag[t] != 0)  //如果已被排序
			t++;
		maxnum = t;                    //设置最大相似度物品号。
		for (int j = 0; j<itemSum; j++)
		{
			if (simArr[j]>simArr[maxnum] && finflag[j] == 0)
			{
				maxnum = j;
			}
		}
		simStruct[m].num = maxnum;
		simStruct[m].value = simArr[maxnum];
		finflag[maxnum] = 1;             //本次循环最大相似度物品标记已经排序
		m++;
		//cout<<i<<":"<<maxnum<<":"<<simArr[maxnum]<<"  ";

	}

	return 1;
#endif
}

//得到用户i对物品j预测兴趣程度，用于推荐
double getUserLikeItem(int i, int j, int k)
{
	for (int x = 1; x <= k; x++)//从物品j最相似的k个物品中，找出用户i有过行为的物品,因为第一个ArrOrderedItemSimi[][]中每行第一个存放的是自己，所以从第二个开始算，x=1
	{
		if (ArrTrain[ArrOrderedItemSimi[j][x].num][i] > 0)//若这个用户同样对相似物品也有过行为
		{
			trainuseritem[i][j] += ArrOrderedItemSimi[j][x].value;
		}
	}
	return trainuseritem[i][j];
}
/*通过物品兴趣程度，推荐前recommendNum个*/
int getRecommend()
{
	int maxnum;//当前最感兴趣物品号
	for (int i = 0; i < userSum; i++)
	{
		int finflag[itemSum] = { 0 };//是否已排好序标示
		for (int x = 0; x < recommendNum; x++)//推荐recommendNum个
		{
			maxnum = 0;
			while (finflag[maxnum] != 0)  //判断此次循环的第一个未被排序过物品
				maxnum++;
			for (int j = 0; j < itemSum; j++)  //每循环一次就寻找此次感兴趣最大的物品
			{
				if (trainuseritem[i][maxnum] < trainuseritem[i][j] && finflag[j] == 0)
				{
					maxnum = j;
				}
			}
			finflag[maxnum] = 1;
			ArrRecommended[i][x] = maxnum;//recommend数组从0开始使用
		}
	}
	return 1;
}


//计算召回率
double Recall()   //ArrTest[usersum][itemSum], recommend[usersum][recommendNum],recommendNum为推荐数
{
	int i, j, k, tnum = 0, rnum = 0, count[userSum] = { 0 };  //tnum为测试集上喜欢的物品总数;count[i]为第i个用户的推荐结果与测试集上命中的物品个数交集，rnum为总共命中的物品个数
	for (i = 0; i < userSum; i++)
	{
		for (j = 0; j < itemSum; j++)
		{
			if (ArrTest[j][i] != 0)          //如果用户i对物品j感兴趣
			{
				tnum++;
				for (k = 0; k < recommendNum; k++)
				{
					if (ArrRecommended[i][k] == j)    //用户i对物品j感兴趣的同时，物品j还是被推荐的物品
						count[i]++;
				}
			}
		}
		rnum = rnum + count[i];
	}
	return rnum / (tnum*1.0);
}
//计算准确率
double Precision()   //ArrTest[usersum][itemSum], recommend[usersum][recommendNum],recommendNum为推荐数
{
	int i, j, k, rnum = 0, count[userSum] = { 0 };  //count[i]为第i个用户的推荐结果与测试集上命中的物品个数，rnum为总共命中的物品个数
	for (i = 0; i < userSum; i++)
	{
		for (j = 0; j < itemSum; j++)
		{
			if (ArrTest[j][i] != 0)          //如果用户i对物品j感兴趣
			{
				for (k = 0; k < recommendNum; k++)
				{
					if (ArrRecommended[i][k] == j)    //用户i对物品j感兴趣的同时，物品j还是被推荐的物品
						count[i]++;
				}
			}
		}
		rnum = rnum + count[i];
	}
	return rnum / (recommendNum*userSum*1.0);
}
//计算覆盖率
double Coverage()
{
	int i, j, k, sum = 0, count[itemSum] = { NULL };  ////count[i]！=NULL表示第i个物品被覆盖了,sum统计被覆盖的物品个数
	for (i = 0; i < userSum; i++)
	{
		for (j = 0; j < recommendNum; j++)
		{
			k = ArrRecommended[i][j];      // k=推荐给用户i的第j个物品的物品号
			if (count[k] == NULL)      //判断第k个物品是否已经覆盖
			{
				count[k] = 1;
				sum++;
			}
		}
	}
	return sum / (itemSum*1.0);
}
//计算多样性
double Diversity(double **a)
{
	double diversity, count = 0, sum = 0;   //count存放对每一个用户推荐的物品之间的相似性值，sum存放所有推荐物品的相似性值总和
	for (int i = 0; i < userSum; i++)
	{
		for (int j = 0; j < recommendNum; j++)
		{
			for (int k = j + 1; k < recommendNum; k++)
			{
				count = count + a[ArrRecommended[i][j]][ArrRecommended[i][k]];
			}
		}
		sum = sum + 1 - 2 * count / (recommendNum*(recommendNum - 1));  //将每个用户的多样性值累加到sum
		count = 0;
	}
	diversity = sum / userSum;
	return diversity;
}



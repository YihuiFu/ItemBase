﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;
using System.Data.Sql;
using System.Data.SqlClient;


namespace RecommendedHelper.ReadDatasetToSQL
{
    public class main
    {
        const int userTotal = 6040; //用户总数
        const int itemTotal = 3952; //物品总数
        const int recommendNum = 10;// 推荐数量

        int[,] TrainSet = new int[itemTotal, userTotal];  //训练集
        int[,] TestSet = new int[itemTotal, userTotal]; //测试集

        double [,] SimilarityMatrix=new double[itemTotal,itemTotal];  //相似性矩阵， 存储物品的相似度
        double[,] UserInterestMatrix = new double[userTotal, itemTotal];  // 用户兴趣物品矩阵

        int[,] RecommendSet = new int[userTotal, recommendNum];// 存储用户推荐结果

        /// <summary>
        /// 将数据集分成训练集 和 测试集
        /// </summary>
        /// <param name="i"></param>
        /// <param name="j"></param>
        public void SplitDataSet(int i,int j)
        {
            string sqlStr = "select * from Ratings";

            var dataReader = SqlSeverProvider.ExecuteReader(sqlStr);
            var userID= dataReader.GetInt32(0);
            var itemID = dataReader.GetInt32(1);
            var rate = dataReader.GetInt32(2);

            Random rand = new Random(DateTime.Now.Millisecond);
            if (rand.Next()%(i-1)==j)
            {
                TestSet[itemID-1,userID]=1;
            }
            else
            {
                TrainSet[itemID - 1, userID - 1] = 1;
            }
        }

        /// <summary>
        /// 计算物品 A 和 B的相似性
        /// </summary>
        /// <param name="itemA"></param>
        /// <param name="itemB"></param>
        public double CalculateSimilarityForA_B(int itemA,int itemB)
        {
            int commonCount = 0; //物品A 和物品 B都被用户评论的个数
            int ACount = 0;
            int BCount = 0;
            double similarity = 0.0;
            for (int i = 0; i < userTotal; i++)
            {
                if (TrainSet[itemA,i]>0 && TrainSet[itemB,i]>0)
                {
                    commonCount++;
                }
                if (TrainSet[itemA,i]>0)
                {
                    ACount++;
                }
                if (TrainSet[itemB,i]>0)
                {
                    BCount++;
                }
            }

            double temp = Math.Sqrt(ACount*BCount);
            if (temp==0)
            {
                return 0.0;
            }
            else
            {
                similarity = commonCount / temp;
                return similarity;
            }
        }

        /// <summary>
        /// 计算所有物品之间的相似性，得到相似性矩阵
        /// </summary>
        public void GetSimilarityMatrix()
        {
            for (int i = 0; i < itemTotal; i++)
            {
                for (int j = 0; j < itemTotal; j++)
                {
                    SimilarityMatrix[i, j] = CalculateSimilarityForA_B(i,j);
                }
            }
        }

        /// <summary>
        /// 推荐物品
        /// </summary>
        public void GetRecommend()
        {
            int mostInterestNum; //当前最感兴趣物品
            for (int i = 0; i < recommendNum; i++)
            {
                mostInterestNum = 0;
            }
        }

        /// <summary>
        /// 计算召回率 和 准确率
        /// </summary>
        /// <returns></returns>
        public void GetResult_Recall_Precision()
        {
            int test_InterestNum = 0;   //测试集上用户兴趣的物品数量
            int totalNum = 0;     //总共命中的物品个数
            int []count=new int[userTotal];      //cout[i] 为用户的推荐结果和测试集命中物品个数交集
            double recall = 0.0;    //召回率
            double precision = 0.0;   //准确率
            for (int i = 0; i < userTotal; i++)
            {
                for (int j = 0; j < itemTotal; j++)
                {
                    if (TestSet[j,i]!=0)  //用户 i 对物品 j 感兴趣
                    {
                        test_InterestNum++;
                        for (int k = 0; k < recommendNum; k++)
                        {
                            if (RecommendSet[i,k]==j)    //用户对物品 j 感兴趣，同时也是物品 j 也是推荐物品
                            {
                                count[i]++;
                            }
                        }
                    }
                }
                totalNum = totalNum + count[i];
            }


            recall= totalNum / (test_InterestNum * 1.0); // 召回率
            precision = totalNum / (recommendNum * userTotal * 1.0);  //准确率
        }


    }
}

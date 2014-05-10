﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Data;
using System.Data.Sql;
using System.Data.SqlClient;


namespace RecommendedHelper.ReadDatasetToSQL
{
    public class main
    {
        public const int userTotal = 6040; //用户总数
        public const int itemTotal = 3952; //物品总数
        public const int recommendNum = 10;// 推荐数量

        public double recall = 0.0;    //召回率
        public double precision = 0.0;   //准确率

        public int[,] TrainSet = new int[itemTotal, userTotal];  //训练集
        public int[,] TestSet = new int[itemTotal, userTotal]; //测试集

        public double[,] SimilarityMatrix = new double[itemTotal, itemTotal];  //相似性矩阵， 存储物品的相似度
        public double[,] UserInterestMatrix = new double[userTotal, itemTotal];  // 用户兴趣物品矩阵

        public int[,] RecommendSet = new int[userTotal, recommendNum];// 存储用户推荐结果
        public struct simi
        {
            public double value;  //相似值
            public int num;   // 相似物品号
        };

        public simi[,] OrderedSimilarityMatrix = new simi[itemTotal, itemTotal];  //排序后的相似性矩阵

        /// <summary>
        /// 将数据集分成训练集 和 测试集  ----------------Success
        /// </summary>
        /// <param name="i"></param>
        /// <param name="j"></param> 
        public void SplitDataSet(int i, int j)
        {
            Array.Clear(TestSet, 0, 3952 * 6040);
            Array.Clear(TrainSet, 0, 3952 * 6040);
            Thread.Sleep(1000);
            string sqlStr = "select * from Ratings";
            var dataReader = SqlSeverProvider.ExecuteReader(sqlStr);
            while (dataReader.Read())
            {
                var userID = dataReader.GetInt32(0);
                var itemID = dataReader.GetInt32(1);
                var rate = dataReader.GetInt32(2);
                Random rand = new Random(DateTime.Now.Millisecond);
                try
                {
                    if (rand.Next() % (i - 1) == j)
                    {
                        TestSet[itemID - 1, userID] = 1;
                    }
                    else
                    {
                        TrainSet[itemID - 1, userID - 1] = 1;
                    }
                }
                catch { }
            }
        }

        /// <summary>
        /// 计算物品 A 和 B的相似性
        /// </summary>
        /// <param name="itemA"></param>
        /// <param name="itemB"></param>
        public double CalculateSimilarityForA_B(int itemA, int itemB)
        {
            int commonCount = 0; //物品A 和物品 B都被用户评论的个数
            int ACount = 0;
            int BCount = 0;
            double similarity = 0.0;
            for (int i = 0; i < userTotal; i++)
            {
                if (TrainSet[itemA, i] > 0 && TrainSet[itemB, i] > 0)
                {
                    commonCount++;
                }
                if (TrainSet[itemA, i] > 0)
                {
                    ACount++;
                }
                if (TrainSet[itemB, i] > 0)
                {
                    BCount++;
                }
            }

            double temp = Math.Sqrt(ACount * BCount);
            if (temp == 0)
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
        /// 计算所有物品之间的相似性，得到相似性矩阵 --------------Success
        /// </summary>
        public void GetSimilarityMatrix()
        {
            for (int i = 0; i < itemTotal; i++)
            {
                for (int j = 0; j < itemTotal; j++)
                {
                    SimilarityMatrix[i, j] = CalculateSimilarityForA_B(i, j);
                    //double similar= CalculateSimilarityForA_B(i, j);
                    //string sqlStr = string.Format("insert into SimilarityMatrix values({0},{1},{2})",i+1,j+1,similar);
                    //SqlSeverProvider.ExecuteNonQuery(sqlStr);
                }
            }

            // 输出相似性矩阵
            //for (int i = 0; i < 3; i++)
            //{
            //    Console.WriteLine("item " + i + ":");
            //    for (int j = 0; j < 100; j++)
            //    {
            //        Console.Write(SimilarityMatrix[i, j] + "  ");
            //    }
            //}



        }

       /// <summary>
        /// 排序物品相似性矩阵
       /// </summary>
       /// <param name="itemIndex">矩阵行index</param>
        public void SortSimilarityMatrix(int itemIndex)
        {
            int[] orderFlag = new int[itemTotal];  //是否已排序好
            int maxnum;
            int m = 0;
            int t = 0;
            for (int i = 0; i < itemTotal; i++)
            {
                t = 0;
                while (orderFlag[t] != 0)
                    t++;
                maxnum = t;
                for (int j = 0; j < itemTotal; j++)
                {
                    if (SimilarityMatrix[itemIndex, j] > SimilarityMatrix[itemIndex, maxnum] && orderFlag[j] == 0)
                    {
                        maxnum = j;
                    }
                }
                simi temp = new simi {num=maxnum,value=SimilarityMatrix[itemIndex,maxnum] };

                OrderedSimilarityMatrix[itemIndex, m] = temp;
                orderFlag[maxnum] = 1;
                m++;
            }
        }


        public void GetOrderSimilarityMatrixForAllItem()
        {
            for (int i = 0; i < itemTotal; i++)
            {
                SortSimilarityMatrix(i);
            }
        }

        /// <summary>
        /// 通过 K 个最相近邻居，获取用户user 对item 的兴趣程度
        /// </summary>
        /// <param name="user">用户ID</param>
        /// <param name="item">物品ID</param>
        /// <param name="k"> K 个最近邻居</param>
        /// <returns></returns>
        public double CalculateUserInterest(int userId,int itemId,int k)
        {
            for (int i = 1; i < k; i++)
            {
                UserInterestMatrix[userId, itemId] += OrderedSimilarityMatrix[itemId, i].value;
            }
            return UserInterestMatrix[userId, itemId];
        }

        /// <summary>
        /// 计算所有用户的兴趣程度矩阵
        /// </summary>
        public void GetUserInterestMatrix()
        {
            for (int i = 0; i < userTotal; i++)
            {
                for (int j = 0; j < itemTotal; j++)
                {
                    if (TrainSet[j,i]==0)
                    {
                        UserInterestMatrix[i, j] = CalculateUserInterest(i,j,10);//----------K默认是10
                    }
                }
            }
        }

        /// <summary>
        /// 推荐物品
        /// </summary>
        public void GetRecommend()
        {
            int mostInterestNum; //当前最感兴趣物品
            for (int i = 0; i < userTotal; i++)
            {
                int []orderFlag=new int[itemTotal];
                for (int j = 0; j < recommendNum; j++)
                {
                    mostInterestNum = 0;
                    while (orderFlag[mostInterestNum] != 0)
                        mostInterestNum++;
                    for (int k = 0; k < itemTotal; k++)
                    {
                        if (UserInterestMatrix[i,mostInterestNum]<UserInterestMatrix[j,k]&&orderFlag[k]==0)
                        {
                            mostInterestNum = k;
                        }
                    }
                    orderFlag[mostInterestNum] = 1;
                    RecommendSet[i, j] = mostInterestNum;
                }
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
            int[] count = new int[userTotal];      //cout[i] 为用户的推荐结果和测试集命中物品个数交集

            for (int i = 0; i < userTotal; i++)
            {
                for (int j = 0; j < itemTotal; j++)
                {
                    if (TestSet[j, i] != 0)  //用户 i 对物品 j 感兴趣
                    {
                        test_InterestNum++;
                        for (int k = 0; k < recommendNum; k++)
                        {
                            if (RecommendSet[i, k] == j)    //用户对物品 j 感兴趣，同时也是物品 j 也是推荐物品
                            {
                                count[i]++;
                            }
                        }
                    }
                }
                totalNum = totalNum + count[i];
            }


            recall = totalNum / (test_InterestNum * 1.0); // 召回率
            precision = totalNum / (recommendNum * userTotal * 1.0);  //准确率
        }


    }
}

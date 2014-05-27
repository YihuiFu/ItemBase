using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RecommendedSystem.Core
{
   public class ItemBase
    {
        public const int userTotal = 6040; //用户总数
        public const int itemTotal = 3952; //物品总数
        public const int recommendNum = 10;// 推荐数量

        public double recall = 0.0;    //召回率
        public double precision = 0.0;   //准确率

        public int[,] TrainSet = new int[itemTotal, userTotal];  //训练集
        public int[,] TestSet = new int[itemTotal, userTotal]; //测试集

        public double[,] SimilarityMatrix = new double[itemTotal, itemTotal];  //相似性矩阵， 存储物品的相似度
        public double[,] UserInterestMatrix = new double[userTotal, itemTotal];  // 用户对物品的兴趣程度矩阵

        public int[,] RecommendSet = new int[userTotal, recommendNum];// 存储用户推荐结果
        public struct simi
        {
            public double value;  //相似值
            public int num;   // 相似物品号
        };

        public simi[,] OrderedSimilarityMatrix = new simi[itemTotal, itemTotal];  //排序后的相似性矩阵

    }
}

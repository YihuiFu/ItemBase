using System;
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
        public void Recommend()
        { 

        }

        /// <summary>
        /// 计算召回率
        /// </summary>
        /// <returns></returns>
        public double Recall()
        {
            return 0.0;
        }

        public double Precision()
        {
            return 0.0;
        }

    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.Sql;
using System.Data.SqlClient;
using RecommendedSystem.Common;
using RecommendedSystem.Core;

namespace RecommendedSystem.Test
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("计算后的物品相似性矩阵(随机)：");
            OutSimilarMatrix();

            Console.WriteLine("\n用户对物品的兴趣矩阵:");
            OutUsersInterest();

            Console.ReadKey();

        }


        private static void OutSimilarMatrix()
        {
            for (int i = 0; i <3; i++)
            {
                Random rand = new Random(DateTime.Now.Millisecond);
                var item = rand.Next() % 3952;
                Console.WriteLine("item　" + item + ":");
                string sqlStr = string.Format("select top(100) itemTwo,similarity from SimilarityMatrix where itemOne={0} order by itemTwo", item);
                SqlDataReader reader = SqlSeverProvider.ExecuteReader(sqlStr);
                int num = 1;
                while (reader.Read())
                {
                    
                    Console.Write(reader.GetInt32(0) + ":");
                    Console.Write(reader.GetDouble(1).ToString("0.0000") + " ; ");
                    if (num > 0 && num % 5== 0)
                    {
                        Console.Write("\n");
                    }
                    num++;
                }
                Console.Write("\n");

            }


        }

        private static void OutUsersInterest()
        {
            for (int i = 0; i < 5; i++)
            {
                Random rand = new Random(DateTime.Now.Millisecond);
                var user = rand.Next() % 6040;
                Console.WriteLine("User "+ user + ":");
                string sqlStr = string.Format("select top(100) itemID,interest from UserInterest where userID={0} and interest>0 order by itemID", user);
                SqlDataReader reader = SqlSeverProvider.ExecuteReader(sqlStr);
                int num = 1;
                while (reader.Read())
                {

                    Console.Write(reader.GetInt32(0) + ":");
                    Console.Write(reader.GetDouble(1).ToString("0.0000") + " ; ");
                    if (num > 0 && num % 5 == 0)
                    {
                        Console.Write("\n");
                    }
                    num++;
                }
                Console.Write("\n");

            }
        }
    }
}

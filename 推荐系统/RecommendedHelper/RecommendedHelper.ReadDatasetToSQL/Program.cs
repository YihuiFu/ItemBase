using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Data;
using System.Data.SqlClient;
using System.Data.Sql;

namespace RecommendedHelper.ReadDatasetToSQL
{
   public  class Program
    {
        static void Main(string[] args)
        {
            #region ---读数据到SQL
            // string filePath = @"F:\2014-毕设\1智能电子商务系统的用户行为建模和数据挖掘算法研究\Graduation.git\ItemBase\算法实现\ratings.dat";
            // StreamReader reader = new StreamReader(filePath);

            // ArrayList arr = new ArrayList();
            // int count = 0;
            // while (!reader.EndOfStream)
            // {
            //     string readLine = reader.ReadLine();
            //     var firstIndex = readLine.IndexOf(':');
            //     var lastIndex = readLine.LastIndexOf(':');
            //     var userID = readLine.Substring(0, firstIndex);
            //     var itemID = readLine.Substring(firstIndex + 2, lastIndex - firstIndex - 6);
            //     var rate = readLine.Substring(lastIndex - 2, 1);
            //     int userId = int.Parse(userID);
            //     int itemId = int.Parse(itemID);
            //     int rating = int.Parse(rate);
            //     Insert(userId,itemId,rating);
            //     count++;
            //     Console.WriteLine(count);
            //     //Console.WriteLine(itemID);
            //     //  Console.WriteLine(" " + rate);
            // }




            // reader.Close();
            // //arr.Sort();
            // //arr.Reverse();
            // //foreach (var item in arr)
            // //{
            // //    Console.WriteLine(item);
            // //}

            //// string sqlStr = "insert into tb_test values(10)";
            ////var result= SqlSeverProvider.ExecuteNonQuery(sqlStr);
            ////Console.WriteLine(result);
            // Console.WriteLine(count);

            #endregion

            //Recommend recommend = new Recommend();
            //recommend.SplitData();

            //---------测试--------------------------------------------

            //main recommend = new main();
            //recommend.SplitDataSet(8, 1);
            //recommend.GetSimilarityMatrix();

            //----------------------------------------------------------

            //测试数组排序
            //int[,] intArr = new int[,] { {9,3,5,1},{7,0,6,5}};
            //Array.Sort(intArr,0,8);
            //for (int i = 0; i < 2; i++)
            //{
            //    for (int j = 0; j < 4; j++)
            //    {
            //        Console.WriteLine(intArr[i, j]);
            //    }
               
            //}

            Dictionary<int, double> items = new Dictionary<int, double>();
            items.Add(5,0.32);
            items.Add(4,0.21);
            items.Add(6,0.36);
            items.Add(1,0.25);
          //  items.OrderBy(it=>it.Value);
            var result = from it in items orderby it.Value select it;
            foreach (var item in result)
            {
                Console.WriteLine(item.Key+" "+item.Value);
            }

            Console.ReadKey();

        }

        public static void TestArr()
        {
            int[,] arr = new int[2, 2];
            arr[0, 0] = 1;
            arr[0, 1] = 2;
            arr[1, 0] = 3;
            arr[1, 1] = 4;
            for (int i = 0; i < 2; i++)
            {
                
            }

        }

        public static   void Insert(int userId,int itemId,int rate)
        {
            string sqlStr = "insert into Ratings values (@userID,@itemID,@rating)";
            SqlParameter[] parms ={
                                     new SqlParameter("@userID",userId),
                                     new SqlParameter("@itemID",itemId),
                                     new SqlParameter("@rating",rate)
                              };
            var result = SqlSeverProvider.ExecuteNonQuery(sqlStr, parms);

        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;
using System.Data.SqlClient;
using System.Data.Sql;
using System.IO;
using System.IO.MemoryMappedFiles;

namespace RecommendedHelper.ReadDatasetToSQL
{
   public  class Recommend
    {
        //1.分离数据集
       Dictionary<int, Dictionary<int, int>> ArrTrain = new Dictionary<int, Dictionary<int, int>>();
       public void SplitData()
       {
           Dictionary<int, int> item = new Dictionary<int, int>();
           item.Add(193,4);
           ArrTrain.Add(1,item);

           foreach (var i in ArrTrain)
           {
               var user = i.Key;
               var itemRate = i.Value;
               foreach (var j in itemRate)
               {
                   var itemId = j.Key;
                   var rate = j.Value;
               }
           }
           var result = 12;
       }

       #region --内存映射
      

       string filePath = @"F:\2014-毕设\1智能电子商务系统的用户行为建模和数据挖掘算法研究\Graduation.git\ItemBase\算法实现\ratings.dat";
       private const int FILE_SIZE = 1024 * 1024 * 25;
       private MemoryMappedFile memoryFile = null;
       private MemoryMappedViewAccessor accessor;
       public void CreateMemoryMapFile()
       {
           try
           {
               memoryFile = MemoryMappedFile.CreateFromFile(filePath, FileMode.OpenOrCreate, "MyFile", FILE_SIZE);
               accessor = memoryFile.CreateViewAccessor();
               ShowFileContents();
           }
           catch (Exception)
           {

               throw;
           }
       }

       public void ShowFileContents()
       {
           StringBuilder sb = new StringBuilder(FILE_SIZE);
           for (int i = 0; i < FILE_SIZE; i += 2)
           {
               char ch = accessor.ReadChar(i);
               sb.Append(ch);
               var result = sb.ToString();
           }
           Console.Write(sb.ToString());
       } 
       #endregion
       

       //public int[] GetRecomend(int userId,List<int> ?itemIds)
       //{
       //    List<int> items = new List<int>();
       //    items.Sort();
       //    return null;
       //}


       public int[] GetRecommend(int userId,params int[]itemIds)
       {
           Dictionary<int, double> recommendedList = new Dictionary<int, double>();
           //根据用户id 获取最佳推荐的10个，添加到recommendedList
           string sqlStr =string.Format( "select top(10) itemID from UserInterest where userID={0} and interest !=0 order by interest desc",userId);
           SqlSeverProvider.ExecuteReader(sqlStr);

           //根据itemid 获取相似度较高的物品，添加到recommendedList
          // select top(20)* from SimilarityMatrix where itemOne=176 order by similarity desc

           // select top(10) itemID,COUNT(itemID) as total from Ratings group by itemID order by total desc  高访问率

           //根据相似度高低对 recommendedList进行排序
           var result = from it in recommendedList orderby it.Value select it;
           foreach (var item in result.Take(10))
           {
               Console.WriteLine(item.Key);
           }
          
           return null;
       }


   
    }
}


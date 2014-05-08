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
       
   
    }
}


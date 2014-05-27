using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.Sql;
using System.Data.SqlClient;
using RecommendedSystem.Common;

namespace RecommendedSystem.Core
{
   public static class RecommendedSystemHelper
    {
       public static bool Login(string userName)
       {
           int userId = int.Parse(userName);
           string sqlStr = string.Format("select * from Users where userID={0}",userId);
           var result = SqlSeverProvider.ExecuteScalar(sqlStr);
           if (result!=null)
           {
               return true;
           }
           else
           {
               return false;
           }
       }

       /// <summary>
       /// 根据用户的评分行为进行推荐
       /// </summary>
       /// <param name="userId"></param>
       /// <returns></returns>
       public static List<int> GetRecommend(int userId)
       {
           List<int> itemsList = new List<int>();
           string sqlStr = string.Format("select itemID from Recommended where userID={0}",userId);
          SqlDataReader dataReader= SqlSeverProvider.ExecuteReader(sqlStr);
           while(dataReader.Read())
           {
               itemsList.Add(dataReader.GetInt32(0));
           }
           return itemsList;
       }

       /// <summary>
       ///  获取 评论数量最高的item
       /// </summary>
       /// <returns></returns>
       public static List<int> GetTop()
       {
           List<int> itemsList = new List<int>();
           string sqlStr = string.Format("select top(10) itemID,COUNT(itemID) as total from Ratings group by itemID order by total desc");
           SqlDataReader dataReader = SqlSeverProvider.ExecuteReader(sqlStr);
           while (dataReader.Read())
           {
               itemsList.Add(dataReader.GetInt32(0));
           }
           return itemsList;

       }

       /// <summary>
       /// 根据购物车推荐
       /// </summary>
       /// <param name="userId"></param>
       /// <param name="shoppingCart"></param>
       /// <returns></returns>
       public static List<int> GetRecommend(int userId,List<int>shoppingCart)
       {
           List<int> itemsList = new List<int>();
           string sqlStr = string.Format("select top(10) itemID,COUNT(itemID) as total from Ratings group by itemID order by total desc");
           SqlDataReader dataReader = SqlSeverProvider.ExecuteReader(sqlStr);
           while (dataReader.Read())
           {
               itemsList.Add(dataReader.GetInt32(0));
           }
           return itemsList;
       }
    }
}

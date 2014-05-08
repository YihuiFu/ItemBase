using System;
using System.Text;
using System.Data;
using System.Configuration;
using System.Data.SqlClient;


namespace RecommendedHelper.ReadDatasetToSQL
{
   public static class DBConnect
    {
       private static string connectString = @"Data Source=.;Initial Catalog=MovieLens;Trusted_Connection=SSPI";
       public static SqlConnection Connect()  //.\SQLEXPRESS   sql.m125.vhostgo.com
       {
           SqlConnection con = new SqlConnection(connectString);
           return con;
       }
    }
}

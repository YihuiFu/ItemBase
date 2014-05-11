using System;
using System.Data;
using System.Data.Sql;
using System.Data.SqlClient;
using System.Configuration;

namespace RecommendedSystem.Common
{
    public abstract  class SqlSeverProvider
    {
        /// <summary>
        /// Execute  Insert,Update
        /// </summary>
        /// <param name="sql"></param>
        /// <param name="cmdParams"></param>
        /// <returns></returns>
        public static int ExecuteNonQuery(string strSql,params SqlParameter[] cmdParams)
        {
            SqlConnection sqlCon = DBConnect.Connect();
            SqlCommand sqlCmd = new SqlCommand(strSql,sqlCon);
            foreach (SqlParameter parm in cmdParams)
            {
                sqlCmd.Parameters.Add(parm);
            }
            try
            {
                sqlCon.Open();
                int num = sqlCmd.ExecuteNonQuery();
                return num;
            }
            catch
            {
                return 0;
            }
            finally
            {
                sqlCmd.Dispose();
                sqlCon.Close();
                sqlCon.Dispose();
            }
        }


        /// <summary>
        /// Execute Select
        /// </summary>
        /// <param name="strSql"></param>
        /// <param name="cmdParams"></param>
        /// <returns></returns>
        public static DataTable ExecuteQuery(string strSql, params SqlParameter[] cmdParams)
        {
            SqlConnection sqlCon = DBConnect.Connect();
            SqlCommand sqlCmd = new SqlCommand(strSql,sqlCon);
            foreach (SqlParameter parm in cmdParams)
            {
                sqlCmd.Parameters.Add(parm);
            }
            DataTable dt = new DataTable();
            SqlDataAdapter dap = new SqlDataAdapter(sqlCmd);
            try
            {
                dap.Fill(dt);
                return dt;
            }
            catch
            {
                return null;
            }
            finally
            {
                sqlCmd.Dispose();
                sqlCon.Close();
                sqlCon.Dispose();
            }
        }

        public static DataTable ExecuteQuery(string strSql, CommandType cmdType, params SqlParameter[] cmdParams)
        {
            SqlConnection sqlCon = DBConnect.Connect();
            sqlCon.Open();
            SqlCommand sqlCmd = new SqlCommand(strSql, sqlCon);
            foreach (SqlParameter parm in cmdParams)
            {
                sqlCmd.Parameters.Add(parm);
            }
            sqlCmd.CommandType = cmdType;
            DataTable dt = new DataTable();
            SqlDataAdapter dap = new SqlDataAdapter(sqlCmd);
            try
            {
                dap.Fill(dt);
                return dt;
            }
            catch
            {
                return null;
            }
            finally
            {
                sqlCmd.Dispose();
                sqlCon.Close();
                sqlCon.Dispose();
            }
            
        }

        public static object ExecuteScalar(string strSql,params SqlParameter[] cmdParams)
        {
            SqlConnection sqlCon = DBConnect.Connect();
            SqlCommand sqlCmd = new SqlCommand(strSql,sqlCon);
            foreach (SqlParameter parm in cmdParams)
            {
                sqlCmd.Parameters.Add(parm);
            }
            try
            {
                sqlCon.Open();
                object value = sqlCmd.ExecuteScalar();
                return value;
            }
            catch
            {
                return null;
            }
            finally
            {
                sqlCmd.Dispose();
                sqlCon.Close();
                sqlCon.Dispose();
            }
        }

        public static SqlDataReader ExecuteReader(string strSql, params SqlParameter[] cmdParams)
        {
            SqlConnection sqlCon = DBConnect.Connect();
            SqlCommand sqlCmd = new SqlCommand(strSql, sqlCon);
            foreach (SqlParameter parm in cmdParams)
            {
                sqlCmd.Parameters.Add(parm);
            }
            try
            {
                sqlCon.Open();
                SqlDataReader dr = sqlCmd.ExecuteReader(CommandBehavior.CloseConnection);
                return dr;
            }
            catch
            {

                return null;
            }
        }

        public static SqlDataReader ExecuteReader(string strSql, CommandType cmdType, params SqlParameter[] cmdParams)
        {
            SqlConnection sqlCon = DBConnect.Connect();
            SqlCommand sqlCmd = new SqlCommand(strSql, sqlCon);
            foreach (SqlParameter parm in cmdParams)
            {
                sqlCmd.Parameters.Add(parm);
            }
            sqlCmd.CommandType = cmdType;
            try
            {
                sqlCon.Open();
                SqlDataReader dr = sqlCmd.ExecuteReader(CommandBehavior.CloseConnection);
                return dr;
            }
            catch
            {

                return null;
            }
        }

        public static void ExecuteSqlBulkCopy(DataTable dt,string tableName)
        {
            SqlConnection sqlCon = DBConnect.Connect();
            SqlBulkCopy bulkCopy = new SqlBulkCopy(sqlCon,SqlBulkCopyOptions.TableLock,null);
            bulkCopy.DestinationTableName = tableName;
            bulkCopy.BatchSize = dt.Rows.Count;
            sqlCon.Open();
            bulkCopy.WriteToServer(dt);
            bulkCopy.Close();
        }
    }
}

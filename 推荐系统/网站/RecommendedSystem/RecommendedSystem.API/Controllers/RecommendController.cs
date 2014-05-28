using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using RecommendedSystem.Common;
using RecommendedSystem.Core;

namespace RecommendedSystem.API.Controllers
{
    public class RecommendController : ApiController
    {

        // GET api/Recommend/id
        public List<int> Get(int id)
        {
            List<int> recommendList = new List<int>();
            recommendList = RecommendedSystemHelper.GetRecommend(id).Take(6).ToList();
            // 用户已经登录
            //if (System.Web.HttpContext.Current.User.Identity.IsAuthenticated)
            //{
            //    int userId = int.Parse(System.Web.HttpContext.Current.User.Identity.Name);
            //    //从推荐列表中查询
            //    if (RecommendedSystemHelper.GetRecommend(userId).Count > 0)
            //    {
            //        recommendList = RecommendedSystemHelper.GetRecommend(userId).Take(6).ToList();
            //    }
            //    else
            //    {
            //        recommendList = RecommendedSystemHelper.GetTop().Take(6).ToList();
            //    }

            //    //推荐访问率最高的
            //}
            //else // 用户未登录，推荐访问率最高的
            //{
            //    recommendList = RecommendedSystemHelper.GetTop().Take(6).ToList();
            //}
            return recommendList;
        }

        // POST api/Recommend
        public void Post([FromBody]string value)
        {
        }

        // PUT api/Recommend/5
        public void Put(int id, [FromBody]string value)
        {
        }

        // DELETE api/Recommend/5
        public void Delete(int id)
        {
        }

    }
}

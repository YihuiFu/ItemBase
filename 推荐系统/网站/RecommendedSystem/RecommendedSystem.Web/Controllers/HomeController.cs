using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Security;
using RecommendedSystem.Common;
using RecommendedSystem.Core;

namespace RecommendedSystem.Web.Controllers
{
    public class HomeController : Controller
    {
        public ActionResult Index()
        {
            if (System.Web.HttpContext.Current.User.Identity.IsAuthenticated)
            {
                var userid = System.Web.HttpContext.Current.User.Identity.Name;
                ViewBag.Message = "login success!  "+userid;
            }
            else
            {
                ViewBag.Message = "not success";
            }
           

            return View();
        }


        public ActionResult Recommend()
        {
            List<int> recommendList = new List<int>();
            // 用户已经登录
            if (System.Web.HttpContext.Current.User.Identity.IsAuthenticated)
            {
                int userId = int.Parse(System.Web.HttpContext.Current.User.Identity.Name);
                //从推荐列表中查询
                if (RecommendedSystemHelper.GetRecommend(userId).Count>0)
                {
                    recommendList = RecommendedSystemHelper.GetRecommend(userId).Take(6).ToList();
                }
                else
                {
                    recommendList = RecommendedSystemHelper.GetTop().Take(6).ToList();
                }

                //推荐访问率最高的
            }
            else // 用户未登录，推荐访问率最高的
            {
                recommendList = RecommendedSystemHelper.GetTop().Take(6).ToList();
            }
            return View(recommendList);
           // return Json(recommendList,JsonRequestBehavior.AllowGet);
        }

        public ActionResult About()
        {
            ViewBag.Message = "Your app description page.";

            return View();
        }

        public ActionResult Contact()
        {
            ViewBag.Message = "Your contact page.";

            return View();
        }
    }
}

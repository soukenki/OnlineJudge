// 面向用户

#include <iostream>
#include "../comm/httplib.h"
#include "oj_control.hpp"

using namespace httplib;
using namespace ns_control;

int main()
{
    // 用户请求的服务路由
    Server svr;
    Control ctrl;

    // 获取所有的题目列表  
    svr.Get("/all_questions", [&ctrl](const Request &req, Response &resp){
        // 返回一张包含所有题目的网页
        std::string html;
        ctrl.AllQuestions(&html);  // 获取所有题目

        // resp.set_content("すべての問題リスト", "text/plain; charset=utf-8");
        resp.set_content(html, "text/html; charset=utf-8");

    });   // lambda表达式要使用外部对象时，要在[]中&外部对象


    // 用户要根据题目编号，获取题目内容
    // 正则表达式 /question/100 \d数字 +一个或多个
    // R"()"  原始字符串raw
    svr.Get(R"(/question/(\d+))", [&ctrl](const Request &req, Response &resp){
        std::string number = req.matches[1];
        std::string html;
        ctrl.Question(number, &html);

        // resp.set_content("指定された問題：" + number, "text/plain; charset=utf-8");
        resp.set_content(html, "text/html; charset=utf-8");
    });


    // 用户提交代码。使用判题功能（1,每道题的测试用例 2,compile_and_run）
    svr.Post(R"(/judge/(\d+))", [&ctrl](const Request &req, Response &resp){
        std::string number = req.matches[1];
        // resp.set_content("指定された問題 " + number + " のテスト", "text/plain; charset=utf-8");

        std::string result_json;
        ctrl.Judge(number, req.body, &result_json);   
        resp.set_content(result_json, "application/json;charset=utf-8");
    
    });


    svr.set_base_dir("./wwwroot");
    
    svr.listen("0.0.0.0", 8080);

    return 0;
}
// 面向用户

#include <iostream>
#include "../comm/httplib.h"

using namespace httplib;

int main()
{
    // 用户请求的服务路由
    Server svr;

    // 获取所有的题目列表
    svr.Get("/all_questions", [](const Request &req, Response &resp){
        resp.set_content("すべての問題リスト", "text/plain; charset=utf-8");
    });

    // 用户要根据题目编号，获取题目内容
    // 正则表达式 /question/100 \d数字 +一个或多个
    // R"()"  原始字符串raw
    svr.Get(R"(/question/(\d+))", [](const Request &req, Response &resp){
        std::string number = req.matches[1];
        resp.set_content("指定された問題：" + number, "text/plain; charset=utf-8");
    });

    // 用户提交代码。使用判题功能（1,每道题的测试用例 2,compile_and_run）
    svr.Get(R"(/judge/(\d+))", [](const Request &req, Response &resp){
        std::string number = req.matches[1];
        resp.set_content("指定された問題 " + number + " のテスト", "text/plain; charset=utf-8");
    });

    svr.set_base_dir("./wwwroot");
    
    svr.listen("0.0.0.0", 8080);

    return 0;
}
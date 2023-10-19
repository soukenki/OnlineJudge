// 调用compile_run + 网络连接

#include "compile_run.hpp"
#include "../comm/httplib.h"

using namespace ns_compile_and_run;
using namespace httplib;


void Usage(std::string proc)
{
    std::cerr << "Usage: " << "\n\t" << proc << " proc" << std::endl;
}


// 编译可能同时被多人请求，形成的源文件需要具有唯一性
//  ./compile_server port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]); // 指令参数量不为2
        return 1;
    }

    Server svr;

    svr.Get("/hello", [](const Request &req, Response &resp){
        // 基本测试
        resp.set_content("你好呀 hello http!!!!!こんにちは！私は波奈子です", "text/plain;charset=utf-8");
    });

    // svr.set_base_dir("./wwwroot");
    svr.Post("/compile_and_run", [](const Request &req, Response &resp){
        // 用户请求的服务正文是我们要的json string
        std::string in_json = req.body;
        std::string out_json;
        if (!in_json.empty())
        {
            CompileAndRun::Start(in_json, &out_json);
            resp.set_content(out_json, "application/json;charset=utf-8");
        }
    });

    svr.listen("0.0.0.0", atoi(argv[1])); // 启动http服务




    // 通过http 让client给我们上传一个json string
    // in_json: {"code":"#include...", "input":"..输入", "cpu_limit":1, "mem_limit":1024}
    // out_json: {"status":"0", "reason":"", "stdout":"", "stderr":""}

    // test客户端的json
    std::string in_json;

    Json::Value in_value;
    in_value["code"] = R"(
    #include <iostream>
    int main(){
        std::cout << "code666666666666666" << std::endl;
        int a = 5;
        jhjhbjhb
        return 0;
    })";
    in_value["input"] = "";
    in_value["cpu_limit"] = 1;
    in_value["mem_limit"] = 10240 * 3;  // 30MB
    
    Json::FastWriter writer;
    in_json = writer.write(in_value);  // 序列化
    std::cout << in_json << std::endl;


    std::string out_json;  // 这是之后输出返回给客户端的json
    CompileAndRun::Start(in_json, &out_json);

    std::cout << out_json << std::endl;

    return 0;
}
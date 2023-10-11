// 调用compile_run + 网络连接

#include "compile_run.hpp"

using namespace ns_compile_and_run;

// 编译可能同时被多人请求，形成的源文件需要具有唯一性
int main()
{
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
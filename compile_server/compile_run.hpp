// 调用 编译+运行 / 定制通信协议 / 形成唯一文名

#pragma once

#include <jsoncpp/json/json.h>

#include "compiler.hpp"
#include "runner.hpp"
#include "../comm/log.hpp"
#include "../comm/util.hpp"

namespace ns_compile_and_run
{
    using namespace ns_log;
    using namespace ns_util;
    using namespace ns_compiler;
    using namespace ns_runner;

    class CompileAndRun
    {
    public:
        // >0 <0 ==0 对于三种情况的处理
        static std::string CodeToDesc(int code)
        {

        }

        /*************************
         * 输入：
         * code：用户提交的代码
         * input：用户给自己提交的代码对应的输入（不处理）
         * cpu_limit：时间要求
         * mem_limit：空间要求
         *
         * 输出：
         * status：状态码 （必填）
         * reason：请求结果（必填）
         * stdout：我的程序运行完的结果（选填）
         * stderr：我的程序运行完的错误结果（选填）
         *
         * 参数：
         * in_json: {"code":"#include...", "input":"..", "cpu_limit":1, "mem_limit":1024}
         * out_json: {"status":"0", "reason":"", "stdout":"", "stderr":""}
         **************************/
        static void Start(const std::string &in_json, std::string *out_json)
        {
            // json反序列化
            Json::Value in_value;
            Json::Reader reader;
            reader.parse(in_json, in_value); // in_json反序列解析到in_value

            std::string code = in_value["code"].asString(); // 提取代码
            std::string input = in_value["input"].asString();
            int cpu_limit = in_value["cpu_limit"].asInt(); // 以int形式取出
            int mem_limit = in_value["mem_limit"].asInt();

            int status_code = 0;
            Json::Value out_value; // 输出用的json
            int run_result = 0;
            std::string file_name; // 需要内部形成的唯一文件名

            if (code.size() == 0)
            {
                // 没有代码
                status_code = -1; // 代码为空
                goto END;
            }

            // 形成唯一文件名，但没有路径和后缀
            // 毫秒级时间戳+原子性递增唯一值：保证唯一性
            file_name = FileUtil::UniqFileName(); // 形成唯一文件名

            if (!FileUtil::WriteFile(PathUtil::Src(file_name), code)) // 代码写到文件（形成临时src文件）
            {
                status_code = -2; // 未知错误
                goto END;
            }

            if (!Compiler::Compile(file_name)) // 编译
            {
                status_code = -3; // 代码编译时发生错误
                goto END;
            }

            run_result = Runner::Run(file_name, cpu_limit, mem_limit); // 运行
            if (run_result < 0)
            {
                status_code = -2; // 未知错误
            }
            else if (run_result > 0)
            {
                // 程序运行崩溃
                status_code = run_result;
            }
            else
            {
                // 运行成功
                status_code = 0;
            }

        END:
            out_value["status"] = status_code;
            out_value["reason"] = CodeToDesc(status_code);
            if (status_code == 0)
            {
                // 整个过程全部成功
                out_value["stdout"] = FileUtil::ReadFile(PathUtil::Stdout(file_name));  // 读文件内容
                out_value["stderr"] = FileUtil::ReadFile(PathUtil::Stderr(file_name));
            }

            // 序列化
            Json::StyledWriter writer;
            *out_json = writer.write(out_value);  // 通过 输出型参数 回传
        }
    };
}

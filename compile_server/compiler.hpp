// 只负责代码的编译服务

#pragma once

#include <iostream>
#include <unistd.h>

#include "../comm/util.hpp"


namespace ns_compiler
{
    // 引入路径拼接功能
    using namespace ns_util;

    class Compiler
    {
    public:
        Compiler()
        {}

        ~Compiler()
        {}

        // 返回值： 编译成功true 否则false
        // 输入参数： 编译的文件名
        // file_name: 111
        // 111 -> ./temp/111.cpp
        // 111 -> ./temp/111.exe
        // 111 -> ./temp/111.stderr
        static bool Compile(const std::string &file_name)
        {
            pid_t res = fork();
            if (res < 0)
            {
                return false;
            }
            else if (res == 0)
            {
                // 子进程：调用编译器，完成对代码的编译工作
                // g++ -o target src -std=c++11
                execlp("g++", "-o", PathUtil::Exe(file_name).c_str(), \
                        PathUtil::Src(file_name).c_str(), "-std=c++11", nullptr); // 进程替换, 结尾nullptr不要忘

            }
            else
            {
                //父进程
            }
        }
    
    private:

    };
}



// 只负责代码的编译服务

#pragma once

#include <iostream>

#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

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
            pid_t pid = fork();
            if (pid < 0)
            {
                return false;
            }
            else if (pid == 0)
            {
                // 打开文件，把错误信息重定向到文件
                int _stderr = open(PathUtil::Stderr(file_name).c_str(), O_CREAT | O_WRONLY, 0644);  
                if (_stderr < 0)
                {
                    exit(1);
                }
                // 重定向标准错误到_stderr
                dup2(_stderr, 2);  // 2重定向到文件
                

                // 子进程：调用编译器，完成对代码的编译工作
                // g++ -o target src -std=c++11
                execlp("g++", "-o", PathUtil::Exe(file_name).c_str(), \
                        PathUtil::Src(file_name).c_str(), "-std=c++11", nullptr); // 进程替换, 结尾nullptr不要忘，程序替换不影响文件描述符表

                exit(2);
            }
            else
            {
                // 父进程
                waitpid(pid, nullptr, 0);
                // 编译是否成功，看是否形成可执行文件
                if (FileUtil::IsFileExists(PathUtil::Exe(file_name)))
                {
                    return true; // 编译成功，运行文件存在
                }
            }

            return false;  // 编译失败，运行文件不在
        }
    
    private:

    };
}



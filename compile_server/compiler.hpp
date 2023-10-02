// 只负责代码的编译服务

#pragma once

#include <iostream>

#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "../comm/util.hpp"
#include "../comm/log.hpp"


namespace ns_compiler
{
    // 引入路径拼接/LOG功能
    using namespace ns_util;
    using namespace ns_log;

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
                LOG(ERROR);
                std::cout << "内部错误， 创建子进程失败" << std::endl;
                return false;
            }
            else if (pid == 0)
            {
                // 打开文件，把错误信息重定向到文件
                int _stderr = open(PathUtil::Stderr(file_name).c_str(), O_CREAT | O_WRONLY, 0644);  
                if (_stderr < 0)
                {
                    LOG(WARNING);
                    std::cout << "没有成功形成stderr文件" << std::endl;
                    exit(1);
                }
                // 重定向标准错误到_stderr
                dup2(_stderr, 2);  // 2重定向到文件
                

                // 子进程：调用编译器，完成对代码的编译工作
                // g++ -o target src -std=c++11
                execlp("g++", "g++", "-o", PathUtil::Exe(file_name).c_str(), \
                        PathUtil::Src(file_name).c_str(), "-std=c++11", nullptr); // 进程替换, 结尾nullptr不要忘，程序替换不影响文件描述符表
                
                LOG(ERROR);
                std::cout << "启动编译器g++失败，可能参数错误" << std::endl;
                exit(2);
            }
            else
            {
                // 父进程
                waitpid(pid, nullptr, 0);
                // 编译是否成功，看是否形成可执行文件
                if (FileUtil::IsFileExists(PathUtil::Exe(file_name)))
                {
                    LOG(INFO);
                    std::cout << PathUtil::Src(file_name) << " 编译成功" << std::endl;
                    return true; // 编译成功，运行文件存在
                }
            }

            LOG(ERROR);
            std::cout << "编译失败，没有形成可执行程序" << std::endl;
            return false;  // 编译失败，运行文件不在
        }
    
    private:

    };
}



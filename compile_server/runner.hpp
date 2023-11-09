// 运行服务

#pragma once

#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "../comm/util.hpp"
#include "../comm/log.hpp"
#include "../comm/limit.hpp"


namespace ns_runner
{
    using namespace ns_util;
    using namespace ns_log;

    class Runner
    {
    public:
        Runner()
        {
        }
        ~Runner()
        {
        }

    public:
        /*********************************
         * 指名文件名即可，不需要代理路径和后缀
         * 返回值 > 0 : 异常，退出时收到的信号
         * 返回值 == 0 : 正常运行完，结果保存到文件
         * 返回值 < 0 : 内部err
         * 
         * cpu_limit : 该程序运行时，最大可用的CPU资源上限
         * mem_limit ： 该程序运行时，最大可用的内存大小（KB）
         *********************************/ 
        static int Run(const std::string &file_name, int cpu_limit, int mem_limit)
        {
            /*********************************
             * run运行模块：
             * 1.代码跑完，结果正确
             * 2.代码跑完，结果不正确
             * 3.代码没跑完，异常
             * 结果正确与否，是由测试用例决定的，与本run模块无关，只考虑是否run完。
             *
             * 标准输入：不处理
             * 标准输出：程序运行完成，输出结果
             * 标准错误：运行时错误信息
             *********************************/
            std::string _execute = PathUtil::Exe(file_name);   // 可执行程序名
            std::string _stdin = PathUtil::Stdin(file_name);   // 输入/输出/错误 文件
            std::string _stdout = PathUtil::Stdout(file_name);
            std::string _stderr = PathUtil::Stderr(file_name);

            umask(0);
            int _stdin_fd = open(_stdin.c_str(), O_CREAT | O_RDONLY, 0644);
            int _stdout_fd = open(_stdout.c_str(), O_CREAT | O_WRONLY, 0644);
            int _stderr_fd = open(_stderr.c_str(), O_CREAT | O_WRONLY, 0644);
            if (_stdin_fd < 0 || _stdout_fd < 0 || _stderr_fd < 0)
            {
                LOG(ERROR);
                std::cout << "运行时打开标准文件失败" << std::endl;
                return -1;  // -1 打开文件失败
            }

            pid_t pid = fork();
            if (pid < 0)
            {
                LOG(ERROR);
                std::cout << "创建子进程失败" << std::endl;
                close(_stdin_fd);
                close(_stdout_fd);
                close(_stderr_fd);
                return -2;    // -2子进程创建失败
            }
            else if (pid == 0)
            {
                // 子进程重定向
                dup2(_stdin_fd, 0);
                dup2(_stdout_fd, 1);
                dup2(_stderr_fd, 2);

                // 限制内存/CPU
                ns_limit::SetProcLimit(cpu_limit, mem_limit);
                
                std::cout << "限制内存/CPU成功" << std::endl;

                // 程序替换
                execl(_execute.c_str(), _execute.c_str(), nullptr);  // 执行谁，命令行上如何执行
                
                std::cout << "程序替换成功" << std::endl;
                
                exit(1);
            }
            else
            {
                // 父进程
                close(_stdin_fd);
                close(_stdout_fd);
                close(_stderr_fd);
                
                int status = 0;
                waitpid(pid, &status, 0);   // 阻塞等待，等待结果保存到status
                // 程序异常，一定收到了信号
                LOG(ERROR);
                std::cout << "运行完毕, info: " << (status & 0x7F) <<std::endl;
                return status & 0x7F;
            }
        }
    };
}
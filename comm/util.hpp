// 工具
#pragma once

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>



namespace ns_util
{
    const std::string temp_path = "./temp/";  // 路径

    class PathUtil
    {
    public:
        // 加后缀
        // 123 -> ./temp/123.cpp
        static std::string AddSuffix(const std::string &file_name, const std::string &suffix)
        {
            std::string path_name = temp_path;
            path_name += file_name;
            path_name += suffix;
            return path_name;
        }

        // 编译时，需要的临时文件
        // 构建源文件路径，并加后缀的完整文件名
        static std::string Src(const std::string &file_name)
        {
            return AddSuffix(file_name, ".cpp");
        }

        // 构建可执行程序的完整路径，并加后缀名
        static std::string Exe(const std::string &file_name)
        {
            return AddSuffix(file_name, ".exe");
        }

        // 构建编译时报错
        static std::string CompilerError(const std::string &file_name)
        {
            return AddSuffix(file_name, ".compile_error");
        }



        // 运行时，需要的临时文件
        // 构建标准输入
        static std::string Stdin(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stdin");
        }

        // 构建标准输出
        static std::string Stdout(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stdout");
        }
        
        // 构建该程序标准错误的完整路径，并加后缀名
        static std::string Stderr(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stderr");
        }


    };


    class FileUtil
    {
    public:
        // 判断文件是否存在
        static bool IsFileExists(const std::string &path_name)
        {
            struct stat st;
            if (stat(path_name.c_str(), &st) == 0)
            {
                // 获取属性成功，文件存在
                return true;
            }
            return false;
        }

        // 生成代码文件唯一名
        static std::string UniqFileName()
        {
            return "";
        }

        // 代码写到文件（形成临时src文件）
        static bool WriteFile(const std::string &target, const std::string &code)
        {
            return true;
        }

        // 读取文件内容
        static std::string ReadFile(const std::string &target)
        {
            return "";
        }
    };

    class TimeUtil
    {
    public:
        // 获取时间戳
        static std::string GetTimeStamp()
        {
            struct timeval _time;
            gettimeofday(&_time, nullptr);    
            return std::to_string(_time.tv_sec);
        }
    };
}

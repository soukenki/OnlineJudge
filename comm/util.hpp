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

        // 构建该程序标准错误的完整路径，并加后缀名
        static std::string Stderr(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stderr");
        }
    };


    class FileUtil
    {
    public:
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

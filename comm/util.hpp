// 工具
#pragma once

#include <iostream>
#include <string>

namespace ns_util
{
    const std::string temp_path = "./temp/";  // 路径

    class PathUtil
    {
    public:
        // 构建源文件路径，并加后缀的完整文件名
        static std::string Src(const std::string &file_name)
        {
            
        }

        // 构建可执行程序的完整路径，并加后缀名
        static std::string Exe(const std::string &file_name)
        {

        }

        // 构建该程序标准错误的完整路径，并加后缀名
        static std::string Stderr(const std::string &file_name)
        {

        }
    };
}

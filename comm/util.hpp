// 工具
#pragma once

#include <iostream>
#include <string>
#include <atomic>
#include <fstream>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <boost/algorithm/string.hpp>


namespace ns_util
{
    class TimeUtil
    {
    public:
        // 获取时间戳 秒
        static std::string GetTimeStamp()
        {
            struct timeval _time;
            gettimeofday(&_time, nullptr);    
            return std::to_string(_time.tv_sec);
        }

        // 获得 毫秒 时间戳
        static std::string GetTimeMs()
        {
            struct timeval _time;
            gettimeofday(&_time, nullptr);
            return std::to_string(_time.tv_sec * 1000 + _time.tv_usec / 1000); // 毫秒级时间戳
        }
    };

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
            static std::atomic_uint id(0);  
            id++; // id每次操作都是原子的
            // 毫秒级时间戳+原子性递增唯一值：保证唯一性
            std::string ms = TimeUtil::GetTimeMs();
            std::string uniq_id = std::to_string(id);
            return ms + "_" + uniq_id;
        }

        // 代码写到文件（形成临时src文件）
        static bool WriteFile(const std::string &target, const std::string &content)
        {
            std::ofstream out(target);
            if (!out.is_open())
            {
                // 文件流打开失败，写入失败
                return false;
            }

            out.write(content.c_str(), content.size());  // 写入
            
            out.close();  // 关闭文件流
            
            return true;
        }

        // 读取文件内容
        static bool ReadFile(const std::string &target, std::string *content, bool keep = false)
        {
            (*content).clear();   // 清空

            std::ifstream in(target);  // 打开流
            if (!in.is_open())
            {
                return false;
            }

            std::string line;
            // getline函数不保存行分隔符，如果需要保留\n，设置keep值为true，默认是false
            // getline内部重载了强制类型转换，所以while能直接判断它的返回值
            // 从in的流里面读，读到line中
            while (std::getline(in, line))
            {
                (*content) += line;
                (*content) += (keep ? "\n" : "");
            }

            in.close();  // 关闭文件流
            return true;
        }
    };

    class StringUtil
    {
    public:
        /* 切分字符串 
         * str：输入型，目标要切分的字符串
         * target：输出型，保存切分完毕的结果
         * sep：指定的分割符
        ************************************/ 
        static void SplitString(const std::string &str, std::vector<std::string> *target, std::string sep)
        {
            // boost 切分
            boost::split((*target), str, boost::is_any_of(sep), boost::algorithm::token_compress_on); // 放到哪，切分谁，分割符是谁，连续出现分隔符时是否保留
        }
    };
}

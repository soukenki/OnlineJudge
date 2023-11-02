// 数据交互模块，对题库增删改查

#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cassert>
#include <cstdlib>

#include "../comm/log.hpp"
#include "../comm/util.hpp"

// 根据题目list文件，加载所有题目信息到内存
// model：主要用于和数据进行交互，对外提供访问题库数据的接口

namespace ns_model
{
    using namespace std;
    using namespace ns_log;
    using namespace ns_util;

    struct Question
    {
        std::string number;  // 题目编号
        std::string title;   // 题目标题
        std::string star;    // 难度 （简单 中等 困难）
        int cpu_limit;       // 时间要求（S秒）
        int men_limit;       // 空间要求（KB）
        std::string desc;    // 题目描述
        std::string header;  // 题目预设的代码
        std::string tail;    // 测试用例
    };

    const std::string question_list = "./questions/questions.list";
    const std::string question_path = "./questions/";

    class Model
    {
    public:
        Model()
        {
            assert(LoadQuestionList(question_list));
        }

        ~Model()
        {}

        // 加载配置文件  questions/questions.list + 题目编号文件
        bool LoadQuestionList(const std::string &question_list)
        {
            ifstream in(question_list);  
            if (!in.is_open())
            {
                LOG(FATAL); 
                std::cout << "加载题库失败，请检查是否存在题库文件" << std::endl;
                return false;  // 打开文件流失败
            }

            std::string line;
            while (getline(in, line))  // 从in流，读到line
            {
                std::vector<string> tokens;
                // 切分字符串
                StringUtil::SplitString(line, &tokens, " ");
                // "1 题目 简单 1 30000"
                // 切分为5个部分
                if (tokens.size() != 5)
                {
                    LOG(WARNING);
                    std::cout << "加载部分题目失败，请检查文件格式" << std::endl;
                    continue;  // 太长或太短，都视为损坏，跳过
                }
                
                // 切分后对号入座
                Question q;
                q.number = tokens[0];
                q.title = tokens[1];
                q.star = tokens[2];
                q.cpu_limit = atoi(tokens[3].c_str());
                q.men_limit = atoi(tokens[4].c_str());

                // 各个题目的唯一路径
                std::string path = question_path;
                path += q.number;
                path += "/";

                // 读取文件内容
                FileUtil::ReadFile(path + "desc.txt", &(q.desc), true);  // 路径，输出到哪，是否保留'\n'
                FileUtil::ReadFile(path + "header.cpp", &(q.header), true);
                FileUtil::ReadFile(path + "tail.cpp", &(q.tail), true);

                // 读到内容后，插入到 key:value ，方便之后查找 
                questions.insert({q.number, q});
            }

            LOG(INFO);
            std::cout << "加载题库...成功！" << std::endl;
            in.close();  // 关闭文件流
            
            return true;
        }

        // 获取全部题目
        bool GetAllQuestions(std::vector<Question> *out)
        {
            if (questions.size() == 0)
            {
                LOG(ERROR);
                std::cout << "用户获取题库失败" << std::endl;
                return false;
            }

            for (const auto &q : questions)
            {
                out->push_back(q.second);  // first:key, second:value
            }
            return true;
        }

        // 获取一个题目
        bool GetOneQuestion(const std::string &number, Question *q)
        {
            const auto& iter = questions.find(number);
            if (iter == questions.end())
            {
                LOG(ERROR);
                std::cout << "用户获取题目失败，题目编号：" << number << std::endl;
                return false;  // 没找到
            }

            (*q) = iter->second;
            return true;
        }

    private:
        // 题目：题目细节
        std::unordered_map<string, Question> questions;
    };
}

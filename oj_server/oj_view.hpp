// 拿到数据之后进行构建网页，渲染网页内容展示给用户

#pragma once 

#include <iostream>
#include <string>
#include <ctemplate/template.h>

#include "oj_model.hpp"


// struct Question
// {
//     std::string number;  // 题目编号
//     std::string title;   // 题目标题
//     std::string star;    // 难度 （简单 中等 困难）
//     int cpu_limit;       // 时间要求（S秒）
//     int men_limit;       // 空间要求（KB）
//     std::string desc;    // 题目描述
//     std::string header;  // 题目预设的代码
//     std::string tail;    // 测试用例
// };

const std::string template_path = "./template_html/";

namespace ns_view
{
    using namespace ns_model;

    class View
    {
    public:
        View()
        {}
        ~View()
        {}

        // 渲染所有题目
        void AllExpandHtml(const std::vector<ns_model::Question> questions, std::string *html)
        {
            // 题目的编号 题目的标题 题目的难度
            // 用表格显示

            // 1.形成路径
            std::string src_html = template_path + "all_questions.html";
            
            // 2.形成数据字典
            ctemplate::TemplateDictionary root("all_questions");  // 父字典
            for (const auto &q : questions) 
            {
                ctemplate::TemplateDictionary *sub = root.AddSectionDictionary("question_list"); // 子字典
                sub->SetValue("number", q.number);
                sub->SetValue("title", q.title);
                sub->SetValue("star", q.star);
            }

            // 3.获取被渲染的html
            ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);

            // 4.执行渲染功能
            tpl->Expand(html, &root);
        }

        // 渲染一个题目
        void OneExpandHtml(const ns_model::Question &q, std::string *html)
        {
            // 1.形成路径
            std::string src_html = template_path + "one_question.html";

            // 2.形成数据字典
            ctemplate::TemplateDictionary root("one_question");
            root.SetValue("number", q.number);
            root.SetValue("title", q.title);
            root.SetValue("star", q.star);
            root.SetValue("desc", q.desc);
            root.SetValue("pre_code", q.header);
            
            // 3.获取被渲染的html
            ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
            
            //开始渲染
            tpl->Expand(html, &root);
        }
    };
}
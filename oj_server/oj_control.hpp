// 控制器，核心业务逻辑

#pragma once 

#include <iostream>
#include <string>
#include <vector>

#include "oj_model.hpp"
#include "oj_view.hpp"
#include "../comm/log.hpp"
#include "../comm/util.hpp"

namespace ns_control
{
    using namespace std;
    using namespace ns_log;
    using namespace ns_util;
    using namespace ns_model;
    using namespace ns_view;

    class Control
    {
    public:
        Control()
        {}

        ~Control()
        {}

        // 根据题目数据构建网页    html：输出型参数
        // 全部题目
        bool AllQuestions(std::string *html)
        {
            bool ret = true;
            std::vector<ns_model::Question> all;
            if (_model.GetAllQuestions(&all))
            {
                // 获取题目信息成功，用数据构建网页
                _view.AllExpandHtml(all, html);
            }
            else
            {
                *html = "問題の取得に失敗した。問題リストの作成に失敗した。";
                ret = false;
            }

            return ret;
        }
        
        // 单个题目
        bool Question(const std::string &number, std::string *html)
        {
            bool ret = true;
            ns_model::Question q;
            if (_model.GetOneQuestion(number, &q))
            {
                // 获取题目成功，把数据构建网页
                _view.OneExpandHtml(q, html);
            }
            else
            {
                *html = "指定された問題：" + number + " がない！";
                ret = false;
            }
            return ret;
        }

    private:
        Model _model;  // 提供后台数据
        View _view;    // 提供html渲染功能
    };
}
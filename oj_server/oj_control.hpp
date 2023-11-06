// 核心业务逻辑控制器 + 负载均衡

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <cassert>
#include <fstream>

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

    // 提供服务的主机
    class Machine
    {
    public:
        std::string _ip;  // 编译服务的ip地址
        int _port;        // 端口号
        uint64_t _load;   // 负载数
        std::mutex *_mtx; // mutex禁止拷贝，使用指针

    public:
        Machine()
            : _ip(""), _port(0), _load(0), _mtx(nullptr)
        {
        }

        ~Machine()
        {
        }

        // 提升主机负载
        void IncLoad()
        {
            if (_mtx)
            {
                _mtx->lock();
            }

            ++_load;

            if (_mtx)
            {
                _mtx->unlock();
            }
        }

        // 减少主机负载
        void DecLoad()
        {
            if (_mtx)
            {
                _mtx->lock();
            }

            --_load;

            if (_mtx)
            {
                _mtx->unlock();
            }
        }

        // 获取主机负载 , 只为了统一接口
        uint64_t Load()
        {
            uint64_t load = 0;
            if (_mtx)
            {
                _mtx->lock();
            }

            load = _load;

            if (_mtx)
            {
                _mtx->unlock();
            }

            return load;
        }
    };

    // 主机列表配置文件地址
    const std::string service_machine = "./conf/service_machine.conf";

    // 负载均衡模块
    class LoadBlance
    {
    public:
        LoadBlance()
        {
            assert(LoadConf(service_machine));
            LOG(INFO);
            std::cout << "加载" << service_machine << " 成功" << std::endl;
        }

        ~LoadBlance()
        {
        }

        // 加载配置文件
        bool LoadConf(const std::string &machine_conf)
        {
            std::ifstream in(machine_conf); // 打开文件流
            if (!in.is_open())
            {
                LOG(FATAL);
                std::cout << "加载：" << machine_conf << "失败" << std::endl;
                return false;
            }

            std::string line;
            while (std::getline(in, line)) // 循环从in读到line
            {
                std::vector<std::string> tokens;
                StringUtil::SplitString(line, &tokens, ":"); // 切分
                if (tokens.size() != 2)
                {
                    LOG(WARNING);
                    std::cout << "切分" << line << "失败" << std::endl;
                    continue;
                }

                // 切分后的信息，导入到主机中
                Machine m;
                m._ip = tokens[0];
                m._port = atoi(tokens[1].c_str());
                m._load = 0;
                m._mtx = new std::mutex();

                // 添加主机到列表
                _online.push_back(_machines.size()); // 主机数量作为在线数量
                _machines.push_back(m);
            }

            in.close(); // 关闭文件流
            return true;
        }

        // 智能选择主机
        bool SmartChoice(int *id, Machine **m) // 2个输出型参数
        {
            // 1.使用选择好的主机（并更新负载）
            // 2.可能需要离线该主机
            _mtx.lock();
            // 负载均衡算法
            // 1.随机+hash
            // 2.轮询+hash
            int online_num = _online.size();
            if (online_num == 0) // 没有在线的主机
            {
                _mtx.unlock();
                LOG(FATAL);
                std::cout << "すべてのサーバーがオフラインです。できるだけ早く修正してください" << std::endl;
                return false;
            }

            // 通过遍历，找到负载最小的主机
            uint64_t min_load = _machines[_online[0]].Load();
            *id = _online[0];
            *m = &_machines[_online[0]];

            for (int i = 0; i < online_num; i++) // 遍历
            {
                uint64_t curr_load = _machines[_online[i]].Load();
                if (min_load > curr_load)
                {
                    min_load = curr_load;
                    *id = _online[i];
                    *m = &_machines[_online[i]];
                }
            }

            _mtx.unlock();
            return true;
        }

        // 离线主机
        void OfflineMachine()
        {
        }

        // 上线主机
        void OnlineMachine()
        {
        }

    private:
        // 每台主机在vector中都有下标，把下标用作id
        std::vector<Machine> _machines; // 主机列表，可以提供服务的所有主机
        std::vector<int> _online;       // 在线主机的id
        std::vector<int> _offline;      // 离线主机的id
        std::mutex _mtx;                // 保证LoadBlance的数据安全
    };



    // 控制器模块
    class Control
    {
    public:
        Control()
        {
        }

        ~Control()
        {
        }

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

        // 判题
        // id: 100
        // code: #include...
        // input: ""
        void Judge(const std::string in_json, std::string *out_json)
        {
            // 1. in_json进行反序列化，得到题目id和用户提交的源代码，input
            // 2. 重新拼接用户代码 + 测试用例代码，形成新的代码
            // 3. 选择负载最低的主机（差错处理）
            // 4. 然后发起http请求，得到结果
            // 5. 将结果赋值给out_json
        }

    private:
        Model _model;              // 提供后台数据
        View _view;                // 提供html渲染功能
        LoadBlance _load_blance;   // 负载均衡器
    };
}
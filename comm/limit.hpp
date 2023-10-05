// 限制

#pragma once

#include <iostream>

#include <sys/time.h>
#include <sys/resource.h>

namespace ns_limit
{
    /**************
     * 提供设置进程占用资源大小的接口
     * _cpu_limit : 该程序运行时，最大可用的CPU资源上限
     * _mem_limit ： 该程序运行时，最大可用的内存大小（KB）
     **************/ 
    void SetProcLimit(int _cpu_limit, int _mem_limit)
    {
        // 设置CPU时长
        struct rlimit cpu_rlimit;
        cpu_rlimit.rlim_max = RLIM_INFINITY;  // 无穷大
        cpu_rlimit.rlim_cur = _cpu_limit;
        setrlimit(RLIMIT_CPU, &cpu_rlimit);

        // 设置内存大小
        struct rlimit mem_rlimit;
        mem_rlimit.rlim_max = RLIM_INFINITY;
        mem_rlimit.rlim_cur = _mem_limit * 1024;  // 转化为KB
        setrlimit(RLIMIT_AS, &mem_rlimit);
        
    }
}
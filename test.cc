// test

#include <iostream>

#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <jsoncpp/json/json.h>

void handler(int signo)
{
    std::cout << "signo : " << signo << std::endl;
}

int main()
{
    // 信号
    // for (int i = 1; i <= 31; i++)
    // {
    //     signal(i, handler);
    // }

    // 限制累计运行时长
    // struct rlimit r;
    // r.rlim_cur = 3;  // 3秒
    // r.rlim_max = RLIM_INFINITY;
    
    // setrlimit(RLIMIT_CPU, &r);  // CPU，3秒
    
    // while (true);

    // 限制空间
    // struct rlimit r;
    // r.rlim_cur = 1024 * 1024 * 20;  // 20M
    // r.rlim_max = RLIM_INFINITY;
    // setrlimit(RLIMIT_AS, &r);   // 内存

    // int count = 0;
    // while (true)
    // {
    //     int *p = new int[1024 * 1024];
    //     count++;
    //     std::cout << "size: " << count << std::endl;
    //     sleep(1);
    // }

    // test json序列化
    Json::Value root;
    root["code"] = "mycode";
    root["user"] = "skk";
    root["age"] = 15;
    
    Json::StyledWriter writer;
    std::string str = writer.write(root);  // 形成字符串
    std::cout << str << std::endl;


    return 0;
}
// // test


#include <iostream>
#include <string>
#include <ctemplate/template.h>

int main()
{
    std::string in_html = "./test.html";
    std::string value = "Test for CTEMPLATE";

    // 形成数据字典
    ctemplate::TemplateDictionary root("test"); //unordered_map test;
    root.SetValue("key", value);  // test.insert({});
    
    // 获取被渲染网页对象
    ctemplate::Template *tpl = ctemplate::Template::GetTemplate(in_html, ctemplate::DO_NOT_STRIP); // 参数：要获取的网页内容 保持网页原貌
    
    // 添加字典数据到网页中
    std::string out_html;
    tpl->Expand(&out_html, &root);  

    // 完成渲染
    std::cout << out_html << std::endl;


    return 0;
}




// #include <iostream>
// #include <vector>
// #include <boost/algorithm/string.hpp>

// // #include <unistd.h>
// // #include <sys/time.h>
// // #include <sys/resource.h>
// // #include <signal.h>
// // #include <jsoncpp/json/json.h>

// void handler(int signo)
// {
//     std::cout << "signo : " << signo << std::endl;
// }

// int main()
// {
//     std::vector<std::string> tokens;
//     const std::string str = "1:判断回文::::::简单:1:30000";
//     const std::string sep = ":"; 
//     boost::split(tokens, str, boost::is_any_of(sep), boost::algorithm::token_compress_on); // 放到哪，切分谁，分割符是谁，连续出现分隔符时是否保留
//     // boost::split(tokens, str, boost::is_any_of(sep), boost::algorithm::token_compress_off); // 放到哪，切分谁，分割符是谁，连续出现分隔符时是否保留

//     for (auto &iter : tokens)
//     {
//         std::cout << iter << std::endl;
//     }

//     // 信号
//     // for (int i = 1; i <= 31; i++)
//     // {
//     //     signal(i, handler);
//     // }

//     // 限制累计运行时长
//     // struct rlimit r;
//     // r.rlim_cur = 3;  // 3秒
//     // r.rlim_max = RLIM_INFINITY;
    
//     // setrlimit(RLIMIT_CPU, &r);  // CPU，3秒
    
//     // while (true);

//     // 限制空间
//     // struct rlimit r;
//     // r.rlim_cur = 1024 * 1024 * 20;  // 20M
//     // r.rlim_max = RLIM_INFINITY;
//     // setrlimit(RLIMIT_AS, &r);   // 内存

//     // int count = 0;
//     // while (true)
//     // {
//     //     int *p = new int[1024 * 1024];
//     //     count++;
//     //     std::cout << "size: " << count << std::endl;
//     //     sleep(1);
//     // }

//     // // test json序列化
//     // Json::Value root;
//     // root["code"] = "mycode";
//     // root["user"] = "skk";
//     // root["age"] = 15;
    
//     // Json::StyledWriter writer;
//     // std::string str = writer.write(root);  // 形成字符串
//     // std::cout << str << std::endl;


//     return 0;
// }
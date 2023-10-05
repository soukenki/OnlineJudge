// test
#include <iostream>
#include <unistd.h>

int main()
{
    
    std::cout << "hello skk" << std::endl;
    std::cerr << "hello error" << std::endl;

    // // 测试内存限制
    // int count = 0;
    // while (true)
    // {
    //     int *p = new int[1024 * 1024];
    //     count++;
    //     std::cout << "size: " << count << std::endl;
    //     sleep(1);
    // }

    // 测试CPU限制
    // while (true)
    // {
    //     int i = 0;
    //     std::cout << "123 : " << i << std::endl;
    //     i++;
    //     sleep(1);
    // }

    return 0;
}
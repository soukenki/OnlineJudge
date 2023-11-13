// 测试用例
// 没有定义COMPILER_ONLINE时，加上header.hpp，仅仅为了写代码时不要报警告
// 编译时，-D COMPILER_ONLINE 定义它，则不会加上header.hpp

#ifndef COMPILER_ONLINE
#include "header.cpp"

#endif


void Test1()
{
    // 通过定义临时对象来完成方法的调用
    std::string str1 = "asdfghjkl";
    std::string str2 = "lkjhgfdsa";
    std::string swap = Solution().Swap(str1);
    
    if (!swap.compare(str2))
    {
        std::cout << "テストケース 1 が実行した..... OK!" << std::endl;
    }
    else
    {
        std::cout << "テストケース 1 は失敗した" << std::endl;
    }
}

void Test2()
{
    // 通过定义临时对象来完成方法的调用
    std::string str1 = "!@#456 iop .... asd";
    std::string str2 = "dsa .... poi 654#@!";
    std::string swap = Solution().Swap(str1);
    
    if (!swap.compare(str2))
    {
        std::cout << "テストケース 2 が実行した..... OK!" << std::endl;
    }
    else
    {
        std::cout << "テストケース 2 は失敗した" << std::endl;
    }
}

int main()
{
    Test1();
    Test2();
    return 0;
}
// 测试用例

#ifndef COMPILER_ONLINE

#include "header.cpp"

#endif


void Test1()
{
    // 通过定义临时对象来完成方法的调用
    bool ret = Solution().isPalindrome(121);
    if (ret)
    {
        std::cout << "テストケース 1 が実行した、テスト値は 121 ... OK!" << std::endl;
    }
    else
    {
        std::cout << "テストケース 1 は失敗した、テスト値は 121 " << std::endl;
    }
}

void Test2()
{
    // 通过定义临时对象来完成方法的调用
    bool ret = Solution().isPalindrome(-10);
    if (!ret)
    {
        std::cout << "テストケース 2 が実行した、テスト値は -10 ... OK!" << std::endl;
    }
    else
    {
        std::cout << "テストケース 2 は失敗した、テスト値は -10 " << std::endl;
    }
}

int main()
{
    Test1();
    Test2();
    return 0;
}
// 测试用例

#ifndef COMPILER_ONLINE

#include "header.cpp"

#endif


void Test1()
{
    // 通过定义临时对象来完成方法的调用
    vector<int> v = {1,2,3,4,5,6,-1,15,6};
    bool ret = Solution().Max(v);
    if (ret == 15)
    {
        std::cout << "テストケース 1 が実行した、テスト値は {1,2,3,4,5,6,-1,15,6} ... OK!" << std::endl;
    }
    else
    {
        std::cout << "テストケース 1 は失敗した、テスト値は {1,2,3,4,5,6,-1,15,6} " << std::endl;
    }
}

void Test2()
{
    // 通过定义临时对象来完成方法的调用
    vector v = {-1,-2,-3};
    bool ret = Solution().Max(v);
    if (ret == -1)
    {
        std::cout << "テストケース 2 が実行した、テスト値は {-1,-2,-3} ... OK!" << std::endl;
    }
    else
    {
        std::cout << "テストケース 2 は失敗した、テスト値は {-1,-2,-3} " << std::endl;
    }
}

int main()
{
    Test1();
    Test2();
    return 0;
}
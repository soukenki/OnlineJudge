// 调用compile_run + 网络连接

#include "compiler.hpp"

using namespace ns_compiler;

int main()
{
    std::string code = "code";
    Compiler::Compile(code);
    return 0;
}
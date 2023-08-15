#pragma once

#include <cstddef>
#include <functional>

namespace cxc
{

using ParseExprFn = std::function<int(int, int)>;

enum class ExprType
{
    
};

class Expr
{
public:
    Expr();

    template<ExprType Type>
    auto operator new(std::size_t count, ParseExprFn function) -> void*;
};

class Parser
{
public:
};

}

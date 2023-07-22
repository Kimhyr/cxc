#pragma once

#include <unordered_map>
#include <string_view>

#include <cxc/syntax.h>

namespace cxc
{

class SyntaxTree
{
private:
    struct Node
    {
        Node*      prior;
        Node*      next;
        SyntaxType type;
    };       
};

}

#pragma once
#include <stdexcept>

namespace WorldGenerator
{
    class NotImplemented : public std::logic_error
    {
    public:
        NotImplemented() : std::logic_error("Function not yet implemented") { };
    };
}
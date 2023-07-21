#pragma once

#include <cstdint>
#include <format>
#include <filesystem>
#include <fstream>

namespace stdfs = std::filesystem;

namespace cxc
{
    struct Position
    {
        std::size_t row;
        std::size_t column;
    };

    struct Span
    {
        Position start;
        Position end;
    };

    struct Location
    {
        stdfs::path path;
        Position    position;
    };

    struct File
    {
        stdfs::path  path;
        std::fstream stream;
    };

    struct FileLocation
    {
        File     file;
        Position position;
    };
}

namespace std
{
    template<>
    struct formatter<cxc::Position>
        : formatter<string>
    {
        template<typename Ctx>
        auto format(cxc::Position const& t, Ctx& ctx) const
        {
            return formatter<string>::format(std::format(
                "{},{}", t.row, t.column), ctx);
        }
    };

    template<>
    struct formatter<cxc::Span>
        : formatter<string>
    {
        template<typename Ctx>
        auto format(cxc::Span const& t, Ctx& ctx) const
        {
            return formatter<string>::format(std::format(
                "{}:{}", t.start, t.end), ctx);
        }
    };
 
    template<>
    struct formatter<cxc::Location>
        : formatter<string>
    {
        template<typename Ctx>
        auto format(cxc::Location const& t, Ctx& ctx) const
        {
            return formatter<string>::format(std::format(
                "{}:{}", t.path.string(), t.position), ctx);
        }
    };
}

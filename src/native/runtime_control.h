#pragma once

#include <exception>

class TitleTermination final : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "The title requested termination";
    }
};
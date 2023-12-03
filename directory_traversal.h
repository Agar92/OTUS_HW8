#pragma once

#include <file_comparison.h>

class DirectoryTraversal
{
public:
    virtual bool is_traversed() const = 0;
    virtual File get_next_file() = 0;  
};

#pragma once

#include <directory_traversal.h>
#include <file_comparison.h>

#include <memory>
#include <vector>

class Searcher
{

public:
    Searcher(std::unique_ptr<DirectoryTraversal> directory_traversal) :
             mDirectoryTraversal(move(directory_traversal)) {}    
    void search();
    std::vector<File> mComparisonFiles; 

private:
    std::unique_ptr<DirectoryTraversal> mDirectoryTraversal;
};

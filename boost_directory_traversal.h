#pragma once

#include <fstream_file_reader.h>
#include <directory_traversal.h>
#include <scan_depth.h>
#include <settings.h>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <map>
#include <memory>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

template<typename IteratorType>
bool points_to_directory(const IteratorType& iterator)
{
    return boost::filesystem::is_directory(iterator->path().string());
}

template <typename IteratorType>
class TraversalExcluderBase
{
public:
    TraversalExcluderBase<IteratorType>(const std::vector<std::string>& excludeDirectories,
                                        const std::vector<std::string>& includeFileMask,
                                        size_t minFileSize) :
                                        mExcludeDirectories(excludeDirectories),
                                        mMinFileSize(minFileSize)
    {
        mIncludeFileMask.reserve(includeFileMask.size());  
        for(const auto& globMask: includeFileMask)
        {
            auto replacedMask = globMask;
            for(const auto& pair: mGlobToRegex)
            {
                std::stringstream stream;
                std::ostream_iterator<char> out_iterator(stream);
                boost::regex_replace(out_iterator, replacedMask.begin(), replacedMask.end(), boost::regex(pair.first), pair.second);
                replacedMask = stream.str();
            }
            mIncludeFileMask.push_back(boost::regex("^" + replacedMask + "$"));
        }
    }

    void filter_exclude_dirs(IteratorType& iterator)
    {
        //std::cout<<"filter_exclude_dirs:"<<std::endl;
        //std::cout<<"Check mExcludeDirectories:"<<std::endl;
        //for(auto dir : mExcludeDirectories) std::cout<<dir<<std::endl;
        //std::cout<<std::endl;
        //std::cout<<"File="<<iterator->path().string()+"/"<<std::endl;

        if (std::find(mExcludeDirectories.begin(),
                      mExcludeDirectories.end(),
                      iterator->path().string()) != mExcludeDirectories.end() ||
            std::find(mExcludeDirectories.begin(),
                      mExcludeDirectories.end(),
                      std::string( iterator->path().string() + "/" ) ) != mExcludeDirectories.end())
        {
            //std::cout<<"found"<<std::endl;
            exclude_directory(iterator);
        }
        else
        {
            //std::cout<<"not found"<<std::endl;
        }
    }

    bool should_include_file(const IteratorType& iterator)
    {
        std::string path = iterator->path().string();
        if(boost::filesystem::file_size(path) < mMinFileSize)
            return false;
        if(boost::filesystem::is_regular_file(path))
        {
            for(const auto& mask: mIncludeFileMask)
            {
                boost::smatch what;
                auto path = iterator->path();
                std::cout<<"@$@mask="<<mask<<" path.filename().string()="<<path.filename().string()<<std::endl;
                if(boost::regex_match(path.filename().string(), what, mask))
                    return true;
            }
        }
        return mIncludeFileMask.empty();
    }

private:
    std::vector<std::string> mExcludeDirectories;
    std::vector<boost::regex> mIncludeFileMask;
    size_t mMinFileSize = 0;
    const std::vector<std::pair<std::string, std::string>> mGlobToRegex = {
                                                                           {"\\.", "\\\\."},
                                                                           {"\\*", ".*"},
                                                                           {"\\?", "."}
                                                                          };
    virtual void exclude_directory(IteratorType& iterator){
        (void)iterator;
        //std::cout<<__PRETTY_FUNCTION__<<std::endl;
    }
};

template <typename IteratorType>
class TraversalExcluder : public TraversalExcluderBase<IteratorType>
{
public:
    TraversalExcluder(const std::vector<std::string>& excludeDirectories,
                      const std::vector<std::string>& includeFileMask,
                      size_t minFileSize) :
                      TraversalExcluderBase<IteratorType>(excludeDirectories,
                                                          includeFileMask,
                                                          minFileSize) {}
};

template<>
class TraversalExcluder<boost::filesystem::recursive_directory_iterator> 
    : public TraversalExcluderBase<boost::filesystem::recursive_directory_iterator>
{
public:
    TraversalExcluder(const std::vector<std::string>& excludeDirectories,
                      const std::vector<std::string>& includeFileMask,
                      size_t minFileSize) :
                        TraversalExcluderBase(excludeDirectories,
                                              includeFileMask,
                                              minFileSize) {}

private:
//https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0218r0.html
//Note: disable_recursion_pending() is used to prevent unwanted recursion into a directory.
    void exclude_directory(boost::filesystem::recursive_directory_iterator& iterator) override
    {
        //std::cout<<__PRETTY_FUNCTION__<<std::endl;
        iterator.disable_recursion_pending();
    }
};

template <typename IteratorType> 
class BoostDirectoryTraversal : public DirectoryTraversal
{
public:
    BoostDirectoryTraversal(const CMDSettings& settings, 
                            std::shared_ptr<Hash> hasher) :
                            mDirectories(settings.mIncudeDirs),
                            mFileBlockSize(settings.mBlockSize),
                            mHasher(hasher)
    {
        mPathFilter = std::make_unique<TraversalExcluder<IteratorType>>(settings.mExcludeDirs, settings.mFileMasks, settings.mMinSize);
        mDirectoriesIterator = mDirectories.cbegin();        
        mFilesIterator = mDirectoriesIterator != mDirectories.cend() 
            ? IteratorType(*mDirectoriesIterator)
            : IteratorType();
        mFilesIteratorEnd = IteratorType();
        if(points_to_directory(mFilesIterator) || !mPathFilter->should_include_file(mFilesIterator))
            move_to_next_file();
    }
    
    bool is_traversed() const override
    {
        return (mFilesIterator == mFilesIteratorEnd) && (mDirectoriesIterator == mDirectories.cend()); 
    }

    File get_next_file() override
    {
        std::string path = mFilesIterator->path().string();
        move_to_next_file();
        return File(path, boost::filesystem::file_size(path),
                              move(std::make_shared<IfstreamFileReader>(path, mFileBlockSize)),
                              mHasher);
    }  

private:
    std::vector<std::string> mDirectories;
    std::unique_ptr<TraversalExcluder<IteratorType>> mPathFilter; 
    std::vector<std::string>::const_iterator mDirectoriesIterator; 
    IteratorType mFilesIterator; 
    IteratorType mFilesIteratorEnd;
    size_t mFileBlockSize;
    std::shared_ptr<Hash> mHasher;

    void move_to_next_file()
    {
        do
        {
            if(points_to_directory(mFilesIterator))
            {
                //std::cout<<"filter_exclude:"<<std::endl;
                mPathFilter->filter_exclude_dirs(mFilesIterator);
            }
            ++mFilesIterator;
            if(mFilesIterator == mFilesIteratorEnd)
            {
                ++mDirectoriesIterator;
                if(mDirectoriesIterator != mDirectories.cend())
                    mFilesIterator = IteratorType(*mDirectoriesIterator);
            }
        }
        while (!is_traversed() && (points_to_directory(mFilesIterator) || !mPathFilter->should_include_file(mFilesIterator))); 
    }
};

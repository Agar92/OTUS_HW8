#include <file_comparison.h>
#include <stdexcept>

using namespace std;

std::vector<uint8_t>& File::Iterator::operator*()
{
    if(mBlocksIterator != mHashedBlocks.end())
        return *mBlocksIterator;
    else if(mFileReader && !mFileReader->is_EOF())
    {
        auto read_bytes_count = mFileReader->read_block(mBuffer);
        auto padding_count = mFileReader->maximum_block_size() - read_bytes_count;
        for(size_t i = 0; i < padding_count; i++)
            mBuffer.push_back(static_cast<uint8_t>(0));
        auto hashed_block = mHasher->hash_block(mBuffer);
        mBlocksIterator = mHashedBlocks.insert(mBlocksIterator, hashed_block);
    }
    else
        throw std::range_error("***ERROR: Attempt to read an iterator at the end of file already.");
    return *mBlocksIterator;
}

std::vector<uint8_t>* File::Iterator::operator->(){return &(*mBlocksIterator);}

File::Iterator& File::Iterator::operator++() 
{
    mBlocksIterator++;
    return *this; 
}

File::Iterator File::Iterator::operator++(int) 
{
    Iterator tmp = *this;
    ++(*this);
    return tmp; 
}

string File::get_path() const {return mPath;}

size_t File::get_file_size() const {return mFileSize;}

list<std::string> File::get_duplicates() const {return mDuplicatePaths;}

void File::add_duplicate(const std::string& path) {mDuplicatePaths.push_back(path);}

bool operator==(const File::Iterator& a,
                const File::Iterator& b)
{
    if(a.mBlocksIterator != b.mBlocksIterator) return false;
    return (a.mFileReader && !a.mFileReader->is_EOF()) == (b.mFileReader && !b.mFileReader->is_EOF()); 
} 

bool operator!=(const File::Iterator& a,
                const File::Iterator& b)
{
    if(a.mBlocksIterator != b.mBlocksIterator) return true;
    return (a.mFileReader && !a.mFileReader->is_EOF()) != (b.mFileReader && !b.mFileReader->is_EOF());
}
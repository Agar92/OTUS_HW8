#define BOOST_TEST_MODULE Test_bayan_Options
#include <boost/test/included/unit_test.hpp>

#include <boost/program_options.hpp>
#include <iostream>

#include <settings.h>

using namespace std;

ostream& operator<<(ostream& os, const ScanDepth& d)
{
    os << static_cast<unsigned short>(d);
    return os;
}

const size_t char_ptr_size = sizeof(char*);

BOOST_AUTO_TEST_SUITE(Logic)


BOOST_AUTO_TEST_CASE(test_help_options)
{
    const char *argv[] = {"bayan", "--h"};
    int argc = sizeof(argv)/char_ptr_size;;
    auto settings = CMDSettings::parse_from_arguments(argc, argv);
    BOOST_TEST(settings.mShowHelp);
    BOOST_CHECK(settings.mShowHelp==1);
    BOOST_TEST(!settings.mHelpText.empty());
    std::cout<<"HELL"<<std::endl;
}

BOOST_AUTO_TEST_CASE(test_default_options)
{
    const char *argv[] = {"bayan"};
    int argc = sizeof(argv)/char_ptr_size;
    auto settings = CMDSettings::parse_from_arguments(argc, argv);  
    BOOST_TEST(!settings.mShowHelp);
    BOOST_TEST(settings.mIncudeDirs.size() > 0);
    BOOST_TEST(settings.mExcludeDirs.size() == 0);
    BOOST_TEST(settings.mScanDepth == ScanDepth::SCAN_CURRENT);
    BOOST_TEST(settings.mMinSize == 1);
    BOOST_TEST(settings.mFileMasks.size() == 0);
    BOOST_TEST(settings.mBlockSize > 0);
    BOOST_TEST(!settings.mHashAlgorithm.empty());
}

BOOST_AUTO_TEST_CASE(test_options_parsing)
{
    const char * include_dirs[] = {"aaaa", "bbb", ".."};
    const char * exclude_dirs[] = {"dddd", "eeee", "/"};
    const char * depth_str = "1";
    const char * min_size_str = "777";
    const char * file_masks[] = {"*.cpp", "*.txt"};
    const char * block_size_str = "999";
    const char * hash_algorithm = "md5";

    const char *argv[] = 
    {
        "bayan", 
        "--i", include_dirs[0], include_dirs[1], include_dirs[2],
        "--e", exclude_dirs[0], exclude_dirs[1], exclude_dirs[2],
        "--d", depth_str,
        "--m", min_size_str,
        "--f", file_masks[0], file_masks[1],
        "--b", block_size_str,
        "--a", hash_algorithm
    };

    int argc = sizeof(argv)/char_ptr_size;

    auto settings = CMDSettings::parse_from_arguments(argc, argv);  

    BOOST_TEST(!settings.mShowHelp);

    auto include_vector = vector<string>(include_dirs, include_dirs + sizeof(include_dirs)/char_ptr_size); 
    BOOST_TEST(settings.mIncudeDirs == include_vector);

    auto exclude_vector = vector<string>(exclude_dirs, exclude_dirs + sizeof(exclude_dirs)/char_ptr_size); 
    BOOST_TEST(settings.mExcludeDirs == exclude_dirs);

    BOOST_TEST(settings.mScanDepth == static_cast<ScanDepth>(stoi(depth_str)));
    BOOST_TEST(settings.mMinSize == stoi(min_size_str));

    auto masks_vector = vector<string>(file_masks, file_masks + sizeof(file_masks)/char_ptr_size); 
    BOOST_TEST(settings.mFileMasks == masks_vector);
    
    BOOST_TEST(settings.mBlockSize == stoi(block_size_str));
    BOOST_TEST(settings.mHashAlgorithm == string(hash_algorithm));
}

BOOST_AUTO_TEST_CASE(test_incorrect_depth)
{
    const char *argv[] = 
    {
        "bayan",
        "--d", "100"
    };
    
    int argc = sizeof(argv)/char_ptr_size;

    BOOST_CHECK_THROW(CMDSettings::parse_from_arguments(argc, argv), boost::program_options::validation_error);  
}

BOOST_AUTO_TEST_CASE(test_incorrect_block_size)
{
    const char *argv[] = 
    {
        "bayan",
        "--b", "-1"
    };
    
    int argc = sizeof(argv)/char_ptr_size;

    BOOST_CHECK_THROW(CMDSettings::parse_from_arguments(argc, argv), boost::program_options::validation_error);  
}

BOOST_AUTO_TEST_CASE(test_incorrect_file_size)
{
    const char *argv[] = 
    {
        "bayan",
        "--m", "-1"
    };
    
    int argc = sizeof(argv)/char_ptr_size;

    BOOST_CHECK_THROW(CMDSettings::parse_from_arguments(argc, argv), boost::program_options::validation_error);  
}

BOOST_AUTO_TEST_CASE(test_incorrect_hash_algorithm)
{
    const char *argv[] = 
    {
        "bayan",
        "--a", "abra"
    };
    
    int argc = sizeof(argv)/char_ptr_size;

    BOOST_CHECK_THROW(CMDSettings::parse_from_arguments(argc, argv), boost::program_options::validation_error);  
}

BOOST_AUTO_TEST_SUITE_END()
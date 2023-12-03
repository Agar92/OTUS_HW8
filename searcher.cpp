#include <searcher.h>
#include <iostream>

void Searcher::search()
{
    while(!mDirectoryTraversal->is_traversed())
    {
        File file = mDirectoryTraversal->get_next_file();
        auto is_duplicate = false;
        for(auto& compared_file: mComparisonFiles)
        {
            //std::cout<<"mComparisonFiles.size()="<<mComparisonFiles.size()
            //         <<" compared_file="<<compared_file.get_path()
            //         <<" file="<<file.get_path()
            //         <<std::endl;
            if(compared_file.get_file_size() != file.get_file_size()) continue;
            bool blocks_match = true;
            auto blocks_iterator = file.begin();
            auto compared_iter = compared_file.begin();
            for( ;(blocks_iterator != file.end()) && (compared_iter != compared_file.end()); ++blocks_iterator, ++compared_iter)
            {
                if(*blocks_iterator != *compared_iter)
                {
                    blocks_match = false;
                    break;
                }
            }
            if((blocks_iterator == file.end()) && (compared_iter == compared_file.end()) && blocks_match)
            {
                is_duplicate = true;
                compared_file.add_duplicate(file.get_path());
                //std::cout<<"is_duplicate: "<<file.get_path()<<" "<<compared_file.get_path()<<std::endl;
            }
        }
        if(!is_duplicate)
        {
            //std::cout<<"!is_duplicate: "<<file.get_path()
            //         <<" mComparisonFiles.size()="<<mComparisonFiles.size()
            //         <<std::endl;
            mComparisonFiles.push_back(file);
        }
    }
}
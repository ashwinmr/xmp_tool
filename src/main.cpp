#include "args.hpp"
#include "db_tools.hpp"
#include "xmp_tools.hpp"
#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main(int argc, const char *argv[]) {

    // Parse args
    Args args(argc, argv);
    if (!args.valid) {
        return 0;
    }

    // Perform subcommand
    if(args.sub_cmd == "load"){

        // Store args
        std::string path = args.path;
        std::string db_path = args.db_path;

        // Check if path exists
        if (fs::exists(path)) {
            if (fs::is_regular_file(path)) {
                std::vector<std::string> paths;
                paths.push_back(path);
                GetAndStoreTags(paths,db_path);
            } else if (fs::is_directory(path)) {
                std::vector<std::string> paths = GetFilePaths(path, true);
                GetAndStoreTags(paths,db_path);
            } else {
                std::cout << "Input path exists, but is not a file or directory" << std::endl;
            }
        } else {
            std::cout << "Input path does not exist" << std::endl;
        }
    }
    else if(args.sub_cmd == "get"){
        // Store args
        std::string db_path = args.db_path;
        std::string tag_query = args.tag_query;

        // Check if path exists
        if (fs::is_regular_file(db_path)){
            PrintPathsForTagQuery(db_path,tag_query);
        }
        else{
            std::cout << "Input path is not a file" << std::endl;
        }
    }
    else if(args.sub_cmd == "add"){
        // Store args
        std::vector<std::string> file_paths = args.file_paths;
        std::vector<std::string> tags = args.tags;

        AddTagsToFiles(file_paths,tags);
    }
    else if(args.sub_cmd == "rem"){
        // Store args
        std::vector<std::string> file_paths = args.file_paths;
        std::vector<std::string> tags = args.tags;
        bool remove_all = args.remove_all;

        RemoveTagsFromFiles(file_paths,tags,remove_all);
    }
    else if(args.sub_cmd == "read"){
        // Store args
        std::vector<std::string> file_paths = args.file_paths;

        for(auto &file_path: file_paths){
            ReadXmpFromFile(file_path);
            std::cout << std::endl;
        }
    }

    return 0;
}

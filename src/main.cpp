#include "args.hpp"
#include "db_tools.hpp"
#include "xmp_tools.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <set>

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
        bool force_create = args.force_create;

        // Check if db path is a directory
        if(fs::is_directory(db_path)){
            std:: cout << "Input db path is a directory" << std::endl;
            return 0;
        }

        // Add extension if db path doesnt have one
        if(!fs::path(db_path).has_extension()){
            db_path = fs::path(db_path).replace_extension(".db").string();
        }

        // Check if db path exists
        if(fs::exists(db_path)){
            if(force_create){
                fs::remove(db_path);
            }
            else{
                std::cout << "Database " << db_path << " already exists. Use -f to replace." << std::endl;
                return 0;
            }
        }

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
    else if(args.sub_cmd == "query"){
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
        std::set<std::string> tags(args.tags.begin(), args.tags.end());

        AddTagsToFiles(file_paths,tags);
    }
    else if(args.sub_cmd == "rem"){
        // Store args
        std::vector<std::string> file_paths = args.file_paths;
        std::set<std::string> tags(args.tags.begin(), args.tags.end());
        bool remove_all = args.remove_all;
        bool remove_duplicates = args.remove_duplicates;

        if(remove_all){
            RemoveAllTagsFromFiles(file_paths);
        }
        else if(remove_duplicates){
            RemoveDuplicateTagsFromFiles(file_paths);
        }
        else{
            RemoveTagsFromFiles(file_paths,tags);
        }
    }
    else if(args.sub_cmd == "read"){
        // Store args
        std::vector<std::string> file_paths = args.file_paths;

        for(auto &file_path: file_paths){
            ReadXmpFromFile(file_path);
            std::cout << std::endl;
        }
    }
    else if(args.sub_cmd == "get"){
        // Store args
        std::vector<std::string> file_paths = args.file_paths;

        for(auto &file_path: file_paths){
            std::vector<std::string>tags = GetTagsFromFile(file_path);
            for(auto& tag: tags){
                std::cout << tag << std::endl;
            }
            std::cout << std::endl;
        }
    }

    return 0;
}

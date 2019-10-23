#ifndef ARGS_H_
#define ARGS_H_

#include <string>
#include <vector>

class Args {
   public:
    bool valid = false;
    bool remove_all = false;
    std::string path;
    std::string db_path;
    std::string tag_query;
    std::string sub_cmd;
    std::vector<std::string> file_paths;
    std::vector<std::string> tags; 

    /**
     * Constructor parses command line arguments
     */
    Args(int argc, const char** argv);
};

#endif  // ARGS_H_

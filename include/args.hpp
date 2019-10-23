#ifndef ARGS_H_
#define ARGS_H_

#include <string>

class Args {
   public:
    bool valid = false;
    std::string path;
    std::string db_path;
    std::string tag_query;
    std::string sub_cmd;

    /**
     * Constructor parses command line arguments
     */
    Args(int argc, const char** argv);
};

#endif  // ARGS_H_

#include "args.hpp"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

/**
 * Parse command line arguments
 */
Args::Args(int argc, const char **argv) {
    try {
        // Create args map
        po::variables_map args;

        // Declare supported options
        po::options_description desc("Allowed options:");
        desc.add_options()("help,h", "Help message")
        ("path,p", po::value<std::string>()->required(), "Path to file or directory")
        ("db_path,d", po::value<std::string>()->default_value(""),"Path to database")
        ;

        // Make options positional
        po::positional_options_description desc_p;
        desc_p.add("path", 1);

        // Store options
        po::store(po::command_line_parser(argc, argv).options(desc).positional(desc_p).run(), args);

        // Handle help before checking for errors
        if (args.count("help") || (argc < 2)) {
            std::cout << desc << std::endl;
            return;
        }

        // Check inputs for errors
        po::notify(args);

        // Store inputs
        this->path = args["path"].as<std::string>();
        this->db_path = args["db_path"].as<std::string>();

        // Parsing successful
        this->valid = true;

        return;
    } catch (po::error &e) {
        std::cout << "Error: " << e.what() << std::endl;
        return;
    }
}

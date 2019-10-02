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
        ("path,p", po::value<std::string>()->required(), "Path to file or directory");

        // Make options positional
        po::positional_options_description desc_p;
        desc_p.add("path", 1);

        // Store options
        po::store(po::command_line_parser(argc, argv).options(desc).positional(desc_p).run(), args);

        // Handle help before checking for errors
        if (args.count("help") || (argc < 2)) {
            std::cout << desc << std::endl;
            this->valid = false;
            return;
        }

        // Check inputs for errors
        po::notify(args);

        // Store inputs
        this->path = args["path"].as<std::string>();

        // Parsing successful
        return;
    } catch (po::error &e) {
        this->valid = false;
        std::cout << "Error: " << e.what() << std::endl;
        return;
    }
}

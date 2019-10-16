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

        // Declare supported main options
        po::options_description main_desc("main options:");
        main_desc.add_options()("help,h", "help message")
        ("sub_cmd", po::value<std::string>()->required(), "sub command to execute")
        ("sub_args", po::value<std::string>(),"arguments for sub command")
        ;

        // Make options positional
        po::positional_options_description main_desc_p;
        main_desc_p.add("sub_cmd", 1);
        main_desc_p.add("sub_args", -1);

        // Parse options
        po::parsed_options main_parsed = po::command_line_parser(argc, argv).
            options(main_desc).
            positional(main_desc_p).
            allow_unregistered().
            run();

        // Store options
        po::store(main_parsed, args);

        // Handle help before checking for errors
        if (args.count("help") || (argc < 2)) {
            std::cout << main_desc << std::endl;
            return;
        }

        // Check inputs for errors
        po::notify(args);

        // Handle sub commands
        std::string sub_cmd = args["sub_cmd"].as<std::string>();

        if(sub_cmd == "load"){

            // load command has the following options:
            po::options_description load_desc("load options");
            load_desc.add_options()
            ("help,h", "help message")
            ("path,p",po::value<std::string>()->required(), "path to file or directory")
            ("db_path,d",po::value<std::string>()->default_value(""),"path to database")
            ;

            // Make options positional
            po::positional_options_description load_desc_p;
            load_desc_p.add("path", 1);
            load_desc_p.add("db_path",2);

            // Collect all the unrecognized options from the first pass. This will include the
            // (positional) command name, so we need to erase that.
            std::vector<std::string> opts = po::collect_unrecognized(main_parsed.options, po::include_positional);
            opts.erase(opts.begin());

            // Parse again...
            po::parsed_options load_parsed = po::command_line_parser(opts).
                options(load_desc).
                positional(load_desc_p).
                run();

            // Store options
            po::store(load_parsed, args);

            // Handle help before checking for errors
            if (args.count("help") || (opts.size() < 1)) {
                std::cout << load_desc << std::endl;
                return;
            }

            // Check inputs for errors
            po::notify(args);

            // Store inputs
            this->path = args["path"].as<std::string>();
            this->db_path = args["db_path"].as<std::string>();

            // Parsing successful
            this->valid = true;
        }

        return;
    } catch (po::error &e) {
        std::cout << "Error: " << e.what() << std::endl;
        return;
    }
}

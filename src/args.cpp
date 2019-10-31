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
        po::options_description main_desc("tool for modifying xmp data");
        main_desc.add_options()("help,h", "help message")
        ("sub_cmd", po::value<std::string>()->required(), "sub command to execute")
        ("sub_args", po::value<std::vector<std::string>>()->multitoken(),"arguments for sub command")
        ;

        // Make options positional
        po::positional_options_description main_desc_p;
        main_desc_p.add("sub_cmd", 1);
        main_desc_p.add("sub_args", -1); // Multitoken is needed if this is not provided

        // Parse options
        po::parsed_options main_parsed = po::command_line_parser(argc, argv).
            options(main_desc).
            positional(main_desc_p).
            allow_unregistered(). // Allow dash options to be ignored
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
        this->sub_cmd = sub_cmd;

        if(sub_cmd == "load"){

            // load command has the following options:
            po::options_description load_desc("load file tags into database");
            load_desc.add_options()
            ("help,h", "help message")
            ("path,p",po::value<std::string>()->required(), "path to file or directory")
            ("db_path,d",po::value<std::string>()->default_value("temp.db"),"path to database")
            ("force_create,f",po::bool_switch(),"delete existing database")
            ;

            // Make options positional
            po::positional_options_description load_desc_p;
            load_desc_p.add("path", 1);
            load_desc_p.add("db_path",1);

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
            this->force_create = args["force_create"].as<bool>();

            // Parsing successful
            this->valid = true;
        }
        else if(sub_cmd == "query"){

            // query command has the following options:
            po::options_description query_desc("find files based on tag query");
            query_desc.add_options()
            ("help,h", "help message")
            ("db_path,d",po::value<std::string>()->required(),"path to database")
            ("tag_query,t",po::value<std::string>()->default_value(""),"tag query")
            ;

            // Make options positional
            po::positional_options_description query_desc_p;
            query_desc_p.add("db_path",1);
            query_desc_p.add("tag_query",1);

            // Collect all the unrecognized options from the first pass. This will include the
            // (positional) command name, so we need to erase that.
            std::vector<std::string> opts = po::collect_unrecognized(main_parsed.options, po::include_positional);
            opts.erase(opts.begin());

            // Parse again...
            po::parsed_options query_parsed = po::command_line_parser(opts).
                options(query_desc).
                positional(query_desc_p).
                run();

            // Store options
            po::store(query_parsed, args);

            // Handle help before checking for errors
            if (args.count("help") || (opts.size() < 1)) {
                std::cout << query_desc << std::endl;
                return;
            }

            // Check inputs for errors
            po::notify(args);

            // Store inputs
            this->db_path = args["db_path"].as<std::string>();
            this->tag_query = args["tag_query"].as<std::string>();

            // Parsing successful
            this->valid = true;
        }
        else if(sub_cmd == "add"){

            // add command has the following options:
            po::options_description add_desc("add tags to files");
            add_desc.add_options()
            ("help,h", "help message")
            ("file_paths,f",po::value<std::vector<std::string>>()->multitoken(),"paths to files")
            ("tags,t",po::value<std::vector<std::string>>()->multitoken()->default_value(std::vector<std::string>{""},""),"tags") //Vector default values require << for ostream
            ;

            // Make options positional
            po::positional_options_description add_desc_p;
            add_desc_p.add("file_paths",-1);

            // Collect all the unrecognized options from the first pass. This will include the
            // (positional) command name, so we need to erase that.
            std::vector<std::string> opts = po::collect_unrecognized(main_parsed.options, po::include_positional);
            opts.erase(opts.begin());

            // Parse again...
            po::parsed_options add_parsed = po::command_line_parser(opts).
                options(add_desc).
                positional(add_desc_p).
                run();

            // Store options
            po::store(add_parsed, args);

            // Handle help before checking for errors
            if (args.count("help")) {
                std::cout << add_desc << std::endl;
                return;
            }

            // Check inputs for errors
            po::notify(args);

            // Store inputs
            
            // If no file paths input, get from stdin
            if(args.count("file_paths")){
                this->file_paths = args["file_paths"].as<std::vector<std::string>>();
            }
            else{
                std::string temp;
                while(std::getline(std::cin,temp)){
                    this->file_paths.push_back(temp);
                }
            }
            this->tags = args["tags"].as<std::vector<std::string>>();

            // Parsing successful
            this->valid = true;
        }
        else if(sub_cmd == "rem"){

            // rem command has the following options:
            po::options_description rem_desc("remove tags from files");
            rem_desc.add_options()
            ("help,h", "help message")
            ("file_paths,f",po::value<std::vector<std::string>>()->multitoken(),"paths to files")
            ("all,a",po::bool_switch(),"remove all")
            ("duplicates,d",po::bool_switch(),"remove duplicates")
            ("tags,t",po::value<std::vector<std::string>>()->multitoken()->default_value(std::vector<std::string>{""},""),"tags") //Vector default values require << for ostream
            ;

            // Make options positional
            po::positional_options_description rem_desc_p;
            rem_desc_p.add("file_paths",-1);

            // Collect all the unrecognized options from the first pass. This will include the
            // (positional) command name, so we need to erase that.
            std::vector<std::string> opts = po::collect_unrecognized(main_parsed.options, po::include_positional);
            opts.erase(opts.begin());

            // Parse again...
            po::parsed_options rem_parsed = po::command_line_parser(opts).
                options(rem_desc).
                positional(rem_desc_p).
                run();

            // Store options
            po::store(rem_parsed, args);

            // Handle help before checking for errors
            if (args.count("help")){
                std::cout << rem_desc << std::endl;
                return;
            }

            // Check inputs for errors
            po::notify(args);

            // Store inputs

            // If no file paths input, get from stdin
            if(args.count("file_paths")){
                this->file_paths = args["file_paths"].as<std::vector<std::string>>();
            }
            else{
                std::string temp;
                while(std::getline(std::cin,temp)){
                    this->file_paths.push_back(temp);
                }
            }

            this->remove_all = args["all"].as<bool>();
            this->remove_duplicates = args["duplicates"].as<bool>();
            this->tags = args["tags"].as<std::vector<std::string>>();

            // Parsing successful
            this->valid = true;
        }
        else if(sub_cmd == "read"){

            // read command has the following options:
            po::options_description read_desc("read xmp data from files");
            read_desc.add_options()
            ("help,h", "help message")
            ("file_paths,f",po::value<std::vector<std::string>>()->multitoken(),"paths to files")
            ;

            // Make options positional
            po::positional_options_description read_desc_p;
            read_desc_p.add("file_paths",-1);

            // Collect all the unrecognized options from the first pass. This will include the
            // (positional) command name, so we need to erase that.
            std::vector<std::string> opts = po::collect_unrecognized(main_parsed.options, po::include_positional);
            opts.erase(opts.begin());

            // Parse again...
            po::parsed_options read_parsed = po::command_line_parser(opts).
                options(read_desc).
                positional(read_desc_p).
                run();

            // Store options
            po::store(read_parsed, args);

            // Handle help before checking for errors
            if (args.count("help")) {
                std::cout << read_desc << std::endl;
                return;
            }

            // Check inputs for errors
            po::notify(args);

            // Store inputs

            // If no file paths input, get from stdin
            if(args.count("file_paths")){
                this->file_paths = args["file_paths"].as<std::vector<std::string>>();
            }
            else{
                std::string temp;
                while(std::getline(std::cin,temp)){
                    this->file_paths.push_back(temp);
                }
            }

            // Parsing successful
            this->valid = true;
        }
        else if(sub_cmd == "get"){

            // get command has the following options:
            po::options_description get_desc("get tags from files");
            get_desc.add_options()
            ("help,h", "help message")
            ("file_paths,f",po::value<std::vector<std::string>>()->multitoken(),"paths to files")
            ;

            // Make options positional
            po::positional_options_description get_desc_p;
            get_desc_p.add("file_paths",-1);

            // Collect all the unrecognized options from the first pass. This will include the
            // (positional) command name, so we need to erase that.
            std::vector<std::string> opts = po::collect_unrecognized(main_parsed.options, po::include_positional);
            opts.erase(opts.begin());

            // Parse again...
            po::parsed_options get_parsed = po::command_line_parser(opts).
                options(get_desc).
                positional(get_desc_p).
                run();

            // Store options
            po::store(get_parsed, args);

            // Handle help before checking for errors
            if (args.count("help")) {
                std::cout << get_desc << std::endl;
                return;
            }

            // Check inputs for errors
            po::notify(args);

            // Store inputs

            // If no file paths input, get from stdin
            if(args.count("file_paths")){
                this->file_paths = args["file_paths"].as<std::vector<std::string>>();
            }
            else{
                std::string temp;
                while(std::getline(std::cin,temp)){
                    this->file_paths.push_back(temp);
                }
            }

            // Parsing successful
            this->valid = true;
        }
        else{
            std::cout << "Invalid sub command" << std::endl;
        }
        return;
    } catch (po::error &e) {
        std::cout << "Error: " << e.what() << std::endl;
        return;
    }
}

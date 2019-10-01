#include <iostream>
#include "xmp_tools.hpp"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

using namespace std;

bool ParseArgs(int argc, const char **argv, po::variables_map &args)
{
	try
	{
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
		if (args.count("help") || (argc < 2))
		{
			std::cout << desc << std::endl;
			return false;
		}

		// Check inputs for errors
		po::notify(args);

		// Parsing successful
		return true;
	}
	catch (po::error &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return false;
	}
}

int main(int argc, const char *argv[])
{
	// Parse args
	po::variables_map args;
	if (!ParseArgs(argc, argv, args))
	{
		return 0;
	}

	for (auto &str : GetTags(args["path"].as<string>()))
	{
		cout << str << endl;
	}

	return 0;
}

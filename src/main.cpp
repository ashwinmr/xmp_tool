#include <iostream>
#include "xmp_tools.hpp"

using namespace std;

int main(int argc, const char *argv[])
{
	for (auto &str : GetTags("C:/Ashwin/My_Programming/Programs/xmp/temp/1.jpg"))
	{
		cout << str << endl;
	}

	return 0;
}

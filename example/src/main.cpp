#include <iostream>
#include <boost/filesystem.hpp>

#include "strings.hpp"

int main(int argc, char* argv[]) {
   std::cout << hello_world << "\n";
   std::cout << boost::filesystem::file_size(argv[0]) << "\n";
}

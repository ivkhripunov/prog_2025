// 06.17.cpp (главный исполняемый файл)
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

int main() {
    std::cout << "Available libraries:\n";
    std::cout << "  1. libshared_v1.so\n";
    std::cout << "  2. libshared_v2.so\n";
    std::cout << "Enter library name: ";

    std::string lib_name;
    std::cin >> lib_name;

    fs::path lib_path = boost::dll::program_location().parent_path() / lib_name;

    auto func = boost::dll::import_symbol<void()>(lib_path.string(), "test");
    func();

    return 0;
}
// 06.17.cpp (главный исполняемый файл)
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include "library.hpp"

namespace fs = boost::filesystem;

int main() {
    // Вызов статической функции из библиотеки
    library::test();

    std::cout << "Available libraries:" << std::endl;
    std::cout << "  1. libshared_v1.so" << std::endl;
    std::cout << "  2. libshared_v2.so" << std::endl;
    std::cout << "Enter library name: ";

    std::string library_name;
    std::cin >> library_name;

    std::vector<std::function<void()>> functions;

    try {
        // Поиск библиотеки в нескольких возможных местах
        fs::path library_path;
        std::vector<fs::path> search_paths = {
            fs::current_path(),                    // Текущая директория
            fs::current_path() / "bin",            // Директория bin
            "/usr/local/lib",                       // Системная директория
            fs::path(library_name).is_absolute() ?  // Если ввели абсолютный путь
                fs::path(library_name) : fs::path()
        };

        // Удаляем пустые пути
        search_paths.erase(
            std::remove_if(search_paths.begin(), search_paths.end(),
                          [](const fs::path& p) { return p.empty(); }),
            search_paths.end()
        );

        // Ищем библиотеку
        for (const auto& path : search_paths) {
            fs::path full_path = path / library_name;
            if (fs::exists(full_path)) {
                library_path = full_path;
                break;
            }
        }

        // Если не нашли, пробуем использовать введенное имя как есть
        if (library_path.empty()) {
            if (fs::exists(library_name)) {
                library_path = library_name;
            } else {
                throw std::runtime_error("Library not found: " + library_name);
            }
        }

        std::cout << "Loading library: " << library_path << std::endl;

        // Импортируем функцию test из динамической библиотеки
        functions.push_back(
            boost::dll::import_symbol<void()>(library_path.string(), "test")
        );

        std::cout << "Successfully loaded test() from " << library_name << std::endl;

    } catch (const boost::system::system_error& e) {
        std::cerr << "Boost.DLL error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Вызываем все загруженные функции
    for (const auto& function : functions) {
        function();
    }

    return 0;
}
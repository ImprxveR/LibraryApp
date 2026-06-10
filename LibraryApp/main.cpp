#include "Library.h"
#include "FileIO.h"
#include "Menu.h"
#include <windows.h>
#include <iostream>

int main() {
    // Отключаем кириллицу, работаем с латиницей
    // Так не будет проблем с кодировками

    Library library;

    std::cout << "Loading catalog from data/catalog.txt...\n";
    FileIO::loadFromFile("data/catalog.txt", library);
    std::cout << "Loaded books: " << library.getAllBooks().size() << "\n";

    Menu menu(library);
    menu.run();

    return 0;
}
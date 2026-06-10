#include "Library.h"
#include "FileIO.h"
#include "Menu.h"
#include <windows.h>
#include <iostream>

int main() {
    // Настройка консоли на UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Дополнительно: включить поддержку Unicode в консоли (требуется для корректного ввода)
    // Это нужно, чтобы std::cin читал UTF-8
    system("chcp 65001 > nul");

    Library library;

    std::cout << "Загрузка каталога из data/catalog.txt...\n";
    FileIO::loadFromFile("data/catalog.txt", library);
    std::cout << "Загружено книг: " << library.getAllBooks().size() << "\n";

    Menu menu(library);
    menu.run();

    return 0;
}
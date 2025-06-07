#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    std::cout << "SFML version: "
              << SFML_VERSION_MAJOR << "."
              << SFML_VERSION_MINOR << "."
              << SFML_VERSION_PATCH << "\n";
    std::cout << "Press Enter to exit...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return 0;
}

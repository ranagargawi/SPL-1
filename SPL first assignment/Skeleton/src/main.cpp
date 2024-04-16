#include "WareHouse.h"
#include <iostream>
#include <iostream>
using namespace std;

WareHouse *backup = nullptr;

int main(int argc, char **argv)
{
    std::cout << "hello world" << std::endl;

    if (argc != 2)
    {
        std::cout << "usage: warehouse <config_path>" << std::endl;
        return 0;
    }
    string configFilePath = argv[1];
    WareHouse wareHouse(configFilePath);
    wareHouse.start();
    if (backup != nullptr)
    {
        delete backup;
        backup = nullptr;
    }
    return 0;
}
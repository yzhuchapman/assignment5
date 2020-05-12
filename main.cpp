#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <deque>
#include "menu.cpp"


using namespace std;

int main()
{
	Menu* menu = new Menu();
    menu->initDatabases();
    menu->doMenu();
    menu->saveDatabases();

    return 0;
}

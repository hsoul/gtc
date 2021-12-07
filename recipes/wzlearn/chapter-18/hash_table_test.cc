#include "HashTable.hpp"
#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

int main()
{
    HashTable<int, std::string> hash_table;
    hash_table.put(1, "sdf");
    hash_table.put(2, "s");
    hash_table.printAll();
}

#include "LRUBaseHashTable.h"
#include "fmt_include.h"

int main()
{
    LRUBaseHashTable<int, std::string> lru_table;

    lru_table.add(1, "a");
    lru_table.add(7, "b");
    lru_table.add(3, "c");
    lru_table.add(5, "d");
    lru_table.add(9, "e");

    lru_table.printAll();

    lru_table.remove(3);

    lru_table.printAll();

    lru_table.get(1);
    lru_table.printAll();

    lru_table.add(37, "k");
    lru_table.add(11, "h");
    lru_table.printAll();

    lru_table.clear();

    lru_table.printAll();

    return 0;
}

#include <iostream>
#include "linked_list.hpp"

int main() {
    List<int> intList;
    list_init(intList);
    int i;
    for(int i = 0; i < 10; ++i) {
        list_insert(intList, i);
    }
    list_print(intList, std::cout);
    std::cout << std::endl;
    list_destroy(intList);
}

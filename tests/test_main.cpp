#include <iostream>

void testGridConstruction();
void testGridBounds();
void testGridMutation();
void testGridResizeDataRetention();

void testSaveSystem();

int main() {
    testGridConstruction();
    testGridBounds();
    testGridMutation();
    testGridResizeDataRetention();

    testSaveSystem();

    std::cout<< "All tests passed!\n";
}
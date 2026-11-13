#include <iostream>
#include <cassert>
#include "core/grid.hpp"
// #include "app/common.hpp"

void testGridConstruction() {
    Grid<int> g(3,4,10);

    assert(g.getWidth() == 3);
    assert(g.getHeight() == 4);
    assert(g.size() == 12);
    assert(g.at(0,0) == 10);
}

void testGridBounds() {
    Grid<int> g(3,4,10);

    assert(g.inBounds(0,0));
    assert(!g.inBounds(-1,0));
    assert(!g.inBounds(3,4));
}

void testGridMutation() {
    Grid<int> g(3,4,10);

    assert(g.at(1,2) == 10);
    g.at(1,2) = 42;
    assert(g.at(1,2) == 42);
}

void testGridResizeDataRetention() {
    Grid<int> g(3,4,10);
    g.at(1,2) = 69;
    
    g.resize(5,5,0);

    assert(g.getWidth() == 5);
    assert(g.getHeight() == 5);
    assert(g.size() == 25);
    assert(g.at(0,0) == 10);
    assert(g.at(3,4) == 0);
    assert(g.at(1,2) == 69);
}

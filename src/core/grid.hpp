#pragma once
#include <vector>
#include <cassert>

template <typename T>
class Grid {
    int m_width;
    int m_height;
    std::vector<T> m_data;
public:
    Grid() : m_width(0), m_height(0) {}

    Grid(int width, int height, T fillValue = T{})
    : m_width(width),
      m_height(height),
      m_data(width * height, fillValue) {}

    T& at(int x, int y) {
        assert(inBounds(x, y) && "Grid::at out of bounds!");
        return m_data[y*m_width+x];

    }
    const T& at(int x, int y) const {
        assert(inBounds(x, y) && "Grid::at out of bounds!");
        return m_data[y*m_width+x];
    }

    bool inBounds(int x, int y) const {
        return x >= 0 && x < m_width && y >= 0 && y < m_height;
    }

    void resize(int newWidth, int newHeight, T fillValue) {
        std::vector<T> newData = std::vector<T>(newWidth*newHeight, fillValue);

        int minWidth = newWidth > m_width ? m_width : newWidth;
        int minHeight = newHeight > m_height ? m_height : newHeight;

        for(int i=0; i<minHeight; i++) {
            for(int j=0; j<minWidth; j++) {
                int index = i*newWidth + j;
                assert(index < newData.size() && "Grid::resize newData out of bounds");

                int index2 = i*m_width + j;
                assert(index2 < m_data.size() && "Grid::resize m_data ouf of bounds");

                newData[index] = m_data[index2];
            }
        }

        m_data = newData;
        m_width = newWidth;
        m_height = newHeight;
    }

    int getWidth() const {return m_width; }
    int getHeight() const {return m_height; }
    int size() const {return static_cast<int>(m_data.size()); }
};
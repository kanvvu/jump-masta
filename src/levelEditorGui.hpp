#pragma once

#include <string>
#include <vector>
#include "common.hpp"
#include <functional>
#include <raylib.h>
#include "interfaces.hpp"

struct EdgeDropDownRow {
    int edgeIndex = -1;
    std::string label;
    std::vector<std::string> options;
    int selectedIndex = -1;
};

class ListView : public IDraw {
private:
    std::function<void()> onChange;

    std::vector<std::string> tiles;
    Rectangle bounds;

public:
    ListView();
    ListView(Rectangle bounds, std::vector<std::string> tiles, std::function<void()> onChange);

    int scrollIndex = 0;
    int selectedIndex = -1;
    int hoveredIndex = -1;

    int getTilesSize() {return tiles.size();};
    std::string getTile(int index);

    void setTiles(const std::vector<std::string>& tiles);

    void changeSelected(int index, int maxScroll = 6);

    void draw();
};

class Button : public IDraw {
    private:
    Rectangle bounds;
    std::string text;
    std::function<void()> onClick;

    public:
    Button();
    Button(Rectangle bounds, std::string text, std::function<void()> onClick);

    void draw();
};

class SmallTextBox : public IDraw {
    private:
    Rectangle bounds;
    std::function<void()> onExit;
    bool editMode = 0;
    
    public:
    SmallTextBox();
    SmallTextBox(Rectangle bounds, std::function<void()> onExit, std::string defaultText);
    
    char value[64];
    bool getEditMode() {return editMode;}

    void draw();

};

class DropDownBox : public IDraw {
    Rectangle bounds;
    std::function<void()> onChange;
    
    bool editMode = 0;
    
    public:
    DropDownBox();
    DropDownBox(Rectangle bounds, std::vector<std::string> options);
    DropDownBox(Rectangle bounds, std::vector<std::string> options, std::function<void()> onChange);
    
    int selectedIndex = -1;
    std::vector<std::string> options;

    void setOnChange(std::function<void()> onChange);
    void draw();
};

class TextLabel : public IDraw {
    private:
    Vector2 position;
    std::string text;
    int size;
    Color color;

    public:
    TextLabel();
    TextLabel(Vector2 position, std::string text, int size, Color color);

    void draw();
};


class TextLabelAndDropDownBoxList : public IDraw {
    private:
    std::vector<DropDownBox> dropdownboxes;
    std::vector<TextLabel> textLabels;
    
    Vector2 position;
    
    public:
    using RowChangedCallback = std::function<void(int, int)>;

    TextLabelAndDropDownBoxList();
    TextLabelAndDropDownBoxList(Vector2 position, const std::vector<EdgeDropDownRow>& rows, RowChangedCallback onRowChanged = {});

    void update(const std::vector<EdgeDropDownRow>& rows, RowChangedCallback onRowChanged = {});

    void draw();

};



// struct LevelEditorGui
// {
//     std::string dropDownOptions;
//     std::vector<DropDownState> dropDowns;
    
//     void rebuildDropDowns(std::vector<std::string> tileNames);

//     int saveButton = 0;
//     int loadButton = 0;


// };
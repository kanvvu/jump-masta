#pragma once

#include <string>
#include <vector>
#include "common.hpp"

struct LevelEditorGui
{
    std::vector<std::string> tileNames;
    std::vector<std::string> tileListStorage;
    std::vector<char *> tileList;

    int tileListScroll = 0;
    int tileListSelected = -1;
    int tileListHovered = -1;

    void changeSelected(int index, int maxScroll = 6);

    void rebuildTileList();
    
    std::string dropDownOptions;
    std::vector<DropDownState> dropDowns;
    
    void rebuildDropDowns();

    int saveButton = 0;
    int loadButton = 0;

    char tbName[64] = "level";
    bool tbNameEditMode = 0;

    char tbWidth[64] = "10";
    bool tbWidthEditMode = 0;

    char tbHeight[64] = "15";
    bool tbHeightEditMode = 0;
};
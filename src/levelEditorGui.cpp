#include "levelEditorGui.hpp"

#include <iostream>


void LevelEditorGui::changeSelected(int index, int maxScroll)
{
    if (index >= tileNames.size()) {
        std::cerr<<"ERROR [LevelEditor::changeSelected] invalid index: "<<index<<" >="<<tileNames.size()<<"\n";
        return;
    }
    if (index <  0) {
        std::cerr<<"ERROR [LevelEditor::changeSelected] invalid index: "<<index<<" < "<<" 0\n";
        return;
    }

    int scroll = tileList.size() - maxScroll;
    scroll = scroll < 0 ? 0 : scroll;

    if (tileListScroll > scroll) {
        tileListScroll = scroll;
    }

    tileListSelected = index;
    // std::cout<<index<<" "<<scroll<<std::endl;
}

void LevelEditorGui::rebuildTileList()
{
    tileList.clear();

    for (std::string& tileName : tileNames) {
        tileList.push_back(const_cast<char*>(tileName.c_str()));
    }
    
    if (tileListSelected >= static_cast<int>(tileList.size())) {
        tileListSelected = (tileList.empty() ? -1 : tileList.size()-1);
    }
}

void LevelEditorGui::rebuildDropDowns()
{
    dropDowns.clear();
    dropDownOptions = "";
    for(int i=0; i<tileNames.size(); i++) {
        dropDownOptions += tileNames.at(i);
        if (i + 1 != tileNames.size()) dropDownOptions += ";";
    }
}
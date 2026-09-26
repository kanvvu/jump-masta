#include "levelEditorGui.hpp"

#include <iostream>
#include "raygui.h"



void ListView::changeSelected(int index, int maxTilesCount)
{
    if (index >= tiles.size()) {
        std::cerr<<"ERROR [LevelEditor::changeSelected] invalid index: "<<index<<" >="<<tiles.size()<<"\n";
        return;
    }
    if (index <  0) {
        std::cerr<<"ERROR [LevelEditor::changeSelected] invalid index: "<<index<<" < "<<" 0\n";
        return;
    }
    

    int maxScroll = tiles.size() - maxTilesCount;
    maxScroll = maxScroll < 0 ? 0 : maxScroll;
    
    if (scrollIndex > maxScroll) {
        scrollIndex = maxScroll;
    }
    

    if (index - (maxTilesCount - 1) > scrollIndex) {
        scrollIndex = maxScroll;
    }
    
    selectedIndex = index;
    std::cout<<"SELECTED: "<<index<<", SCROLL: "<<maxScroll<<", SCROLL INDEX: "<<scrollIndex<<std::endl;

    // std::cout<<index<<" "<<scroll<<std::endl;
}

// void LevelEditorGui::rebuildDropDowns(std::vector<std::string> tileNames)
// {
//     dropDowns.clear();
//     dropDownOptions = "";
//     for(int i=0; i<tileNames.size(); i++) {
//         dropDownOptions += tileNames.at(i);
//         if (i + 1 != tileNames.size()) dropDownOptions += ";";
//     }
// }
ListView::ListView() : bounds({0,0,0,0}), onChange([]{}), tiles({}) {};

ListView::ListView(Rectangle bounds, std::vector<std::string> tiles, std::function<void()> onChange) : bounds(bounds), onChange(onChange), tiles(tiles)
{
    setTiles(tiles);
};

std::string ListView::getTile(int index)
{
    if (index < 0 || index >= tiles.size()) {
        std::cerr<<"ERROR [LevelEditorGui::ListView::getTile] invalid index: "<<index<<" >= "<<tiles.size()<<"\n";
        return "";
    }
    return tiles[index];
}

void ListView::setTiles(const std::vector<std::string> &tiles)
{
    this->tiles = tiles;

    if (selectedIndex >= static_cast<int>(tiles.size())) {
        selectedIndex = (tiles.empty() ? -1 : tiles.size()-1);
    }
}

void ListView::draw()
{
    std::vector<char *> tilesCharPtr;
    for(auto& tile : tiles) tilesCharPtr.push_back(const_cast<char*>(tile.c_str()));

    if (GuiListViewEx(bounds, tilesCharPtr.data(), static_cast<int>(tilesCharPtr.size()), &scrollIndex, &selectedIndex, &hoveredIndex)) {
        onChange();
        std::cout<<"selected: "<<selectedIndex<<", scroll: "<<scrollIndex<<std::endl;
    }
}

Button::Button() : bounds({0,0,0,0}), text("Text"), onClick([]{}) 
{
}

Button::Button(Rectangle bounds, std::string text, std::function<void()> onClick)
: bounds(bounds),
  text(text),
  onClick(onClick)
{
}

void Button::draw()
{
    if (GuiButton(bounds, text.c_str())){
        onClick();
    }
}

SmallTextBox::SmallTextBox() : bounds({0,0,0,0}), onExit([]{})
{
    for(int i=0; i<sizeof(value); i++) value[i] = '\0';
}

SmallTextBox::SmallTextBox(Rectangle bounds, std::function<void()> onExit, std::string defaultText)
: bounds(bounds),
  onExit(onExit)
{
    snprintf(value, sizeof(value), defaultText.c_str());
}

void SmallTextBox::draw()
{
    if (GuiTextBox(bounds, value, sizeof(value), editMode)) {
        if (editMode) {
            onExit();
        }

        editMode = !editMode;
    }
}

DropDownBox::DropDownBox()
: bounds({0,0,0,0}),
  options(std::vector<std::string>()),
  onChange([]{})
{
}

DropDownBox::DropDownBox(Rectangle bounds, std::vector<std::string> options)
: bounds(bounds),
  options(options),
  onChange([]{})
{
}

DropDownBox::DropDownBox(Rectangle bounds, std::vector<std::string> options, std::function<void()> onChange)
: bounds(bounds),
  options(options),
  onChange(onChange)
{
}

void DropDownBox::setOnChange(std::function<void()> onChange)
{
    this->onChange = onChange;
}

void DropDownBox::draw()
{
    std::string optionsString = "";
    for(int i=0; i<options.size(); i++) {
        optionsString += options.at(i);
        if (i + 1 != options.size()) optionsString += ";";
    }

    if (GuiDropdownBox(bounds, optionsString.c_str(), &selectedIndex, editMode)) {
        editMode = !editMode;
        onChange();
        // e.assignedNextLevel = gui.dropDowns[i-1].dropdownActive;
    }
}

TextLabel::TextLabel()
: position({0,0}),
  text(""),
  size(20),
  color(BLACK)
{
}

TextLabel::TextLabel(Vector2 position, std::string text, int size, Color color)
: position(position),
  text(text),
  size(size),
  color(color)
{
}

void TextLabel::draw()
{
    DrawText(text.c_str(), position.x, position.y, size, color);
}
TextLabelAndDropDownBoxList::TextLabelAndDropDownBoxList() : position({0,0}) {};

TextLabelAndDropDownBoxList::TextLabelAndDropDownBoxList(Vector2 position, LevelData &level, std::vector<std::string> levelNames) :
position(position)
{
    update(level, levelNames);
}

void TextLabelAndDropDownBoxList::update(LevelData &level, std::vector<std::string> levelNames)
{
    textLabels.clear();
    textLabels.reserve(level.getEdgesSize());
    dropdownboxes.clear();
    dropdownboxes.reserve(level.getEdgesSize());
    for(int i=level.getEdgesSize(); i>0; i--) {
        Edge& e = level.getEdge(i-1);
        std::string str = "EDGE ";
        str += std::to_string(i) + " (" + std::to_string(e.tilesCount) + ")";
        // Vector2 textPos = {10, (i-1)*30 + 300 + 210 + 10 + 60 + 80};
        Vector2 textPos = {position.x, (i-1)*30 + position.y};
        int textWidth = MeasureText(str.c_str(), 20);
        
        textLabels.push_back(TextLabel(textPos, str, 20, BLACK));
        dropdownboxes.push_back(DropDownBox({ textPos.x + textWidth + 10, textPos.y, 200, 25 }, levelNames));
        DropDownBox& dropDownBox = dropdownboxes.back();
        dropDownBox.selectedIndex = e.assignedNextLevel;
        dropdownboxes.back().setOnChange([this, &e, &dropDownBox]() {e.assignedNextLevel = dropDownBox.selectedIndex;});
    }
}

void TextLabelAndDropDownBoxList::draw()
{
    for(auto& textLabel: textLabels) textLabel.draw();
    for(auto& dropdownbox : dropdownboxes) dropdownbox.draw();
}

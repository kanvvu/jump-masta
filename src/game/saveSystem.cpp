#include "saveSystem.hpp"
#include <fstream>

bool saveGame(const SaveData &data, const std::string &filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
        return false;

    file<<data.levelIndex<<std::endl;

    file<<data.playerPosition.x<<std::endl;
    file<<data.playerPosition.y<<std::endl;
    file<<data.playerVelocity.x<<std::endl;
    file<<data.playerVelocity.y<<std::endl;
    file<<data.facingRight<<std::endl;
    file<<data.onGround<<std::endl;

    file<<data.jumpCount<<std::endl;
    file<<data.elapsedTime<<std::endl;

    return file.good();
}

bool loadGame(SaveData &data, const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    file>>data.levelIndex;

    file>>data.playerPosition.x;
    file>>data.playerPosition.y;
    file>>data.playerVelocity.x;
    file>>data.playerVelocity.y;
    file>>data.facingRight;
    file>>data.onGround;

    file>>data.jumpCount;
    file>>data.elapsedTime;

    return !file.fail();
}

bool hasSaveGame(const std::string &filename)
{
    std::ifstream file(filename);
    return file.good();
}

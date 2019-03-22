#pragma once

#include <vector>
#include <iostream>

#include "ZombieWaveElement.h"

struct ZombieWave
{
    ZombieWave(Game* game, nlohmann::basic_json<>& json);
    std::vector<ZombieWaveElement> elements;
    float getDuration() const;
    std::string comment;
};

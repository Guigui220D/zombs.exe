#pragma once

#include "Zombie.h"

struct ZombieWaveElement
{
    ZombieWaveElement(Game* game, nlohmann::basic_json<>& json);

    Zombie clone;

    unsigned int count;
    unsigned int current = 0;
    float start_t;
    float t;

    sf::Clock clk;

};

#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>
#include <assert.h>
#include <iostream>

#include "Level.h"
#include "Building.h"
#include "Buildings.h"
#include "Resources.h"
#include "Zombie.h"
#include "ZombieWave.h"

class Game
{
    public:
        static const int MAP_SQUARE_SIZE;
        static const int WINDOW_SQUARE_SIZE;

        Game();
        virtual ~Game();

        void run();
        void init();

        TargetInfo getNearestZombie(sf::Vector2f point) const;
        TargetInfo getNearestZombie() const;

        inline Level* getLevel() { return &level; };

        static sf::Vector2f getRandomZombiePos();

        int money = 150;

        inline Treasure* getTreasure() const { return treasure; };

    private:
        sf::RenderWindow window;

        sf::RectangleShape buildableZone;
        sf::Text moneyText;
        sf::Text gameOver;
        sf::Text upgradeCostText;
        sf::Text deleteText;
        sf::Text nextWaveText;
        sf::Font lucon;

        std::vector<std::unique_ptr<Building>> buildings;
        std::vector<Zombie> zombies;
        Level level;

        Treasure* treasure;

        bool inWave = false;
        std::vector<ZombieWave> waves;
        int waveCount;
        int currentWave = -1;
        sf::Clock waveClock;
};

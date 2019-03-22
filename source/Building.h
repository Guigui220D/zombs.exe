#pragma once

#define MAX_UPGRADE 5

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Resources.h"

class Game;

class Building
{
    public:
        Building(Game* game, sf::Vector2i pos, sf::Vector2i size, sf::Texture* colorBase, sf::Texture* base, int upgradeCost, int lifeMultiplyer = 1);
        virtual ~Building();

        void baseDraw(sf::RenderTarget& target) const;
        void drawHealthbar(sf::RenderTarget& target) const;
        virtual void drawMore(sf::RenderTarget& target) const;
        virtual void update(float delta);
        void updateLife();

        inline sf::Vector2i getPos() const { return pos; };
        inline sf::Vector2i getSize() const { return size; };
        inline sf::Vector2f getCenter() const { return center; };
        float getAngularPos(sf::Vector2f target) const;

        inline int getLife() const { return life; };
        inline int getMaxLife() const { return maxLife; };
        inline void regen() { life = maxLife; };
        inline void dealDamage(int damage) { life -= damage; };

        inline bool isDead() const { return dead; };

        static const uint32_t UPGRADE_COLORS[];
        static const int LIFE_VALUES[];

        inline int getUpgradeCost() const { return level * upgradeCostMultiplyer; };
        inline bool isLastLevel() const { return level ==  MAX_UPGRADE; };
        bool canBeUpgraded() const;
        bool upgrade();
        inline int getLevel() const { return level; };

    protected:
        int level;
        int upgradeCostMultiplyer;

        inline void setLife(int newLife) { life = newLife; };

        inline void setColor(sf::Color newColor) { colorBaseSprite.setColor(newColor); };

        bool dead = false;

        inline Game* getGame() { return game; };

    private:
        Game* game;

        sf::Vector2i pos;
        sf::Vector2i size;
        sf::Vector2f center;

        int life;
        int lifeMultiplyer;
        int maxLife;

        sf::Sprite colorBaseSprite, baseSprite;
        sf::RectangleShape healthBar, healthBarBase;

        static float toDegrees(float rads);
        static float toRads(float degrees);
};

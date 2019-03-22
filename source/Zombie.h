#pragma once

#include <SFML/Graphics.hpp>

#include "json.hpp"

class Game;
class Level;

using json = nlohmann::json;

class Zombie : public sf::CircleShape
{
    public:
        Zombie(Game* game, sf::Vector2f pos, int life = 10, int damage = 1, float damageInterval = 1.5f, float speed = 60.f, int value = 4, float radius = 16.f);
        Zombie(Game* game, nlohmann::basic_json<>& json);
        virtual ~Zombie();

        void init(sf::Vector2f pos);
        void update(float delta);
        void drawHealthbar(sf::RenderTarget& target) const;

        static sf::Vector2f target;

        inline void dealDamage(int damage) { life -= damage; if (life <= 0) dead = true; };
        inline bool mustBeRemoved() const { return deadReady && deathClock.getElapsedTime().asSeconds() >= 1.f; };
        inline bool isDead() const { return dead; };

    private:
        Game* game;
        Level* level;

        float radius;
        float speed;
        int damage;
        float damageInterval;
        int life;
        int maxLife;

        int value;

        bool dead = false;
        bool deadReady = false;
        sf::Clock deathClock;

        sf::Clock damageClock;

        sf::Vector2f vectorTowardTarget;

        sf::RectangleShape healthBar, healthBarBase;
};

struct TargetInfo
{
    TargetInfo(Zombie* z, float d) { zombie = z; squaredDistance = d; };
    inline bool isValid() const { return !!zombie; };
    Zombie* zombie;
    float squaredDistance;
};

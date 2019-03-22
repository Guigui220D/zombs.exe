#include "Zombie.h"

#include <cmath>

#include "Level.h"
#include "Building.h"
#include "Game.h"

sf::Vector2f Zombie::target;

Zombie::Zombie(Game* game, sf::Vector2f pos, int life, int damage, float damageInterval, float speed, int value, float radius) :
    game(game),
    level(game->getLevel()),
    speed(speed),
    damage(damage),
    damageInterval(damageInterval),
    life(life),
    maxLife(life),
    value(value)
{
    setRadius(radius);
    setOrigin(sf::Vector2f(radius, radius));
    setOutlineColor(sf::Color::Black);
    setOutlineThickness(4.0f);
    setFillColor(sf::Color(50, 100, 50));

    healthBarBase.setSize(sf::Vector2f(30.f, 5.f));
    healthBarBase.setFillColor(sf::Color::Black);

    healthBar.setSize(sf::Vector2f(30.f, 5.f));
    healthBar.setFillColor(sf::Color::Yellow);

    init(pos);
}

Zombie::Zombie(Game* game, nlohmann::basic_json<>& json) :
    game(game),
    level(game->getLevel()),
    speed(60.f),
    damage(1),
    damageInterval(1.5f),
    life(10),
    maxLife(10),
    value(4)
{
    auto jend = json.end();
    auto jlife = json.find("life");
    auto jspeed = json.find("speed");
    auto jdamage = json.find("damage");
    auto jdamage_interval = json.find("damage_interval");
    auto jradius = json.find("radius");
    auto jvalue = json.find("value");

    if (jlife != jend)
        maxLife = life = jlife->get<int>();

    if (jspeed != jend)
        speed = jspeed->get<float>();

    if (jdamage != jend)
        damage = jdamage->get<int>();

    if (jdamage_interval != jend)
        damageInterval = jdamage_interval->get<float>();

    float radius = 16.f;
    if (jradius != jend)
        radius = jradius->get<float>();
    setRadius(radius);

    if (jvalue != jend)
        value = jvalue->get<int>();

    setOrigin(sf::Vector2f(radius, radius));
    setOutlineColor(sf::Color::Black);
    setOutlineThickness(4.0f);
    setFillColor(sf::Color(50, 100, 50));

    healthBarBase.setSize(sf::Vector2f(30.f, 5.f));
    healthBarBase.setFillColor(sf::Color::Black);

    healthBar.setSize(sf::Vector2f(30.f, 5.f));
    healthBar.setFillColor(sf::Color::Yellow);
}

Zombie::~Zombie()
{
    //dtor
}

void Zombie::init(sf::Vector2f pos)
{
    setPosition(pos);
    vectorTowardTarget =  target - pos;
    float lenght = sqrt(vectorTowardTarget.x * vectorTowardTarget.x + vectorTowardTarget.y * vectorTowardTarget.y);
    vectorTowardTarget = sf::Vector2f(vectorTowardTarget.x / lenght, vectorTowardTarget.y / lenght);
}

void Zombie::update(float delta)
{
    if (!dead)
    {
        healthBarBase.setPosition(getPosition() - healthBarBase.getSize() / 2.f);
        healthBar.setPosition(healthBarBase.getPosition());
        healthBar.setSize(sf::Vector2f(30.f * ((float)life / maxLife), 5.f));

        sf::Vector2f pos = getPosition();
        setPosition(pos + vectorTowardTarget * delta * speed);
        sf::Vector2f newpos = getPosition();
        if (newpos.x > 100.f && newpos.y > 100.f && newpos.x < 32.f * 24.f + 100.f && newpos.y < 32.f * 24.f + 100.f)
        {
            sf::Vector2i gridpos((pos.x - 100.f) / 32.f, (pos.y - 100.f) / 32.f);
            Building* building = level->get(gridpos);
            if (building)
            {
                setPosition(pos);
                if (damageClock.getElapsedTime().asSeconds() >= damageInterval)
                {
                    damageClock.restart();
                    building->dealDamage(damage);
                }
            }
        }
    }
    else
    {
        if (!deadReady)
        {
            setFillColor(sf::Color::Black);
            setOutlineColor(sf::Color::Black);
            dead = true;
            deathClock.restart();
            game->money += value;
            deadReady = true;
        }
    }
}

void Zombie::drawHealthbar(sf::RenderTarget& target) const
{
    if (life == maxLife)
        return;
    target.draw(healthBarBase);
    if (life <= 0)
        return;
    target.draw(healthBar);
}

#include "Building.h"

#include <assert.h>
#include <cmath>

#include "Game.h"

const uint32_t Building::UPGRADE_COLORS[MAX_UPGRADE] { 0xFFFF00FF, 0x00FF00FF, 0x00FFFFFF, 0xFF00FFFF, 0xFF0000FF };
const int Building::LIFE_VALUES[MAX_UPGRADE] { 8, 10, 13, 15, 17 };

Building::Building(Game* game, sf::Vector2i pos, sf::Vector2i size, sf::Texture* colorBase, sf::Texture* base, int upgradeCost, int lifeMultiplyer) :
    level(1),
    upgradeCostMultiplyer(upgradeCost),
    game(game),
    pos(pos),
    size(size),
    lifeMultiplyer(lifeMultiplyer),
    maxLife(lifeMultiplyer * LIFE_VALUES[level - 1])
{
    assert(size.x > 0 && size.y > 0);
    assert(maxLife > 0);

    regen();

    colorBaseSprite.setTexture(*colorBase);
    colorBaseSprite.scale(4.f, 4.f);
    colorBaseSprite.setPosition(sf::Vector2f(32.f * pos.x + 100.f, 32.f * pos.y + 100.f));
    colorBaseSprite.setColor(sf::Color(UPGRADE_COLORS[level - 1]));

    baseSprite.setTexture(*base);
    baseSprite.scale(4.f, 4.f);
    baseSprite.setPosition(sf::Vector2f(32.f * pos.x + 100.f, 32.f * pos.y + 100.f));
    baseSprite.setColor(sf::Color::White);

    healthBarBase.setSize(sf::Vector2f(30.f, 5.f));
    healthBarBase.setPosition(sf::Vector2f(32.f * pos.x + 100.f + 1.f + (size.x - 1) * 16.f, 32.f * pos.y + 100.f + 26.f  + (size.y - 1) * 32.f));
    healthBarBase.setFillColor(sf::Color::Black);

    healthBar.setSize(sf::Vector2f(30.f, 5.f));
    healthBar.setPosition(sf::Vector2f(32.f * pos.x + 100.f + 1.f + (size.x - 1) * 16.f, 32.f * pos.y + 100.f + 26.f + (size.y - 1) * 32.f));
    healthBar.setFillColor(sf::Color::Green);

    center = baseSprite.getPosition() + sf::Vector2f(16.f * size.x, 16.f * size.y);
}

Building::~Building()
{
    //dtor
}

void Building::baseDraw(sf::RenderTarget& target) const
{
    target.draw(colorBaseSprite);
    target.draw(baseSprite);
}

void Building::drawHealthbar(sf::RenderTarget& target) const
{
    if (life == maxLife)
        return;
    target.draw(healthBarBase);
    if (life < 0)
        return;
    target.draw(healthBar);
}

void Building::drawMore(sf::RenderTarget& target) const {}

void Building::update(float delta)
{
    updateLife();
}

void Building::updateLife()
{
    if (dead)
        return;
    healthBar.setSize(sf::Vector2f(30.f * ((float)life / maxLife), 5.f));
    if (life <= 0)
    {
        dead = true;
        setColor(sf::Color::Black);
        game->getLevel()->removeBuilding(this);
    }
}

bool Building::canBeUpgraded() const
{
     return (level < MAX_UPGRADE && game->money >= level * upgradeCostMultiplyer && (this == game->getTreasure() || level < game->getTreasure()->getLevel()));
}

bool Building::upgrade()
{
    if (canBeUpgraded())
    {
        game->money -= level * upgradeCostMultiplyer;
        level++;
        colorBaseSprite.setColor(sf::Color(UPGRADE_COLORS[level - 1]));
        maxLife = lifeMultiplyer * LIFE_VALUES[level - 1];
        regen();
        return true;
    }
    else
        return false;
}

float Building::getAngularPos(sf::Vector2f target) const
{
    sf::Vector2f rpos = target - getCenter();
    float dist = sqrt(rpos.x * rpos.x + rpos.y * rpos.y);
    float xdist = rpos.x / dist;
    float pangl = toDegrees(acos(xdist));
    if (rpos.y < 0)
    {
        pangl = -pangl;
    }
    return pangl;
}

float Building::toDegrees(float rads)
{
    return (rads / (2.f * 3.141592f)) * 360.f;
}

float Building::toRads(float degrees)
{
    return (degrees / 360.f) * (2.f * 3.141592f);
}


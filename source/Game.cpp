#include "Game.h"

#include <ctime>
#include <string>
#include <fstream>

const int Game::MAP_SQUARE_SIZE = 24;
const int Game::WINDOW_SQUARE_SIZE = MAP_SQUARE_SIZE * 32 + 200;

Game::Game() :
    window(sf::VideoMode(WINDOW_SQUARE_SIZE, WINDOW_SQUARE_SIZE), "Zombs dotexe remake", sf::Style::Close),
    level(sf::Vector2i(MAP_SQUARE_SIZE, MAP_SQUARE_SIZE))
{
    //Load waves from file
    {
        std::ifstream is("res/json/waves.json");
        json j;
        is >> j;
        auto jwaves = j["waves"];
        for (unsigned int i = 0; i < jwaves.size(); i++)
            waves.push_back(ZombieWave(this, jwaves[i]));
        waveCount = waves.size();
    }


    window.setFramerateLimit(60);

    lucon.loadFromFile("res/ttf/lucon.ttf");
    moneyText.setFont(lucon);
    moneyText.setString("MONEY");
    moneyText.setPosition(sf::Vector2f(10.f, 0.f));
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setOutlineColor(sf::Color::Black);
    moneyText.setOutlineThickness(2.f);

    gameOver.setFont(lucon);
    gameOver.setString("GAME OVER");
    gameOver.setFillColor(sf::Color::Red);
    gameOver.setOutlineColor(sf::Color::Yellow);
    gameOver.setOutlineThickness(2.f);
    gameOver.setCharacterSize(170);
    //gameOver.setPosition(gameOver.getGlo)

    upgradeCostText.setFont(lucon);
    upgradeCostText.setString("UPGRADE");
    upgradeCostText.setPosition(sf::Vector2f(10.f, 30.f));
    upgradeCostText.setFillColor(sf::Color::Green);
    upgradeCostText.setOutlineColor(sf::Color::Black);
    upgradeCostText.setOutlineThickness(2.f);

    deleteText.setFont(lucon);
    deleteText.setString("x : Delete (no refund)");
    deleteText.setPosition(sf::Vector2f(10.f, 60.f));
    deleteText.setFillColor(sf::Color::Red);
    deleteText.setOutlineColor(sf::Color::Black);
    deleteText.setOutlineThickness(2.f);

    nextWaveText.setFont(lucon);
    nextWaveText.setString("Next wave : 1 / " + std::to_string(waveCount));
    nextWaveText.setPosition(sf::Vector2f(10.f, -100.f + window.getSize().y));
    nextWaveText.setRotation(-90.f);
    nextWaveText.setFillColor(sf::Color::Cyan);
    nextWaveText.setOutlineColor(sf::Color::Black);
    nextWaveText.setOutlineThickness(2.f);
}

Game::~Game()
{
    //dtor
}

TargetInfo Game::getNearestZombie(sf::Vector2f point) const
{
    Zombie* ptr = nullptr;
    float mindist = 100000000.f;
    for (auto i = zombies.begin(); i < zombies.end(); i++)
    {
        if (i->isDead())
            continue;
        sf::Vector2f rpos = i->getPosition() - point;
        float dist = rpos.x * rpos.x + rpos.y * rpos.y;
        if (dist < mindist)
        {
            mindist = dist;
            ptr = const_cast<Zombie*>(&*i);
        }
    }
    return TargetInfo(ptr, mindist);
}

TargetInfo Game::getNearestZombie() const
{
    Zombie* ptr = nullptr;
    float mindist = 100000000.f;
    for (auto i = zombies.begin(); i < zombies.end(); i++)
    {
        if (i->isDead())
            continue;
        sf::Vector2f rpos = i->getPosition() - Zombie::target;
        float dist = rpos.x * rpos.x + rpos.y * rpos.y;
        if (dist < mindist)
        {
            mindist = dist;
            ptr = const_cast<Zombie*>(&*i);
        }
    }
    return TargetInfo(ptr, mindist);
}

sf::Vector2f Game::getRandomZombiePos()
{
    int rnd = rand() % WINDOW_SQUARE_SIZE;
    int side = rand() % 4;
    switch (side)
    {
    case 0:
        return sf::Vector2f(rnd, 0);
    case 1:
        return sf::Vector2f(0, rnd);
    case 2:
        return sf::Vector2f(rnd, WINDOW_SQUARE_SIZE);
    case 3:
        return sf::Vector2f(WINDOW_SQUARE_SIZE, rnd);
    default:
        return sf::Vector2f();
    }
}

void Game::init()
{
    srand(time(0));

    assert(Textures::loadTextures());

    treasure = new Treasure(this, sf::Vector2i(11, 11));
    buildings.push_back(std::unique_ptr<Treasure>(treasure));
    assert(level.placeBuilding(treasure));
    Zombie::target = sf::Vector2f(16.f * MAP_SQUARE_SIZE + 100.f, 16.f * MAP_SQUARE_SIZE + 100.f);

    buildableZone.setSize(sf::Vector2f(32.f * MAP_SQUARE_SIZE, 32.f * MAP_SQUARE_SIZE));
    buildableZone.setFillColor(sf::Color(0, 50, 0));
    buildableZone.setPosition(sf::Vector2f(100.f, 100.f));
    buildableZone.setOutlineColor(sf::Color::Black);
    buildableZone.setOutlineThickness(3.f);
}

void Game::run()
{
    sf::Clock clk;
    float delta;

    while (window.isOpen())
    {
        delta = clk.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i gridpos((event.mouseButton.x - 100) / 32, (event.mouseButton.y - 100) / 32);
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (level.testSpaceAvailability(gridpos, sf::Vector2i(2, 2)))
                    {
                        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
                        {
                            if (money >= 50)
                            {
                                Tower1* t = new Tower1(this, gridpos);
                                buildings.push_back(std::unique_ptr<Tower1>(t));
                                assert(level.placeBuilding(t));
                                money -= 50;
                            }
                        }
                        else
                        {
                            if (money >= 80)
                            {
                                Sniper* t = new Sniper(this, gridpos);
                                buildings.push_back(std::unique_ptr<Sniper>(t));
                                assert(level.placeBuilding(t));
                                money -= 80;
                            }
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (money >= 3 && level.testSpaceAvailability(gridpos, sf::Vector2i(1, 1)))
                    {
                        Wall* wall = new Wall(this, gridpos);
                        buildings.push_back(std::unique_ptr<Wall>(wall));
                        assert(level.placeBuilding(wall));
                        money -= 3;
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed)
            {
                //if (event.key.code == sf::Keyboard::Z)
                //    zombies.push_back(Zombie(this, getRandomZombiePos(), 3, 0));
                if (event.key.code == sf::Keyboard::E)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2i gridpos((mousePos.x - 100) / 32, (mousePos.y - 100) / 32);
                    if (level.get(gridpos))
                    {
                        level.get(gridpos)->upgrade();
                    }
                }
                if (event.key.code == sf::Keyboard::X)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2i gridpos((mousePos.x - 100) / 32, (mousePos.y - 100) / 32);
                    Building* b = level.get(gridpos);
                    if (b && b != treasure)
                    {
                        b->dealDamage(b->getLife());
                    }
                }
                if (event.key.code == sf::Keyboard::Space && !inWave)
                {
                    currentWave++;
                    if (currentWave >= 0 && currentWave < waveCount)
                    {
                        nextWaveText.setString("Running wave : " + std::to_string(currentWave + 1) + " / " + std::to_string(waveCount));
                        std::cout << waves.at(currentWave).comment << "\n";
                        inWave = true;
                        waveClock.restart();
                    }
                }
            }
        }
        if (!treasure->isDead())
        {
            //Remove dead buildings
            for (auto i = buildings.begin(); i < buildings.end(); )
            {
                if ((*i)->isDead())
                {
                    (*i).reset();
                    i = buildings.erase(i);
                }
                else i++;
            }
            for (auto i = zombies.begin(); i < zombies.end(); )
            {
                if (i->mustBeRemoved())
                {
                    i = zombies.erase(i);
                }
                else i++;
            }
            for (auto i = buildings.begin(); i < buildings.end(); i++)
                (*i)->update(delta);
            for (auto i = zombies.begin(); i < zombies.end(); i++)
                i->update(delta);
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2i gridpos((mousePos.x - 100) / 32, (mousePos.y - 100) / 32);
        Building* b = level.get(gridpos);
        if (b)
        {
            if (b->canBeUpgraded())
            {
                upgradeCostText.setFillColor(sf::Color::Green);
                upgradeCostText.setString("e : Upgrade (-$ " + std::to_string(level.get(gridpos)->getUpgradeCost()) + ")");
            }
            else if (!b->isLastLevel())
            {
                upgradeCostText.setFillColor(sf::Color(0, 127, 0));
                if (b->getLevel() == treasure->getLevel() && b != treasure)
                {
                    upgradeCostText.setString("You need to upgrade the treasure");
                }
                else
                    upgradeCostText.setString("Can be upgraded for " + std::to_string(level.get(gridpos)->getUpgradeCost()));
            }
            else
            {
                upgradeCostText.setFillColor(sf::Color::Cyan);
                upgradeCostText.setString("MAX LEVEL");
            }
        }
        moneyText.setString("$ " + std::to_string(money));

        if (inWave)
        {
            ZombieWave& current = waves.at(currentWave);
            for (auto i = current.elements.begin(); i != current.elements.end(); i++)
            {
                if (waveClock.getElapsedTime().asSeconds() >= (i->current * i->t + i->start_t) && i->current < i->count)
                {
                    i->current++;
                    Zombie z = i->clone;
                    z.init(getRandomZombiePos());
                    zombies.push_back(z);
                }
            }
            if (waveClock.getElapsedTime().asSeconds() >= current.getDuration() && zombies.size() == 0)
            {
                inWave = false;
                nextWaveText.setString("Next wave : " + std::to_string(currentWave + 2) + " / " + std::to_string(waveCount));
            }
        }

        window.clear(sf::Color(200, 200, 127));
        window.draw(buildableZone);
        for (auto i = buildings.begin(); i < buildings.end(); i++)
        {
            (*i)->baseDraw(window);
            (*i)->drawMore(window);
        }
        for (auto i = zombies.begin(); i < zombies.end(); i++)
        {
            window.draw(*i);
            i->drawHealthbar(window);
        }
        for (auto i = buildings.begin(); i < buildings.end(); i++)
            (*i)->drawHealthbar(window);

        if (b)
        {
            window.draw(upgradeCostText);
            if (b != treasure)
                window.draw(deleteText);
        }
        window.draw(moneyText);
        window.draw(nextWaveText);
        if (treasure->isDead())
            window.draw(gameOver);
        window.display();
    }
}

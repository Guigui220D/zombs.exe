#include "ZombieWaveElement.h"

ZombieWaveElement::ZombieWaveElement(Game* game, nlohmann::basic_json<>& json) :
    clone(game, *json.find("zombie"))
{
    count = json.find("count")->get<int>();
    start_t = json.find("start")->get<float>();
    t = json.find("interval")->get<float>();
}





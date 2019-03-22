#include "ZombieWave.h"

ZombieWave::ZombieWave(Game* game, nlohmann::basic_json<>& json)
{
    auto groups = json["groups"];
    for (unsigned int i = 0; i < groups.size(); i++)
        elements.push_back(ZombieWaveElement(game, groups[i]));
    comment = "";
    auto jcom = json.find("comment");
    if (jcom != json.end())
        comment = jcom->get<std::string>();
}

float ZombieWave::getDuration() const
{
    float duration = 0.f;
    for (ZombieWaveElement w : elements)
    {
        float wd = w.count * w.t + w.start_t;
        duration = wd > duration ? wd : duration;
    }
    return duration + 1.f;
}

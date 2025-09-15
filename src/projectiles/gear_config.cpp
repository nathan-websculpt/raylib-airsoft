#include "../../include/projectiles/gear_config.h"

inline void from_json(const nlohmann::json& j, GearConfig& cfg) {
    cfg.projectileSpeed   = j.at("projectileSpeed").get<float>();
    cfg.gravity           = j.at("gravity").get<float>();
    cfg.muzzleOffsetRight = j.at("muzzleOffsetRight").get<float>();
    cfg.muzzleOffsetUp    = j.at("muzzleOffsetUp").get<float>();
    cfg.fireCooldown      = j.at("fireCooldown").get<float>();
    cfg.projectileRadius  = j.at("projectileRadius").get<float>();
    cfg.canBounce         = j.at("canBounce").get<bool>();

    std::vector<int> color = j.at("projectileColor").get<std::vector<int>>();
    if (color.size() == 4)
        cfg.projectileColor = { (unsigned char)color[0], (unsigned char)color[1],
                                (unsigned char)color[2], (unsigned char)color[3] };
    else
        cfg.projectileColor = BLUE;
}

std::map<std::string, GearConfig> LoadGearConfigs(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Could not open " + filename);
    nlohmann::json j;
    file >> j;
    std::map<std::string, GearConfig> configs;
    for (auto& [name, cfg] : j.items()) {
        configs[name] = cfg.get<GearConfig>();
    }
    return configs;
}
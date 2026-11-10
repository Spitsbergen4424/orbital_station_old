#pragma once

#include "modules/Module.h"

class ResearchModule : public Module {
public:
    ResearchModule(const std::string& name, int durability, int energyConsumption,
                   int capacity, int efficiency);
};

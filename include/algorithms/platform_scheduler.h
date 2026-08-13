#pragma once
#include <vector>
#include "train.h"

class platform_scheduler
{
public:
    std::vector<train> schedule(std::vector<train> trains);
};
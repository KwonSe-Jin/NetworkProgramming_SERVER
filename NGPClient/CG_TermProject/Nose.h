#pragma once
#include "Animals.h"
class Nose : public Animals
{
    int monster;
public:
    Nose();
    Nose(glm::vec3 Color, int monster);
    ~Nose();

    void update() override;
    void BindVAO() override;
    void DrawArrays() override;

};

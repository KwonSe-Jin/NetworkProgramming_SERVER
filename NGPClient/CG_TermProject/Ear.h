#pragma once
#include "Animals.h"
class Ear : public Animals
{
    int i;
    int monster;
public:
    Ear();
    Ear(glm::vec3 Color, int i, int monster);
    ~Ear();

    void update() override;
    void BindVAO() override;
    void DrawArrays() override;

};

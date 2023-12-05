#pragma once
#include "Animals.h"
class Body : public Animals
{
    int monster;
public:
    Body();
    Body(glm::vec3 Color, int monster);
    ~Body();

    void update() override;
    void BindVAO() override;
    void DrawArrays() override;

};


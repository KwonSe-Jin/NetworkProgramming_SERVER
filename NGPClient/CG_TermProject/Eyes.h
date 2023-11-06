#pragma once
#include "Animals.h"
class Eyes : public Animals
{
    int i;
    int monster;

public:
    Eyes();
    Eyes(glm::vec3 Color, int i, int monster);
    ~Eyes();

    void update() override;
    void BindVAO() override;
    void DrawArrays() override;

};

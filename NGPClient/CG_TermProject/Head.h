#pragma once
#include "Animals.h"


class Head : public Animals
{
    int monster;
public:
    Head();
    Head(glm::vec3 Color, int monster);
    ~Head();
    
    void update() override;
    void BindVAO() override;
    void DrawArrays() override;

};


#pragma once
#include "Animals.h"
class Beard : public Animals
{
    int i;
    int monster;
    int pos;

public:
    Beard();
    Beard(glm::vec3 Color, int i,int pos, int monster);
    ~Beard();
    
    void update() override;
    void BindVAO() override;
    void DrawArrays() override;

};

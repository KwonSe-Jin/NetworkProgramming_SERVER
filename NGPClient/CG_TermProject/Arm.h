#pragma once
#include "Animals.h"
#include "CatAttack.h"
#include "Hero.h"

class Arm : public Animals
{
private:
    int i;
    int monster;
    float rotate;
    bool isrotDirec;
public:
    Arm();
    Arm(glm::vec3 Color, int i, int monster);    //i´Â ¹æÇâ monster 1 --> Cat // 2 --> Dog // 3--> Bear 
    ~Arm();

    void update() override;
    void BindVAO() override;
    void DrawArrays() override;

};


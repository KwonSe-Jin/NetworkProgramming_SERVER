#include "Draw.h"
#include "DogAttack.h"
#include "BearAttack.h"
#include <algorithm>
#include <ranges>


//전역 변수
GLuint shaderID;

glm::vec3 TermGunDir;
glm::vec3 cameraPos;

bool RestartPress = false;
Sound playSound;

void draw();
void AnimalCollideCat();
void AnimalCollideDog();
void BulletCollideCat();
void BulletCollideDog();

void CatAndRoomCollision();
void DogAndRoomCollision();
void Bearroomtest();

bool isCollideCatroom(Cat r1, Room r2);
bool isCollideDogroom(Dog r1, Room r2);
bool isCollideBearroom(Bear r1, Room r2);
bool HeroVSRoom(Hero r1, Room r2);
void HeroVSDog();
void HeroVSBear();
void HeroVSCat();

bool isCollideDog(Dog r1, Gun r2);
bool isCollide2D(Cat r1, Gun r2);
bool isCollideBear(Bear r1, Gun r2);


int catdead{};
int dogdead{};
int beardead{};

int HerogetHP = 10;

float lightPosX = 0.0;
float lightPosY = 30.0;
float lightPosZ = 0.0;

int lightcolorN = 0;
float lightRot;

bool isParticle = false;
bool isBullet = false;
bool BearLife = true;

float openingCat = 0;
float openingDog = 0;
int i = 0;

bool catopen = true;
bool dogopen = true;

bool toggle1 = true;
bool toggle2 = true;


/// <summary>
/// 주인공
/// </summary>
float HeroMovY = 0;


float MovX = 0;
float MovZ = 0;


bool catlive = false;
bool doglive = false;
bool bearlive = false;
bool herodead = false;

float HeroLocationX = 0;
float HeroLocationZ = 0;
CatAttack catattack[AnimCnt];
DogAttack dogattack[AnimCnt];
BearAttack bearattack;
bool jumpUp = true;
int global_ID = -1;

void Restartinit();
vector<Gun*> gun;

int CatCnt = 0;
int DogCnt = 0;


int ball_count = 0;
int BulletLimit;
bool is_ballfire = false;
int balltimer = 0;
int RestartCnt = 0;



vector<Particle*> particle{ new Particle,new Particle, new Particle, new Particle, new Particle
                  ,new Particle, new Particle,new Particle,new Particle,new Particle,
                   new Particle,new Particle, new Particle, new Particle, new Particle
                  ,new Particle, new Particle,new Particle,new Particle,new Particle,new Particle,new Particle, new Particle, new Particle, new Particle
                  ,new Particle, new Particle,new Particle,new Particle,new Particle,
                   new Particle,new Particle, new Particle, new Particle, new Particle
                  ,new Particle, new Particle,new Particle,new Particle,new Particle };

vector<Cat*> cats{ new Cat, new Cat, new Cat, new Cat, new Cat, new Cat };
vector<Dog*> dogs{ new Dog, new Dog, new Dog, new Dog, new Dog, new Dog };
Bear bear;

Hero h0(0);
Hero h1(1);
Hero h2(2);

Hero hero[3] = { h0,h1,h2 };


World world{};
Tree tree[400];
Grass grass[600];
Crown crown{};
CastleObj castle{};
Room catRoom{ 0 };
Room dogRoom{ 1 };
Room bearRoom{ 2 };
Tunnel Dogopen{ 2 };
DoorL Dogleft{ 2 };
DoorR Dogright{ 2 };

Tunnel Catopen{ 1 };
DoorL Catleft{ 1 };
DoorR Catright{ 1 };

struct Score
{
    int id;
    int score;
    string name;
};

float CatEndPosX;
float CatEndPosZ;

int gun_size;

vector <Score> scorevec;


bool ScoreOnce = true;
//NetworkManager networkManager("127.0.0.1", 7777);

//random_device rd;
//default_random_engine dre(rd());
//uniform_real_distribution<float> urd{ 0, 255 };

void makeInfo(SC_PLAYER_PACKET* p)
{
    hero[p->player_id].setInfo(p);
    if (hero[global_ID].ready == true && isR == true) {
        cout << "================" << endl;
        cout << "My id : " << global_ID << endl;

        for (int i = 0; i < 3; i++)
        {
            if (i != global_ID) {
                if (hero[i].ready == true)
                    cout << "player" << i << ": ready" << endl;
            }
        }
        cout << "================" << endl;
        //this_thread::sleep_for(0.5ms);
    }
    isR = false;

}

void animalInfo(SC_MONSTER_PACKET* p)
{
    // to_do 일단 캣으로!!
    if (p->animal_type == CAT)
    {
        cats[p->Monster_id]->catinfo(p);
        cats[p->Monster_id]->update();
    }
    else if (p->animal_type == DOG)
    {
        dogs[p->Monster_id]->doginfo(p);
        dogs[p->Monster_id]->update();
    }
    else if (p->animal_type == BEAR)
    {
        bear.bearinfo(p);
        bear.update();
    }

}

void restartCnt(SC_PLAYER_PACKET* p) {
    if(p->restart_cnt!=0)
        RestartCnt =  p->restart_cnt;
    cout << "RestartCnt== " << p->restart_cnt << endl;
}

void gun_clear()
{
    gun.clear();
}


void bulletInfo(SC_BULLET_PACKET* p)
{
    if (p->size > gun.size())
        gun.push_back(new Gun{ p->dirx,p->diry, p->dirz });
    //cout << gun.size() << endl;
    gun[p->id]->setInfo(p);
}


void get_vangleandStatus(float* x, float* y, bool* status, float* dirx, float* diry, float* dirz, string * name)
{
    hero[global_ID].get_Info(x, y, status, dirx, diry, dirz, name);
}

int t_count;

GLvoid drawScene()
{

    GLuint SelectColor = glGetUniformLocation(shaderID, "SelectColor");
    glUniform1i(SelectColor, 1);

    if (hero[global_ID].lightColorR < 0.35)
        glClearColor(0.f, 0.f, 0.f, 1.0f);
    else
        glClearColor(1.0, 1.0, 1.0, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DITHER);
    //glEnable(GL_CULL_FACE);  
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);

    //glm::mat4 projection;

    hero[global_ID].cameraProjection();
    hero[global_ID].camera();
    draw();
    hero[global_ID].DrawHP();
    hero[global_ID].cameraProjection2();
    //hero.TopView();
    //draw();
    glutSwapBuffers();
};


void draw() {
    if (RestartPress && RestartCnt == 3) {
        Restartinit();
        RestartPress = false;
    }

    if (hero[global_ID].HP <= 0) {
        herodead = true;
    }

    ///재시작 !! 
 

    if (beardead) {
        hero[global_ID].lightColorR = 1.0f;
        hero[global_ID].lightColorG = 1.0f;
        hero[global_ID].lightColorB = 1.0f;
    }
    if (herodead) {
        hero[global_ID].lightColorR = 0.5f;
        hero[global_ID].lightColorG = 0.5f;
        hero[global_ID].lightColorB = 0.5f;

    }

    unsigned int lightPosLocation = glGetUniformLocation(shaderID, "lightPos");      //--- lightPos 
    glm::vec4 tempv(lightPosX, lightPosY, lightPosZ, 1);
    glm::mat4 Lightrotate = glm::rotate(glm::mat4(1.f), glm::radians(lightRot), glm::vec3(0, 1, 0));
    tempv = Lightrotate * tempv;
    glUniform3f(lightPosLocation, tempv.x, tempv.y, tempv.z);
    unsigned int lightColorLocation = glGetUniformLocation(shaderID, "lightColor");   //--- lightColor 
    glUniform3f(lightColorLocation, hero[global_ID].lightColorR, hero[global_ID].lightColorG, hero[global_ID].lightColorB);
    unsigned int aColor = glGetUniformLocation(shaderID, "objectColor");   //--- object Color
    glUniform4f(aColor, 1, 1., 1., 1.);

    world.Draw();

    glEnable(GL_BLEND); //투명 객체 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    for (int i = 0; i < 400; ++i) {
        tree[i].Draw();
        tree[i].Update();
    }


    for (int i = 0; i < 600; ++i) {
        grass[i].Draw();
        grass[i].Update();
    }

    glDisable(GL_BLEND);


    if (catdead == 6) {
        catopen = true;
        dogopen = false;
    }


    if (dogdead == 6) {
        catopen = false;
        dogopen = true;
    }


    catRoom.Draw();
    catRoom.Update();
    dogRoom.Draw();
    dogRoom.Update();
    bearRoom.Draw();
    bearRoom.Update();

    castle.Draw();
    castle.Update();

    Catleft.Draw();
    Catleft.Update();
    Catright.Draw();
    Catright.Update();

    Dogleft.Draw();
    Dogleft.Update();
    Dogright.Draw();
    Dogright.Update();


    GLuint selectColorLocation = glGetUniformLocation(shaderID, "selectColor");
    glUniform1i(selectColorLocation, 0);

    Dogopen.Draw();
    Dogopen.Update();
    Catopen.Draw();
    Catopen.Update();

    crown.Draw();
    crown.Update();




    int c_dead = 0;

    for (int i = 0; i < cats.size(); ++i) {
        if (cats[i]->HP > 0)
        {
            cats[i]->draw();
        }
        else {
            ++c_dead;
        }
        if (c_dead == 6) {
            catdead = 6;
            if (toggle1) {
                cout << "toggle1 == " << toggle1 << endl;

                gun.erase(gun.begin(), gun.end());
                toggle1 = !toggle1;

            }
            c_dead++;
        }
    }
    int d_dead = 0;
    for (int i = 0; i < dogs.size(); ++i) {
        if (dogs[i]->HP > 0) {
            dogs[i]->draw();
        }
        else {
            ++d_dead;
        }
        if (d_dead == 6) {
            dogdead = 6;
            if (toggle2) {
                cout << "toggle2 == " << toggle2 << endl;

                gun.erase(gun.begin(), gun.end());
                toggle2 = !toggle2;
            }
            d_dead++;
        }
    }

    if (BearLife) {
        if (bear.HP > 0)
            bear.draw();
    }

    if (bear.HP <= 0) {
        beardead = true;
    }

    for (int i = 0; i < 3; ++i) // to_do 두명일 때는?!?!?!!
    {
        hero[i].Update();
    }


    hero[global_ID].Update();
    // to_do 내가 죽었는데 다른 사람 플레이어가 안보임! 
    for (int i = 0; i < 3; ++i) // to_do 두명일 때는?!?!?!!
    {
        if (i != global_ID && hero[i].Hero_ID != -1 && hero[i].status) {
            hero[i].Draw(); //나 자신은 안 그리도록 코드를 짰음 
        }
    }



    for (Gun*& gunbullet : gun) {
        gunbullet->Update();
        if (gunbullet->status)
            gunbullet->Draw();

    }

    glBindVertexArray(HeroHPVAO);
    aColor = glGetUniformLocation(shaderID, "objectColor");
    glUniform4f(aColor, 1.0, 0.0, 0., 1);

    if (beardead && ScoreOnce) {
        for (int i = 0; i < 3; ++i)
        {
            int temp;
            if (hero[i].InfoHP() - 2 == 0)
                temp = 180;
            else if (hero[i].InfoHP() - 2 == 1)
                temp = 190;
            else if (hero[i].InfoHP() - 2 == 2)
                temp = 200;
            else if (hero[i].InfoHP() - 2 == 3)
                temp = 210;
            else if (hero[i].InfoHP() - 2 == 4)
                temp = 220;
            else if (hero[i].InfoHP() - 2 == 5)
                temp = 230;
            else if (hero[i].InfoHP() - 2 == 6)
                temp = 240;
            else if (hero[i].InfoHP() - 2 == 7)
                temp = 250;
            else if (hero[i].InfoHP() - 2 == 8)
                temp = 260;
            else
                temp = 0;
            Score s = { i,temp, hero[i].nickname };
            scorevec.push_back(s);
        }
        std::sort(scorevec.begin(), scorevec.end(), [](const Score& a, const Score& b) {
            return a.score > b.score;
            });

        for (int i = 0; i < 3; ++i)
        {
            if (i == 0)
                cout << "================" << endl;
            cout << "Rank" << i + 1 << " : " << scorevec[i].name << " player " << scorevec[i].id << endl;
            cout << "Score : " << scorevec[i].score << endl;
            if (i == 2)
                cout << "================" << endl;
        }


        ScoreOnce = false;
    }
}

void Restartinit()
{
    RestartCnt = 0;
    CatEndPosX=0;
    CatEndPosZ=0;

    gun_size=0;
    ScoreOnce = true;

    //vector <Score> scorevec;
    scorevec.erase(scorevec.begin(), scorevec.end());

    //cats.erase(cats.begin(), cats.end());
    //dogs.erase(dogs.begin(), dogs.end());

    //for (int i = 0; i < 6; ++i) {
    //    cats.push_back(new Cat);
    //    dogs.push_back(new Dog);
    //}
    for (int i = 0; i < 6; ++i) {
        cats[i]->initCat();
        dogs[i]->initDog();
    }
    bear.InitBear();

    for (int i=0; i<3 ; ++i)
        hero[i].initHero();
    herodead = false;

    ball_count = 0;
    BulletLimit;
    is_ballfire = false;
    balltimer = 0;
    playSound.stopBGM();
    TermGunDir = { 0,0,0 };
    cameraPos = { 0,0,0 };
    catdead = 0;
    dogdead = 0;
    beardead = false;
    HerogetHP = 10;
    lightPosX = 0.0;
    lightPosY = 30.0;
    lightPosZ = 0.0;
    lightcolorN = 0;
    lightRot = 0;
    isParticle = false;
    isBullet = false;
    openingCat = 0;
    openingDog = 0;
    i = 0;
    HeroMovY = 0;
    DogCnt = 0;
    CatCnt = 0;
    MovX = 0;
    MovZ = 0;
    toggle1 = true;
    toggle2 = true;

    // cat 초기화 
    //for (int i = 0; i < cats.size(); ++i) {
    //    cats[i]->initCat();
    //    cats[i]->Index = CatCnt;
    //    ++CatCnt;
    //}

    //if (cats.size() != 6)
    //{
    //    while (cats.size() < 6)
    //    {
    //        cats.emplace_back(new Cat());
    //    }

    //}
    //// 강아지 초기화 

    //for (int i = 0; i < dogs.size(); ++i) {
    //    dogs[i]->initDog();
    //    dogs[i]->Index = DogCnt;
    //    ++DogCnt;
    //}

    //if (dogs.size() != 6)
    //{
    //    while (dogs.size() < 6)
    //    {
    //        dogs.emplace_back(new Dog());
    //    }
    //}

    ////곰 초기화
    //bear.InitBear();

    gun.erase(gun.begin(), gun.end());

    BearLife = true;

    //주인공 초기화 
    catlive = false;
    doglive = false;
    bearlive = false;

    herodead = false;

    catopen = true;
    dogopen = true;

    HeroLocationX = 0;
    HeroLocationZ = 0;
    DogCnt = 0;
    CatCnt = 0;


    bearattack.initBearAttack();
    jumpUp = true;



    ///왕관 초기화
    crown.initCrown();



}
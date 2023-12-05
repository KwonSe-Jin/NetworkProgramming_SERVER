#pragma once
constexpr int SC_PLAYER = 1;
constexpr int SC_MONSTER = 2;
constexpr int SC_BULLET = 3;
constexpr int CS_PLAYER = 4;

enum AnimalNum
{
	CAT,
	DOG,
	BEAR,
};

#pragma pack(push,1)
struct SC_PLAYER_PACKET
{
	int packet_type;
	bool restart=false;
	int player_id;
	int player_hp;
	bool status = true;
	bool ready = false;
	string nickname;

	struct Player_pos {

		float x;
		float y;
		float z;

	}Player_pos;

	struct Player_light {
		float R;
		float G;
		float B;
	}Player_light;



};

struct SC_MONSTER_PACKET
{
	int packet_type;
	int animal_type;
	int Monster_id;
	int hp;
	float direction;
	float x;
	float y;
	float z;
};

struct SC_BULLET_PACKET
{
	int packet_type;
	int size;
	int id;
	bool status;

	float dirx;
	float diry;
	float dirz;
};

struct CS_PLAYER_PACKET
{
	int player_id;
	int player_hp;
	bool status = true;
	bool ready = false;
	string nickname;
	bool restart = false;


	struct Player_key {
		bool is_p = false;
		bool is_w = false;
		bool is_a = false;
		bool is_s = false;
		bool is_d = false;
		bool is_q = false;
		bool is_space = false;
		bool is_bullet = false;
		float dirx = 0;
		float diry = 0;
		float dirz = 0;

	}Player_key;


	struct camera {

		float VangleX;
		float VangleY;
	}camera;

};
#pragma pack(pop) 
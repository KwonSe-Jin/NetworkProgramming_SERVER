#pragma once
constexpr int SC_PLAYER = 1;
constexpr int SC_MONSTER = 2;
constexpr int SC_BULLET = 3;
constexpr int CS_PLAYER = 4;

enum AnimalNUM
{
	CAT,
	DOG,
	BEAR,
};


#pragma pack(push,1)
struct SC_PLAYER_PACKET
{
	int packet_type;
	int player_id;
	int player_hp;
	bool status = true;
	bool ready = false;

	struct Player_pos {

		float x;
		float y;
		float z;

	}Player_pos;

	struct Player_key {
		bool is_w;
		bool is_a;
		bool is_s;
		bool is_d;
		bool is_space;
		bool is_bullet;
		float dirx;
		float diry;
		float dirz;

	}Player_key;

};

struct SC_MONSTER_PACKET
{
	int packet_type;
	int animal_type;
	bool status;
	float direction;
	int count;
	float x;
	float y;
	float z;
};

struct SC_BULLET_PACKET
{
	int packet_type;
	int id;
	float x;
	float y;
	float z;
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

	struct Player_pos {

		float x;
		float y;
		float z;

	}Player_pos;

	struct Player_key {
		bool is_w;
		bool is_a;
		bool is_s;
		bool is_d;
		bool is_space;
		bool is_bullet;
		float dirx;
		float diry;
		float dirz;

	}Player_key;

};
#pragma pack(pop) 
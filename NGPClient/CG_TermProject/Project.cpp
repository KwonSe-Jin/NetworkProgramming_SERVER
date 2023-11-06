#include "NetworkManager.h"
#include "Header.h"
#include "Draw.h"
#include "Timer.h"
#include "keyboard.h"
#include "Motion.h"
#include "Mouse.h"
#include "Camera.h"
#include "KeyboardUP.h"
#include "Sound.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

vector<glm::vec3> vertex1; //박스
vector<glm::vec3> vertex2; // 구
vector<glm::vec3> vertex3; //피라미드
vector<glm::vec3> vertex4; //잔디
vector<glm::vec3> vertex5; //성 
vector<glm::vec3> vcolor5;
vector<glm::vec3> vertex6; //왕관

extern Sound playSound;

void makeCastleColor() {
	random_device sk;
	default_random_engine skdre(sk());
	uniform_real_distribution<float> skurd{0.f, 1.f};

	for (int i = 0; i < vertex5.size() / 2; ++i) {
		vcolor5.push_back(glm::vec3(skurd(skdre), skurd(skdre), skurd(skdre)));
	}
}
//랜덤

//random_device rd1;
//default_random_engine dre1(rd1());
//uniform_real_distribution<float> urd1{ 0.01f, 0.017f };

//gl에서 쓰는 함수 
void InitBuffer();
void InitTexture();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
bool make_vertexShaders();
bool make_fragmentShaders();
GLuint make_shaderProgram();
void timer(int value);
GLvoid Motion(int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
//창 크기
GLint width{ 1000 }, height{ 800 };

//VAO, VBO
GLuint VAO, VBO;
GLuint crownVAO, crownVBO;
GLuint sphereVAO, sphereVBO;
GLuint pyramidVAO, pyramidVBO;
GLuint HeroHPVAO, HeroHPVBO;
GLuint GrassVAO, GrassVBO;
GLuint castleVAO, castleVBO, castleColorVBO;
GLuint TreeTexture[3];
GLuint GrassTexture[3];
GLuint Texture[6];
GLuint ScoreTexture[9];
GLuint GameoverTexture;
GLuint RoomTexture[3];
GLuint DeadTexture[3];

GLuint DoorTexture[2];
GLuint CastleTexture;
GLuint CastleSideTexTure;
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

char* filetobuf(string file);
void ReadObj(string file, vector<glm::vec3>* vertex, vector<glm::vec3>* vcolor, vector<glm::ivec3>* face);

const glm::mat4 Unit = glm::mat4(1.0f);

float HeroHP[4][3]{
	{-0.1, 0.1, 0},
	{-0.1, -0.1, 0},
	{0.1, -0.1, 0},
	{0.1, 0.1, 0}
};


float textureVertex[36][2]{
	{1,0},	{0,1},	{0,0},	{1,0},	{1,1},	{0,1},

	{0,0},	{1,1},	{0,1},	{0,0},	{1,0},	{1,1},

	{0,1},	{1,0},	{1,1},	{0,1},	{0,0},	{1,0},

	{1,0},	{1,1},	{0,1},	{1,0},	{0,1},	{0,0},

	{0,0},	{1,0},	{1,1},	{0,0},	{1,1},	{0,1},

	{0,0},	{1,0},	{1,1},	{0,0},	{1,1},	{0,1}
};

GLuint crossVAO, crossVBO;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	NetworkManager networkManager("127.0.0.1", 7777);

	if (!networkManager.Connect()) {
		std::cerr << "Failed to connect to the server" << std::endl;
		return;
	}

	// Send data to the server
	std::string dataToSend;
	std::cin >> dataToSend;
	if (!networkManager.SendData(dataToSend)) {
		return;
	}

	// Receive data from the server
	std::string receivedData;
	if (networkManager.ReceiveData(receivedData)) {
		std::cout << "Received from the server: " << receivedData << std::endl;
	}
	//sndPlaySound(L"전투.wav", SND_ASYNC | SND_LOOP);
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Project");
	
	
	glewExperimental = GL_TRUE;
	glewInit();

	shaderID = make_shaderProgram(); //--- 세이더 프로그램 만들기
	glUseProgram(shaderID);

	InitBuffer();
	InitTexture();
	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	//glutMotionFunc(Motion);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);

	glutKeyboardUpFunc(KeyboardUP);
	glutPassiveMotionFunc(passiveMotion);
	glutMouseFunc(Mouse);
	glutSpecialFunc(SpecialKeyboardUP);
	glutTimerFunc(10, timer, 1);
	glutMainLoop();
	glutLeaveMainLoop();
}

GLvoid Reshape(int w, int h)
{
	width = w;
	height = h;


}


char* filetobuf(string file) {
	ifstream in(file);

	string* str_buf = new string;
	while (in) {
		string temp;
		getline(in, temp);
		str_buf->append(temp);
		str_buf->append("\n");
	}
	return const_cast<char*>(str_buf->c_str());
}



void ReadObj(string file, vector<glm::vec3>& vertexInfo)
{
	vector<glm::vec3> vertex;
	vector<glm::vec3> vNormal;

	vector<glm::ivec3> vFace;
	vector<glm::ivec3> vnFace;

	ifstream in(file);
	if (!in) {
		cout << "OBJ File NO Have" << endl;
		return;
	}

	while (in) {
		string temp;
		getline(in, temp);

		if (temp[0] == 'v' && temp[1] == ' ') {
			istringstream slice(temp);

			glm::vec3 vertemp;
			char tmpword;
			slice >> tmpword >> vertemp.x >> vertemp.y >> vertemp.z;

			vertex.push_back(vertemp);
		}

		else if (temp[0] == 'v' && temp[1] == 'n' && temp[2] == ' ') {
			istringstream slice(temp);

			glm::vec3 vertemp;
			string tmpword;
			slice >> tmpword >> vertemp.x >> vertemp.y >> vertemp.z;

			vNormal.push_back(vertemp);
		}

		else if (temp[0] == 'f' && temp[1] == ' ') {
			istringstream slice(temp);

			glm::ivec3 vfacetemp;
			glm::ivec3 vnfacetemp;
			for (int i = -1; i < 3; ++i) {
				string word;
				getline(slice, word, ' ');
				if (i == -1) continue;                  // f 읽을땐 건너뛴다
				if (word.find("/") != string::npos) {
					istringstream slash(word);
					string slashtmp;
					getline(slash, slashtmp, '/');

					vfacetemp[i] = atoi(slashtmp.c_str()) - 1;         //f 읽을땐 첫번째값만(v)   //배열인덱스 쓸거라 -1해줌

					getline(slash, slashtmp, '/');
					getline(slash, slashtmp, '/');
					vnfacetemp[i] = atoi(slashtmp.c_str()) - 1;
				}
				else {
					vfacetemp[i] = atoi(word.c_str()) - 1;         //f 읽을땐 첫번째값만(v)   //배열인덱스 쓸거라 -1해줌
				}
			}
			vFace.push_back(vfacetemp);
			vnFace.push_back(vnfacetemp);
		}
	}

	for (int i = 0; i < vFace.size(); ++i) {
		vertexInfo.push_back(vertex[vFace[i].x]);
		vertexInfo.push_back(vNormal[vnFace[i].x]);

		vertexInfo.push_back(vertex[vFace[i].y]);
		vertexInfo.push_back(vNormal[vnFace[i].y]);

		vertexInfo.push_back(vertex[vFace[i].z]);
		vertexInfo.push_back(vNormal[vnFace[i].z]);
	}

	
}

void InitBuffer()
{
	ReadObj("cube.obj",vertex1);
	//ReadObj("pyramid.obj", &pyramidvertex, &pyramidcolor, &pyramidface);


	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	//--- VAO 객체 생성 및 바인딩

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex1.size() * sizeof(glm::vec3), &vertex1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);      // 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	GLuint textureVBO;
	glGenBuffers(1, &textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertex), textureVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(2);

	///////////////////////////////////////////////////////////


	ReadObj("nsphere.obj", vertex2);

	glGenVertexArrays(1, &sphereVAO);

	glGenBuffers(1, &sphereVBO);

	glBindVertexArray(sphereVAO);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, vertex2.size() * sizeof(glm::vec3), &vertex2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);      // 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	///////////////////////////////////////////////////////////

	ReadObj("pyramid.obj", vertex3);

	glGenVertexArrays(1, &pyramidVAO);

	glGenBuffers(1, &pyramidVBO);

	glBindVertexArray(pyramidVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
	glBufferData(GL_ARRAY_BUFFER, vertex3.size() * sizeof(glm::vec3), &vertex3[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);      // 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	


	///////////////////////////////////////////////////////////
	ReadObj("castle.obj", vertex5);
	makeCastleColor();
	glGenVertexArrays(1, &castleVAO);

	glGenBuffers(1, &castleVBO);
	glGenBuffers(1, &castleColorVBO);

	glBindVertexArray(castleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, castleVBO);
	glBufferData(GL_ARRAY_BUFFER, vertex5.size() * sizeof(glm::vec3), &vertex5[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);      // 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, castleColorVBO);
	glBufferData(GL_ARRAY_BUFFER, vcolor5.size() * sizeof(glm::vec3), &vcolor5[0], GL_STATIC_DRAW);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(5);      // 버텍스 속성 배열을 사용하도록 한다.(5번 배열 활성화)

	
	/////////////////////////////////////////////////////////
	glGenVertexArrays(1, &HeroHPVAO);
	glGenBuffers(1, &HeroHPVBO);

	//// 정점, 색상 접근 규칙 만들기
	glBindVertexArray(HeroHPVAO);
	glBindBuffer(GL_ARRAY_BUFFER, HeroHPVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(HeroHP), HeroHP, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/////////////////////////////////////////////////////

	ReadObj("crown1.obj", vertex6);

	glGenVertexArrays(1, &crownVAO);

	glGenBuffers(1, &crownVBO);

	glBindVertexArray(crownVAO);

	glBindBuffer(GL_ARRAY_BUFFER, crownVBO);
	glBufferData(GL_ARRAY_BUFFER, vertex6.size() * sizeof(glm::vec3), &vertex6[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);      // 버텍스 속성 배열을 사용하도록 한다.(0번 배열 활성화)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
}


void InitTexture()
{
	glGenTextures(6, Texture);

	for (int i = 0; i < 6; ++i) {
		glBindTexture(GL_TEXTURE_2D, Texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int ImageWidth, ImageHeight, numberOfChannel;
		stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
		string filename;

		switch (i) {
		case 0: {
			filename = "옆면2.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}
		case 1: {
			filename = "옆면3.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}
		case 2: { //위 
			filename = "옆면3.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break; 
		}

		case 3: {
			filename = "옆면1.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}
		case 4: {//바닥
			filename = "바닥.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 4, ImageWidth, ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}
		case 5: { //3
			filename = "옆면4.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}
		}
		
	}

	glGenTextures(3, TreeTexture);
	for (int i = 0; i <3; ++i) {
		glBindTexture(GL_TEXTURE_2D, TreeTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int ImageWidth, ImageHeight, numberOfChannel;
		stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
		string filename;

		switch (i) {
		case 0:
			filename = "투명나무1.png";
			break;
		case 1:
			filename = "나무2.png";
			break;
		case 2:
			filename = "나무3.png";
			break;
		}
		GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, ImageWidth, ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	
	glGenTextures(9, ScoreTexture);
	for (int i = 0; i < 9; ++i) {
		glBindTexture(GL_TEXTURE_2D, ScoreTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int ImageWidth, ImageHeight, numberOfChannel;
		stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
		string filename;

		switch (i) {
		case 0:
			filename = "180.png";
			break;
		case 1:
			filename = "190.png";
			break;
		case 2:
			filename = "200.png";
			break;
		case 3:
			filename = "210.png";
			break;
		case 4:
			filename = "220.png";
			break;
		case 5:
			filename = "230.png";
			break;
		case 6:
			filename = "240.png";
			break;
		case 7:
			filename = "250.png";
			break;
		case 8:
			filename = "260.png";
			break;

		}
		GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glGenTextures(3, GrassTexture);
	for (int i = 0; i < 3; ++i) {
		glBindTexture(GL_TEXTURE_2D, GrassTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int ImageWidth, ImageHeight, numberOfChannel;
		stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
		string filename;

		switch (i) {
		case 0:
			filename = "잔디1.png";
			break;
		case 1:
			filename = "잔디2.png";
			break;
		case 2:
			filename = "잔디3.png";
			break;
		}
		GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, ImageWidth, ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glGenTextures(3, RoomTexture);
	for (int i = 0; i < 3; ++i) {
		glBindTexture(GL_TEXTURE_2D, RoomTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int ImageWidth, ImageHeight, numberOfChannel;
		//stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
		string filename;

		switch (i) {
		case 0:
			filename = "Catroom.png"; //회색 
			break;
		case 1:
			filename = "Dogroom.png"; //갈색 
			break;
		case 2:
			filename = "Bearroom.png"; //핑크 
			break;
		//case 3:
		//	filename = "Catroomdead.png"; //회색 
		//	break;
		//case 4:
		//	filename = "Dogroomdead.png"; //갈색 
		//	break;
		//case 5:
		//	filename = "Bearroomdead.png"; //핑크 
		//	break;
		}
		GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glGenTextures(3, DeadTexture);
	for (int i = 0; i < 3; ++i) {
		glBindTexture(GL_TEXTURE_2D, DeadTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int ImageWidth, ImageHeight, numberOfChannel;
		//stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
		string filename;

		switch (i) {
	
			case 0:
				filename = "Catroomdead.png"; //회색 
				break;
			case 1:
				filename = "Dogroomdead.png"; //갈색 
				break;
			case 2:
				filename = "Bearroomdead.png"; //핑크 
				break;
		}
		GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glGenTextures(1, &GameoverTexture);
	glBindTexture(GL_TEXTURE_2D, GameoverTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int ImageWidth, ImageHeight, numberOfChannel;
	stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
	string filename = "종료.PNG";
	GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	
	glGenTextures(2, DoorTexture);
	for (int i = 0; i < 2; ++i) {
		glBindTexture(GL_TEXTURE_2D, DoorTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int ImageWidth, ImageHeight, numberOfChannel;
		stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
		string filename;

		switch (i) {
		case 0:
			filename = "제발1.png";
			break;
		case 1:
			filename = "제발2.png";
			break;

		}
		GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, ImageWidth, ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

}


bool make_vertexShaders()
{
	GLchar* vertexSource;
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return false;
	}
}

//--- 프래그먼트 세이더 객체 만들기
bool make_fragmentShaders()
{
	GLchar* fragmentSource;
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return false;
	}
}

//--- 세이더 프로그램 만들기
GLuint make_shaderProgram()
{

	//--- 세이더 읽어와서 세이더 프로그램 만들기
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기

	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기3
	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		return false;
	}
	glUseProgram(ShaderProgramID);         //--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.
	return ShaderProgramID;

	//깃테스트용메모
}


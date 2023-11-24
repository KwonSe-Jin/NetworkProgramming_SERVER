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

vector<glm::vec3> vertex1; //�ڽ�
vector<glm::vec3> vertex2; // ��
vector<glm::vec3> vertex3; //�Ƕ�̵�
vector<glm::vec3> vertex4; //�ܵ�
vector<glm::vec3> vertex5; //�� 
vector<glm::vec3> vcolor5;
vector<glm::vec3> vertex6; //�հ�
NetworkManager networkManager;
extern Sound playSound;

void makeCastleColor() {
	random_device sk;
	default_random_engine skdre(sk());
	uniform_real_distribution<float> skurd{ 0.f, 1.f };

	for (int i = 0; i < vertex5.size() / 2; ++i) {
		vcolor5.push_back(glm::vec3(skurd(skdre), skurd(skdre), skurd(skdre)));
	}
}
//����

//random_device rd1;
//default_random_engine dre1(rd1());
//uniform_real_distribution<float> urd1{ 0.01f, 0.017f };

//gl���� ���� �Լ� 
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
void start(int argc, char* argv[]);

GLvoid Mouse(int button, int state, int x, int y);
//â ũ��
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
GLuint vertexShader; //--- ���ؽ� ���̴� ��ü
GLuint fragmentShader; //--- �����׸�Ʈ ���̴� ��ü

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

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{


	if (!networkManager.Connect()) {
		std::cerr << "Failed to connect to the server" << std::endl;
		return;
	}

	//// Receive data from the server
	//std::string receivedData;
	//if (networkManager.ReceiveData(receivedData)) {
	//	std::cout << "Received from the server: " << receivedData << std::endl;
	//}

	// Send data to the server
	std::string dataToSend;
	std::cin >> dataToSend;
	if (!networkManager.SendData(dataToSend)) {
		return;
	}


	std::thread networkThread([&]() {
		while (true) {
			networkManager.SendIdlePlayer();
			networkManager.recvData();
		}
		});



	start(argc, argv);
	// ��Ŷ�� �޾ƿ��� ������
	networkThread.detach();
}

void start(int argc, char* argv[])
{

	//sndPlaySound(L"����.wav", SND_ASYNC | SND_LOOP);
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Project");


	glewExperimental = GL_TRUE;
	glewInit();

	shaderID = make_shaderProgram(); //--- ���̴� ���α׷� �����
	glUseProgram(shaderID);

	InitBuffer();
	InitTexture();
	glutDisplayFunc(drawScene); //--- ��� �ݹ� �Լ�
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
				if (i == -1) continue;                  // f ������ �ǳʶڴ�
				if (word.find("/") != string::npos) {
					istringstream slash(word);
					string slashtmp;
					getline(slash, slashtmp, '/');

					vfacetemp[i] = atoi(slashtmp.c_str()) - 1;         //f ������ ù��°����(v)   //�迭�ε��� ���Ŷ� -1����

					getline(slash, slashtmp, '/');
					getline(slash, slashtmp, '/');
					vnfacetemp[i] = atoi(slashtmp.c_str()) - 1;
				}
				else {
					vfacetemp[i] = atoi(word.c_str()) - 1;         //f ������ ù��°����(v)   //�迭�ε��� ���Ŷ� -1����
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
	ReadObj("cube.obj", vertex1);
	//ReadObj("pyramid.obj", &pyramidvertex, &pyramidcolor, &pyramidface);


	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	//--- VAO ��ü ���� �� ���ε�

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex1.size() * sizeof(glm::vec3), &vertex1[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);      // ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	GLuint textureVBO;
	glGenBuffers(1, &textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertex), textureVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); //--- �ؽ�ó ��ǥ �Ӽ�
	glEnableVertexAttribArray(2);

	///////////////////////////////////////////////////////////


	ReadObj("nsphere.obj", vertex2);

	glGenVertexArrays(1, &sphereVAO);

	glGenBuffers(1, &sphereVBO);

	glBindVertexArray(sphereVAO);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, vertex2.size() * sizeof(glm::vec3), &vertex2[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);      // ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
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
	glEnableVertexAttribArray(0);      // ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
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
	glEnableVertexAttribArray(0);      // ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, castleColorVBO);
	glBufferData(GL_ARRAY_BUFFER, vcolor5.size() * sizeof(glm::vec3), &vcolor5[0], GL_STATIC_DRAW);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(5);      // ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(5�� �迭 Ȱ��ȭ)


	/////////////////////////////////////////////////////////
	glGenVertexArrays(1, &HeroHPVAO);
	glGenBuffers(1, &HeroHPVBO);

	//// ����, ���� ���� ��Ģ �����
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
	glEnableVertexAttribArray(0);      // ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�.(0�� �迭 Ȱ��ȭ)
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
		stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
		string filename;

		switch (i) {
		case 0: {
			filename = "����2.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}
		case 1: {
			filename = "����3.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}
		case 2: { //�� 
			filename = "����3.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}

		case 3: {
			filename = "����1.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}
		case 4: {//�ٴ�
			filename = "�ٴ�.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 4, ImageWidth, ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}
		case 5: { //3
			filename = "����4.png";
			GLubyte* data = stbi_load(filename.c_str(), &ImageWidth, &ImageHeight, &numberOfChannel, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			break;
		}
		}

	}

	glGenTextures(3, TreeTexture);
	for (int i = 0; i < 3; ++i) {
		glBindTexture(GL_TEXTURE_2D, TreeTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int ImageWidth, ImageHeight, numberOfChannel;
		stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
		string filename;

		switch (i) {
		case 0:
			filename = "��������1.png";
			break;
		case 1:
			filename = "����2.png";
			break;
		case 2:
			filename = "����3.png";
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
		stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
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
		stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
		string filename;

		switch (i) {
		case 0:
			filename = "�ܵ�1.png";
			break;
		case 1:
			filename = "�ܵ�2.png";
			break;
		case 2:
			filename = "�ܵ�3.png";
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
		//stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
		string filename;

		switch (i) {
		case 0:
			filename = "Catroom.png"; //ȸ�� 
			break;
		case 1:
			filename = "Dogroom.png"; //���� 
			break;
		case 2:
			filename = "Bearroom.png"; //��ũ 
			break;
			//case 3:
			//	filename = "Catroomdead.png"; //ȸ�� 
			//	break;
			//case 4:
			//	filename = "Dogroomdead.png"; //���� 
			//	break;
			//case 5:
			//	filename = "Bearroomdead.png"; //��ũ 
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
		//stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
		string filename;

		switch (i) {

		case 0:
			filename = "Catroomdead.png"; //ȸ�� 
			break;
		case 1:
			filename = "Dogroomdead.png"; //���� 
			break;
		case 2:
			filename = "Bearroomdead.png"; //��ũ 
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
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	string filename = "����.PNG";
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
		stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
		string filename;

		switch (i) {
		case 0:
			filename = "����1.png";
			break;
		case 1:
			filename = "����2.png";
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
	//--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
	//--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
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
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return false;
	}
}

//--- �����׸�Ʈ ���̴� ��ü �����
bool make_fragmentShaders()
{
	GLchar* fragmentSource;
	//--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
	fragmentSource = filetobuf("fragment.glsl"); // �����׼��̴� �о����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
		return false;
	}
}

//--- ���̴� ���α׷� �����
GLuint make_shaderProgram()
{

	//--- ���̴� �о�ͼ� ���̴� ���α׷� �����
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����

	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- ���̴� ���α׷� �����3
	glAttachShader(ShaderProgramID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(ShaderProgramID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
	glLinkProgram(ShaderProgramID); //--- ���̴� ���α׷� ��ũ�ϱ�
	glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program ���� ����\n" << errorLog << endl;
		return false;
	}
	glUseProgram(ShaderProgramID);         //--- ������� ���̴� ���α׷� ����ϱ�
	//--- ���� ���� ���̴����α׷� ���� �� �ְ�, �� �� �Ѱ��� ���α׷��� ����Ϸ���
	//--- glUseProgram �Լ��� ȣ���Ͽ� ��� �� Ư�� ���α׷��� �����Ѵ�.
	//--- ����ϱ� ������ ȣ���� �� �ִ�.
	return ShaderProgramID;

	//���׽�Ʈ��޸�
}


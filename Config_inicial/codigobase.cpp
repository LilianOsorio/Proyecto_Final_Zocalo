// Proyecto Zocalo 
// Equipo 2 
// 423069439 
// 319211454
// 421025550
// RECORRIDO VIRTUAL CELEBRACIÓN DEL 15 DE SEPTIEMBRE EN EL ZÓCALO DE LA CIUDAD DE MÉXICO
// FECHA: 11/Noviembre/2026


#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Load Models
#include "SOIL2/SOIL2.h"

#define NOMINMAX
#include <windows.h>

#pragma comment(lib, "gdi32.lib")

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Skybox.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

void DrawSidewalk(
	Shader shader,
	GLuint VAO,
	glm::vec3 position,
	glm::vec3 size,
	glm::vec3 color
);

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(1.0f, -0.8f, 7.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active = false;

bool lucesFachadaActivas = true;
bool lucesDecorativasActivas = true;
float ambienteFachada = 0.55f;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 2.0f, 0.0f),       // Luz 0
	glm::vec3(8.5f, 0.3f, 0.0f),       // Luz 1
	glm::vec3(-3.5f, 5.0f, -18.0f),    // Luz 2
	glm::vec3(0.0f, 5.0f, -30.0f),     // Luz 3
	glm::vec3(3.5f, 5.0f, -42.0f),     // Luz 4

	// luces de la fachada del Palacio
	glm::vec3(-9.0f, 6.0f, 12.0f),     // Luz 5
	glm::vec3(0.0f, 6.0f, 12.0f),      // Luz 6
	glm::vec3(9.0f, 6.0f, 12.0f),       // Luz 7
	glm::vec3(-8.5f, 0.3f, 0.0f)       // Luz 8
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

// Anim
float rotBall = 0.0f;
bool AnimBall = false;
bool AnimFox = false;
bool AnimDog = false;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;
float walkTime = 0.0f;

// Recorrido zorro
const glm::vec3 foxRoute[] = {
	glm::vec3(-12.0f, -2.0f, 12.0f),
	glm::vec3(-12.0f, -2.0f, -12.0f),
	glm::vec3(0.0f, -2.0f, -12.0f),

	// Se desvía antes de llegar a la campana
	glm::vec3(0.0f, -2.0f, 8.0f),

	glm::vec3(12.0f, -2.0f, 12.0f),
	glm::vec3(12.0f, -2.0f, -12.0f),
	glm::vec3(-12.0f, -2.0f, -12.0f)
};

const int foxRouteCount = sizeof(foxRoute) / sizeof(foxRoute[0]);

glm::vec3 foxPos = foxRoute[0];
int foxNextWaypoint = 1;
float foxYaw = 0.0f;

// Posicion y orientacion del perro
glm::vec3 dogPos(0.0f, -1.5f, 0.0f);
float dogYaw = 0.0f;
float dogRot = 0.0f;

// NPC 1
float npcPosX = -4.0f;
float npcPosZ = 2.0f;
float npcVelocidad = 1.5f;
float npcLimiteIzquierdo = -4.0f;
float npcLimiteDerecho = 4.0f;
int npcDireccion = 1;

// NPC 2
float npc2PosZ = -10.0f;
float npc2PosX = 2.0f;
float npc2Velocidad = 1.2f;
float npc2LimiteFondo = -35.0f;
float npc2LimiteFrente = -5.0f;
int npc2Direccion = 1;

// NPC 3: Recorrido cerca de la campana
float npc3PosX = 3.0f;
float npc3PosZ = 10.0f;
float npc3Velocidad = 1.0f;
int npc3Direccion = 1;

// NPC 4: Recorrido por la explanada
float npc4PosX = 7.0f;
float npc4PosZ = 10.0f;
float npc4Velocidad = 1.0f;
int npc4Direccion = 1;

// NPC 5: Recorrido por la zona central
float npc5PosX = -9.0f;
float npc5PosZ = 7.0f;
float npc5Velocidad = 0.8f;
int npc5Direccion = 1;

// Camara
float cameraDistance = 5.5f;
float cameraHeight = 1.0f;
bool step = false;

// Vistas de camara
bool vistaPanoramica = false;
bool teclaCAntes = false;

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

float intensidadCampana = 0.0f;
glm::vec3 posicionCampana(0.0f, -2.0f, 11.0f);

GLuint CrearTexturaInformativa(const wchar_t* titulo, const wchar_t* mensaje)
{
	const int ancho = 512;
	const int alto = 180;

	HDC hdc = CreateCompatibleDC(NULL);

	BITMAPINFO informacion = {};
	informacion.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	informacion.bmiHeader.biWidth = ancho;
	informacion.bmiHeader.biHeight = -alto;
	informacion.bmiHeader.biPlanes = 1;
	informacion.bmiHeader.biBitCount = 32;
	informacion.bmiHeader.biCompression = BI_RGB;

	void* pixeles = nullptr;

	HBITMAP bitmap = CreateDIBSection(
		hdc, &informacion, DIB_RGB_COLORS, &pixeles, NULL, 0
	);

	HGDIOBJ bitmapAnterior = SelectObject(hdc, bitmap);

	RECT fondo = { 0, 0, ancho, alto };
	HBRUSH brocha = CreateSolidBrush(RGB(20, 28, 45));
	FillRect(hdc, &fondo, brocha);
	DeleteObject(brocha);

	HBRUSH borde = CreateSolidBrush(RGB(220, 175, 65));
	FrameRect(hdc, &fondo, borde);
	DeleteObject(borde);

	HFONT fuenteTitulo = CreateFontW(
		30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Arial"
	);

	SelectObject(hdc, fuenteTitulo);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 215, 100));

	RECT areaTitulo = { 22, 15, 490, 55 };
	DrawTextW(hdc, titulo, -1, &areaTitulo, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	HFONT fuenteMensaje = CreateFontW(
		23, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Arial"
	);

	SelectObject(hdc, fuenteMensaje);
	SetTextColor(hdc, RGB(245, 245, 245));

	RECT areaMensaje = { 22, 65, 485, 165 };
	DrawTextW(hdc, mensaje, -1, &areaMensaje, DT_LEFT | DT_WORDBREAK | DT_TOP);

	GLuint textura;
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ancho, alto, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixeles);

	SelectObject(hdc, bitmapAnterior);
	DeleteObject(fuenteTitulo);
	DeleteObject(fuenteMensaje);
	DeleteObject(bitmap);
	DeleteDC(hdc);

	return textura;
}

GLuint LoadWallTexture(const char* path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);

	if (!image)
	{
		cout << "No se pudo cargar la imagen: " << path << endl;
		glDeleteTextures(1, &textureID);
		return 0;
	}

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint wallVAO, wallVBO, wallEBO;
GLuint texNorte, texSur, texEste, texOeste, texPalacio;
GLuint texAguila;

void SetupWalls()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,     0, 0, 1,     0, 1,
		 0.5f, -0.5f, 0.0f,     0, 0, 1,     1, 1,
		 0.5f,  0.5f, 0.0f,     0, 0, 1,     1, 0,
		-0.5f,  0.5f, 0.0f,     0, 0, 1,     0, 0
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &wallVAO);
	glGenBuffers(1, &wallVBO);
	glGenBuffers(1, &wallEBO);

	glBindVertexArray(wallVAO);
	glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	texNorte = LoadWallTexture("images/fachadas/norte.jpeg");
	texSur = LoadWallTexture("images/fachadas/sur.jpeg");
	texEste = LoadWallTexture("images/fachadas/este.jpeg");
	texOeste = LoadWallTexture("images/fachadas/oeste.png");
	texPalacio = LoadWallTexture("images/fachadas/palacio.jpeg");
	texAguila = LoadWallTexture("images/fachadas/aguila.jpg");
}

void DrawWall(Shader shader, GLuint texture, glm::vec3 position, float width, float height, float rotation)
{
	if (texture == 0) return;

	glm::mat4 model(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(width, height, 1.0f));

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(glGetUniformLocation(shader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shader.Program, "material.specular"), 0);

	// Activar el efecto tricolor solamente en las fachadas
	glUniform1i(
		glGetUniformLocation(shader.Program, "wallTricolorOn"),
		lucesDecorativasActivas ? 1 : 0
	);

	// El ambiente de las fachadas se controla con L
	float ambiente = lucesDecorativasActivas ? 0.55f : 0.12f;

	glUniform3f(
		glGetUniformLocation(shader.Program, "dirLight.ambient"),
		ambiente, ambiente * 0.96f, ambiente * 0.91f
	);

	glBindVertexArray(wallVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Desactivar el efecto para los demas modelos
	glUniform1i(
		glGetUniformLocation(shader.Program, "wallTricolorOn"),
		0
	);



	glBindTexture(GL_TEXTURE_2D, 0);
}

// ANIMACION DE BANDERA MEDIANTE KEYFRAMES
float ObtenerAmplitudViento(float tiempo)
{
	const float tiempos[] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f
	};

	const float amplitudes[] = {
		0.50f, 1.80f, 0.70f, 2.00f, 0.50f
	};

	const int cantidad = 5;
	const float duracion = 4.0f;

	float fase = fmod(tiempo, duracion);

	for (int i = 0; i < cantidad - 1; i++)
	{
		if (fase >= tiempos[i] && fase < tiempos[i + 1])
		{
			float progreso =
				(fase - tiempos[i]) /
				(tiempos[i + 1] - tiempos[i]);

			return glm::mix(
				amplitudes[i],
				amplitudes[i + 1],
				progreso
			);
		}
	}

	return amplitudes[cantidad - 1];
}


int main()
{
	ma_engine engine;
	ma_sound musica;
	ma_sound campanaAudio;

	glfwInit();

	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Recorrido Virtual: Noche Mexicana - 423069439, 319211454, 421025550  ", nullptr, nullptr);


	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	glfwSetKeyCallback(window, KeyCallback);

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	if (ma_engine_init(NULL, &engine) != MA_SUCCESS)
	{
		std::cout << "Error al inicializar el audio" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	if (ma_sound_init_from_file(&engine, "audio/musica.mp3", MA_SOUND_FLAG_STREAM, NULL, NULL, &musica) != MA_SUCCESS)
	{
		std::cout << "No se pudo cargar la musica" << std::endl;
		ma_engine_uninit(&engine);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	if (ma_sound_init_from_file(&engine, "audio/campana.mp3", 0, NULL, NULL, &campanaAudio) != MA_SUCCESS)
	{
		std::cout << "No se pudo cargar el sonido de la campana" << std::endl;
		ma_sound_uninit(&musica);
		ma_engine_uninit(&engine);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	ma_sound_set_volume(&campanaAudio, 0.75f);
	ma_sound_set_volume(&musica, 0.35f);
	ma_sound_set_looping(&musica, MA_TRUE);
	ma_sound_start(&musica);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxShader("skybox.vs", "skybox.frag");
	Shader infoShader("info.vs", "info.frag");
	Shader fireworksShader("fireworks.vs", "fireworks.frag");

	SetupWalls();

	vector<string> skyboxFaces = {
		"images/Skybox/right.jpg",
		"images/Skybox/left.jpg",
		"images/Skybox/top.jpg",
		"images/Skybox/bottom.jpg",
		"images/Skybox/front.jpg",
		"images/Skybox/back.jpg"
	};

	Skybox nightSky(skyboxFaces);

	// Models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso/Mrf+Pavement+Set03.obj");

	Model Puesto((char*)"Models/puesto/asian food vendor.obj");
	Model Lampara((char*)"Models/lampara/Street_lamp_1.obj");
	Model Lantern((char*)"Models/lantern/lantern.obj");
	Model Zorro((char*)"Models/Zorro/zorroCompleto.obj");
	Model BanderaMexico((char*)"Models/Astil/FlagMexico.obj");
	Model SombreroCharro((char*)"Models/Zorro/sombrero_charro.obj");
	Model puestoFruta((char*)"Models/puesto2/fruit shop.obj");
	Model Angel((char*)"Models/AngelIndependencia/AngelInd.obj");

	Model Persona1((char*)"Models/persona1/Audience.obj");
	Model Persona2((char*)"Models/persona2/Audience.obj");
	Model Persona3((char*)"Models/persona3/Audience.obj");
	Model Persona4((char*)"Models/persona4/Audience.obj");
	Model Persona5((char*)"Models/persona5/Audience.obj");
	Model Persona6((char*)"Models/persona6/Audience.obj");
	Model Persona7((char*)"Models/persona7/Audience.obj");
	Model Persona8((char*)"Models/persona8/Audience.obj");

	Model carritoComida((char*)"Models/carritocomida/hot-dog-cart.obj");
	Model Campana((char*)"Models/campana/objBell.obj");

	// CARGA DE LAS AVES
	Model PeregrineFalcon((char*)"Models/peregrine falcon/peregrine falcon.obj");
	Model Woodpecker((char*)"Models/Woodpecker/Woodpecker.obj");

	GLuint infoPozole = CrearTexturaInformativa(
		L"EL GRITO DE DOLORES",
		L"La madrugada del 16 de septiembre de 1810, Miguel Hidalgo llamo a las armas con la campana de la parroquia en Dolores, Guanajuato."
	);

	GLuint infoFruta = CrearTexturaInformativa(
		L"LA CAMPANA DE DOLORES",
		L"Conocida como la Campana de la Independencia. Cada 15 de septiembre a las 11:00 PM, el Presidente la repica desde Palacio Nacional."
	);

	GLuint infoHotDog = CrearTexturaInformativa(
		L"LOS FUEGOS PATRIOS",
		L"La pirotecnia tricolor ilumina el Zocalo representando la alegria y el orgullo por la libertad alcanzada por los heroes patrios."
	);

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint infoVAO, infoVBO;
	GLfloat infoVertices[] = {
		-1.0f, -0.5f, 0.0f,    0.0f, 1.0f,
		1.0f, -0.5f, 0.0f,    1.0f, 1.0f,
		1.0f,  0.5f, 0.0f,    1.0f, 0.0f,

		-1.0f, -0.5f, 0.0f,    0.0f, 1.0f,
		1.0f,  0.5f, 0.0f,    1.0f, 0.0f,
		-1.0f,  0.5f, 0.0f,    0.0f, 0.0f
	};

	glGenVertexArrays(1, &infoVAO);
	glGenBuffers(1, &infoVBO);
	glBindVertexArray(infoVAO);
	glBindBuffer(GL_ARRAY_BUFFER, infoVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(infoVertices), infoVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	GLuint fireworksVAO, fireworksVBO;
	glGenVertexArrays(1, &fireworksVAO);
	glGenBuffers(1, &fireworksVBO);
	glBindVertexArray(fireworksVAO);
	glBindBuffer(GL_ARRAY_BUFFER, fireworksVBO);
	glBufferData(GL_ARRAY_BUFFER, 150 * 3 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	lightingShader.Use();
	
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
	float volumenMusica = 0.35f;

	// Game loop
	while (!glfwWindowShouldClose(window))

	{
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (deltaTime > 0.05f) deltaTime = 0.05f;

		glfwPollEvents();

		static float tiempoAnteriorNPC = (float)glfwGetTime();
		float tiempoActualNPC = (float)glfwGetTime();
		float deltaTimeNPC = tiempoActualNPC - tiempoAnteriorNPC;
		tiempoAnteriorNPC = tiempoActualNPC;

		npcPosX += npcVelocidad * npcDireccion * deltaTimeNPC;
		if (npcPosX >= npcLimiteDerecho) { npcPosX = npcLimiteDerecho; npcDireccion = -1; }
		else if (npcPosX <= npcLimiteIzquierdo) { npcPosX = npcLimiteIzquierdo; npcDireccion = 1; }

		npc2PosZ += npc2Velocidad * npc2Direccion * deltaTimeNPC;
		if (npc2PosZ >= npc2LimiteFrente) { npc2PosZ = npc2LimiteFrente; npc2Direccion = -1; }
		else if (npc2PosZ <= npc2LimiteFondo) { npc2PosZ = npc2LimiteFondo; npc2Direccion = 1; }

		// NPC 3
		npc3PosX += npc3Velocidad * npc3Direccion * deltaTimeNPC;

		if (npc3PosX >= 5.0f)
		{
			npc3PosX = 5.0f;
			npc3Direccion = -1;
		}
		else if (npc3PosX <= 2.5f)
		{
			npc3PosX = 2.5f;
			npc3Direccion = 1;
		}

		// NPC 4: Lado derecho de la explanada
		npc4PosZ += npc4Velocidad * npc4Direccion * deltaTimeNPC;

		if (npc4PosZ >= 12.0f)
		{
			npc4PosZ = 12.0f;
			npc4Direccion = -1;
		}
		else if (npc4PosZ <= 7.0f)
		{
			npc4PosZ = 7.0f;
			npc4Direccion = 1;
		}

		// NPC 5: Lado izquierdo de la explanada
		npc5PosZ += npc5Velocidad * npc5Direccion * deltaTimeNPC;

		if (npc5PosZ >= 10.0f)
		{
			npc5PosZ = 10.0f;
			npc5Direccion = -1;
		}
		else if (npc5PosZ <= 4.0f)
		{
			npc5PosZ = 4.0f;
			npc5Direccion = 1;
		}

		DoMovement();
		Animation();

		glm::vec2 posicionPerro(dogPos.x, dogPos.z);
		glm::vec2 posicionCampanaXZ(posicionCampana.x, posicionCampana.z);
		float distanciaCampana = glm::distance(posicionPerro, posicionCampanaXZ);

		if (distanciaCampana < 5.0f)
		{
			intensidadCampana += deltaTime * 2.0f;
			if (!ma_sound_is_playing(&campanaAudio))
			{
				ma_sound_seek_to_pcm_frame(&campanaAudio, 0);
				ma_sound_start(&campanaAudio);
			}
		}
		else
		{
			intensidadCampana -= deltaTime * 1.2f;
			ma_sound_stop(&campanaAudio);
		}

		intensidadCampana = glm::clamp(intensidadCampana, 0.0f, 1.0f);

		float volumenObjetivo = ma_sound_is_playing(&campanaAudio) ? 0.15f : 0.35f;
		volumenMusica += (volumenObjetivo - volumenMusica) * deltaTime * 3.0f;
		ma_sound_set_volume(&musica, volumenMusica);

		glClearColor(0.008f, 0.012f, 0.045f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 modelTemp = glm::mat4(1.0f);

		lightingShader.Use();
		glUniform1i(
			glGetUniformLocation(lightingShader.Program, "facadeLightsOn"),
			lucesFachadaActivas ? 1 : 0
		);

		glUniform1i(
			glGetUniformLocation(lightingShader.Program, "decorLightsOn"),
			lucesDecorativasActivas ? 1 : 0
		);

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Ambiente general controlado con F
		float ambienteGeneral = lucesFachadaActivas ? 0.30f : 0.08f;

		glUniform3f(
			glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),
			ambienteGeneral, ambienteGeneral, ambienteGeneral + 0.10f
		);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.45f, 0.45f, 0.50f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.15f, 0.15f, 0.20f);

		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = abs(sin(glfwGetTime() * Light1.z));

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x * 0.08f, lightColor.y * 0.08f, lightColor.z * 0.08f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x * 0.35f, lightColor.y * 0.35f, lightColor.z * 0.35f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.12f, 0.075f, 0.035f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 0.68f, 0.32f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.9f, 0.72f, 0.45f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// LUZ VERDE
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.15f, 0.40f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.30f, 1.0f, 0.20f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.15f, 0.25f, 0.15f);

		// LUZ BLANCA
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.35f, 0.35f, 0.35f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.25f, 0.25f, 0.25f);

		// LUZ ROJA
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), 0.12f, 0.035f, 0.035f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), 0.65f, 0.18f, 0.18f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 0.25f, 0.12f, 0.12f);

		// Luces de fachadas

		// LUZ 5 - Lado izquierdo
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"),
			pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), 0.08f, 0.045f, 0.015f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), 1.0f, 0.65f, 0.28f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 0.3f, 0.2f, 0.1f);

		// LUZ 6 - Centro
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"),
			pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), 0.08f, 0.045f, 0.015f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), 1.0f, 0.65f, 0.28f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), 0.3f, 0.2f, 0.1f);

		// LUZ 7 - Lado derecho
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].position"),
			pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].ambient"), 0.08f, 0.045f, 0.015f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].diffuse"), 1.0f, 0.65f, 0.28f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].specular"), 0.3f, 0.2f, 0.1f);

		// LUZ 8 - Segundo faro
		glUniform3f(
			glGetUniformLocation(lightingShader.Program, "pointLights[8].position"),
			pointLightPositions[8].x,
			pointLightPositions[8].y,
			pointLightPositions[8].z
		);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].ambient"),
			0.12f, 0.075f, 0.035f);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].diffuse"),
			1.0f, 0.68f, 0.32f);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].specular"),
			0.9f, 0.72f, 0.45f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].quadratic"), 0.032f);

		// Atenuación de las tres luces nuevas
		for (int i = 5; i <= 7; i++)
		{
			std::string base = "pointLights[" + std::to_string(i) + "].";

			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "linear").c_str()), 0.07f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "quadratic").c_str()), 0.017f);
		}

		for (int i = 2; i <= 4; i++)
		{
			std::string base = "pointLights[" + std::to_string(i) + "].";
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "linear").c_str()), 0.09f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "quadratic").c_str()), 0.032f);
		}

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.18f, 0.16f, 0.10f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.65f, 0.58f, 0.42f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		glm::mat4 view = camera.GetViewMatrix();

		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		// FUEGOS ARTIFICIALES
		const int NUM_PARTICULAS_POR_FUEGO = 150;
		const int CANTIDAD_FUEGOS = 6;
		static float tiemposExplosion[CANTIDAD_FUEGOS] = { 0.0f, 0.3f, 0.6f, 0.9f, 1.2f, 1.5f };

		static glm::vec3 centrosFuegos[CANTIDAD_FUEGOS] = {
			glm::vec3(0.0f, 21.0f, 11.0f),
			glm::vec3(-4.0f, 19.0f, -4.0f),
			glm::vec3(3.0f, 20.0f, 2.0f),
			glm::vec3(-7.0f, 18.0f, -28.0f),
			glm::vec3(0.0f, 22.0f, -55.0f),
			glm::vec3(6.0f, 19.0f, -42.0f)
		};

		static glm::vec3 coloresFuegos[CANTIDAD_FUEGOS] = {
			glm::vec3(0.95f, 0.05f, 0.05f),
			glm::vec3(0.0f, 0.85f, 0.2f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.95f, 0.05f, 0.05f),
			glm::vec3(0.0f, 0.85f, 0.2f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		};

		static glm::vec3 velocidadesFuego[NUM_PARTICULAS_POR_FUEGO];
		static bool inicializadoVelocidades = false;

		if (!inicializadoVelocidades) {
			for (int i = 0; i < NUM_PARTICULAS_POR_FUEGO; i++) {
				float anguloH = i * 2.4f;
				float anguloV = (i % 13) * 0.25f - 1.25f;
				glm::vec3 dir(cos(anguloV) * cos(anguloH), sin(anguloV), cos(anguloV) * sin(anguloH));
				float factorVolumen = 0.2f + 0.8f * ((i * 17) % 100) / 100.0f;
				velocidadesFuego[i] = dir * (1.0f + factorVolumen * 4.0f);
			}
			inicializadoVelocidades = true;
		}

		fireworksShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(fireworksShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(fireworksShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(fireworksShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_PROGRAM_POINT_SIZE);

		glBindVertexArray(fireworksVAO);

		for (int f = 0; f < CANTIDAD_FUEGOS; f++) {
			tiemposExplosion[f] += deltaTime;
			if (tiemposExplosion[f] >= 2.0f) tiemposExplosion[f] = 0.0f;

			float t = tiemposExplosion[f];
			std::vector<GLfloat> posicionesFuegoTemp(NUM_PARTICULAS_POR_FUEGO * 3);

			for (int i = 0; i < NUM_PARTICULAS_POR_FUEGO; i++) {
				float friccion = exp(-1.5f * t);
				glm::vec3 desplazamiento = velocidadesFuego[i] * (1.0f - friccion) / 1.5f;
				glm::vec3 posicion = centrosFuegos[f] + desplazamiento + glm::vec3(0.0f, -2.5f * t * t, 0.0f);

				posicionesFuegoTemp[i * 3] = posicion.x;
				posicionesFuegoTemp[i * 3 + 1] = posicion.y;
				posicionesFuegoTemp[i * 3 + 2] = posicion.z;
			}

			glBindBuffer(GL_ARRAY_BUFFER, fireworksVBO);
			glBufferData(GL_ARRAY_BUFFER, posicionesFuegoTemp.size() * sizeof(GLfloat), posicionesFuegoTemp.data(), GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glUniform3f(glGetUniformLocation(fireworksShader.Program, "objectColor"), coloresFuegos[f].r, coloresFuegos[f].g, coloresFuegos[f].b);
			glDrawArrays(GL_POINTS, 0, NUM_PARTICULAS_POR_FUEGO);
		}

		glBindVertexArray(0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		lightingShader.Use();

		// Explanada
		glDisable(GL_CULL_FACE);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		for (int i = -15; i <= 15; i++)
		{
			for (int j = -15; j <= 15; j++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i * 1.0f, -2.0f, j * 1.0f));
				model = glm::scale(model, glm::vec3(0.002f, 0.002f, 0.002f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Piso.Draw(lightingShader);
			}
		}

		// Calle continuacion piso
		for (int i = -4; i <= 4; i++)
		{
			for (int j = -65; j <= -16; j++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i * 1.0f, -2.0f, j * 1.0f));
				model = glm::scale(model, glm::vec3(0.002f, 0.002f, 0.002f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Piso.Draw(lightingShader);
			}
		}

		// BANQUETAS
		lampShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		DrawSidewalk(lampShader, VAO, glm::vec3(-5.0f, -1.85f, -35.5f), glm::vec3(2.0f, 0.30f, 39.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		DrawSidewalk(lampShader, VAO, glm::vec3(5.0f, -1.85f, -35.5f), glm::vec3(2.0f, 0.30f, 39.0f), glm::vec3(0.5f, 0.5f, 0.5f));

		lightingShader.Use();
		glEnable(GL_CULL_FACE);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		float alturaMuro = 13.0f;
		float centroY = 3.0f;
		float anchoMuro = 32.0f;

		DrawWall(lightingShader, texNorte, glm::vec3(-11.0f, centroY, -16.0f), 10.0f, alturaMuro, 0.0f);
		DrawWall(lightingShader, texNorte, glm::vec3(11.0f, centroY, -16.0f), 10.0f, alturaMuro, 0.0f);
		DrawWall(lightingShader, texEste, glm::vec3(16.0f, centroY, 0.0f), anchoMuro, alturaMuro, -90.0f);
		DrawWall(lightingShader, texOeste, glm::vec3(-16.0f, centroY, 0.0f), anchoMuro, alturaMuro, 90.0f);

		DrawWall(lightingShader, texEste, glm::vec3(-6.1f, centroY, -51.75f), 71.5f, alturaMuro, 90.0f);
		DrawWall(lightingShader, texSur, glm::vec3(6.1f, centroY, -51.75f), 71.5f, alturaMuro, -90.0f);

		DrawWall(lightingShader, texPalacio, glm::vec3(0.0f, centroY, 15.0f), 32.0f, alturaMuro, 180.0f);
		DrawWall(lightingShader, texAguila, glm::vec3(0.0f, centroY, -68.0f), 12.2f, alturaMuro, 0.0f);

		glEnable(GL_CULL_FACE);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// Papel picado
		lampShader.Use();
		glDisable(GL_CULL_FACE);

		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::vec3 verdePatrio(0.0f, 0.55f, 0.20f);
		glm::vec3 blancoPatrio(0.95f, 0.95f, 0.90f);
		glm::vec3 rojoPatrio(0.80f, 0.05f, 0.08f);
		glm::vec3 colorCable(0.12f, 0.12f, 0.12f);

		float posicionesZ[] = { -60.0f, -55.0f, -50.0f, -45.0f, -40.0f, -35.0f, -30.0f, -25.0f, -20.0f, -15.0f, -10.0f, -5.0f, 0.0f, 5.0f };

		for (int h = 0; h < 14; h++)
		{
			float z = posicionesZ[h];
			DrawSidewalk(lampShader, VAO, glm::vec3(0.0f, 6.0f, z), glm::vec3(25.0f, 0.035f, 0.035f), colorCable);

			for (int i = 0; i < 9; i++)
			{
				float x = -12.0f + (i * 3.0f);
				glm::vec3 color = (i % 3 == 0) ? verdePatrio : ((i % 3 == 1) ? blancoPatrio : rojoPatrio);
				DrawSidewalk(lampShader, VAO, glm::vec3(x, 5.5f, z), glm::vec3(0.85f, 0.9f, 0.06f), color);
			}
		}

		// Focos
		DrawSidewalk(lampShader, VAO, glm::vec3(0.0f, 5.8f, -9.5f), glm::vec3(25.0f, 0.025f, 0.025f), colorCable);
		DrawSidewalk(lampShader, VAO, glm::vec3(-10.0f, 5.8f, -9.5f), glm::vec3(0.22f), verdePatrio);
		DrawSidewalk(lampShader, VAO, glm::vec3(-6.0f, 5.8f, -9.5f), glm::vec3(0.22f), blancoPatrio);
		DrawSidewalk(lampShader, VAO, glm::vec3(-2.0f, 5.8f, -9.5f), glm::vec3(0.22f), rojoPatrio);
		DrawSidewalk(lampShader, VAO, glm::vec3(2.0f, 5.8f, -9.5f), glm::vec3(0.22f), verdePatrio);
		DrawSidewalk(lampShader, VAO, glm::vec3(6.0f, 5.8f, -9.5f), glm::vec3(0.22f), blancoPatrio);
		DrawSidewalk(lampShader, VAO, glm::vec3(10.0f, 5.8f, -9.5f), glm::vec3(0.22f), rojoPatrio);

		// Banderas
		float posicionesBanderas[] = { -8.0f, -20.0f, -32.0f, -44.0f };
		float ladosFachadas[] = { -7.5f, 7.5f };

		for (int lado = 0; lado < 2; lado++)
		{
			float x = ladosFachadas[lado];
			for (int i = 0; i < 4; i++)
			{
				float z = posicionesBanderas[i];
				DrawSidewalk(lampShader, VAO, glm::vec3(x, 4.8f, z - 1.15f), glm::vec3(0.07f, 2.2f, 0.07f), colorCable);
				DrawSidewalk(lampShader, VAO, glm::vec3(x, 5.0f, z - 0.72f), glm::vec3(0.06f, 1.3f, 0.72f), verdePatrio);
				DrawSidewalk(lampShader, VAO, glm::vec3(x, 5.0f, z), glm::vec3(0.06f, 1.3f, 0.72f), blancoPatrio);
				DrawSidewalk(lampShader, VAO, glm::vec3(x, 5.0f, z + 0.72f), glm::vec3(0.06f, 1.3f, 0.72f), rojoPatrio);

				glm::vec3 cafeEscudo(0.35f, 0.18f, 0.08f);
				float desplazamientoEscudo = (lado == 0) ? 0.045f : -0.045f;
				DrawSidewalk(lampShader, VAO, glm::vec3(x + desplazamientoEscudo, 5.0f, z), glm::vec3(0.035f, 0.32f, 0.32f), cafeEscudo);
			}
		}

		glEnable(GL_CULL_FACE);
		lightingShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// Perro
		glm::vec3 dogPosElevado = dogPos + glm::vec3(0.0f, 0.05f, 0.0f);
		modelTemp = model = glm::translate(glm::mat4(1.0f), dogPosElevado);
		modelTemp = model = glm::rotate(model, glm::radians(dogRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);

		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);

		// Puesto
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Puesto.Draw(lightingShader);

		// Lampara
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.2f, 3.2f, 3.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Lampara.Draw(lightingShader);

		// FARO 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.5f, -2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(-22.48075f, -0.3702f, 0.11045f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lantern.Draw(lightingShader);

		// FARO 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.5f, -2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::translate(model, glm::vec3(-22.48075f, -0.3702f, 0.11045f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lantern.Draw(lightingShader);

	
		// Zorro
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(foxPos.x, -2.0f + 1.2143f * 0.55f, foxPos.z));
		model = glm::rotate(model, glm::radians(foxYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.55f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Zorro.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(foxPos.x, -2.0f + 1.2143f * 0.55f, foxPos.z));
		model = glm::rotate(model, glm::radians(foxYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.55f));
		model = glm::translate(model, glm::vec3(0.0f, 0.61f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		SombreroCharro.Draw(lightingShader);

		// Bandera con animacion por KeyFrames
		float tiempoBandera = (float)glfwGetTime();

		glUniform1i(
			glGetUniformLocation(lightingShader.Program, "windEnabled"),
			1
		);

		glUniform1f(
			glGetUniformLocation(lightingShader.Program, "windTime"),
			tiempoBandera
		);

		glUniform1f(
			glGetUniformLocation(lightingShader.Program, "windAmplitude"),
			ObtenerAmplitudViento(tiempoBandera)
		);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(foxRoute[2].x + 3.5f, -1.73395f, foxRoute[2].z));
		model = glm::scale(model, glm::vec3(2.4f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		glDisable(GL_CULL_FACE);
		BanderaMexico.Draw(lightingShader);
		glEnable(GL_CULL_FACE);

		glUniform1i(glGetUniformLocation(lightingShader.Program, "windEnabled"), 0);

		// Puesto fruta
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.0f, -2.0f, -2.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		puestoFruta.Draw(lightingShader);

		// Campana
		glm::mat4 modelBase = glm::mat4(1.0f);
		modelBase = glm::translate(modelBase, posicionCampana);
		modelBase = glm::scale(modelBase, glm::vec3(0.6f));

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBase));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		Campana.DrawMaterial(lightingShader, "lambert10SG");

		glm::mat4 modelMetal = modelBase;
		float anguloCampana = sin(glfwGetTime() * 8.0f) * glm::radians(12.0f) * intensidadCampana;
		modelMetal = glm::rotate(modelMetal, anguloCampana, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMetal));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Campana.DrawMaterial(lightingShader, "blinn5SG");

		glDisable(GL_BLEND);

		// Carrito Hotdogs
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.5f, -2.0f, -6.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		carritoComida.Draw(lightingShader);

		// Cuadros informativos
		glm::vec3 jugador = dogPos;
		glm::vec3 posPozole(3.0f, 2.5f, 0.0f);
		glm::vec3 posFruta(-6.0f, 2.5f, -2.0f);
		glm::vec3 posCarrito(-1.5f, 2.5f, -6.0f);

		float distanciaPozole = glm::length(glm::vec2(jugador.x - posPozole.x, jugador.z - posPozole.z));
		float distanciaFruta = glm::length(glm::vec2(jugador.x - posFruta.x, jugador.z - posFruta.z));
		float distanciaCarrito = glm::length(glm::vec2(jugador.x - posCarrito.x, jugador.z - posCarrito.z));

		infoShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(infoShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(infoShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform1i(glGetUniformLocation(infoShader.Program, "textura"), 0);

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(infoVAO);
		glDisable(GL_CULL_FACE);

		if (distanciaPozole < 7.0f)
		{
			glm::vec3 direccion = camera.GetPosition() - posPozole;
			float angulo = atan2(direccion.x, direccion.z) + glm::radians(180.0f);
			glm::mat4 panel(1.0f);
			panel = glm::translate(panel, posPozole);
			panel = glm::rotate(panel, angulo, glm::vec3(0.0f, 1.0f, 0.0f));
			panel = glm::scale(panel, glm::vec3(-2.3f, 1.15f, 1.0f));
			glUniformMatrix4fv(glGetUniformLocation(infoShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(panel));
			glBindTexture(GL_TEXTURE_2D, infoPozole);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		if (distanciaFruta < 7.0f)
		{
			glm::vec3 direccion = camera.GetPosition() - posFruta;
			float angulo = atan2(direccion.x, direccion.z);
			glm::mat4 panel(1.0f);
			panel = glm::translate(panel, posFruta);
			panel = glm::rotate(panel, angulo, glm::vec3(0.0f, 1.0f, 0.0f));
			panel = glm::scale(panel, glm::vec3(2.3f, 1.15f, 1.0f));
			glUniformMatrix4fv(glGetUniformLocation(infoShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(panel));
			glBindTexture(GL_TEXTURE_2D, infoFruta);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		if (distanciaCarrito < 7.0f)
		{
			glm::vec3 direccion = camera.GetPosition() - posCarrito;
			float angulo = atan2(direccion.x, direccion.z);
			glm::mat4 panel(1.0f);
			panel = glm::translate(panel, posCarrito);
			panel = glm::rotate(panel, angulo, glm::vec3(0.0f, 1.0f, 0.0f));
			panel = glm::scale(panel, glm::vec3(2.3f, 1.15f, 1.0f));
			glUniformMatrix4fv(glGetUniformLocation(infoShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(panel));
			glBindTexture(GL_TEXTURE_2D, infoHotDog);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_CULL_FACE);

		lightingShader.Use();

		// Multitud
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-14.0f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona2.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-14.0f, -2.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona3.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-14.0f, -2.0f, 2.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona4.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(14.0f, -2.0f, -8.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona5.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(14.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona6.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(14.0f, -2.0f, 6.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona7.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.0f, -2.0f, 13.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona8.Draw(lightingShader);

		// NPC 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(npc3PosX, -2.0f, npc3PosZ));
		model = glm::scale(model, glm::vec3(0.01f));

		if (npc3Direccion == 1)
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
		else
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona1.Draw(lightingShader);

		// NPC 4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(npc4PosX, -2.0f, npc4PosZ));
		model = glm::scale(model, glm::vec3(0.01f));

		if (npc4Direccion == 1)
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));
		else
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona3.Draw(lightingShader);

		// NPC 5
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(npc5PosX, -2.0f, npc5PosZ));
		model = glm::scale(model, glm::vec3(0.01f));

		if (npc5Direccion == 1)
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));
		else
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona5.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.0f, -2.0f, 13.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona1.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, -2.0f, 13.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona2.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.0f, -2.0f, 13.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona3.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.2f, -2.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona3.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.0f, -2.0f, -1.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona4.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.5f, -2.0f, -4.6f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona5.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -2.0f, -6.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(170.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona6.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.0f, -2.0f, 1.8f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona7.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, -2.0f, 2.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona8.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona1.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, -2.0f, 3.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona2.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.5f, -2.0f, 4.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona3.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, -2.0f, 6.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona4.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, -2.0f, 6.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona5.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.5f, -1.70f, -12.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona6.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.5f, -1.70f, -20.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona7.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.5f, -1.70f, -28.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona8.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.5f, -1.70f, -36.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona1.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.5f, -1.70f, -44.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona2.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.5f, -1.70f, -15.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona3.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.5f, -1.70f, -22.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona4.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.5f, -1.70f, -30.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona5.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.5f, -1.70f, -38.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona6.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.5f, -2.0f, -47.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona7.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.5f, -2.0f, -47.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona8.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -2.0f, -46.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona1.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -2.0f, -46.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona2.Draw(lightingShader);

		// NPCs dinámicos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(npcPosX, -2.0f, npcPosZ));
		model = glm::scale(model, glm::vec3(0.01f));
		if (npcDireccion == 1)
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		else
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona7.Draw(lightingShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(npc2PosX, -2.0f, npc2PosZ));
		model = glm::scale(model, glm::vec3(0.01f));
		if (npc2Direccion == 1)
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		else
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Persona8.Draw(lightingShader);

		// Ángel
		glDisable(GL_CULL_FACE);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -55.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Angel.Draw(lightingShader);
		glEnable(GL_CULL_FACE);

		// ANIMACIÓN Y RENDERIZADO DE LAS AVES (VOLANDO EN CÍRCULOS)
		float flightTime = (float)glfwGetTime();
		float flightSpeed = 1.2f;                    // Velocidad de giro
		float radius = 14.0f;                        // Radio del círculo de vuelo
		glm::vec3 flightCenter(0.0f, 12.0f, -10.0f);  // Centro y altura sobre el Zócalo

		// 1. Woodpecker
		float woodpeckerAngle = flightTime * flightSpeed;
		float woodpeckerX = flightCenter.x + radius * cos(woodpeckerAngle);
		float woodpeckerZ = flightCenter.z + radius * sin(woodpeckerAngle);
		float woodpeckerYaw = glm::degrees(atan2(-sin(woodpeckerAngle), cos(woodpeckerAngle)));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(woodpeckerX, flightCenter.y, woodpeckerZ));
		model = glm::rotate(model, glm::radians(woodpeckerYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		glDisable(GL_CULL_FACE);
		Woodpecker.Draw(lightingShader);
		glEnable(GL_CULL_FACE);

		// 2. Peregrine Falcon (Halcón persiguiendo)
		float lagAngle = 0.35f; // Desfase para volar detrás
		float falconAngle = woodpeckerAngle - lagAngle;
		float falconX = flightCenter.x + radius * cos(falconAngle);
		float falconZ = flightCenter.z + radius * sin(falconAngle);
		float falconYaw = glm::degrees(atan2(-sin(falconAngle), cos(falconAngle)));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(falconX, flightCenter.y + 0.3f, falconZ));
		model = glm::rotate(model, glm::radians(falconYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		glDisable(GL_CULL_FACE);
		PeregrineFalcon.Draw(lightingShader);
		glEnable(GL_CULL_FACE);

		// Lamps
		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[1]);
		model = glm::scale(model, glm::vec3(0.10f, 0.16f, 0.10f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(lampShader.Program, "objectColor"), 1.0f, 0.72f, 0.35f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		nightSky.Draw(skyboxShader, view, projection);

		glfwSwapBuffers(window);
	}

	ma_sound_uninit(&campanaAudio);
	ma_sound_uninit(&musica);
	ma_engine_uninit(&engine);

	glfwTerminate();
	return 0;
}

void DrawSidewalk(Shader shader, GLuint VAO, glm::vec3 position, glm::vec3 size, glm::vec3 color)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, size);

	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), color.r, color.g, color.b);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


//COLISIONES 

struct CajaColision
{
	float minX;
	float maxX;
	float minZ;
	float maxZ;
};

bool HayColisionConObjeto(float x, float z)
{
	// Radio aproximado del perro
	const float radioPerro = 0.35f;

	// Hitboxes de los objetos del escenario
	const CajaColision objetos[] =
	{
		// Puesto de comida
		{ 1.15f, 4.85f, -1.5f, 1.5f },

		// Lampara
		{ -2.75f, -1.25f, -0.85f, 0.85f },

		// Puesto de frutas
		{ -7.8f, -4.2f, -3.8f, -0.2f },

		// Carrito de hot dogs
		{ -2.6f, -0.4f, -7.1f, -4.9f },

		// Campana
		{ -1.5f, 1.5f, 9.5f, 12.5f },

		// Faro
		{ 3.5f, 4.5f, -0.6f, 0.7f },

		// Angel de la Independencia
		{ -2.5f, 2.5f, -57.0f, -50.0f },

		// Bandera de México
		{ 2.3f, 6.7f, -6.5f, -12.0f },
	};

	const int cantidadObjetos =
		sizeof(objetos) / sizeof(objetos[0]);

	for (int i = 0; i < cantidadObjetos; i++)
	{
		float puntoX = glm::clamp(
			x, objetos[i].minX, objetos[i].maxX
		);

		float puntoZ = glm::clamp(
			z, objetos[i].minZ, objetos[i].maxZ
		);

		float distanciaX = x - puntoX;
		float distanciaZ = z - puntoZ;

		float distanciaCuadrada =
			distanciaX * distanciaX +
			distanciaZ * distanciaZ;

		if (distanciaCuadrada <= radioPerro * radioPerro)
		{
			return true;
		}
	}

	return false;
}



void DoMovement()
{
	bool moving = false;
	const float moveSpeed = 3.0f;
	const float turnSpeed = 100.0f;
	float dt = deltaTime;

	if (AnimFox)
	{
		const float foxSpeed = 3.0f;
		glm::vec3 toWaypoint = foxRoute[foxNextWaypoint] - foxPos;
		float distanceToWaypoint = glm::length(toWaypoint);
		float stepDistance = foxSpeed * dt;
		if (distanceToWaypoint <= stepDistance)
		{
			foxPos = foxRoute[foxNextWaypoint];
			foxNextWaypoint = (foxNextWaypoint + 1) % foxRouteCount;
		}
		else if (distanceToWaypoint > 0.0f)
		{
			glm::vec3 direction = toWaypoint / distanceToWaypoint;
			foxPos += direction * stepDistance;
			foxYaw = glm::degrees(atan2(direction.x, direction.z));
		}
	}

	if (keys[GLFW_KEY_A]) dogYaw += turnSpeed * dt;
	if (keys[GLFW_KEY_D]) dogYaw -= turnSpeed * dt;

	float yawRad = glm::radians(dogYaw);
	glm::vec3 forward(sin(yawRad), 0.0f, cos(yawRad));


	// Movimiento con deteccion de colisiones
	glm::vec3 desplazamiento(0.0f);

	if (keys[GLFW_KEY_W])
	{
		desplazamiento += forward;
	}

	if (keys[GLFW_KEY_S])
	{
		desplazamiento -= forward;
	}

	// Evitar que el movimiento diagonal sea mas rapido
	if (glm::length(desplazamiento) > 0.0f)
	{
		desplazamiento = glm::normalize(desplazamiento);
	}

	desplazamiento *= moveSpeed * dt;

	// Comprobar movimiento en X
	float nuevoX = dogPos.x + desplazamiento.x;

	if (!HayColisionConObjeto(nuevoX, dogPos.z))
	{
		if (nuevoX != dogPos.x)
			moving = true;

		dogPos.x = nuevoX;
	}

	// Comprobar movimiento en Z
	float nuevoZ = dogPos.z + desplazamiento.z;

	if (!HayColisionConObjeto(dogPos.x, nuevoZ))
	{
		if (nuevoZ != dogPos.z)
			moving = true;

		dogPos.z = nuevoZ;
	}


	dogRot = dogYaw;
	dogAnim = moving ? 1 : 0;

	float limiteX = 15.0f;
	dogPos.x = glm::clamp(dogPos.x, -limiteX, limiteX);

	if (dogPos.x >= -4.0f && dogPos.x <= 4.0f)
	{
		dogPos.z = glm::clamp(dogPos.z, -65.0f, 15.0f);
	}
	else
	{
		dogPos.z = glm::clamp(dogPos.z, -15.0f, 15.0f);
	}

	dogPos.y = -1.5f;

	// Alternar vista panoramica con C
	if (keys[GLFW_KEY_C] && !teclaCAntes)
	{
		vistaPanoramica = !vistaPanoramica;
	}

	teclaCAntes = keys[GLFW_KEY_C];

	// Camara siguiendo al perrito o vista panoramica
	glm::vec3 cameraPos;
	glm::vec3 lookTarget;

	if (vistaPanoramica)
	{
		// Vista panoramica: mas alejada 
		cameraPos = dogPos - forward * 10.5f
			+ glm::vec3(0.0f, 4.0f, 0.0f);

		lookTarget = dogPos + glm::vec3(0.0f, 1.5f, 0.0f);
	}
	else
	{
		// Vista normal: sigue al perrito
		cameraPos = dogPos - forward * cameraDistance
			+ glm::vec3(0.0f, cameraHeight, 0.0f);

		lookTarget = dogPos + glm::vec3(0.0f, 0.35f, 0.0f);
	}

	camera.SetPositionAndLookAt(cameraPos, lookTarget);

	if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
	if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;
	if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
	if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;
	if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
	if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) keys[key] = true;
		else if (action == GLFW_RELEASE) keys[key] = false;
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		active = !active;
		Light1 = active ? glm::vec3(0.2f, 0.8f, 1.0f) : glm::vec3(0.0f);
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		AnimFox = !AnimFox;
	}
	if (keys[GLFW_KEY_B])
	{
		dogAnim = 1;
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		lucesFachadaActivas = !lucesFachadaActivas;
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		lucesDecorativasActivas = !lucesDecorativasActivas;
	}
}

void Animation()
{
	if (AnimBall) rotBall += 0.4f;
	if (AnimDog) rotDog -= 0.6f;

	if (dogAnim == 1)
	{
		walkTime += deltaTime;
		float movement = sin(walkTime * 8.0f);
		FLegs = movement * 28.0f;
		RLegs = movement * 28.0f;
		head = sin(walkTime * 8.0f) * 2.0f;
		tail = sin(walkTime * 10.0f) * 5.0f;
	}
	else
	{
		FLegs = 0.0f;
		RLegs = 0.0f;
		head = 0.0f;
		tail = 0.0f;
		walkTime = 0.0f;
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
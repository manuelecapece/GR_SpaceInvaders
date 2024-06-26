#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <irrklang/irrKlang.h>
#include "render_text.h"
#include "shader_m.h"
#include "camera.h"
#include "model.h"

#include <iostream>
#include <random>
#include <cmath>
#include <stack>

#pragma comment(lib, "irrKlang.lib") 

#include "alieno.h"
#include "navicella.h"
#include "proiettile.h"
#include "ufo.h"
#include "barriera.h"
#include "roccia.h"
#include "pianeta.h"
#include "esplosione.h"
#include "suono.h"

//Dichiarazione classi
Alieno alieno;
Navicella navicella;
Proiettile proiettileNavicella;
Proiettile proiettileUfo;
Proiettile proiettileSpeciale;
Ufo ufo;
Barriera barriera;
Roccia roccia;
Pianeta pianeta;
Esplosione esplosione;
Suono suono;

//Dichiarazione shader
Shader frecciaShader;
Shader alienoShader;
Shader proiettileShader;
Shader barrieraShader;
Shader navicellaShader;
Shader ufoRetroShader;
Shader rocciaShader;
Shader pianetaShader;
Shader blendingShader;
Shader stencilShader;

//Dichiarazione modelli
Model modelFreccia;
Model modelSfera;
Model modelCubo;
Model modelNavicella;
Model modelUfoRetro;
Model modelAlieno1;
Model modelAlieno2;
Model modelAlieno3;
Model modelAlieno4;
Model modelAlieno5;
Model modelRoccia;
Model modelPianeta1;
Model modelPianeta2;
Model modelPianeta3;
Model modelPianeta4;
Model modelPianeta5;


float random_x;

float startTimeDelta = 5.0f;
double stSparoUfo = glfwGetTime();
double stSparoAlieni  = glfwGetTime();
double startTime2s  = glfwGetTime();
double startTime = glfwGetTime();
double stSpawnUfo = glfwGetTime();
double startTimexs = glfwGetTime();
double startTimeGameOver = 0;
double currentxs;
double deltaxs = 0;
float deltaSparoAlieni = 2.0f;
float intervallo = 1.5f;
int stepDx = 1;
int stepSx = 1;
bool restart = false;
int score = 0;
int record = 0;
float gameOverScale = 0.1f;
float gameOverGrowthRate = 0.02f; // La velocità con cui la scritta aumenta di dimensione


//Dichiarazione matrici di trasformazione
//glm::mat4 view = glm::mat4(1.0f);	//identity matrix;
glm::mat4 projection = glm::mat4(1.0f);	//identity matrix

//Dichiarazione metodi
void renderQuad();
void renderQuad2();
void render(Shader shaderBlur, Shader shaderBloomFinal);
float generaNumeroCasualeFloat(float estremoInferiore, float estremoSuperiore);
float generaNumeroCasualeInt(int estremoInferiore, int estremoSuperiore);
void muoviAlieni();
void muoviCamera(float deltaTime);
void checkGameWin();
void checkGameLost();
void ripristinaCameraPos();
void ripristinaGioco();
int leggiScoreDalFile(const std::string& nomeFile);
void aggiornaScoreSeMaggiore(const std::string& nomeFile);
void checkNavicellaIsInvincibile();
void renderText();

const float PI = 3.14159265358979323846;

// settings
int SCR_WIDTH = 1920;
int SCR_HEIGHT = 1080;

//// settings
//int SCR_WIDTH = 2560;
//int SCR_HEIGHT = 1440;

//Bloom settings
bool bloom = true;
bool bloomKeyPressed = false;
float exposure = 1.0f;
unsigned int hdrFBO;
unsigned int pingpongFBO[2];
unsigned int pingpongColorbuffers[2];
unsigned int colorBuffers[2];

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

std::stack<glm::mat4> glm_ModelViewMatrix;

vector<glm::vec3> windows
{
	glm::vec3(-1.5f, 0.0f, -0.48f),
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
	glm::vec3(-0.3f, 0.0f, -2.3f),
	glm::vec3(0.5f, 0.0f, -0.6f)
};

float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

int vista;

glm::vec3 cameraPos;  // Posizione camera
glm::vec3 cameraAt;	// Punto in cui "guarda" la camera
glm::vec3 cameraUp(0.0, 1.0, 0.0); // Vettore up...la camera e sempre parallela al piano
glm::vec3 cameraDir(0.0, 0.0, -0.1); // Direzione dello sguardo
glm::vec3 cameraSide(1.0, 0.0, 0.0); // Direzione spostamento laterale

float speed = navicella.getSpeed();

unsigned int cubeVAO;
unsigned int cubeVBO;

//Timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//Mosse possibili
bool moveLeft = false;
bool moveRight = false;
bool spara = true;
bool exitGame = false;
bool respawnNavicella = false;

unsigned int frameCount = 0;
double previousTime = 0;
double timeInterval = 0;
unsigned int fps = 0;

// Creazione di un vettore di pianeti
std::vector<Pianeta> pianeti(5);


void calculateFPS() {
	//  Incrementiamo il contatore
	frameCount++;
	//  Determiniamo il numero di millisecondi trascorsi dalla glutInit
	double currentTime3s = glfwGetTime();
	//  Calcoliamo il tempo trascorso
	timeInterval = currentTime3s - previousTime;

	// Se e passato un secondo aggiorna la variabile fps
	if (timeInterval > 1.0f) {
		//  frameCount mantiene il numero di frame generati in un secondo
		fps = frameCount;

		//  Salviamo il tempo trascorso per riutilizzarlo la prossima volta
		previousTime = currentTime3s;

		//  Azzeriamo il contatore dei tempi
		frameCount = 0;
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || exitGame == true) {
		glfwSetWindowShouldClose(window, true);
		//suono.dropSoundEngine();
		aggiornaScoreSeMaggiore("../src/score.txt");
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveLeft = true;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveRight = true;

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {

	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (spara && !navicella.getIsHitted() && alieno.getSpawnaAlieni()) {
			navicella.inizializzaProiettile(proiettileNavicella, suono);
			navicella.inizializzaProiettileSpeciale(proiettileSpeciale, alieno.getLivello());
			spara = false;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
		moveLeft = false;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
		moveRight = false;
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {

	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		spara = true;
	}
		
}

int leggiScoreDalFile(const std::string& nomeFile) {
	int scoreDalFile = 0;
	std::ifstream file(nomeFile);

	if (file.is_open()) {
		file >> scoreDalFile;
		file.close();
	}
	else {
		std::cerr << "Errore nell'apertura del file " << nomeFile << std::endl;
	}

	return scoreDalFile;
}

void aggiornaScoreSeMaggiore(const std::string& nomeFile) {
	int scoreDalFile = leggiScoreDalFile(nomeFile);
	
	if (score > scoreDalFile) {
		std::ofstream file(nomeFile);

		if (file.is_open()) {
			file << score;
			file.close();
			std::cout << "Score aggiornato nel file " << nomeFile << std::endl;
		}
		else {
			std::cerr << "Errore nell'apertura del file " << nomeFile << std::endl;
		}
	}
	else {
		std::cout << "Il valore dello score non e' maggiore di quello nel file. Nessun aggiornamento effettuato." << std::endl;
	}
}

void idle()
{
	suono.soundGameStart();
	double ctSparoUfo = glfwGetTime(); 
	double ctSparoAlieni = glfwGetTime();
	double currentTime2s = glfwGetTime();
	double ctSpawnUfo = glfwGetTime();

	double currentTime = glfwGetTime();
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	double deltaTimeExecute = currentTime - startTime;

	score = alieno.getScore() + ufo.getScore();

	navicella.setTranslateSpeed(navicella.getSpeed() * deltaTime);
	ufo.setTranslateSpeed(ufo.getSpeed() * deltaTime);
	alieno.setTranslateSpeedx(alieno.getSpeedx() * deltaTime);
	alieno.setTranslateSpeedz(alieno.getSpeedz() * deltaTime);
	proiettileNavicella.setTranslateSpeed(proiettileNavicella.getSpeed() * deltaTime);
	proiettileSpeciale.setTranslateSpeed(proiettileSpeciale.getSpeed() * deltaTime);
	proiettileUfo.setTranslateSpeed(proiettileUfo.getSpeed() * deltaTime);
	alieno.setTranslateSpeedProiettili(deltaTime);
	esplosione.setTranslateSpeed(esplosione.getSpeed() * deltaTime);

	//Inizia il gioco dopo startTimeDelta secondi
	if (deltaTimeExecute >= startTimeDelta) {


		if (ctSparoUfo - stSparoUfo >= 0.5) {
			ufo.inizializzaProiettile(proiettileUfo);
			stSparoUfo = ctSparoUfo;
		}

		if (ctSparoAlieni - stSparoAlieni >= deltaSparoAlieni && alieno.getSpawnaAlieni()) {
			for (int id_colonna = 0; id_colonna < alieno.getColonneAlieni(); id_colonna++) {
				int id_riga = generaNumeroCasualeInt(0, alieno.getRigheAlieni()-1);
				//if (alieno.getMap()[id_riga][id_colonna] != 0) {
				//	suono.soundSparoAlieno();
				//}
				alieno.inizializzaProiettili(proiettileShader, modelCubo, id_riga, id_colonna);
				stSparoAlieni = ctSparoAlieni;
			}
		}

		muoviAlieni();

		if (ctSpawnUfo - stSpawnUfo >= 30.0f) {
			ufo.ripristinaPosizioneIniziale();
			stSpawnUfo = ctSpawnUfo;
		}

	}

	if (navicella.getIsHitted() && (glfwGetTime() - navicella.getStartTimeHitted()) > 1.0f) {
		respawnNavicella = true;
		navicella.setIsInvincibile(true);
	}

	if (!alieno.getSpawnaAlieni() && (glfwGetTime() - alieno.getStartTimeLoadNewLevel() > 3.0f)) {
		ripristinaGioco();
	}


	if (vista == 1) {
		muoviCamera(deltaTime);
	}

	if (respawnNavicella && (vista == 0 || vista == 1) && navicella.getVite() >= 0) {
		navicella.setHisHitted(false);
		respawnNavicella = false;
		ripristinaCameraPos();
	}

	checkNavicellaIsInvincibile();
	checkGameWin();
	checkGameLost();

}

void checkNavicellaIsInvincibile() {
	if (!navicella.getIsInvincibile()) {
		return;
	}

	if (navicella.getIsInvincibile() && glfwGetTime() - navicella.getStartTimeHitted() > 4.0f) {
		navicella.setIsInvincibile(false);
	}
}

void ripristinaGioco() {
	barriera.ripristina();
	barriera.inizializzaMaps();
	navicella.ripristinaPosizioneIniziale();
	ripristinaCameraPos();
	alieno.setSpawnaAlieni(true);
	alieno.setMuoviVersoDx(true);
	startTime = glfwGetTime();
	stSparoUfo = glfwGetTime();
	stSparoAlieni = glfwGetTime();
	stSpawnUfo = glfwGetTime();

	if (deltaSparoAlieni > 0.5f) {
		deltaSparoAlieni = deltaSparoAlieni - 0.25f;
	}
	stepDx = 1;
	stepSx = 1;
	intervallo = 1.5f;

	proiettileNavicella.ripristinaColpiSparati();
	proiettileUfo.ripristinaColpiSparati();
	ufo.ripristinaPos();
	alieno.inizializzaBonus();
	alieno.inizializzaMapHitted();
	proiettileSpeciale.ripristinaColpiSpeciali();
	suono.ripristina();
}

void ripristinaCameraPos() {

	if (vista == 1) {
		cameraPos = glm::vec3(0.0f, 6.5f, 17.5f);
		cameraAt = glm::vec3(0.0, 0.0, 0.0);
		cameraUp = glm::vec3(0.0, 1.0, 0.0);
	}
}

void checkGameLost() {
	if (navicella.getVite() < 0 && startTimeGameOver == 0) {
		startTimeGameOver = glfwGetTime();
	}else if (glfwGetTime() - startTimeGameOver > 0.5f && startTimeGameOver != 0) {
		suono.soundGameOver();
	}
}

void checkGameWin() {
	if (alieno.getAlieniEliminati() == (alieno.getColonneAlieni() * alieno.getRigheAlieni())) {
 		//cout << "Hai vinto!" << endl;
		ufo.setSpeed(0.0f);
		navicella.setSpeed(0.0f);
	}
}

void muoviCamera(float deltaTime) {
	float cameraSpeed = speed * deltaTime;
	//cameraPos.x = navicella.getPos().x;

	if (respawnNavicella && vista == 1 && navicella.getVite() >= 0) {
		cameraPos = glm::vec3(0.0f, 6.5f, 17.5f);
		cameraAt = glm::vec3(0.0, 0.0, 0.0);
		cameraUp = glm::vec3(0.0, 1.0, 0.0);
		navicella.setHisHitted(false);
		respawnNavicella = false;
	}

	if (moveRight && !navicella.getIsHitted())
	{
		cameraPos.x = cameraPos.x + cameraSpeed;
		cameraAt.x = cameraAt.x + cameraSpeed;
		cameraUp.x = cameraUp.x + cameraSpeed/100;
		cameraUp = normalize(cameraUp);
	}

	if (moveLeft && !navicella.getIsHitted())
	{
		cameraPos.x = cameraPos.x - cameraSpeed;
		cameraAt.x = cameraPos.x - cameraSpeed;
		cameraUp.x = cameraUp.x - cameraSpeed / 100;
		cameraUp = normalize(cameraUp);
	}
}

void muoviAlieni() {

	if (alieno.getMuoviVersoDx()) {
		
		if (alieno.getSpeedx() == 0 && !restart ) {
			currentxs = glfwGetTime();
			deltaxs = (currentxs - startTimexs) - startTimeDelta;
			restart = true;
		}

		double current = glfwGetTime();
		double delta = current - currentxs;

		if (delta > (intervallo) && restart) {
			alieno.stepVersoDx(stepDx);

			if (alieno.getSpeedx() == 0.0f) {
				stepDx++;
				suono.setPlayMovimentoAlieni(true);
				alieno.setSuono(suono);
				restart = false;

			}

		}

	}

	if (alieno.getMuoviVersoDown()) {

		double current = glfwGetTime();
		double delta = current - currentxs;

		if (delta > (intervallo * (stepDx + stepSx - 1))) {
			suono.soundMovimentoAlieni();
			alieno.stepVersoDown(intervallo);
			stepDx = -1;
			stepSx = 1;

		}
		else {
			suono.setPlayMovimentoAlieni(true);
			alieno.setSuono(suono);
		}

	}

	if (alieno.getMuoviVersoSx()) {
		
		if (alieno.getSpeedx() == 0 && !restart) {
			currentxs = glfwGetTime();
			restart = true;
		}

		double current = glfwGetTime();
		double delta = current - currentxs;

		if (delta > (intervallo) && restart) {
			alieno.stepVersoSx(stepSx);

			if (alieno.getSpeedx() == 0.0f) {
				stepSx++;
				suono.setPlayMovimentoAlieni(true);
				alieno.setSuono(suono);
				restart = false;
			}
		}
	}

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// load and create a texture standard per jpg
unsigned int loadTexture1(std::string filename)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return -1;
	}
	stbi_image_free(data);

	return texture;
}

// load and create a texture per le trasparenti, le carica storte
unsigned int loadTexture2(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// utility function for loading a 2D texture from file con gammaCorrection
unsigned int loadTexture3(char const* path, bool gammaCorrection)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		if (nrComponents == 1)
		{
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

int main()
{
	record = leggiScoreDalFile("../src/score.txt");

	bool schermoIntero = true;

	vista = 1;

	if (vista == 0) {
		//Vista isometrica frontale dall'alto
		cameraPos = glm::vec3(0.0f, 42.0f, -7.0f);
		cameraAt = glm::vec3(0.0f, 0.0f, -7.1f);
	}
	
	if (vista == 1) {
		//Vista dinamica frontale 
		cameraPos = glm::vec3(0.0f, 6.5f, 17.5f);
		cameraAt = glm::vec3(0.0, 0.0, 0.0);
	}

	suono.inizializza();

	const GLFWvidmode* videoMode = NULL;
	GLFWmonitor* primaryMonitor = NULL;

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	if (schermoIntero) {
		// Ottenere il monitor primario
		primaryMonitor = glfwGetPrimaryMonitor();
		// Ottenere la modalita video corrente del monitor primario
		videoMode = glfwGetVideoMode(primaryMonitor);
		SCR_WIDTH = videoMode->width;
		SCR_HEIGHT = videoMode->height;
	}

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL 3.3 - Space invaders!", primaryMonitor, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (schermoIntero) {
		// Impostare la finestra in modalita schermo intero
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	initRenderText(SCR_WIDTH, SCR_HEIGHT);

	// load models
	modelFreccia = Model("../src/models/freccia/freccia.obj");
	modelSfera = Model("../src/models/sfera/sphere.obj");
	modelCubo = Model("../src/models/cubo.obj");
	modelNavicella = Model("../src/models/navicella/navicella.obj");
	modelUfoRetro = Model("../src/models/retroUfo/retroUfo.obj");
	modelAlieno1 = Model("../src/models/alieni/alieno1/alieno1.obj");
	modelAlieno2 = Model("../src/models/alieni/alieno2/alieno2.obj");
	modelAlieno3 = Model("../src/models/alieni/alieno3/alieno3.obj");
	modelAlieno4 = Model("../src/models/alieni/alieno4/alieno4.obj");
	modelAlieno5 = Model("../src/models/alieni/alieno5/alieno5.obj");
	modelRoccia = Model("../src/models/roccia/roccia.obj");
	modelPianeta1 = Model("../src/models/pianeti/pianeta1/pianeta1.obj");
	modelPianeta2 = Model("../src/models/pianeti/pianeta2/pianeta2.obj");
	modelPianeta3 = Model("../src/models/pianeti/pianeta3/pianeta3.obj");
	modelPianeta4 = Model("../src/models/pianeti/pianeta4/pianeta4.obj");
	modelPianeta5 = Model("../src/models/pianeti/pianeta5/pianeta5.obj");


	//Shader
	frecciaShader = Shader("freccia.vs", "freccia.fs");
	alienoShader = Shader("alieno.vs", "alieno.fs");
	proiettileShader = Shader("proiettile.vs", "proiettile.fs");
	barrieraShader = Shader("barriera.vs", "barriera.fs");
	navicellaShader = Shader("navicella.vs", "navicella.fs");
	ufoRetroShader = Shader("ufoRetro.vs", "ufoRetro.fs");
	rocciaShader = Shader("roccia.vs", "roccia.fs");
	pianetaShader = Shader("pianeta.vs", "pianeta.fs");
	blendingShader = Shader("blending.vs", "blending.fs");
	stencilShader = Shader("stencilTesting.vs", "stencilTesting.fs");

	
	// build and compile shaders
	// -------------------------
	Shader shaderBlur("blur.vs", "blur.fs");
	Shader shaderBloomFinal("bloom_final.vs", "bloom_final.fs");


	// configure (floating point) framebuffers
	// ---------------------------------------
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);



	// create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)

	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}

	// Crea il renderbuffer per depth e stencil
	unsigned int rboDepthStencil;
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);


	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ping-pong-framebuffer for blurring

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}



	//Binding per mattoni con texture diffuse e speculari
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Operazioni finali
	glBindVertexArray(0);

	// create transformations
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(cameraPos, cameraAt, cameraUp);

	//draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	frecciaShader.use();
	frecciaShader.setMat4("projection", projection);

	alienoShader.use();
	alienoShader.setMat4("projection", projection);

	proiettileShader.use();
	proiettileShader.setMat4("projection", projection);

	barrieraShader.use();
	barrieraShader.setMat4("projection", projection);

	navicellaShader.use();
	navicellaShader.setMat4("projection", projection);

	ufoRetroShader.use();
	ufoRetroShader.setMat4("projection", projection);

	rocciaShader.use();
	rocciaShader.setMat4("projection", projection);

	pianetaShader.use();
	pianetaShader.setMat4("projection", projection);

	blendingShader.use();
	blendingShader.setMat4("projection", projection);

	stencilShader.use();
	stencilShader.setMat4("projection", projection);


	// shader configuration
	// --------------------
	shaderBlur.use();
	shaderBlur.setInt("image", 0);

	shaderBloomFinal.use();
	shaderBloomFinal.setInt("scene", 0);
	shaderBloomFinal.setInt("bloomBlur", 1);

	// Creo le classi

	alieno.setShader(alienoShader);
	alieno.setModelSfera(modelSfera);
	alieno.setModels(modelAlieno1, modelAlieno2, modelAlieno3, modelAlieno4, modelAlieno5);
	alieno.inizializzaBonus();
	alieno.inizializzaMapHitted();
	alieno.setSuono(suono);
	alieno.setBonusShader(stencilShader);

	navicella.setShader(navicellaShader);
	navicella.setShaderStencil(stencilShader);
	navicella.setModel(modelNavicella);
	navicella.setModelSfera(modelSfera);
	navicella.setSuono(suono);
	navicella.setBonusShader(blendingShader);

	ufo.setShader(ufoRetroShader);
	ufo.setModel(modelUfoRetro);
	ufo.setModelSfera(modelSfera);
	ufo.setSuono(suono);

	roccia.setShader(rocciaShader);
	roccia.setModel(modelRoccia);

	// Inizializzazione dei pianeti con posizione, scala, modello, shader, velocità di rotazione e orbita
	pianeti[0].setShader(pianetaShader);
	pianeti[0].setModel(modelPianeta1);
	pianeti[0].setPosition(glm::vec3(-25.0f, 2.0f, -11.0f)); // Posizione di esempio
	pianeti[0].setScale(1.0f); // Scala di esempio
	pianeti[0].setRotationSpeed(100.0f); // Velocità di rotazione del primo pianeta
	pianeti[0].setOrbitSpeed(15.0f); // Velocità di orbita del primo pianeta

	pianeti[1].setShader(pianetaShader);
	pianeti[1].setModel(modelPianeta2);
	pianeti[1].setPosition(glm::vec3(-15.0f, 7.0f, -21.0f)); // Posizione di esempio
	pianeti[1].setScale(1.5f); // Scala di esempio
	pianeti[1].setRotationSpeed(60.0f); // Velocità di rotazione del secondo pianeta
	pianeti[1].setOrbitSpeed(10.0f); // Velocità di orbita del secondo pianeta

	pianeti[2].setShader(pianetaShader);
	pianeti[2].setModel(modelPianeta3);
	pianeti[2].setPosition(glm::vec3(0.0f, -5.0f, -26.0f)); // Posizione di esempio
	pianeti[2].setScale(2.0f); // Scala di esempio
	pianeti[2].setRotationSpeed(25.0f); // Velocità di rotazione del terzo pianeta
	pianeti[2].setOrbitSpeed(12.0f); // Velocità di orbita del terzo pianeta

	pianeti[3].setShader(pianetaShader);
	pianeti[3].setModel(modelPianeta4);
	pianeti[3].setPosition(glm::vec3(18.0f, -9.0f, -21.0f)); // Posizione di esempio
	pianeti[3].setScale(2.5f); // Scala di esempio
	pianeti[3].setRotationSpeed(35.0f); // Velocità di rotazione del quarto pianeta
	pianeti[3].setOrbitSpeed(8.0f); // Velocità di orbita del quarto pianeta

	pianeti[4].setShader(pianetaShader);
	pianeti[4].setModel(modelPianeta5);
	pianeti[4].setPosition(glm::vec3(25.0f, 7.0f, -23.0f)); // Posizione di esempio
	pianeti[4].setScale(3.0f); // Scala di esempio
	pianeti[4].setRotationSpeed(18.0f); // Velocità di rotazione del quinto pianeta
	pianeti[4].setOrbitSpeed(14.0f); // Velocità di orbita del quinto pianeta


	proiettileNavicella.setShader(proiettileShader);
	proiettileNavicella.setModel(modelCubo);
	proiettileSpeciale.setShader(proiettileShader);
	proiettileSpeciale.setModel(modelCubo);
	proiettileSpeciale.setAltezza(proiettileSpeciale.getAltezza() * 1.5f);
	proiettileSpeciale.setLunghezza(proiettileSpeciale.getLunghezza() * 1.5f);
	proiettileSpeciale.setLarghezza(proiettileSpeciale.getLarghezza() * 1.5f);

	proiettileUfo.setShader(proiettileShader);
	proiettileUfo.setModel(modelCubo);
	proiettileUfo.setSpeed(4.0f);

	esplosione.setShader(barrieraShader);
	esplosione.setModel(modelCubo);
	esplosione.setSuono(suono);

	barriera.setShader(barrieraShader);
	barriera.setModel(modelCubo);
	barriera.setPosX(alieno.getRaggio() * 2, alieno.getSpazio());
	barriera.setSpazio(alieno.getRaggio() * 2, alieno.getSpazio());
	barriera.inizializzaMaps();
	barriera.setSuono(suono);

	//float limX_pos = alieno.getPos().x + 5 * alieno.getRaggio() * 2.0f * alieno.getSpazio();
	//navicella.setLimXpos(limX_pos);


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		
		// input
		processInput(window);
		idle();

		// create transformations
		view = glm::lookAt(cameraPos, cameraAt, cameraUp);

		frecciaShader.use();
		frecciaShader.setMat4("view", view);
		alienoShader.use();
		alienoShader.setMat4("view", view);
		proiettileShader.use();
		proiettileShader.setMat4("view", view);
		barrieraShader.use();
		barrieraShader.setMat4("view", view);
		navicellaShader.use();
		navicellaShader.setMat4("view", view);
		ufoRetroShader.use();
		ufoRetroShader.setMat4("view", view);
		rocciaShader.use();
		rocciaShader.setMat4("view", view);
		roccia.update(deltaTime);
		pianetaShader.use();
		pianetaShader.setMat4("view", view);
		blendingShader.use();
		blendingShader.setMat4("view", view);
		blendingShader.setFloat("alpha", 0.2);
		stencilShader.use();
		stencilShader.setMat4("view", view);
		//stencilShader.setVec3("color", glm::vec3(1.0f, 1.0f, 0.26f));
		
		for (auto& pianeta : pianeti) {
			pianeta.update(deltaTime);
		}

		render(shaderBlur, shaderBloomFinal);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
		calculateFPS();
		std::string framePerSecond = std::to_string(fps);
		glfwSetWindowTitle(window, framePerSecond.c_str());
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

void checkCollisioneAlieniBarriere() {
	for (int i = 0; i < alieno.getRigheAlieni(); i++){
		for (int j = 0; j < alieno.getColonneAlieni(); j++){

			if (alieno.getMap()[i][j] != 0){

				float x = (alieno.getPos().x + j * alieno.getRaggio() * 2.0f * alieno.getSpazio());
				float z = (alieno.getPos().z + i * alieno.getRaggio() * 2.0f * alieno.getSpazio());
				glm::vec3 posAlieno = glm::vec3(x, 0.0f, z);

				for (int k = 0; k < 3; k++) {
					glm::vec3 posBarriera = glm::vec3(barriera.getPosX() + k * barriera.getSpazio(), 0.0f, barriera.getPosZ());

					if (k == 0) {
						barriera.checkCollisioneBarrieraAlieno(1, posAlieno, posBarriera, alieno.getRaggio());
					}

					if (k == 1) {
						barriera.checkCollisioneBarrieraAlieno(2, posAlieno, posBarriera, alieno.getRaggio());
					}

					if (k == 2) {
						barriera.checkCollisioneBarrieraAlieno(3, posAlieno, posBarriera, alieno.getRaggio());
					}
				}

			}
		}
	}
}


void render(Shader shaderBlur, Shader shaderBloomFinal)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // don't forget to clear the stencil buffer!
	// 1. render scene into floating point framebuffer
	// -----------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	//alieno.render(proiettileNavicella, navicella, esplosione);
	for (auto& pianeta : pianeti) {
		pianeta.render();
	}

	barriera.renderBarriere(proiettileNavicella);
	barriera.renderBarriere(proiettileSpeciale);
	barriera.renderBarriere(proiettileUfo);

	proiettileNavicella.render(glm::vec3(1.0f, 1.0f, 1.0f));
	proiettileSpeciale.render(glm::vec3(1.0f, 0.0f, 0.0f));
	navicella.render(moveRight, moveLeft, proiettileSpeciale);

	alieno.render(proiettileNavicella, proiettileSpeciale, navicella, esplosione);
	alieno.renderProiettili(navicella, barriera, esplosione);

	roccia.render();

	ufo.render(esplosione);
	proiettileUfo.render(glm::vec3(0.0f, 1.0f, 1.0f));
	navicella.checkIsHitted(proiettileUfo,esplosione, alieno.getSpawnaAlieni());
	ufo.checkIsHitted(proiettileNavicella);
	ufo.checkIsHitted(proiettileSpeciale);

	esplosione.render();
	proiettileSpeciale.checkColpiBonus(navicella.getIsHitted());
	checkCollisioneAlieniBarriere();


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. blur bright fragments with two-pass Gaussian Blur 
	// --------------------------------------------------
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	shaderBlur.use();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		shaderBlur.setInt("horizontal", horizontal);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		renderQuad2();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
	// --------------------------------------------------------------------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderBloomFinal.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
	shaderBloomFinal.setInt("bloom", bloom);
	shaderBloomFinal.setFloat("exposure", exposure);
	renderQuad2();

	glDisable(GL_DEPTH_TEST);
	
	renderText();

	glEnable(GL_DEPTH_TEST);
}

void renderText() {

	float centro = SCR_WIDTH / 2.0f;
	float delta = SCR_WIDTH / 5.0f;
	float puntoAltezza = SCR_HEIGHT - (SCR_HEIGHT / 10.0f);
	float dimensione = 0.5f * (SCR_HEIGHT/1000.0f);

	std::string viteNavicella = "LIFES:" + std::to_string(navicella.getVite());

	if (navicella.getVite() > 0) {
		RenderText(viteNavicella.c_str(), centro, puntoAltezza, dimensione, glm::vec3(1.0, 1.0f, 1.0f));
	}

	std::string recordScore = "RECORD:" + std::to_string(record);
	RenderText(recordScore.c_str(), centro + delta, puntoAltezza, dimensione, glm::vec3(1.0, 1.0f, 1.0f));

	std::string livello = "LEVEL:" + std::to_string(alieno.getLivello());
	RenderText(livello.c_str(), centro - delta, puntoAltezza, dimensione, glm::vec3(1.0, 1.0f, 1.0f));

	std::string punteggio = "SCORE:" + std::to_string(score);
	RenderText(punteggio.c_str(), centro - (delta * 2), puntoAltezza, dimensione, glm::vec3(1.0, 1.0f, 1.0f));

	if (navicella.getVite() == 0) {
		RenderText(viteNavicella.c_str(), centro, puntoAltezza, dimensione, glm::vec3(1.0, 0.0f, 0.0f));
	}

	if (navicella.getVite() < 0) {
		viteNavicella = "LIFES:" + std::to_string(navicella.getVite() + 1);
		RenderText(viteNavicella.c_str(), centro, puntoAltezza, dimensione, glm::vec3(1.0, 1.0f, 1.0f));

		// Game over logic
		float centerX = SCR_WIDTH / 2.0f;
		float centerY = SCR_HEIGHT / 2.0f;

		std::string gameOverText = "GAME OVER";
		RenderText(gameOverText.c_str(), centerX - 200 * gameOverScale, centerY, gameOverScale, glm::vec3(1.0, 1.0f, 1.0f));

		// Increase the size of the text
		gameOverScale += gameOverGrowthRate;

		// Stop growing after a certain size
		if (gameOverScale > dimensione * 4) {
			gameOverScale = dimensione * 4; // Max size
		}
	}
}

// renders a 1x1 quad in NDC with manually calculated tangent vectors
// ------------------------------------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		// positions
		glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
		glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
		glm::vec3 pos3(1.0f, -1.0f, 0.0f);
		glm::vec3 pos4(1.0f, 1.0f, 0.0f);
		// texture coordinates
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);
		// normal vector
		glm::vec3 nm(0.0f, 0.0f, 1.0f);

		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		// triangle 1
		// ----------
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		tangent1 = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
		bitangent1 = f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		tangent2 = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
		bitangent2 = f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);


		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};
		// configure plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// renderQuad2() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO2 = 0;
unsigned int quadVBO2;
void renderQuad2()
{
	if (quadVAO2 == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO2);
		glGenBuffers(1, &quadVBO2);
		glBindVertexArray(quadVAO2);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

float generaNumeroCasualeFloat(float estremoInferiore, float estremoSuperiore) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(estremoInferiore, estremoSuperiore);
	float random = dis(gen);
	return random;
}

float generaNumeroCasualeInt(int estremoInferiore, int estremoSuperiore) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(estremoInferiore, estremoSuperiore);
	int random = dis(gen);
	return random;
}





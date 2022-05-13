#include <iostream>
#include <cmath>

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

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion();
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
bool anim;
//variables para la animacion del Pterodactylo
float rotPtero = 0.0f;
float rotAlasptero = 0.0f;
float movPteroX = 0.0f;
float movPteroZ = 0.0f;
bool r1Ptero = true;
bool r2Ptero = false;
bool r3Ptero = false;
bool r4Ptero = false;
bool r5Ptero = false;
float desfase1 = 0.0f;
float desfase2 = 0.0f;
bool aleteoAbajo = true;
bool aleteoArriba = false;

//variables para la animacion del Megalodon
float movMegaX = 0.0f;
float movMegaZ = 0.0f;
bool r1Mega = true;
bool r2Mega = false;
bool r3Mega = false;
bool r4Mega = false;
bool r5Mega = false;
float rotMega = 0.0f;
float rotColaMega = 0.0f;
float tiempo; //variable para animar el agua
bool aleteoIzq = true;
bool aleteoDer = false;

//variables para la animacion del Trex
float movRexX = 0.0f;
float movRexZ = 0.0f;
float rotCuerpoRex = 0.0f;
float rotPiernaRex = 0.0f;
float rotPataRex = 0.0f;
float rotMandibulaRex = 0.0f;
bool caminarRex = true;
bool paso1Rex = true;
bool paso2Rex = false;
bool rugidoRex = false;
bool inclinacionAbajoRex = true;
bool inclinacionArribaRex = false;

//variables para la aniacion del raptor
float movRaptorX = 0.0f;
float movRaptorZ = 0.0f;
float rotCuerpoRaptor = 0.0f;
float rotPiernaRaptor = 0.0f;
float rotPataRaptor = 0.0f;
float rotMandibulaRaptor = 0.0f;
bool caminarRaptor = true;
bool paso1Raptor = true;
bool paso2Raptor = false;
bool comerRaptor = false;
bool inclinacionAbajoRaptor = true;
bool inclinacionArribaRaptor = false;
bool mordidaRaptor = false;
bool mordidaArriba = true;
bool mordidaAbajo = false;


// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(5.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  5.0f),
	glm::vec3(5.0f,0.0f, 5.0f)
};

glm::vec3 spotlightPosition(2.5f, 0.0f, 2.5f);
glm::vec3 spotlightDir(0.0f, -1.0f, 0.0f);

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
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);
glm::vec3 Light4 = glm::vec3(0);

glm::vec3 PteroPosIni = glm::vec3(75.0f,13.0f,-9.0f);
glm::vec3 MegaPosIni = glm::vec3(-73.0f, 3.2f, -73.5f);
glm::vec3 RexPosIni = glm::vec3(1.982f, 9.9f, -71.0f);
glm::vec3 RaptorPosIni = glm::vec3(73.314f, 7.1f, -69.799f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Iluminacion 2", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader anim("Shaders/anim3.vs", "Shaders/anim3.frag");

	//Modelos de ambiente
	Model Piso((char*)"Models/Esfera/Piso.obj");
	Model Lamparas((char*)"Models/Lampara/Lamparas.obj");
	Model Tierra((char*)"Models/Esfera/tierra.obj");
	Model Bancas((char*)"Models/Bancas/Bancas.obj");
	Model Fuente((char*)"Models/Fuente/Fuente.obj");
	//Model rex((char*)"Models/Dinos/Trex.obj");
	//Model pterodactylo((char*)"Models/Dinos/pterodactilo.obj");
	Model barosaurus((char*)"Models/Dinos/Barosaurus.obj");
	Model castillo((char*)"Models/Fachada/castillo.obj");
	//Model raptor((char*)"Models/Dinos/raptor.obj");
	Model celdas((char*)"Models/Celda/setCeldas.obj");
	Model vacas((char*)"Models/vaca/vacas.obj");
	//Model megalodon((char*)"Models/Dinos/Megalodon.obj");
	Model agua((char*)"Models/Sea/Sea.obj");
	////Model jeep((char*)"Models/Jeep/Jeep2.obj");
	Model arboles((char*)"Models/Arbol/Arboles.obj");
	Model tiendas((char*)"Models/Tiendas/tiendas2.obj");
	Model tricoBotarga((char*)"Models/Dinos/tricoBotarga.obj");
	Model rexBotarga((char*)"Models/Dinos/rexBotarga.obj");

	//Modelos Perodactylo
	Model pterodactylo_AlaIzq((char*)"Models/Dinos/Pterodactylo/pterodactylo_AlaIzq.obj");
	Model pterodactylo_AlaDer((char*)"Models/Dinos/Pterodactylo/pterodactylo_AlaDer.obj");
	Model pterodactylo_Cuerpo((char*)"Models/Dinos/Pterodactylo/pterodactylo_Cuerpo.obj");

	//Modelos Megalodon
	Model megalodonCola((char*)"Models/Dinos/Megalodon/megalodonCola.obj");
	Model megalodonCuerpo((char*)"Models/Dinos/Megalodon/megalodonCuerpo.obj");

	//Modelos T-rex
	Model rexCuerpo((char*)"Models/Dinos/Trex/rexCuerpo.obj");
	Model rexMandibula((char*)"Models/Dinos/Trex/rexMandibula.obj");
	Model rexPiernaIzq((char*)"Models/Dinos/Trex/rexPiernaIzq.obj");
	Model rexPiernaDer((char*)"Models/Dinos/Trex/rexPiernaDer.obj");
	Model rexPataIzq((char*)"Models/Dinos/Trex/rexPataIzq.obj");
	Model rexPataDer((char*)"Models/Dinos/Trex/rexPataDer.obj");

	//ModelosRaptor
	Model raptorCuerpo((char*)"Models/Dinos/Raptor/raptorCuerpo.obj");
	Model raptorMandibula((char*)"Models/Dinos/Raptor/raptorMandibula.obj");
	Model raptorPiernaIzq((char*)"Models/Dinos/Raptor/raptorPiernaIzq.obj");
	Model raptorPiernaDer((char*)"Models/Dinos/Raptor/raptorPiernaDer.obj");
	Model raptorPataIzq((char*)"Models/Dinos/Raptor/raptorPataIzq.obj");
	Model raptorPataDer((char*)"Models/Dinos/Raptor/raptorPataDer.obj");

	// First, set the container's VAO (and VBO)1
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 200.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.8f, 0.8f, 0.8f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 1.8f);




		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), spotlightPosition.x, spotlightPosition.y, spotlightPosition.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), spotlightDir.x, spotlightDir.y, spotlightDir.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);
		glm::mat4 modeltemp1(1);
		glm::mat4 modeltemp2(1);
		glm::mat4 modeltemp3(1);
		glm::mat4 modeltemp4(1);
		glm::mat4 modeltemp5(1);

		//Carga de modelo 
		//Pisos,Fachada, Bancas y Fuente
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Tierra.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bancas.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
		Lamparas.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Fuente.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
		castillo.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		arboles.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tiendas.Draw(lightingShader);
		/*glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		jeep.Draw(lightingShader);*/
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		rexBotarga.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tricoBotarga.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		celdas.Draw(lightingShader);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		vacas.Draw(lightingShader);

		//T-Rex
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTrasparencia"), 0);
		//rex.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, RexPosIni + glm::vec3(movRexX,0.0f,movRexZ));
		model = glm::rotate(model, glm::radians(rotCuerpoRex),glm::vec3(1.0f,0.0f,0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		rexCuerpo.Draw(lightingShader);
		//Mandibula
		model = glm::translate(model, glm::vec3(0.0f, 3.127f, 7.5f));
		model = glm::rotate(model, glm::radians(rotMandibulaRex), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		rexMandibula.Draw(lightingShader);
		//PienaIzq
		model = glm::mat4(1);
		model = glm::translate(model, RexPosIni + glm::vec3(movRexX, 0.0f, movRexZ));
		model = glm::rotate(model, glm::radians(rotPiernaRex), glm::vec3(1.0f, 0.0f, 0.0f));
		modeltemp1= model = glm::translate(model, glm::vec3(1.909f, 0.978f, -0.755f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		rexPiernaIzq.Draw(lightingShader);
		model = glm::translate(modeltemp1, glm::vec3(0.122f, -3.102f, 1.909f));
		model = glm::rotate(model, glm::radians(rotPataRex), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		rexPataIzq.Draw(lightingShader);
		//Pierna derecha
		model = glm::mat4(1);
		model = glm::translate(model, RexPosIni + glm::vec3(movRexX, 0.0f, movRexZ));
		model = glm::rotate(model, glm::radians(-rotPiernaRex), glm::vec3(1.0f, 0.0f, 0.0f));
		modeltemp2 = model = glm::translate(model, glm::vec3(-1.837f, 0.613f, -0.763f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		rexPiernaDer.Draw(lightingShader);
		model = glm::translate(modeltemp2, glm::vec3(-0.157f, -2.757f, 2.109f));
		model = glm::rotate(model, glm::radians(-rotPataRex), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		rexPataDer.Draw(lightingShader);

		////Raptor
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//raptor.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, RaptorPosIni + glm::vec3(movRaptorX,0.0f,movRaptorZ));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotCuerpoRaptor),glm::vec3(1.0f,0.0f,0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		raptorCuerpo.Draw(lightingShader);
		//Mandibula
		model = glm::translate(model, glm::vec3(0.0f, 0.646f, 3.873f));
		model = glm::rotate(model, glm::radians(rotMandibulaRaptor), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		raptorMandibula.Draw(lightingShader);
		//PienaIzq
		model = glm::mat4(1);
		model = glm::translate(model, RaptorPosIni + glm::vec3(movRaptorX, 0.0f, movRaptorZ));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPiernaRaptor), glm::vec3(1.0f, 0.0f, 0.0f));
		modeltemp3= model = glm::translate(model, glm::vec3(0.813f, -0.116f, -0.225f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		raptorPiernaIzq.Draw(lightingShader);
		model = glm::translate(modeltemp3, glm::vec3(-0.077f, -1.348f, 0.208f));
		model = glm::rotate(model, glm::radians(rotPataRaptor), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		raptorPataIzq.Draw(lightingShader);
		////Pierna derecha
		model = glm::mat4(1);
		model = glm::translate(model, RaptorPosIni + glm::vec3(movRaptorX, 0.0f, movRaptorZ));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotPiernaRaptor), glm::vec3(1.0f, 0.0f, 0.0f));
		modeltemp4 = model = glm::translate(model, glm::vec3(-0.806f, -0.142f, -0.155f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		raptorPiernaDer.Draw(lightingShader);
		model = glm::translate(modeltemp4, glm::vec3(-0.199f, -1.345f, 0.016f));
		model = glm::rotate(model, glm::radians(-rotPataRaptor), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		raptorPataDer.Draw(lightingShader);
	
		//Megalodon
		model = glm::mat4(1);
		model = glm::translate(model,MegaPosIni+glm::vec3(movMegaX,0,movMegaZ));
		model = glm::rotate(model, glm::radians(rotMega), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotColaMega), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		megalodonCola.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, MegaPosIni + glm::vec3(movMegaX, 0, movMegaZ));
		model = glm::rotate(model, glm::radians(rotMega), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotColaMega/2), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		megalodonCuerpo.Draw(lightingShader);

		//Barosuarus
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		barosaurus.Draw(lightingShader);

		//Pterodactylo
		model = glm::mat4(1);
		model = glm::translate(model,PteroPosIni+glm::vec3(movPteroX,0,movPteroZ));
		model = glm::rotate(model, glm::radians(rotPtero), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pterodactylo_Cuerpo.Draw(lightingShader);
		model = glm::mat4(1);;
		model = glm::translate(model,PteroPosIni + glm::vec3(movPteroX, 0, movPteroZ));
		model = glm::rotate(model, glm::radians(rotPtero), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotAlasptero), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pterodactylo_AlaDer.Draw(lightingShader);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.3f));
		model = glm::translate(model,PteroPosIni+glm::vec3(movPteroX,0.0f,movPteroZ));
		model = glm::rotate(model, glm::radians(rotPtero), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(desfase2, 0.0f,desfase1));
		model = glm::rotate(model, glm::radians(rotAlasptero), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pterodactylo_AlaIzq.Draw(lightingShader);

		anim.Use();
		tiempo = glfwGetTime();
		modelLoc = glGetUniformLocation(anim.Program, "model");
		viewLoc = glGetUniformLocation(anim.Program, "view");
		projLoc = glGetUniformLocation(anim.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f,0.1f,0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(anim.Program, "time"), tiempo);
		agua.Draw(anim);
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 1; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}
	//manipulacion de la posicion de Spotlight
	if (keys[GLFW_KEY_K])
	{
		if (rotAlasptero < 45.0f)
			rotAlasptero += 0.1f;
		spotlightPosition.x += 0.01f;
	}
	if (keys[GLFW_KEY_H])
	{
		spotlightPosition.x -= 0.01f;
	}

	if (keys[GLFW_KEY_O])
	{
		anim = true;
		spotlightPosition.y += 0.01f;
	}

	if (keys[GLFW_KEY_L])
	{
		if (rotAlasptero > 0)
			rotAlasptero -= 0.1;
		spotlightPosition.y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		anim = false;
		spotlightPosition.z -= 0.01f;
	}
	if (keys[GLFW_KEY_J])
	{
		spotlightPosition.z += 0.01f;
	}
	//manipulacion de la direccion de Spotlight

	if (keys[GLFW_KEY_B])
	{
		spotlightDir.x += 0.05f;
	}
	if (keys[GLFW_KEY_C])
	{
		spotlightDir.x -= 0.05f;
	}

	if (keys[GLFW_KEY_N])
	{
		spotlightDir.y += 0.05f;
	}

	if (keys[GLFW_KEY_M])
	{
		spotlightDir.y -= 0.05f;
	}
	if (keys[GLFW_KEY_F])
	{
		spotlightDir.z -= 0.05f;
	}
	if (keys[GLFW_KEY_V])
	{
		spotlightDir.z += 0.05f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
			Light2 = glm::vec3(1.0f, 0.0f, 1.0f);
			Light3 = glm::vec3(0.0f, 1.0f, 1.0f);
			Light4 = glm::vec3(1.0f, 0.5f, 1.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
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
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void animacion()
{	//Animacion Pterodactylo
	if (anim)
	{
		if (r1Ptero)
		{
			movPteroX -= 0.025;
			if (movPteroX < -23)
			{
				r1Ptero = false;
				r2Ptero = true;
			}
		}
		if (r2Ptero)
		{
			desfase1 = 1.3f;
			desfase2 = 1.3f;
			rotPtero = 90.0f;
			movPteroZ += 0.025;		
			if (movPteroZ > 18)
			{
				r2Ptero = false;
				r3Ptero = true;
			}
		}
		if (r3Ptero)
		{
			desfase1 = 2.6f;
			desfase2 = 0.0f;
			rotPtero = 180.0f;
			movPteroX += 0.025;
			if (movPteroX > 0)
			{
				r3Ptero = false;
				r4Ptero = true;
			}
		}
		if (r4Ptero)
		{
			desfase1 = 1.3f;
			desfase2 = -1.3f;
			rotPtero = 270.0f;
			movPteroZ -= 0.025;
			if (movPteroZ < 0)
			{
				r4Ptero = false;
				r5Ptero = true;
			}
		}
		if (r5Ptero)
		{
			desfase1 = 0.0f;
			desfase2 = 0.0f;
			rotPtero = 0.0f;
			movPteroX = -0.025f;
			if (movPteroX < 0.0f)
			{
				r5Ptero = false;
				r1Ptero = true;
			}
		}

		if (aleteoArriba)
		{	
			rotAlasptero -= 0.35;
			if (rotAlasptero < 0.0f)
			{
				aleteoAbajo = true;
				aleteoArriba = false;
			}
		}

		if (aleteoAbajo)
		{
			rotAlasptero += 0.35;
			if (rotAlasptero > 45.0f)
			{
				aleteoArriba = true;
				aleteoAbajo = false;
			}
		}

	//Animacion Megalodon
		if (r1Mega)
		{
			movMegaZ += 0.01;
			if (movMegaZ > 16)
			{
				r1Mega = false;
				r2Mega = true;
			}
		}
		if (r2Mega)
		{
			rotMega = 135.0f;
			movMegaX += 0.01;
			movMegaZ -= 0.01;
			if (movMegaX >17)
			{
				r2Mega = false;
				r3Mega = true;
			}
		}
		if (r3Mega)
		{
			rotMega = 0.0f;
			movMegaZ += 0.01;
			if (movMegaZ > 16)
			{
				r3Mega = false;
				r4Mega = true;
			}
		}
		if (r4Mega)
		{
			rotMega = -135.0f;
			movMegaX -= 0.01;
			movMegaZ -= 0.01;
			if (movMegaX < 0)
			{
				r4Mega = false;
				r5Mega = true;
			}
		}
		if (r5Mega)
		{
			rotMega = 0;
			movMegaZ -= 0.01;
			if (movMegaZ < 0.0f)
			{
				r5Mega = false;
				r1Mega = true;
			}
		}
		if (aleteoIzq)
		{
			rotColaMega += 0.1;
			if (rotColaMega > 20)
			{
				aleteoIzq = false;
				aleteoDer = true;
			}
		}
		if (aleteoDer)
		{
			rotColaMega -= 0.1;
			if (rotColaMega < -20)
			{
				aleteoDer = false;
				aleteoIzq = true;
			}
		}
		
		//Animacion Rex
		if (caminarRex)
		{	
			movRexZ += 0.01;
			if (movRexZ > 17.0f)
			{
				caminarRex = false;
				rugidoRex = true;
			}
			if (paso1Rex)
			{
				rotPiernaRex -= 0.2f;
				rotPataRex += 0.2f;
				if (rotPiernaRex < -40.0f)
				{
					paso1Rex = false;
					paso2Rex = true;
				}
			}
			if (paso2Rex)
			{
				rotPiernaRex+= 0.2f;
				rotPataRex -= 0.2f;
				if (rotPiernaRex > 10.0f)
				{
					paso2Rex = false;
					paso1Rex = true;
				}
			}
		}
		if (rugidoRex)
		{
			if (inclinacionAbajoRex)
			{
				rotMandibulaRex = 0.0f;
				rotCuerpoRex += 0.05f;
				rotPiernaRex = 0.0f;
				rotPataRex =0.0f;
				if (rotCuerpoRex > 35.0f)
				{
					inclinacionAbajoRex = false;
					inclinacionArribaRex = true;
				}
			}
			if (inclinacionArribaRex)
			{

				rotMandibulaRex = 30.0f;
				rotCuerpoRex -= 0.05f;
				rotPiernaRex = 0.0f;
				rotPataRex = 0.0f;
				if (rotCuerpoRex < -30.0f)
				{
					inclinacionArribaRex = false;
					inclinacionAbajoRex = true;
				}
			}
		}
		//Animacion Raptor
		if (caminarRaptor)
		{
			movRaptorX -= 0.05f;
			movRaptorZ += 0.05f;
			if (movRaptorX < -15.0f)
			{
				caminarRaptor = false;
				comerRaptor = true;
			}
			if (paso1Raptor)
			{
				rotPiernaRaptor -= 0.5f;
				rotPataRaptor += 0.5f;
				if (rotPiernaRaptor < -40.0f)
				{
					paso1Raptor = false;
					paso2Raptor = true;
				}
			}
			if (paso2Raptor)
			{
				rotPiernaRaptor += 0.5f;
				rotPataRaptor -= 0.5f;
				if (rotPiernaRaptor > 20.0f)
				{
					paso2Raptor = false;
					paso1Raptor = true;
				}
			}
		}
		if (comerRaptor)
		{
			if (inclinacionAbajoRaptor)
			{
				mordidaRaptor = false;
				rotCuerpoRaptor += 0.1f;
				rotPiernaRaptor = 0.0f;
				rotPataRaptor = 0.0f;
				if (rotCuerpoRaptor > 25.0f)
				{
					
					inclinacionAbajoRaptor = false;
					inclinacionArribaRaptor = true;
				}
			}
			if (inclinacionArribaRaptor)
			{
				mordidaRaptor = true;
				rotCuerpoRaptor -= 0.1f;
				rotPiernaRaptor = 0.0f;
				rotPataRaptor = 0.0f;
				if (rotCuerpoRaptor < -30.0f)
				{
					inclinacionArribaRaptor = false;
					inclinacionAbajoRaptor = true;
				}
			}
			if (mordidaRaptor)
			{
				if (mordidaAbajo)
				{
					rotMandibulaRaptor += 0.5f;
					if (rotMandibulaRaptor > 20.0f)
					{
						mordidaAbajo = false;
						mordidaArriba = true;
					}
				}
				if (mordidaArriba)
				{
					rotMandibulaRaptor -= 0.5f;
					if (rotMandibulaRaptor < -20.0f)
					{
						mordidaAbajo = true;
						mordidaArriba = false;
					}
				}
			}

		}
		

	}

}
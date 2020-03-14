/*
 * Program 3 base code - includes modifications to shape and initGeom in preparation to load
 * multi shape objects 
 * CPE 471 Cal Poly Z. Wood + S. Sueda + I. Dunn
 */

#include <iostream>
#include <glad/glad.h>
#include <algorithm> 
#include <ctime>

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "Terrain.h"
#include "Texture.h"
#include "MatrixStack.h"
#include "WindowManager.h"
#include "stb_image.h"
#include "GuiRenderer.h"
#include "GuiTexture.h"

#include "GameObject.h"
#include "Mesh.h"
#include "Transform.h"
#include "Time.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Static time
	shared_ptr<Time> time;

	// Our shader program
	std::shared_ptr<Program> prog;
	std::shared_ptr<Program> prog2;
	std::shared_ptr<Program> prog3;
	std::shared_ptr<Program> prog_gui;
	std::shared_ptr<Program> prog_skybox;

	//GUI textures
	shared_ptr<Texture> texture_gui_test;
	vector< shared_ptr<Texture> * > textures{ &texture_gui_test };
	vector< string > texture_names{ "dolphin_icon.jpg" };

	// GameObjects

	shared_ptr<GameObject> bunny;
	shared_ptr<GameObject> playerDolphin;
	shared_ptr<GameObject> water;



	// Shape to be used (from  file) - modify to support multiple

	/*
	vector<shared_ptr<Shape>> mesh_cube;
	vector<shared_ptr<Shape>> mesh_quad;(*/

	shared_ptr<Mesh> mesh_cube, mesh_quad;

	// Skybox
	vector<std::string> faces{
		"sky_rt.png",
		"sky_lf.png",
		"sky_up.png",
		"sky_dn.png",
		"sky_ft.png",
		"sky_bk.png"
	};
	int texId;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;

	//example data that might be useful when trying to compute bounds on multi-shape
	vec3 gMin;

	//animation data
	float sTheta = 0;
	float eTheta = 0;
	float hTheta = 0;
	float gTrans = 0;
	float lPos = 0;
	int matIndex = 2;

	// view data
	vec3 lookAt = vec3(0, 0, 0);
	vec3 eye = vec3(0, 0, 50);

	float theta = 90;
	float phi = 0;

	bool mouseDown = false;
	vec2 m_startPos;

	bool moveGame = false;
	bool forward = false, backward = false, left = false, right = false;



	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		vec3 view = lookAt - eye;
		if (moveGame)
			view.y = 0;
		vec3 strafe = cross(vec3(0, 1, 0), view);
		float speed = 0.01f;
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_E && action == GLFW_PRESS) {
			lPos += 10;
		}
		if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
			lPos -= 10;
		}
		if (key == GLFW_KEY_A) {
			if (action == GLFW_PRESS)
				left = true;
			else if (action == GLFW_RELEASE)
				left = false;

		}
		if (key == GLFW_KEY_D) {
			if (action == GLFW_PRESS)
				right = true;
			else if (action == GLFW_RELEASE)
				right = false;

		}
		if (key == GLFW_KEY_W) {
			if (action == GLFW_PRESS)
				forward = true;
			else if (action == GLFW_RELEASE)
				forward = false;

		}
		if (key == GLFW_KEY_S) {
			if (action == GLFW_PRESS)
				backward = true;
			else if (action == GLFW_RELEASE)
				backward = false;

		}
		if (key == GLFW_KEY_M && action == GLFW_PRESS) {
			matIndex++;
			matIndex = matIndex % 5;
		}
		if (key == GLFW_KEY_N && action == GLFW_PRESS) {
			moveGame = !moveGame;
		}
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (!mouseDown)
		{
			 glfwGetCursorPos(window, &posX, &posY);
			 cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
			 mouseDown = true;
			 m_startPos = vec2(posX, posY);
		}
	
	}

	void mouseDrag(GLFWwindow *window)
	{

	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void scrollCallback(GLFWwindow* window, double deltaX, double deltaY)
	{
		float mult = 1.0f;
		vec3 dist = lookAt - eye;
		dist = normalize(dist);
		dist *= mult;

		eye += (dist * float(deltaY));

		return;
	}

	void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		float speed = 0.05;
		theta += (xpos - m_startPos.x) * 0.1;
		phi += (ypos - m_startPos.y) * 0.1;

		if (phi > 89.9f)
			phi = 89.0f;
		if (phi < -89.0f)
			phi = -89.0f;

		float radius = length(lookAt - eye);
		eye.x = lookAt.x + (radius * cos(radians(phi)) * cos(radians(theta)));
		eye.y = lookAt.y + (radius * sin(radians(phi)));
		eye.z = lookAt.z + (radius * cos(radians(phi)) * cos((3.14159/2.0) - radians(theta)));

		m_startPos.x = xpos;
		m_startPos.y = ypos;

	}

	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.419f, .85f, .95f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = make_shared<Program>();
		initProg(prog, resourceDirectory, "/water_vert.glsl", "/water_frag.glsl");

		prog2 = make_shared<Program>();
		initProg(prog2, resourceDirectory, "/tex_vert.glsl", "/tex_frag.glsl");

		prog3 = make_shared<Program>();
		initProg(prog3, resourceDirectory, "/tex_vert.glsl", "/tex_frag.glsl");

		prog_skybox = make_shared<Program>();
		initProg(prog_skybox, resourceDirectory, "/cube_vert.glsl", "/cube_frag.glsl");

		prog_gui = make_shared<Program>();
		initProg(prog_gui, resourceDirectory, "/gui_vert.glsl", "/gui_frag.glsl");
		initProgGUI(prog_gui, resourceDirectory, "/gui_vert.glsl", "/gui_frag.glsl");
	}

	// Initializing Prog given vertex shader and frag shader path
	void initProg(shared_ptr<Program> prog, std::string resourceDirectory, string vertShader, string fragShader)
	{
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + vertShader, resourceDirectory + fragShader);
		prog->init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("difCol");
		prog->addUniform("lightPos");
		prog->addUniform("Texture0");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("indicators");
		prog->addAttribute("vertTex");
		prog->addUniform("matAmb");
		prog->addUniform("matSpec");
		prog->addUniform("shine");
		prog->addUniform("lightCol");
		prog->addUniform("eyePos");
		prog->addUniform("lightDir");
		prog->addUniform("time");
	}

	void initProgGUI(shared_ptr<Program> prog, std::string resourceDirectory, string vertShader, string fragShader)
	{
		prog->addUniform("position");
		prog->addUniform("transformationMatrix");
	}

	void initObjects(const std::string& resourceDirectory)
	{
		//Create static time
		time = make_shared<Time>(glfwGetTime());

		//Initialize meshes
		mesh_cube = make_shared<Mesh>("cube.obj", resourceDirectory);
		mesh_quad = make_shared<Mesh>("quad.obj", resourceDirectory);
		mesh_quad->setTexture("dolphin_icon.jpg");

		// Test initializing game objects and meshes
		bunny = make_shared<GameObject>("bunny_4.obj", "bunny_body.png", resourceDirectory);
		bunny->transform.position = vec3(5, 5, 5);
		bunny->transform.scale = vec3(5, 5, 5);
		bunny->prog = prog2;
		bunny->mesh.materialNum = 1;
		bunny->setTime(time);
		bunny->createRigidBody();

		playerDolphin = make_shared<GameObject>("dolphin.obj", "bunny_body.png", resourceDirectory);
		playerDolphin->prog = prog2;
		playerDolphin->mesh.materialNum = 2;
		playerDolphin->setTime(time);
		playerDolphin->createRigidBody();

		water = make_shared<GameObject>("water.obj", "grass.jpg", resourceDirectory);
		water->transform.scale = vec3(15, 15, 15);
		water->prog = prog;
		water->mesh.materialNum = 4;
		water->setTime(time);
	}


	void initDrawProg(shared_ptr<Program> program, shared_ptr<MatrixStack> Projection, mat4 view)
	{
		program->bind();
		glUniformMatrix4fv(program->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glUniformMatrix4fv(program->getUniform("V"), 1, GL_FALSE, value_ptr(view));
		glUniform3f(program->getUniform("difCol"), 0.0, 0.16, 0.9);
		glUniform3f(program->getUniform("lightPos"), 2.0 + lPos, 50, 2.9);
		glUniform3f(program->getUniform("lightDir"), 0, -1, -1);
		glUniform3f(program->getUniform("lightCol"), 1.0, 0.94, 0.78);
		glUniform3f(program->getUniform("eyePos"), eye.x, eye.y, eye.z);
		glUniform1f(program->getUniform("time"), glfwGetTime());
		SetMaterial(2, program);
		program->unbind();
	}

	void initDrawProgGUI(shared_ptr<Program> program, shared_ptr<MatrixStack> Projection, mat4 view)
	{
		program->bind();
		SetMaterial(2, program);
		program->unbind();
	}

	void setModel(std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack>M) {
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
   }

	void Update()
	{
		// Update time
		time->updateTime(glfwGetTime());
		lookAt = playerDolphin->transform.position;

		// Bunny
		bunny->rgbd.direction = normalize(vec3(0, 0, 1));
		bunny->rgbd.speed += 1;
		bunny->updatePos();

		//Dolphin
		if (left) {
			playerDolphin->transform.rotation.y -= 0.05;
		}
		if (right) {
			playerDolphin->transform.rotation.y += 0.05;
		}
		eye += playerDolphin->transform.getForward();
		playerDolphin->transform.position += playerDolphin->transform.getForward();

		//Water

		//vec3 newPos = bunny->rgbd.calcNextPos(bunny->transform.position);
		//cout << time->deltaTime <<endl;
		//cout << newPos.x << ", " << newPos.y << ", " << newPos.z << endl;
		//bunny->transform.position = bunny->rgbd.calcNextPos(bunny->transform.position);
	}

	void render() {
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Use the matrix stack for Lab 6
		float aspect = width/(float)height;

		// Create the matrix stacks - please leave these alone for now
		auto Projection = make_shared<MatrixStack>();
		auto View = make_shared<MatrixStack>();
		auto Model = make_shared<MatrixStack>();

		// Apply perspective projection.
		Projection->pushMatrix();
		Projection->perspective(45.0f, aspect, 0.01f, 3000.0f);

		// View is global translation along negative z for now
		View->pushMatrix();
			View->loadIdentity();
			View->translate(vec3(3, -10, -40));

		// View stuff
		mat4 view = glm::lookAt(eye, lookAt, vec3(0, 1, 0));

		// Skybox
		Model->pushMatrix();
			Model->translate(vec3(0, 0, -20) + eye);
			Model->scale(vec3(2000, 2000, 2000));
			Model->rotate(glfwGetTime() * 0.02, vec3(0, 1, 0));
		//to draw the sky box bind the right shader
		prog_skybox->bind();
		glUniformMatrix4fv(prog_skybox->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
		glDepthFunc(GL_LEQUAL);
		glUniformMatrix4fv(prog_skybox->getUniform("V"), 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(prog_skybox->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
		mesh_cube->meshes[0]->draw(prog_skybox);
		glDepthFunc(GL_LESS);
		prog_skybox->unbind();

		Model->popMatrix();

		// Initializing progs
		initDrawProg(prog, Projection, view);
		initDrawProg(prog2, Projection, view);
		initDrawProgGUI(prog_gui, Projection, view);

		prog2->bind();

		// draw mesh 

		Model->pushMatrix();

			Model->loadIdentity();

			bunny->Draw();
			playerDolphin->Draw();

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// TODO: Remember to translate water up and down?
			water->Draw();

			// Draw GUI
			drawGui(prog_gui, Model);

		Model->popMatrix();

		prog2->unbind();



		//animation update example
		sTheta = sin(glfwGetTime());
		eTheta = std::max(0.0f, (float)sin(glfwGetTime()));
		hTheta = std::max(0.0f, (float)sin(glfwGetTime()));

		// Pop matrix stacks.
		Projection->popMatrix();
		View->popMatrix();

	}

	void drawGui(shared_ptr<Program> prog, shared_ptr<MatrixStack> Model)
	{
		prog->bind();
		SetMaterial(4, prog);
		glUniform2f(prog->getUniform("position"), 0.5, 0.5);
		

		Model->pushMatrix();

		auto Transforms = make_shared<MatrixStack>();
		Transforms->pushMatrix();
			Transforms->loadIdentity();
			Transforms->translate(vec3(-0.7, 0.7, 0));
			Transforms->scale(vec3(0.5, 0.5, 1));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(prog->getUniform("transformationMatrix"), 1, GL_FALSE, value_ptr(Transforms->topMatrix()));
		
		mesh_quad->Draw(prog);
		Model->popMatrix();
		glDisable(GL_BLEND);
		prog->unbind();
	}

	void SetMaterial(int i, shared_ptr<Program> prog) {
		switch (i) {
		case 0: //shiny blue plastic
			glUniform3f(prog->getUniform("matAmb"), 0.02, 0.04, 0.2);
			glUniform3f(prog->getUniform("difCol"), 0.0, 0.16, 0.9);
			glUniform3f(prog->getUniform("matSpec"), 0.14, 0.2, 0.8);
			glUniform1f(prog->getUniform("shine"), 60.0);
			break;
		case 1: // flat grey
			glUniform3f(prog->getUniform("matAmb"), 0.13, 0.13, 0.14);
			glUniform3f(prog->getUniform("difCol"), 0.1, 0.8, 0.1);
			glUniform3f(prog->getUniform("matSpec"), 0.3, 0.3, 0.4);
			glUniform1f(prog->getUniform("shine"), 4.0);
			break;
		case 2: //brass
			glUniform3f(prog->getUniform("matAmb"), 0.3294, 0.2235, 0.02745);
			glUniform3f(prog->getUniform("difCol"), 0.7804, 0.5686, 0.11373);
			glUniform3f(prog->getUniform("matSpec"), 0.9922, 0.941176, 0.80784);
			glUniform1f(prog->getUniform("shine"), 100.0);
			break;
		case 3: //grass
			glUniform3f(prog->getUniform("matAmb"), 0.1, 0.2, 0.1);
			glUniform3f(prog->getUniform("difCol"), 0.1, 0.8, 0.3);
			glUniform3f(prog->getUniform("matSpec"), 0.9922, 0.941176, 0.80784);
			glUniform1f(prog->getUniform("shine"), 100.0);
			break;
		case 4: //water
			glUniform3f(prog->getUniform("matAmb"), 0.3, 0.5, 0.8);
			glUniform3f(prog->getUniform("difCol"), 0.3, 0.6, 0.8);
			glUniform3f(prog->getUniform("matSpec"), 0.2, 0.3, 0.8);
			glUniform1f(prog->getUniform("shine"), 2.0);
			break;
		case 5: //bunny
			glUniform3f(prog->getUniform("matAmb"), 0.3, 0.1, 0.1);
			glUniform3f(prog->getUniform("difCol"), 0.8, 0.3, 0.3);
			glUniform3f(prog->getUniform("matSpec"), 0.5, 0.4, 0.4);
			glUniform1f(prog->getUniform("shine"), 4.0);
			break;
		}
	}

	unsigned int createSky(string dir, vector<string> faces) {
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(false);
		for (GLuint i = 0; i < faces.size(); i++) {
			unsigned char *data =
				stbi_load((dir + "/cracks/" + faces[i]).c_str(), &width, &height, &nrChannels, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else {
				cout << "failed to load: " << (dir + "/" + faces[i]).c_str() << endl;
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		cout << " creating cube map any errors : " << glGetError() << endl;
		return textureID;
	}
};

int main(int argc, char *argv[])
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}
	srand(time(NULL));

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(1200, 800);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->texId = application->createSky(resourceDir, application->faces);

	application->initObjects(resourceDir);

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
		//call update
		application->Update();

		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}

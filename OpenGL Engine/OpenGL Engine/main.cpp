#include <glew.h>
#include <glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include"Program.h"
#include"ProgramManager.h"
#include "Light.h"
#include "Material.h"
#include "GUIMain.h"

#include"Shader.h"

#include <vector>

GUIBase* base;

void renderScene(Program&);
void ContainterInitialize( );
void WallInitialize( );
void FloorInitialize( );
void LampInitialize( );
void renderContainer(void);
void renderWalls(void);
void renderFloor(void);
void renderLamp(void);

void GUIMouse(double xpos, double ypos);

int WindowWidth = 1600;
int WindowHeight = 900;
GLfloat fov = 45.0f;
GLfloat aspectRatio = (GLfloat) WindowWidth / WindowHeight;
glm::vec3 cameraPos(0.0f, 1.0f, 3.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
GLfloat lastX = WindowWidth / 2.0;
GLfloat lastY = WindowHeight / 2.0;
bool keys [1024];
bool firstMouse = true;
GLFWwindow* window;
bool cursorIsNormal = false;
bool ControlTheView = true;
bool CursorIsFree = false;
bool ControlTheKeyboard = true;
GLfloat cameraSpeed = 0.05f;

GUIManager guiManager;
EventQueue GUIQueue;


float ClickPosX;
float ClickPosY;


bool isGuiDraw;
bool isMouseCenter;
bool isPressed;
bool isWireframed;
GLuint ContainerVao;
GLuint FloorVao;
GLuint WallVao;
GLuint LampVao;

GLuint ContainerVbo;
GLuint FloorVbo;
GLfloat yaw = -90.0f, pitch = 0.0f;
GLuint TextureWallVbo;

void GUIMouse(double xpos, double ypos)
{

	guiManager.makeFirst( );
	while (!guiManager.isEnd( ))
	{
		GUIBase* tmp = guiManager.getElement( );
		//tmp->onMouseMove(xpos, ypos);
		guiManager.nextElement( );
	}

}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void window_resizeCallback(GLFWwindow* window, int width, int height)
{
	aspectRatio = (GLfloat) width / height;
	lastX = width / 2.0;
	lastY = height / 2.0;
	WindowWidth = width;
	WindowHeight = height;
}

void mouse_scrollback(GLFWwindow* window, double xoffset, double yoffset)
{
	GLfloat precision = 1.0f;
	if (fov >= 1.0f && fov <= 75.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 75.0f)
		fov = 75.0f;

}

void framebuffer_sizeCallback(GLFWwindow* window, int widht, int height)
{
	glViewport(0, 0, widht, height);
}

void mouse_buttonCallback(GLFWwindow* Window, int button, int action, int mods)
{
	if (isGuiDraw)
	{
		if ((button == GLFW_MOUSE_BUTTON_1 || button == GLFW_MOUSE_BUTTON_2) && action == GLFW_PRESS)
		{
			MouseEvent* click = new MouseEvent(ClickPosX,ClickPosY,button);
			GUIQueue.addEvent(click);
			guiManager.performEvent();
		
		}
	}

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (ControlTheView)
	{

		if (firstMouse || isMouseCenter == true)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
			isMouseCenter = false;
		}
		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		GLfloat sensitivity = 0.05f;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		else if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}
	else if (isGuiDraw == true)
	{
		ClickPosX = xpos;
		ClickPosY = ypos;
	//	MouseEvent* movement = new MouseEvent( ClickPosX, ClickPosX );
		//GUIQueue.addEvent( movement );
		//guiManager.performEvent(movement );
	}



}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F8 && action == GLFW_PRESS && isWireframed == false)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		isWireframed = true;
	}
	else if (key == GLFW_KEY_F8 && action == GLFW_PRESS && isWireframed == true)
	{

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		isWireframed = false;
	}
	if ( key == GLFW_KEY_Q && action == GLFW_PRESS )
	{
		KeyboardEvent* tmpEvent = new KeyboardEvent( );
		GUIQueue.addEvent( tmpEvent );
		guiManager.performEvent( );
	}
	if (action == GLFW_PRESS)
		keys [key] = true;
	else if (action == GLFW_RELEASE)
		keys [key] = false;
	if (action == GLFW_PRESS && key == GLFW_KEY_TAB && CursorIsFree == false)
	{
		std::cout << "Free cursor" << std::endl;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		CursorIsFree = true;
		ControlTheView = false;
		ControlTheKeyboard = false;
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_TAB && CursorIsFree == true)
	{
		std::cout << "Freeze cursor" << std::endl;
		CursorIsFree = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		ControlTheView = true;
		ControlTheKeyboard = true;
		isMouseCenter = true;
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_G && isPressed == false)
	{
		isGuiDraw = true;
		isPressed = true;
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_G && isPressed == true)
	{
		isPressed = false;
		isGuiDraw = false;

	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
	{
		cameraSpeed += 0.2f;
	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
	{
		cameraSpeed -= 0.2f;
	}




}

void doMovement(void)
{
	if (ControlTheKeyboard)
	{
		if (keys [GLFW_KEY_W])
		{
			cameraPos += glm::cross(glm::cross(cameraUp, cameraFront), cameraUp) * cameraSpeed;
		}
		if (keys [GLFW_KEY_S])
		{
			cameraPos += -glm::cross(glm::cross(cameraUp, cameraFront), cameraUp) * cameraSpeed;
		}
		if (keys [GLFW_KEY_A])
		{
			cameraPos += glm::cross(cameraUp, cameraFront) * cameraSpeed;
		}
		if (keys [GLFW_KEY_D])
		{
			cameraPos += -glm::cross(cameraUp, cameraFront)* cameraSpeed;
		}
		if (keys [GLFW_KEY_SPACE])
		{
			cameraPos += cameraUp * cameraSpeed;

		}
		if (keys [GLFW_KEY_LEFT_CONTROL])
		{
			cameraPos += -cameraUp * cameraSpeed;
		}
	}







}
static int i = 0;


//void drawGui( std::vector<GUIBase*>& vector )
void drawGui(void)
{
	/*
	for ( auto vectorIter = vectorIter.begin( ); vectorIter != vector.end( ); ++vectorIter )
	{
		(*vectorIter)->Draw( );
	}
	*/
	guiManager.makeFirst( );
	while (!guiManager.isEnd( ))
	{
		guiManager.getElement( )->Draw( );
		guiManager.nextElement( );
	}






	/*
	glClear( GL_DEPTH_BUFFER_BIT );
	glDepthFunc( GL_ALWAYS );
	glm::vec3 color = glm::vec3( 1.0, 0.0, 0.0 );
	glm::vec3 ButtonColor = glm::vec3( 1.0, 1.0, 1.0 );
	glm::mat4 model = glm::mat4( );
	//model = glm::translate( model, glm::vec3( 100.0, 200.0,  0.0 ) );

	//glm::mat4 projectionMatrix = glm::perspective( glm::radians( fov ), aspectRatio, 0.1f, 100.0f );
	glm::mat4 projectionMatrix = glm::ortho(0.0f, 1600.0f, 900.0f, 0.0f, 1.0f, -1.0f );

	program->use( );
	program->setUniformData( "color", color.x, color.y, color.z );
	//model = glm::scale( model, glm::vec3( 0.2, 0.4, 0.0 ) );
	program->setUniformData( "model", model );
	program->setUniformData( "projection", projectionMatrix );
	glBindVertexArray( vao );
	glDrawArrays( GL_TRIANGLES, 0, 6 );


	model = glm::mat4( );
	model = glm::scale( model, glm::vec3( 0.7, 0.7, 0.7 ) );
	program->setUniformData( "color", ButtonColor.x, ButtonColor.y, ButtonColor.z );
	program->setUniformData( "model", model );
	program->setUniformData( "projection", projectionMatrix );
	glDrawArrays( GL_TRIANGLES, 0, 6 );

	model = glm::scale(model,glm::vec3( 0.8, 0.8, 0.8) );
	program->setUniformData( "color", 0.0, 1.0, 0.0);
	program->setUniformData( "model", model );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );

	glDepthFunc( GL_LESS );
	*/



}


void CreateGuiInterface(Program* GuiProgram)
{

	GUIBaseContent* MainGuiContent = new GUIBaseContent(WindowWidth, WindowHeight, GuiProgram);
	GUIWindow* guiWindow = new GUIWindow(200, 600, 300, 400,"Left window");
	GUIWindow* guiSecondWindow = new GUIWindow(WindowWidth - 200 - 500, 600, 300, 400,"Right window");
	GUIWindow* guiLittleButton = new GUIWindow(0, WindowHeight / 2, 50, 50,"Black button");
	GUIWindow* guiWhitePlank = new GUIWindow(203, 597, 296, 20,"White left plank");
	GUIWindow* guiSecondWhitePlank = new GUIWindow( WindowWidth - 200 - 500 - 3, 597, 296, 20, "White write plank" );
	GUIWindow* redSquare = new GUIWindow( 203 + 296 - 20, 597, 20, 20, "Red left button" );
	GUIWindow* redSecondSquare = new GUIWindow( WindowWidth - 200 - 500 + 300 - 20, 597, 20, 20, "Red right button" );

	guiWindow->setBackGroundColor((glm::vec3(0.2, 0.2, 0.2)));
	guiSecondWindow->setBackGroundColor((glm::vec3(0.2, 0.2, 0.2)));
	guiLittleButton->setBackGroundColor((glm::vec3(0.1, 0.1, 0.1)));
	guiWhitePlank->setBackGroundColor((glm::vec3(1.0, 1.0, 1.0)));
	redSquare->setBackGroundColor(glm::vec3(1.0, 0.0, 0.0));
	guiSecondWhitePlank->setBackGroundColor(glm::vec3(1.0, 1.0, 1.0));
	redSecondSquare->setBackGroundColor(glm::vec3(1.0, 0.0, 0.0));

	guiWindow->setBaseContent(MainGuiContent);
	guiSecondWindow->setBaseContent(MainGuiContent);
	guiLittleButton->setBaseContent(MainGuiContent);
	guiWhitePlank->setBaseContent(MainGuiContent);
	redSquare->setBaseContent(MainGuiContent);
	guiSecondWhitePlank->setBaseContent(MainGuiContent);
	redSecondSquare->setBaseContent(MainGuiContent);

	guiWindow->addChild(guiWhitePlank);
	guiWindow->addChild(redSquare);

	guiSecondWindow->addChild(guiSecondWhitePlank);
	guiSecondWindow->addChild(redSecondSquare);
	
	guiManager.addElement(guiSecondWindow);
	guiManager.addElement(guiWindow);
	guiManager.addElement(guiLittleButton);
	//guiManager.addElement(guiWhitePlank);

	guiManager.addLinkToEventQueue( &GUIQueue );


}


int main(void)
{

	glfwSetErrorCallback(error_callback);

	/* Initialize GLFW */
	if (!glfwInit( ))
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(WindowWidth, WindowHeight, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate( );
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	/* Initialize GLEW */
	glewExperimental = GL_TRUE;
	if (glewInit( ) != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_sizeCallback);
	glfwSetWindowSizeCallback(window, window_resizeCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_buttonCallback);
	glfwSetScrollCallback(window, mouse_scrollback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glViewport(0, 0, WindowWidth, WindowHeight);

	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDER);
	const GLubyte* oglVersion = glGetString(GL_VERSION);
	const GLubyte* version = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("Vendor version = %s\n", vendor);
	printf("Renderer version = %s\n", renderer);
	printf("Ogl version = %s\n", oglVersion);
	printf("Shader language version = %s\n", version);



	// Create manager, that initialize and create all programs

	GLfloat quadVertices [] =
	{
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};



	GLuint ContainerTextures [2];
	glGenTextures(2, ContainerTextures);

	int width, height;
	unsigned char* image;
	image = SOIL_load_image("../Resources/texture/ContainerDiffuse.png", &width, &height, 0, SOIL_LOAD_RGB);
	if (image == 0)
	{
		std::cout << "Diffuse Image don't load" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, ContainerTextures [0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);




	image = SOIL_load_image("../Resources/texture/ContainerSpecular.png", &width, &height, 0, SOIL_LOAD_RGB);
	if (image == 0)
	{
		std::cout << "Specular Image don't load" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, ContainerTextures [1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint FloorTextures [2];
	glGenTextures(1, FloorTextures);
	image = SOIL_load_image("../Resources/texture/road_brick.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (image == 0)
	{
		std::cout << "Floor Image don't load" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, FloorTextures [0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	image = SOIL_load_image("../Resources/texture/brickSpecular6.png", &width, &height, 0, SOIL_LOAD_RGB);
	if (image == 0)
	{
		std::cout << "Floor specular Image don't load" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, FloorTextures [1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint WallTextures [2];
	glGenTextures(2, WallTextures);
	image = SOIL_load_image("../Resources/texture/brickWall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (image == 0)
	{
		std::cout << "Wall Image don't load" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, WallTextures [0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);


	image = SOIL_load_image("../Resources/texture/SpecularBrickWall.png", &width, &height, 0, SOIL_LOAD_RGB);
	if (image == 0)
	{
		std::cout << "Floor specular Image don't load" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, WallTextures [1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint Quad, QuadVbo;
	glGenVertexArrays(1, &Quad);
	glGenBuffers(1, &QuadVbo);
	glBindVertexArray(Quad);

	glBindBuffer(GL_ARRAY_BUFFER, QuadVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	glEnable(GL_DEPTH_TEST);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Program loop
	//glEnable(GL_CULL_FACE);


	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;



	GLuint cubemapProgram;
	cubemapProgram = glCreateProgram( );
	Shader* CubeMapVertexShader = new Shader("../Shader/VertexShader/pointShadowShader.vs", GL_VERTEX_SHADER);
	Shader* CubeMapFragmentShader = new Shader("../Shader/FragmentShader/pointShadowShader.frag", GL_FRAGMENT_SHADER);
	Shader* CubeMapGeometry = new Shader("../Shader/GeometryShader/pointShadowShader.gm", GL_GEOMETRY_SHADER);
	if (!CubeMapGeometry->compileShader( ))
		std::cout << "can't load geometry shader" << std::endl;
	if (!CubeMapVertexShader->compileShader( ))
		std::cout << "can't load vertex shader" << std::endl;
	if (!CubeMapFragmentShader->compileShader( ))
		std::cout << "can't load fragment shader" << std::endl;
	glAttachShader(cubemapProgram, CubeMapVertexShader->returnShaderName( ));
	glAttachShader(cubemapProgram, CubeMapFragmentShader->returnShaderName( ));
	glAttachShader(cubemapProgram, CubeMapGeometry->returnShaderName( ));
	glLinkProgram(cubemapProgram);
	GLint success;
	glGetProgramiv(cubemapProgram, GL_COMPILE_STATUS, &success);
	GLchar ProgramInfoLog [512];
	if (!success)
	{
		glGetProgramInfoLog(cubemapProgram, 512, NULL, ProgramInfoLog);
		std::cout << "\tERROR :: PROGRAM :: " << "Cube map program" << "LINKING FAILED\n" << ProgramInfoLog << std::endl << std::endl;
		return false;
	}
	std::cout << "PROGRAM :: " << "Cube map Program" << " LINKING COMPLETED \n" << std::endl << std::endl;
	CubeMapVertexShader->deleteShader( );
	delete CubeMapVertexShader;
	CubeMapFragmentShader->deleteShader( );
	delete CubeMapFragmentShader;
	CubeMapGeometry->deleteShader( );
	delete CubeMapGeometry;

	ProgramManager manager;
	manager.readConfig( );
	if (!manager.createPrograms( ))
	{
		getchar( );
		return -1;
	}
	// Get programs from manager by name
	Program* mainProgram = manager.returnProgram("mainProgram");
	Program* lightProgram = manager.returnProgram("lightProgram");
	Program* simpleProgram = manager.returnProgram("shadowProgram");
	Program* quadProgram = manager.returnProgram("DepthQuad");
	Program* skyboxProgram = manager.returnProgram("skyboxProgram");
	Program* GuiProgram = manager.returnProgram("GuiProgram");
	if (mainProgram == 0)
		std::cout << "Wrong mainProgram" << std::endl;
	else if (lightProgram == 0)
		std::cout << "Wrong lightProgram" << std::endl;
	else if (simpleProgram == 0)
		std::cout << "Wrong simpelProgram" << std::endl;
	else if (quadProgram == 0)
		std::cout << "Wrong quadProgram" << std::endl;
	else if (skyboxProgram == 0)
		std::cout << "Wrong skyboxProgram" << std::endl;
	else if (GuiProgram == 0)
		std::cout << "Wrong Gui Program" << std::endl;
	/// Get GLint program id from Program
	GLint shaderProgram = mainProgram->returnProgramNameID( );
	GLint lampProgram = lightProgram->returnProgramNameID( );
	GLint shadowProgram = simpleProgram->returnProgramNameID( );
	GLint quadProgramId = quadProgram->returnProgramNameID( );
	GLint skyboxProgramId = skyboxProgram->returnProgramNameID( );


	CreateGuiInterface(GuiProgram);




	glm::vec3 ambientMaterial(1.0f, 1.0f, 0.6f);
	glm::vec3 diffuseMaterial(1.0f, 1.0f, 0.6f);
	glm::vec3 specularMaterial(0.0f, 1.0f, 0.9f);


	Material CubeMaterial(ambientMaterial, diffuseMaterial, specularMaterial, 32.0f);
	Material FloorMaterial(ambientMaterial, diffuseMaterial, specularMaterial, 32.0f);

	glm::vec3 PointAmbientLight(0.0f, 0.0f, 0.0f);
	glm::vec3 PointDiffuseLight(1.0f, 0.0f, 0.0f);
	glm::vec3 PointSpecularLight(0.4f, 0.2f, 0.2f);
	glm::vec3 PointlightPosition = glm::vec3(0.0f, 3.0f, -3.0f);

	glm::vec3 PointAmbientLight2(0.0f, 0.0f, 0.0f);
	glm::vec3 PointDiffuseLight2(0.0f, 1.0f, 0.0f);
	glm::vec3 PointSpecularLight2(0.2f, 0.4f, 0.2f);
	glm::vec3 PointlightPosition2 = glm::vec3(0.0f, 3.0f, 2.0f);

	glm::vec3 PointAmbientLight3(0.0f, 0.0f, 0.0f);
	glm::vec3 PointDiffuseLight3(0.0f, 0.0f, 1.0f);
	glm::vec3 PointSpecularLight3(0.2f, 0.2f, 0.4f);
	glm::vec3 PointlightPosition3 = glm::vec3(-3.0f, 3.0f, 0.0f);

	glm::vec3 DirectAmbientLight(0.1f, 0.1f, 0.1f);
	glm::vec3 DirectDiffuseLight(0.01f, 0.01f, 0.01f);
	glm::vec3 DirectSpecularLight(0.2f, 0.2f, 0.2f);

	glm::vec3 DirectionalLightPosition = glm::vec3(6.0f, 2.0f, 6.0f);
	glm::vec3 DirectionalLightDirection = glm::vec3(-1.0f, -1.0f, -0.5f);

	Light PLight(PointAmbientLight, PointDiffuseLight, PointSpecularLight);
	Light PRLight(PointAmbientLight2, PointDiffuseLight2, PointSpecularLight2);
	Light PLLight(PointAmbientLight3, PointDiffuseLight3, PointSpecularLight3);

	Light DirLight(DirectAmbientLight, DirectDiffuseLight, DirectSpecularLight);
	DirectionalLight dirLight(DirLight, DirectionalLightDirection);
	PointLight pointLight(PLight);
	PointLight pointLight2(PRLight);
	PointLight pointLight3(PLLight);
	pointLight.setPosition(PointlightPosition);
	pointLight.setParamsForDistance(7);
	pointLight2.setPosition(PointlightPosition2);
	pointLight2.setParamsForDistance(7);
	pointLight3.setPosition(PointlightPosition3);
	pointLight3.setParamsForDistance(7);


	ContainterInitialize( );
	FloorInitialize( );
	WallInitialize( );
	LampInitialize( );

	GLuint depthMapFbo;
	glGenFramebuffers(1, &depthMapFbo);

	const GLuint ShadowWidth = 1024, ShadowHeight = 1024;

	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	else
		std::cout << "Frame buffer is complete \n" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// load cube map texture
	std::vector<const GLchar*> faces;
	//faces.push_back( "../Resources/skybox/simple/right.jpg" );
	//faces.push_back( "../Resources/skybox/simple/left.jpg" );
	//faces.push_back( "../Resources/skybox/simple/top.jpg" );
	//faces.push_back( "../Resources/skybox/simple/bottom.jpg" );
	//faces.push_back( "../Resources/skybox/simple/back.jpg");
	//faces.push_back( "../Resources/skybox/simple/front.jpg" );

	faces.push_back("../Resources/skybox/cloudy/yellowcloud_rt.jpg");
	faces.push_back("../Resources/skybox/cloudy/yellowcloud_lf.jpg");
	faces.push_back("../Resources/skybox/cloudy/yellowcloud_up.jpg");
	faces.push_back("../Resources/skybox/cloudy/yellowcloud_dn.jpg");
	faces.push_back("../Resources/skybox/cloudy/yellowcloud_bk.jpg");
	faces.push_back("../Resources/skybox/cloudy/yellowcloud_ft.jpg");
	GLuint skyBoxCubemap;
	glGenTextures(1, &skyBoxCubemap);
	const GLuint SkyBoxCubeMapWidth = 1024, SkyBoxCubeMapHeight = 1024;
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxCubemap);
	for (GLuint i = 0; i < faces.size( ); ++i)
	{
		image = SOIL_load_image(faces [i], &width, &height, 0, SOIL_LOAD_RGB);
		if (!image)
		{
			std::cout << "Can't load skybox " << i << std::endl;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	};
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	GLfloat skyboxVertices [] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	GLuint skyBoxVao;
	glGenVertexArrays(1, &skyBoxVao);
	glBindVertexArray(skyBoxVao);
	GLuint skyBoxVbo;
	glGenBuffers(1, &skyBoxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
	glBindVertexArray(0);


	GLuint depthCubeMap;
	glGenTextures(1, &depthCubeMap);
	const GLuint depthCubeMapWidth = 1024, depthCubeMapHeight = 1024;
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	for (GLuint i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, depthCubeMapWidth, depthCubeMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	};
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	GLuint depthCubeMapFbo;
	glGenFramebuffers(1, &depthCubeMapFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Cube map frame buffer not complete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLfloat cubemapAspect = (GLfloat) depthCubeMapWidth / (GLfloat) depthCubeMapHeight;
	GLfloat nearCubemapPlane = 1.0f;
	GLfloat farCubemapPlane = 25.0f;
	glm::mat4 cubemapProj = glm::perspective(glm::radians(90.0f), cubemapAspect, nearCubemapPlane, farCubemapPlane);
	std::vector<glm::mat4> shadowCubemapTransforms;
	shadowCubemapTransforms.push_back(cubemapProj * glm::lookAt(PointlightPosition, PointlightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowCubemapTransforms.push_back(cubemapProj * glm::lookAt(PointlightPosition, PointlightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowCubemapTransforms.push_back(cubemapProj * glm::lookAt(PointlightPosition, PointlightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowCubemapTransforms.push_back(cubemapProj * glm::lookAt(PointlightPosition, PointlightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowCubemapTransforms.push_back(cubemapProj * glm::lookAt(PointlightPosition, PointlightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowCubemapTransforms.push_back(cubemapProj * glm::lookAt(PointlightPosition, PointlightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));







	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glm::vec3 directionVector;

	GLfloat near_plane = 1.0f;
	GLfloat far_plane = 40.0f;


	while (!glfwWindowShouldClose(window))
	{

		directionVector.x = 5 * sin(glfwGetTime( ) * glm::radians(60.0));
		directionVector.z = 5 * cos(glfwGetTime( ) * glm::radians(60.0));
		directionVector.y = 5.0f;
		// Check and call events
		glfwPollEvents( );
		doMovement( );




		glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(DirectionalLightPosition, DirectionalLightDirection, glm::vec3(1.0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		// Start rendering from direct light position
		glViewport(0, 0, ShadowWidth, ShadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_FACE);








		/*
		mainProgram->use();
		mainProgram->setUniformData("pointLight.constant", constant);
		mainProgram->setUniformData("pointLight.linear", linear);
		mainProgram->setUniformData("pointLight.quadratic", quadratic);
		*/


		/*
mainProgram->setUniformData("dirLight.ambient", ambientLight.x, ambientLight.y, ambientLight.z);
mainProgram->setUniformData("dirLight.diffuse", diffuseLight.x, diffuseLight.y, diffuseLight.z);
mainProgram->setUniformData("dirLight.specular", specularLight.x, specularLight.y, specularLight.z);
mainProgram->setUniformData("dirLight.direction", lightDirection.x, lightDirection.y, lightDirection.z);
// mainProgram->setLight(pointLight);

mainProgram->setUniformData("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

*/
//viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);

		mainProgram->use( );
		//viewMatrix = glm::mat4();
		projectionMatrix = glm::mat4( );
		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));

		//viewMatrix = glm::rotate(viewMatrix,(GLfloat)(glm::radians(10.0f)*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));


		mainProgram->setUniformData("view", viewMatrix);
		mainProgram->setUniformData("projection", projectionMatrix);



		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, ContainerTextures[0]);
		//glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.diffuseTexture"), 0);

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, ContainerTextures[1]);
		//glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.specularTexture"), 1);

		////mainProgram->setUniformData("isSpecular", 1.0);
		//mainProgram->setUniformData("dsmaterial.shininess", CubeMaterial.getShininess());


		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
		simpleProgram->use( );
		//	mainProgram->use();
		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

		glBindVertexArray(ContainerVao);
		simpleProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0, 2.0, -2.0));
		modelMatrix = glm::rotate(modelMatrix, (GLfloat) (glfwGetTime( ))*glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
		glBindVertexArray(ContainerVao);
		simpleProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(ContainerVao);
		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, directionVector);
		modelMatrix = glm::rotate(modelMatrix, (GLfloat) (glfwGetTime( ))*glm::radians(60.0f), glm::vec3(0.0, 1.0, 0.0));
		simpleProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//glBindTexture(GL_TEXTURE_2D, 0);





		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, PointlightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		simpleProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);

		glBindVertexArray(LampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, PointlightPosition2);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		simpleProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);

		glBindVertexArray(LampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, PointlightPosition3);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		simpleProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);

		glBindVertexArray(LampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);




		mainProgram->use( );

		//mainProgram->setUniformData("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

		// mainProgram->setLight(pointLight);

		//glBindTexture(GL_TEXTURE_2D, 0);
		//mainProgram->setUniformData("isSpecular", 0.0);
		//mainProgram->setUniformData("dmaterial.ambient", ambientMaterial.x, ambientMaterial.y, ambientMaterial.z);
		//mainProgram->setUniformData("dmaterial.diffuse", diffuseMaterial.x, diffuseMaterial.y, diffuseMaterial.z);
		//mainProgram->setUniformData("dmaterial.specular", specularMaterial.x, specularMaterial.y, specularMaterial.z);



		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, FloorTextures[0]);
		//glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.diffuseTexture"), 0);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, FloorTextures[1]);
		//glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.specularTexture"), 1);




		//	ambientMaterial = FloorMaterial.getAmbient();
		//	diffuseMaterial = FloorMaterial.getDiffuse();

		//	specularMaterial = FloorMaterial.getSpecular();
		//mainProgram->setUniformData("isSpecular", 1.0);
		//mainProgram->setUniformData("dmaterial.ambient", ambientMaterial.x, ambientMaterial.y, ambientMaterial.z);
		//mainProgram->setUniformData("dmaterial.diffuse", diffuseMaterial.x, diffuseMaterial.y, diffuseMaterial.z);
		//mainProgram->setUniformData("dmaterial.specular", specularMaterial.x, specularMaterial.y, specularMaterial.z);
		//	mainProgram->setUniformData("material.shininess", FloorMaterial.getShininess());


		glBindVertexArray(FloorVao);
		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.55f, 0.0f));
		//modelMatrix = glm::rotate(modelMatrix,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
		//mainProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);


		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		//glBindTexture(GL_TEXTURE_2D,0);


		// Drawing walls
		glBindVertexArray(WallVao);

		// First wall
		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-10.0f, 4.45f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0, 0.5f, 1.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//mainProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);

		//	glActiveTexture(GL_TEXTURE0);
		//	glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.diffuseTexture"), 0);
		//	glBindTexture(GL_TEXTURE_2D, WallTexture);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 4.45f, -10.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0, 0.5f, 1.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//	mainProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("model", modelMatrix);
		simpleProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);
		//simpleProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);

		glDrawArrays(GL_TRIANGLES, 0, 6);




		glBindVertexArray(0);




		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_CULL_FACE);
		// end render from direct light Position



		// Render cube map texture from point light position

		glViewport(0, 0, depthCubeMapWidth, depthCubeMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFbo);
		glClear(GL_DEPTH_BUFFER_BIT);

		glUseProgram(cubemapProgram);
		for (GLuint i = 0; i < 6; ++i)
		{
			glUniformMatrix4fv(glGetUniformLocation(cubemapProgram, ("ShadowMatrices[" + std::to_string(i) + "]").c_str( )), 1, GL_FALSE, glm::value_ptr(shadowCubemapTransforms.at(i)));
		}
		glUniform1f(glGetUniformLocation(cubemapProgram, "far_plane"), farCubemapPlane);
		glUniform3f(glGetUniformLocation(cubemapProgram, "lightPos"), PointlightPosition.x, PointlightPosition.y, PointlightPosition.z);


		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

		glBindVertexArray(ContainerVao);
		glUniformMatrix4fv(glGetUniformLocation(cubemapProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniform1f(glGetUniformLocation(cubemapProgram, "far_plane"), farCubemapPlane);
		glUniform3f(glGetUniformLocation(cubemapProgram, "lightPos"), PointlightPosition.x, PointlightPosition.y, PointlightPosition.z);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0, 2.0, -2.0));
		modelMatrix = glm::rotate(modelMatrix, (GLfloat) (glfwGetTime( ))*glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
		glBindVertexArray(ContainerVao);
		glUniformMatrix4fv(glGetUniformLocation(cubemapProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniform1f(glGetUniformLocation(cubemapProgram, "far_plane"), farCubemapPlane);
		glUniform3f(glGetUniformLocation(cubemapProgram, "lightPos"), PointlightPosition.x, PointlightPosition.y, PointlightPosition.z);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(ContainerVao);
		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, directionVector);
		modelMatrix = glm::rotate(modelMatrix, (GLfloat) (glfwGetTime( ))*glm::radians(60.0f), glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(glGetUniformLocation(cubemapProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniform1f(glGetUniformLocation(cubemapProgram, "far_plane"), farCubemapPlane);
		glUniform3f(glGetUniformLocation(cubemapProgram, "lightPos"), PointlightPosition.x, PointlightPosition.y, PointlightPosition.z);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//glBindTexture(GL_TEXTURE_2D, 0);





		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, PointlightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(cubemapProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniform1f(glGetUniformLocation(cubemapProgram, "far_plane"), farCubemapPlane);
		glUniform3f(glGetUniformLocation(cubemapProgram, "lightPos"), PointlightPosition.x, PointlightPosition.y, PointlightPosition.z);
		glBindVertexArray(LampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, PointlightPosition2);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(cubemapProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniform1f(glGetUniformLocation(cubemapProgram, "far_plane"), farCubemapPlane);
		glUniform3f(glGetUniformLocation(cubemapProgram, "lightPos"), PointlightPosition.x, PointlightPosition.y, PointlightPosition.z);

		glBindVertexArray(LampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, PointlightPosition3);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(cubemapProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniform1f(glGetUniformLocation(cubemapProgram, "far_plane"), farCubemapPlane);
		glUniform3f(glGetUniformLocation(cubemapProgram, "lightPos"), PointlightPosition.x, PointlightPosition.y, PointlightPosition.z);

		glBindVertexArray(LampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glBindVertexArray(FloorVao);
		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.55f, 0.0f));
		//modelMatrix = glm::rotate(modelMatrix,glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f));
		//mainProgram->setUniformData("model", modelMatrix);
		glUniformMatrix4fv(glGetUniformLocation(cubemapProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniform1f(glGetUniformLocation(cubemapProgram, "far_plane"), farCubemapPlane);
		glUniform3f(glGetUniformLocation(cubemapProgram, "lightPos"), PointlightPosition.x, PointlightPosition.y, PointlightPosition.z);


		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		//glBindTexture(GL_TEXTURE_2D,0);


		// Drawing walls
		glBindVertexArray(WallVao);

		// First wall
		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-10.0f, 4.45f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0, 0.5f, 1.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//mainProgram->setUniformData("model", modelMatrix);
		glUniformMatrix4fv(glGetUniformLocation(cubemapProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniform1f(glGetUniformLocation(cubemapProgram, "far_plane"), farCubemapPlane);
		glUniform3f(glGetUniformLocation(cubemapProgram, "lightPos"), PointlightPosition.x, PointlightPosition.y, PointlightPosition.z);

		//	glActiveTexture(GL_TEXTURE0);
		//	glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.diffuseTexture"), 0);
		//	glBindTexture(GL_TEXTURE_2D, WallTexture);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 4.45f, -10.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0, 0.5f, 1.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(cubemapProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniform1f(glGetUniformLocation(cubemapProgram, "far_plane"), farCubemapPlane);
		glUniform3f(glGetUniformLocation(cubemapProgram, "lightPos"), PointlightPosition.x, PointlightPosition.y, PointlightPosition.z);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// end render to cube map depth texture



		glViewport(0, 0, WindowWidth, WindowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);








		//quadProgram->use( );
		//glViewport( WindowWidth - 400, WindowHeight - 400, 300, 300 );
		////glViewport(0, 0, WindowWidth, WindowHeight);
		//glActiveTexture( GL_TEXTURE0 );
		//glBindTexture( GL_TEXTURE_2D, depthMap );
		//glBindVertexArray( Quad );
		//glDrawArrays( GL_TRIANGLES, 0, 6 );
		//glBindVertexArray( 0 );
		//glBindTexture( GL_TEXTURE_2D, 0 );






		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);

		mainProgram->use( );


		mainProgram->setUniformData("far_plane", farCubemapPlane);
		mainProgram->setUniformData("view", viewMatrix);
		mainProgram->setUniformData("projection", projectionMatrix);
		mainProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);


		mainProgram->setUniformData("dirLight.ambient", DirectAmbientLight.x, DirectAmbientLight.y, DirectAmbientLight.z);
		mainProgram->setUniformData("dirLight.diffuse", DirectDiffuseLight.x, DirectDiffuseLight.y, DirectDiffuseLight.z);
		mainProgram->setUniformData("dirLight.specular", DirectSpecularLight.x, DirectSpecularLight.y, DirectSpecularLight.z);
		mainProgram->setUniformData("dirLight.direction", DirectionalLightDirection.x, DirectionalLightDirection.y, DirectionalLightDirection.z);

		mainProgram->setUniformData("pointLight.ambient", PointAmbientLight.x, PointAmbientLight.y, PointAmbientLight.z);
		mainProgram->setUniformData("pointLight.diffuse", PointDiffuseLight.x, PointDiffuseLight.y, PointDiffuseLight.z);
		mainProgram->setUniformData("pointLight.specular", PointSpecularLight.x, PointSpecularLight.y, PointSpecularLight.z);
		mainProgram->setUniformData("pointLight.position", PointlightPosition.x, PointlightPosition.y, PointlightPosition.z);

		mainProgram->setUniformData("pointLight.constant", pointLight.getConstant( ));
		mainProgram->setUniformData("pointLight.linear", pointLight.getLinear( ));
		mainProgram->setUniformData("pointLight.quadratic", pointLight.getQuadratic( ));

		mainProgram->setUniformData("pointLight1.ambient", PointAmbientLight2.x, PointAmbientLight2.y, PointAmbientLight2.z);
		mainProgram->setUniformData("pointLight1.diffuse", PointDiffuseLight2.x, PointDiffuseLight2.y, PointDiffuseLight2.z);
		mainProgram->setUniformData("pointLight1.specular", PointSpecularLight2.x, PointSpecularLight2.y, PointSpecularLight2.z);
		mainProgram->setUniformData("pointLight1.position", PointlightPosition2.x, PointlightPosition2.y, PointlightPosition2.z);

		mainProgram->setUniformData("pointLight1.constant", pointLight2.getConstant( ));
		mainProgram->setUniformData("pointLight1.linear", pointLight2.getLinear( ));
		mainProgram->setUniformData("pointLight1.quadratic", pointLight2.getQuadratic( ));

		mainProgram->setUniformData("pointLight2.ambient", PointAmbientLight3.x, PointAmbientLight3.y, PointAmbientLight3.z);
		mainProgram->setUniformData("pointLight2.diffuse", PointDiffuseLight3.x, PointDiffuseLight3.y, PointDiffuseLight3.z);
		mainProgram->setUniformData("pointLight2.specular", PointSpecularLight3.x, PointSpecularLight3.y, PointSpecularLight3.z);
		mainProgram->setUniformData("pointLight2.position", PointlightPosition3.x, PointlightPosition3.y, PointlightPosition3.z);

		mainProgram->setUniformData("pointLight2.constant", pointLight3.getConstant( ));
		mainProgram->setUniformData("pointLight2.linear", pointLight3.getLinear( ));
		mainProgram->setUniformData("pointLight2.quadratic", pointLight3.getQuadratic( ));
		mainProgram->setUniformData("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.diffuseTexture"), 0);
		glBindTexture(GL_TEXTURE_2D, ContainerTextures [0]);

		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.specularTexture"), 1);
		glBindTexture(GL_TEXTURE_2D, ContainerTextures [1]);
		glActiveTexture(GL_TEXTURE2);
		glUniform1i(glGetUniformLocation(shaderProgram, "shadowMap"), 2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glActiveTexture(GL_TEXTURE3);
		glUniform1i(glGetUniformLocation(shaderProgram, "depthCubeMap"), 3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

		mainProgram->setUniformData("dsmaterial.shininess", CubeMaterial.getShininess( ));
		mainProgram->setUniformData("far_plane", farCubemapPlane);
		mainProgram->setUniformData("IsShadow", 1.0);

		//glActiveTexture(GL_TEXTURE0);

		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		mainProgram->setUniformData("model", modelMatrix);

		glBindVertexArray(ContainerVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);




		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0f, 2.0f, -2.0f));
		modelMatrix = glm::rotate(modelMatrix, (GLfloat) (glfwGetTime( ))*glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
		mainProgram->setUniformData("model", modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, directionVector);
		modelMatrix = glm::rotate(modelMatrix, (GLfloat) (glfwGetTime( ))*glm::radians(60.0f), glm::vec3(0.0, 1.0, 0.0));
		mainProgram->setUniformData("model", modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);


		lightProgram->use( );

		glm::vec3 LightColor = glm::vec3(0.2f, 0.0f, 0.0f);
		glm::vec3 LightColor2 = glm::vec3(0.0f, 0.2f, 0.0f);
		glm::vec3 LightColor3 = glm::vec3(0.0f, 0.0f, 0.2f);
		lightProgram->setUniformData("view", viewMatrix);
		lightProgram->setUniformData("projection", projectionMatrix);
		lightProgram->setUniformData("LightColor", LightColor.x, LightColor.y, LightColor.z);

		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, PointlightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		lightProgram->setUniformData("model", modelMatrix);

		glBindVertexArray(LampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightProgram->setUniformData("LightColor", LightColor2.x, LightColor2.y, LightColor2.z);

		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, PointlightPosition2);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		lightProgram->setUniformData("model", modelMatrix);

		glBindVertexArray(LampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightProgram->setUniformData("LightColor", LightColor3.x, LightColor3.y, LightColor3.z);

		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, PointlightPosition3);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		lightProgram->setUniformData("model", modelMatrix);

		glBindVertexArray(LampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);



		mainProgram->use( );
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FloorTextures [0]);
		glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.diffuseTexture"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, FloorTextures [1]);
		glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.specularTexture"), 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(shaderProgram, "shadowMap"), 2);
		glActiveTexture(GL_TEXTURE3);
		glUniform1i(glGetUniformLocation(shaderProgram, "depthCubeMap"), 3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

		mainProgram->setUniformData("material.shininess", FloorMaterial.getShininess( ));
		mainProgram->setUniformData("far_plane", farCubemapPlane);


		glBindVertexArray(FloorVao);
		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.55f, 0.0f));
		mainProgram->setUniformData("model", modelMatrix);
		mainProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);
		mainProgram->setUniformData("view", viewMatrix);
		mainProgram->setUniformData("projection", projectionMatrix);
		mainProgram->setUniformData("far_plane", farCubemapPlane);


		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);


		glBindVertexArray(WallVao);

		// First wall
		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-10.0f, 4.45f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0, 0.5f, 1.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//mainProgram->setUniformData("model", modelMatrix);
		mainProgram->setUniformData("view", viewMatrix);
		mainProgram->setUniformData("projection", projectionMatrix);
		mainProgram->setUniformData("model", modelMatrix);
		mainProgram->setUniformData("lightSpaceMatrix", lightSpaceMatrix);
		mainProgram->setUniformData("far_plane", farCubemapPlane);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, WallTextures [0]);
		glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.diffuseTexture"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, WallTextures [1]);
		glUniform1i(glGetUniformLocation(shaderProgram, "dsmaterial.specularTexture"), 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(shaderProgram, "shadowMap"), 2);
		glActiveTexture(GL_TEXTURE3);
		glUniform1i(glGetUniformLocation(shaderProgram, "depthCubeMap"), 3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		modelMatrix = glm::mat4( );
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 4.45f, -10.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0, 0.5f, 1.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		mainProgram->setUniformData("model", modelMatrix);
		mainProgram->setUniformData("far_plane", farCubemapPlane);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);




		glDepthFunc(GL_LEQUAL);
		skyboxProgram->use( );
		viewMatrix = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
		skyboxProgram->setUniformData("projection", projectionMatrix);
		skyboxProgram->setUniformData("view", viewMatrix);
		glBindVertexArray(skyBoxVao);
		glActiveTexture(GL_TEXTURE4);
		glUniform1i(glGetUniformLocation(skyboxProgramId, "skybox"), 4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxCubemap);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);


		//	glBindVertexArray(Quad);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//	glBindVertexArray(0);



		glViewport(0, 0, WindowWidth, WindowHeight);
		if (isGuiDraw)
		{
			drawGui();
		}











		// Swap the buffers 
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &ContainerVao);
	glDeleteVertexArrays(1, &WallVao);
	glDeleteVertexArrays(1, &FloorVao);
	glDeleteVertexArrays(1, &LampVao);
	glDeleteBuffers(1, &ContainerVbo);
	glDeleteBuffers(1, &FloorVbo);


	glfwDestroyWindow(window);
	glfwTerminate( );
	exit(EXIT_SUCCESS);
}




void RenderScene(Program& program)
{

	renderContainer( );
	renderLamp( );
	renderWalls( );
	renderFloor( );


}

void renderLamp(void)
{

}

void renderContainer(void)
{
	glBindVertexArray(ContainerVao);

}
void renderWalls(void)
{

}
void renderFloor(void)
{

}
void ContainterInitialize( )
{
	GLfloat BoxVertices [] = {
		// Positions           // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &ContainerVao);
	glBindVertexArray(ContainerVao);


	// Generate vertex array buffer on video card memory
	glGenBuffers(1, &ContainerVbo);

	// Connect Array buffer with our created vertex array buffer
	glBindBuffer(GL_ARRAY_BUFFER, ContainerVbo);

	// Transform our data to array buffer to the car memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(BoxVertices), BoxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}
void WallInitialize( )
{
	GLfloat WallTextureCoordinates [] =
	{
		0.0f, 0.0f,
		6.0f, 0.0f,
		6.0f, 6.0f,
		6.0f, 6.0f,
		0.0f, 6.0f,
		0.0f, 0.0f

	};
	glGenVertexArrays(1, &WallVao);
	glBindVertexArray(WallVao);

	glGenBuffers(1, &TextureWallVbo);



	glBindBuffer(GL_ARRAY_BUFFER, FloorVbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, TextureWallVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(WallTextureCoordinates), WallTextureCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);

	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

}
void FloorInitialize( )
{
	GLfloat GroundVertices [] =
	{
		-10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
		10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 5.0f, 5.0f,
		10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 5.0f, 5.0f,
		-10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,
		-10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
	};

	glGenVertexArrays(1, &FloorVao);
	glBindVertexArray(FloorVao);
	glGenBuffers(1, &FloorVbo);

	glBindBuffer(GL_ARRAY_BUFFER, FloorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GroundVertices), GroundVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void LampInitialize( )
{
	glGenVertexArrays(1, &LampVao);
	glBindVertexArray(LampVao);
	glBindBuffer(GL_ARRAY_BUFFER, ContainerVbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

}
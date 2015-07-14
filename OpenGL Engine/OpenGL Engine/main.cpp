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

int WindowWidth = 1600;
int WindowHeight = 900;
GLfloat fov = 45.0f;
GLfloat aspectRatio = (GLfloat)WindowWidth / WindowHeight;
glm::vec3 cameraPos(0.0f, 1.0f, 3.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f, pitch = 0.0f;
GLfloat lastX = WindowWidth / 2.0;
GLfloat lastY = WindowHeight / 2.0;
bool keys[1024];
bool firstMouse = true;
GLFWwindow* window;
bool cursorIsNormal = false;
bool ControlTheView = true;
bool CursorIsFree = false;
bool ControlTheKeyboard = true;
GLfloat cameraSpeed = 0.05f;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void window_resizeCallback(GLFWwindow* window, int width, int height)
{
	aspectRatio = (GLfloat)width / height;
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
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (ControlTheView)
	{

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
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



}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
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
	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
	{
		cameraSpeed += 0.1f;
	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
	{
		cameraSpeed -= 0.1f;
	}




}

void doMovement(void)
{
	if (ControlTheKeyboard)
	{
		if (keys[GLFW_KEY_W])
		{
			cameraPos += glm::cross(glm::cross(cameraUp, cameraFront), cameraUp) * cameraSpeed;
		}
		if (keys[GLFW_KEY_S])
		{
			cameraPos += -glm::cross(glm::cross(cameraUp, cameraFront), cameraUp) * cameraSpeed;
		}
		if (keys[GLFW_KEY_A])
		{
			cameraPos += glm::cross(cameraUp, cameraFront) * cameraSpeed;
		}
		if (keys[GLFW_KEY_D])
		{
			cameraPos += -glm::cross(cameraUp, cameraFront)* cameraSpeed;
		}
		if (keys[GLFW_KEY_SPACE])
		{
			cameraPos += cameraUp * cameraSpeed;

		}
		if (keys[GLFW_KEY_LEFT_CONTROL])
		{
			cameraPos += -cameraUp * cameraSpeed;
		}
	}







}

// Shaders
const GLchar* vertexShaderSource = "#version 440 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 normalCoords;\n"
//"layout (location = 2) in vec2 texture; \n"
"uniform mat4 model;\n"
"uniform mat4 view; \n"
"uniform mat4 projection; \n"
"out vec3 normal;"
"out vec3 fragPosition;"
//"out vec2 TexCoord; \n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(position.x,position.y,position.z, 1.0);\n"
"normal = mat3(transpose(inverse(model))) * normalCoords;"
"fragPosition = vec3(model * vec4(position,1.0f));"
//"TexCoord = vec2(texture.x, 1.0-texture.y);"
"}\0";

//"vec3 ambient = ambientStrength * lightColor;\n"
//"vec3 result = ambient * objectColor;\n"
//"uniform vec3 objectColor;\n"
//"uniform vec3 lightColor;\n "
//"color = mix(texture(TextureWall,TexCoord),texture(TextureSmile,vec2(1.0 - TexCoord.x , TexCoord.y)),0.2);\n"
//"color = vec4(reuslt,1.0f);\n"
const GLchar* fragmentShaderSource = "#version 440 core\n"
//"in vec2 TexCoord; \n"
"out vec4 color;\n"
"in vec3 normal;\n"
"in vec3 fragPosition;"
//"uniform sampler2D TextureWall;\n"
//"uniform sampler2D TextureSmile;\n"
"uniform vec3 objectColor;"
"uniform vec3 lightColor;"
"uniform vec3 lightPos;"
"uniform vec3 viewPos;"
"void main()\n"
"{\n"
"float ambientStrength = 0.1f;"
"float specularStrength = 0.5f;"
"vec3 norm = normalize(normal);"
"vec3 lightDir = normalize(lightPos - fragPosition);"
"float diff = max ( dot( norm, lightDir), 0.0);"
"vec3 diffuse = diff * lightColor;"
"vec3 ambient = lightColor * ambientStrength;"
"vec3 ViewDir = normalize(viewPos - fragPosition);"
"vec3 reflectDir = reflect(-lightDir,norm);"
"float spec = pow(max(dot(ViewDir,reflectDir),0.0),32);"
"vec3 specular = lightColor * spec * specularStrength;"
"vec3 resultColor = (ambient + diffuse + specular ) * objectColor;"
"color = vec4(resultColor,1.0f);"
//"color = mix(texture(TextureWall,TexCoord),texture(TextureSmile,vec2(1.0 - TexCoord.x , TexCoord.y)),0.2);\n"
"}\n\0";

const GLchar* lampFragmentShaderSource = "#version 440 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f,1.0f,1.0f,1.0f);"
"}\0";



int main(void)
{

	glfwSetErrorCallback(error_callback);

	/* Initialize GLFW */
	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(WindowWidth, WindowHeight, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	/* Initialize GLEW */
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_sizeCallback);
	glfwSetWindowSizeCallback(window, window_resizeCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, mouse_scrollback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glViewport(0, 0, WindowWidth, WindowHeight);



	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};


	GLint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLfloat ground[] =
	{
		-100.0f, -100.0f, 0.0f,
		100.0f, -100.0f, 0.0f,
		100.0f, 100.0f, 0.0f,
		-100.0f, 100.0f, 0.0f,
	};
	GLuint groundIndices[] =
	{
		0, 1, 2,
		2, 3, 0

	};

	GLuint vao, vaoGround;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenVertexArrays(1, &vaoGround);


	GLuint vbo, ebo;
	// Generate vertex array buffer on videocard memory
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// Connect Array buffer with our created vertex array buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Transform our data to array buffer to the car memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	glBindVertexArray(0);

	glBindVertexArray(vaoGround);

	GLuint vboGround, eboGround;
	glGenBuffers(1, &vboGround);
	glGenBuffers(1, &eboGround);
	glBindBuffer(GL_ARRAY_BUFFER, vboGround);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboGround);

	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	GLuint lampVao;
	glGenVertexArrays(1, &lampVao);
	glBindVertexArray(lampVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	
	
	// Create manager, that initialize and create all programs
	ProgramManager manager;
	manager.readConfig();
	if (!manager.createPrograms())
	{
		getchar();
		return -1;
	}


	GLuint textures[2];
	glGenTextures(2, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("../Resources/texture/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("../Resources/texture/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);



	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Program loop

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};


	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;



	
	// Get programs from manager by name
	Program* mainProgram = manager.returnProgram("mainProgram");
	Program* lightProgram = manager.returnProgram("lightProgram");
	/// Get GLint program id from Program
	GLint shaderProgram = mainProgram->returnProgramNameID();
	GLint lampProgram = lightProgram->returnProgramNameID();
	
	while (!glfwWindowShouldClose(window))
	{
		// Check and call events
		glfwPollEvents();
		doMovement();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainProgram->use();
		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniform1i(glGetUniformLocation(shaderProgram, "TextureWall"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glUniform1i(glGetUniformLocation(shaderProgram, "TextureSmile"), 1);


		GLfloat radius = 50.0f;
		glm::vec3 lightPosition = glm::vec3(0.0f, 7.0f, -5.0f);
		
		mainProgram->setUniformData("light.ambient", 0.2f, 0.2f, 0.2f);
		mainProgram->setUniformData("light.diffuse", 0.5f, 0.5f, 0.5f);
		mainProgram->setUniformData("light.specular",1.0f, 1.0f, 1.0f);
		mainProgram->setUniformData("light.direction",-0.2f,-1.0f,-0.3f);

		mainProgram->setUniformData("material.ambient",1.0f, 0.5f, 0.31f);
		mainProgram->setUniformData("material.diffuse",1.0f, 0.5f, 0.31f);
		mainProgram->setUniformData("material.specular",0.5f, 0.5f, 0.5f);
		mainProgram->setUniformData("material.shininess",32.0f);

		mainProgram->setUniformData("lightPos", lightPosition.x, lightPosition.y, lightPosition.z);
		mainProgram->setUniformData("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);


		//viewMatrix = glm::mat4();
		projectionMatrix = glm::mat4();
		modelMatrix = glm::mat4();
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 2.0f, 0.0f));

		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//viewMatrix = glm::rotate(viewMatrix,(GLfloat)(glm::radians(10.0f)*glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);


		mainProgram->setUniformData("view", viewMatrix);
		mainProgram->setUniformData("projection", projectionMatrix);

		glBindVertexArray(vao);
		mainProgram->setUniformData("model", modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		/*
		glBindVertexArray(vaoGround);
		modelMatrix = glm::mat4();
		modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		glDrawElements(GL_TRIANGLES,6 , GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		*/

		lightProgram->use();
 
		lightProgram->setUniformData("view", viewMatrix);
		

		lightProgram->setUniformData("projection", projectionMatrix);

		modelMatrix = glm::mat4();
		modelMatrix = glm::translate(modelMatrix, lightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		lightProgram->setUniformData("model", modelMatrix);
		
		glBindVertexArray(lampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Swap the buffers 
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
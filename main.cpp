#include "libs.h"


Vertex vertices[] =
{

	// Color
	glm::vec3(0.0f, 0.5f, 0.f), glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(0.f, 1.f),
	glm::vec3(1.f, 0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),     glm::vec2(0.f, 0.f),
	glm::vec3(0.f, 1.f, 0.f),	glm::vec3(0.5f, -0.5f, 0.f),  glm::vec2(1.f, 0.f)


};
unsigned nrOfVertices = sizeof(vertices / sizeof(Vertex));

GLuint indices[] = {
	0, 1, 2 };
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

// Beginning of updating input, getting events - TODO
void updateInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

bool loadShaders(GLuint& program)
{
	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	// Vertex
	in_file.open("vertex_core.glsl");
	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{
		std::cout << "Could not open Vertex file"
			<< "\n";
		loadSuccess = false;
	}
	in_file.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLcharARB* vertSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Could not compile Vertex shader"
			<< "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	temp = "";
	src = "";

	// Fragment
	in_file.open("fragment_core.glsl");
	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{
		std::cout << "Could not open fragment file"
			<< "\n";
		loadSuccess = false;
	}
	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLcharARB* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Could not compile fragment shader"
			<< "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	temp = "";
	src = "";

	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "Could not link program"
			<< "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	glUseProgram(0);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return loadSuccess;
}

int main()
{
	// Initialzing GFLW
	glfwInit();

	// Creating Window
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); MAC?

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL test", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	// glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	// glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);

	// Initializing GLEW because it needs a window and OpenGL context

	glewExperimental = GL_TRUE;

	// Error accounting
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED"
			<< "\n";
		glfwTerminate();
	}

	// OpenGL Options
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);

	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// SHADER INIT

	GLuint shader_program;
	if (!loadShaders(shader_program))
		glfwTerminate();

	// Vertex Array Object among others
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Generate EBO, bind and send data
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set VertexAttribPointers and enable, AKA input assembly
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	// Texture Coordinate
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	// Unbind VAO
	glBindVertexArray(0);

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// update input
		glfwPollEvents();
		// update
		updateInput(window);
		// Clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// What shader to use?
		glUseProgram(shader_program);

		// Bind vertex array object

		glBindVertexArray(VAO);

		// Draw
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

		// End Draw

		glfwSwapBuffers(window);
		glFlush();
	};

	// Program Cleanup -- DestroyWindow possibly redundant
	glfwDestroyWindow(window);
	glfwTerminate();
	glDeleteProgram(shader_program);

	return 0;
}
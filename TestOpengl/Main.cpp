#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0 ) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	vertexColor = vec4(1f, 0f, 0f, 1f);\n"	
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"	FragColor = vertexColor;\n"
"}\n\0";

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGl we are using
	// So in this case we are using OpenGl 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the core profile
	// This means we only have access to the modern functions
	// GLFW_OPENGL_COMPAT_PROFILE contains older functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat verticies[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5 * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
	};

	// Create the GLFW window object and specifies some basic properties of the window
	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL Test", NULL, NULL);
	// In the case the window object fails to create, terminate glfw and return
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window";
		glfwTerminate();
		return -1;
	}
	// Make the context of the specified window current
	glfwMakeContextCurrent(window);
	
	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	// In this case our viewport goes from x = 0, y = 0, to x = 1280, y = 720
	glViewport(0, 0, 1280, 720);

	// Create reference to store vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Replaces source code of vertexShader to the vertexShaderSource
	// We do this because vertexShader has no shader source since we just defined it
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compiles the source code strings in vertexShader
	glCompileShader(vertexShader);

	// Create reference to store fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Replaces source code of fragmentShader to the fragmentShaderSource
	// We do this because fragmentShader has no shader source since we just defined it
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compiles the source code strings in fragmentShader
	glCompileShader(fragmentShader);

	// Create Shader program
	GLuint shaderProgram = glCreateProgram();
	// Attach vertex shader
	glAttachShader(shaderProgram, vertexShader);
	// Attach fragment shader
	glAttachShader(shaderProgram, fragmentShader);
	// Links the shaderProgram object
	glLinkProgram(shaderProgram);

	// Delete shaders since we attatched them to the shaderProgram already
	// This frees memory
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// Create vertex array object and vertex buffer object
	GLuint VAO, VBO;

	// Generate vertex array object names
	glGenVertexArrays(1, &VAO);
	// Generate buffer object names
	glGenBuffers(1, &VBO);

	// Bind the vertex array object
	glBindVertexArray(VAO);

	// Bind named buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Create buffer object data
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	// Define an array of generic vertex attribute data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enables the generic vertex attribute array
	glEnableVertexAttribArray(0);

	// Bind both VBO and VAO to 0 so we dont accidentally modify anything
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	 

	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	// Main while loop
	while (!glfwWindowShouldClose(window)) 
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		// Draw the triangle using the GL_TRIANGLES primitive object
		// The second value is the starting index in the enabled arrays
		// The third value is the number of indicies to be rendered 
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffer frames
		// This ensures the image gets updated each frame
		glfwSwapBuffers(window);
		// Process all pending GLFW events
		glfwPollEvents();
	}

	// Delete objects we've already used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
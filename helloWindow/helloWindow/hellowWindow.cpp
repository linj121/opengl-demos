#include <glad/glad.h>;
#include <GLFW/glfw3.h>;
#include <iostream>; // https://stackoverflow.com/questions/11375949/cout-is-not-a-member-of-std

// WHY do we have to declare a function outside of main() ?
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
	// Backward compatibility
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create an OPENGL context for the current calling thread
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Callback for window resize event
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load OPENGL function pointers implemented by OS drivers (in your case, MS Windows)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Main render loop
	// We don't want the program to just render one frame and quit
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Set state
		glClearColor(245.0f / 255.0f, 40.0f / 255.0f, 145.0f / 255.0f, 0.8f);
		// Use the state
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
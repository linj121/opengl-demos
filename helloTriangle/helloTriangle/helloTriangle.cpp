#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


struct Vertex {
    float x;
    float y;
    float z;
};

struct Triangle {
    Vertex v1;
    Vertex v2;
    Vertex v3;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const short int NUMBER_OF_TRIANGLES = 3;
const short int NUMBER_OF_SHADERS = 3;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
short int wireframe = 0; // 0 -> fill; 1 -> wireframe

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSources[] = {
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n" // Red
    "}\n\0",

    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n" // Green
    "}\n\0",

    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n" // Blue
    "}\n\0",
};

// input state
int lastInputState = -99;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "UrMOM", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShaders[NUMBER_OF_SHADERS]{};
    for (int i = 0; i < NUMBER_OF_SHADERS; i++)
    {
        // fragment shader
        fragmentShaders[i] = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaders[i], 1, &fragmentShaderSources[i], NULL);
        glCompileShader(fragmentShaders[i]);
        // check for shader compile errors
        glGetShaderiv(fragmentShaders[i], GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShaders[i], 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    // link shaders
    unsigned int shaderPrograms[NUMBER_OF_SHADERS]{};
    for (int i = 0; i < NUMBER_OF_SHADERS; i++)
    {
        shaderPrograms[i] = glCreateProgram();
        glAttachShader(shaderPrograms[i], vertexShader);
        glAttachShader(shaderPrograms[i], fragmentShaders[i]);
        glLinkProgram(shaderPrograms[i]);
        // check for linking errors
        glGetProgramiv(shaderPrograms[i], GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderPrograms[i], 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }
    // clean ups
    for (int i = 0; i < NUMBER_OF_SHADERS; i++) glDeleteShader(fragmentShaders[i]);
    glDeleteShader(vertexShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    //float vertices[] = {
    //     0.5f,  0.5f, 0.0f,  // top right
    //     0.5f, -0.5f, 0.0f,  // bottom right
    //    -0.5f, -0.5f, 0.0f,  // bottom left
    //    -0.5f,  0.5f, 0.0f   // top left 
    //};
    //unsigned int indices[] = {  // note that we start from 0!
    //    0, 1, 3,  // first Triangle
    //    1, 2, 3   // second Triangle
    //};

    Triangle triangles[NUMBER_OF_TRIANGLES] = {
        {
            { -1.0f, 0.0f,   0.0f, }, // triangle 1 bottom left
            { -0.5f, 0.866f, 0.0f, }, // triangle 1 top
            {  0.0f, 0.0f,   0.0f, }, // triangle 1 bottom right
        },
        {
            { 0.0f, 0.0f,   0.0f, }, // triangle 2 bottom left
            { 0.5f, 0.866f, 0.0f, }, // triangle 2 top
            { 1.0f, 0.0f,   0.0f, }, // triangle 2 bottom right
        },
        {
            {  0.0f,  0.0f,   0.0f, },
            { -0.5f, -0.866f, 0.0f, },
            {  0.5f, -0.866f, 0.0f, },
        }
    };

    // Generate multiple VAOs and VBOs
    unsigned int VAOs[NUMBER_OF_TRIANGLES], VBOs[NUMBER_OF_TRIANGLES];
    glGenVertexArrays(NUMBER_OF_TRIANGLES, VAOs);
    glGenBuffers(NUMBER_OF_TRIANGLES, VBOs);
    
    for (int i = 0; i < NUMBER_OF_TRIANGLES; i++)
    {
        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangles[i]), &triangles[i], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }



    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        
        // Draw wireframe?
        glPolygonMode(GL_FRONT_AND_BACK, wireframe == 1 ? GL_LINE : GL_FILL);

        // Background Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        for (int i = 0; i < NUMBER_OF_TRIANGLES; i++)
        {
            glUseProgram(shaderPrograms[i % NUMBER_OF_SHADERS]);

            glBindVertexArray(VAOs[i]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
        }

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VBOs);
    //glDeleteBuffers(1, &EBO);
    for (int i = 0; i < NUMBER_OF_SHADERS; i++) glDeleteProgram(shaderPrograms[i]);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else
    {   
        // A key press event could exist across several consecutive rendering loops
        // Only toggle the wireframe option when there is a state change
        int spaceKeyState = glfwGetKey(window, GLFW_KEY_SPACE);
        if (spaceKeyState == GLFW_PRESS)
        {
            if (lastInputState != GLFW_PRESS)
            {
                lastInputState = GLFW_PRESS;
                wireframe = 1 - wireframe;
            }
        }
        else
        {
            if (lastInputState != GLFW_RELEASE) lastInputState = GLFW_RELEASE;
        }
           
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

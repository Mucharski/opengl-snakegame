const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <deque>
#include <cstdlib>   
#include <ctime> 
#include <cmath>

#define MAX_LENGTH (100 * 12)
#define MAX_INDICES_LENGTH (MAX_LENGTH * 6)

float snakeXVelocity = 0.010f, snakeYVelocity = 0.000f;

int snakeLength = 3;

float snakeHeadX = 0.000f, snakeHeadY = 0.000f;

float snakeBodyX[MAX_LENGTH];
float snakeBodyY[MAX_LENGTH];

std::deque<std::pair<float, float>> snakeHistoric;

int positionDelay = 8;

float fruitVertice[9];

unsigned int fruitIndice[] = {  // note that we start from 0!
    0, 1, 2,   // first triangle
};

float fruitX = 0.0f, fruitY = 0.0f;

void updateFruitVertice() {
    float half = 0.025f;

    fruitVertice[0] = fruitX;
    fruitVertice[1] = fruitY + half;
    fruitVertice[2] = 0.0f;

    fruitVertice[3] = fruitX - half;
    fruitVertice[4] = fruitY - half;
    fruitVertice[5] = 0.0f;

    fruitVertice[6] = fruitX + half;
    fruitVertice[7] = fruitY - half;
    fruitVertice[8] = 0.0f;
}

void initializeHistoric() {
    int totalSize = snakeLength * MAX_LENGTH + 1;

    for (int i = 0; i < totalSize; i++) {
        snakeHistoric.push_back({ snakeHeadX, snakeHeadY });
    };
}

bool snakeCollisionWithItself() {
    for (int i = 1; i < snakeLength * 6; i++) {
        if (fabs(snakeHeadX - snakeBodyX[i]) < 0.010f && fabs(snakeHeadY - snakeBodyY[i]) < 0.010f) {
            return true;
        }
    }
    return false;
}

void updateSnakePosition() {
    snakeHeadX += snakeXVelocity;
    snakeHeadY += snakeYVelocity;

    if (snakeHeadX > 1.00f) snakeHeadX = -1.00f;
    if (snakeHeadX < -1.00f) snakeHeadX = 1.00f;
    if (snakeHeadY > 1.00f) snakeHeadY = -1.00f;
    if (snakeHeadY < -1.00f) snakeHeadY = 1.00f;

    snakeHistoric.push_back({ snakeHeadX, snakeHeadY });

    int maxHistoricSize = snakeLength * positionDelay + 1;

    if (snakeHistoric.size() > maxHistoricSize) {
        snakeHistoric.pop_front();
    }
}

void updateVertices(float* vertices) {
    float offsetsX[4] = { 0.025f, 0.025f, -0.025f, -0.025f };
    float offsetsY[4] = { -0.025f, 0.025f, 0.025f, -0.025f };

    for (int block = 0; block < snakeLength; block++) {
        int historicIndex = snakeHistoric.size() - 1 - block * positionDelay;

        auto position = snakeHistoric[historicIndex];

        for (int i = 0; i < 4; i++) {
            int verticeIndex = block * 4 + i;

            snakeBodyX[verticeIndex] = position.first + offsetsX[i];
            snakeBodyY[verticeIndex] = position.second + offsetsY[i];
        }
    }

    for (int i = 0; i < snakeLength * 4; i++) {
        vertices[i * 3] = snakeBodyX[i];
        vertices[i * 3 + 1] = snakeBodyY[i];
        vertices[i * 3 + 2] = 0.00f;
    }    
}

void updateIndices(unsigned int* indices) {
    for (int i = 0; i < snakeLength; i++) {
        int vertexoffset = i * 4;
        int indexoffset = i * 6;

        indices[indexoffset + 0] = vertexoffset + 0;
        indices[indexoffset + 1] = vertexoffset + 1;
        indices[indexoffset + 2] = vertexoffset + 2;

        indices[indexoffset + 3] = vertexoffset + 0;
        indices[indexoffset + 4] = vertexoffset + 2;
        indices[indexoffset + 5] = vertexoffset + 3;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::cout << "Redimensionando a janela" << std::endl;

    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        std::cout << "CIMA" << std::endl;

        if (snakeYVelocity == -0.010f) return;

        snakeYVelocity = 0.010f;
        snakeXVelocity = 0.000f;
    }

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        std::cout << "BAIXO" << std::endl;

        if (snakeYVelocity == 0.010f) return;

		snakeYVelocity = -0.010f;
		snakeXVelocity = 0.000f;
	}

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        std::cout << "DIREITA" << std::endl;

        if (snakeXVelocity == -0.010f) return;

        snakeXVelocity = 0.010f;
        snakeYVelocity = 0.000f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        std::cout << "ESQUERDA" << std::endl;

        if (snakeXVelocity == 0.010f) return;

        snakeXVelocity = -0.010f;
        snakeYVelocity = 0.000f;
    }
}

void someOpenGLFunctionThatDrawsOurTriangle(unsigned int shader, unsigned int vao)
{
    glUseProgram(shader);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6 * snakeLength, GL_UNSIGNED_INT, 0);
}

void openGLDrawFuit(unsigned int shader, unsigned int vao) {
    glUseProgram(shader);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void drawFruit(unsigned int vertexShader) {

    updateFruitVertice();

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fruitVertice), fruitVertice, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fruitIndice), fruitIndice, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    openGLDrawFuit(shaderProgram, VAO);
}

void spawnFruit() {
    bool validPosition = false;

    float notNextToSnake = 0.05f;

    while (!validPosition) {
        fruitX = ((rand() % 21) - 10) / 10.0f;
        fruitY = ((rand() % 21) - 10) / 10.0f;

        validPosition = true;

        if (fruitX > 0.90f || fruitY > 0.90f || fruitX < -0.90f || fruitY < -0.90f) {
            validPosition = false;
            continue;
        }

        for (auto& pos : snakeHistoric) {
            if (fabs(fruitX - pos.first) < notNextToSnake && fabs(fruitY - pos.second) < notNextToSnake) {
                validPosition = false;
                break;
            }
        }
    }

    updateFruitVertice();
}

void drawSnake(unsigned int vertexShader) {
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float vertices[MAX_LENGTH];

    updateVertices(vertices);

    unsigned int indices[MAX_INDICES_LENGTH];

    updateIndices(indices);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    someOpenGLFunctionThatDrawsOurTriangle(shaderProgram, VAO);
}

bool snakeAteFruit() {
    float notNextToSnake = 0.05f;
    return (fabs(snakeHeadX - fruitX) < notNextToSnake && fabs(snakeHeadY - fruitY) < notNextToSnake);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //----------------------------------------------------------    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, 800, 600);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    initializeHistoric();

    spawnFruit();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        updateSnakePosition();

        drawFruit(vertexShader);

        drawSnake(vertexShader);

        if (snakeCollisionWithItself() && snakeLength > 3) {
            std::cout << "A cobra bateu" << std::endl;
            break;
        }

        if (snakeAteFruit()) {
            std::cout << "Bateu na fruta" << std::endl;

            snakeLength++;

            spawnFruit();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
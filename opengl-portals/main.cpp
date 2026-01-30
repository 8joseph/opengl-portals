#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "testCube.h"
#include "Camera.h"

#include "Model.h"
#include <iostream>

#include "ShaderClass.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera mainCamera(glm::vec3(1, 0, 0));


//variables for delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
bool captureMouse = true;

float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;



int main(){
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Window creation failed!" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    Shader shader("shader.vs", "shader.fs");
    glEnable(GL_DEPTH_TEST);
    

    stbi_set_flip_vertically_on_load(true);
    Model m("models/test-toilet.obj");
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 view;
        shader.use();
        view = mainCamera.getViewMatrix(); 
        shader.setMat4("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // center it
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // scale it
        shader.setMat4("model", model);


        m.setPosition(m.getPosition() + glm::vec3(sin( glfwGetTime()) *  0.0001f, 0.0f, 0.0f));
        m.draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

//if the window is resized, resize the glfw viewport to match
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    const float camSpeed = 1.4f * deltaTime;
    glm::vec3 cameraPos = mainCamera.getPosition();
    glm::vec3 cameraFront = mainCamera.getFront();
    glm::vec3 cameraUp = mainCamera.getUp();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += camSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= camSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= camSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += camSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }

    mainCamera.setPosition(cameraPos);
}

//taken from learnopengl.com
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    mainCamera.updatePitchAndYaw(yoffset, xoffset);
    
}

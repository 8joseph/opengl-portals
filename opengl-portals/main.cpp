#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Scene.h"
#include "Camera.h"
#include "Portal.h"
#include <glm/gtx/matrix_decompose.hpp>


#include "Model.h"
#include <iostream>

#include "ShaderClass.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void render(Camera camera, Shader shader, Shader portalShader, Scene s, int recursionLevel);


const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
GLFWwindow* window; 


//variables for delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
bool captureMouse = true; //check - think this is redundant

float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;

int maxRecursionLevel = 1;



int main(){
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Portals", NULL, NULL);
    
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
    Shader portalShader("shader.vs", "portalShader.fs");
    Scene scene;

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        
    glEnable(GL_CULL_FACE); //enable face culling
    glCullFace(GL_FRONT);    //cull front faces (Should be back?? but culling front is what works for some reason)
    glFrontFace(GL_CW);     //font faces are clockwise
    
    stbi_set_flip_vertically_on_load(true); //maybe change this 
    


    Model base = Model("models/prototype/prototype.obj");
    base.setScale(glm::vec3(0.2f));
    base.setPosition(glm::vec3(0.0f, -0.8f, 0.0f));
 
    scene.addObject(&base);

    Model monkey = Model("models/monkey/monkey.obj");
    monkey.setScale(glm::vec3(0.3f));
    glm::vec3 monkeyStartPos = glm::vec3(-0.0f, 0.1f, 0.0f);
    monkey.setPosition(monkeyStartPos);
    scene.addObject(&monkey);

    Portal p1 = Portal();
    p1.setPosition(glm::vec3(-1.0f, 0.0f, -1.0f));
    p1.setRotation(glm::vec3(0.0f, 180.0f, 0.0f)); 
    
    Portal p2 = Portal();
    p2.setPosition(glm::vec3(1.0f, 0.0f, -1.0f));
    p2.setRotation(glm::vec3(0.0f, 180.0f, 0.0f));



    p1.setLinkedPortal(&p2);
    p2.setLinkedPortal(&p1);
    
    scene.addPortal(&p2);
    scene.addPortal(&p1);


 

    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        monkey.setRotation(glm::vec3(0.0f, monkey.getRotation().y + sin(deltaTime) * 80, 0.0f));
        monkey.setPosition(glm::vec3( monkeyStartPos.x  + sin(glfwGetTime()) * 1.0f, monkeyStartPos.y, monkeyStartPos.z));

        //m.setPosition(m.getPosition() + glm::vec3(sin( glfwGetTime()) *  0.0001f, 0.0f, 0.0f));
        render(mainCamera, shader, portalShader, scene, 0);
        glfwSwapBuffers(window);


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

void render(Camera camera, Shader shader, Shader portalShader, Scene s, int recursionLevel)
{   
    //glEnable(GL_STENCIL_TEST);
    //glStencilMask(0xFF);

    //glStencilFunc(GL_EQUAL, recursionLevel, 0xFF);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);



    glm::mat4 view;
    view = camera.getViewMatrix();
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);
    
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.use();
    //shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    portalShader.use();
    //portalShader.setMat4("model", model);
    portalShader.setMat4("view", view);
    portalShader.setMat4("projection", projection);


    s.draw(shader, camera);

    glfwPollEvents();

    if (recursionLevel >= maxRecursionLevel)//CHANGE TO maxRecursionLevel!!
    {
        return;
    }

    ///------------------------------------------
    for (Portal* portal : s.getPortals()) //
    {
        //write to the stencil buffer with 1
        glEnable(GL_STENCIL_TEST);

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF); 
        glStencilMask(0xFF);
        
        //disable colour + depth buffer
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        //draw portal so stencil buffer is set
        portalShader.use();
        portalShader.setMat4("view", view);
        portalShader.setMat4("projection", projection);
        portal->draw(portalShader);

        //turn the colour + depth buffers back on
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        //make it so the stencil buffer will only pass fragments when equal to 1
        //and stop writing to the stencil buffer
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilMask(0x00);

        //clear the depth buffer DONT USE
        //glClear(GL_DEPTH_BUFFER_BIT);

        //we cannot clear the depth buffer, so instead set the depth buffer to max distance where the stencil buffer allows
        //turn off colour mask, make sure that depth will always be written
        glDepthMask(GL_TRUE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthFunc(GL_ALWAYS);
        glDepthRange(1.0, 1.0);
        
        //draw portal AGAIN
        portalShader.use();
        portalShader.setMat4("view", view);
        portalShader.setMat4("projection", projection);
        portal->draw(portalShader);

        //reset stuff back
        glDepthFunc(GL_LESS);
        glDepthRange(0.0, 1.0);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);



        //draw the portal so if there is nothing facing the portal the wall behind the portal does not get rendered
        glDisable(GL_DEPTH_TEST);
        portalShader.use(); 
        portalShader.setMat4("view", view);
        portalShader.setMat4("projection", projection);
        portal->draw(portalShader);
        glEnable(GL_DEPTH_TEST);


  
        glm::mat4 portalMatrix = portal->getModelMatrix();
        glm::mat4 linkedPortalMatrix = portal->getLinkedPortal()->getModelMatrix();
        glm::mat4 t2 = linkedPortalMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::inverse(portalMatrix);
        glm::vec3 newCamPos = glm::vec3(t2 * glm::vec4(camera.getPosition(), 1.0f));
        glm::vec3 newCamFront = glm::normalize(glm::vec3(t2 * glm::vec4(camera.getFront(), 0.0f)));
        
        float pitch = glm::degrees(asin(newCamFront.y));
        float yaw = glm::degrees(atan2(newCamFront.z, newCamFront.x));
     
        //trans = glm::translate(trans, )


        Camera newCam(newCamPos);
        newCam.setPitchAndYaw(pitch, yaw);



        // render the reflection
        render(newCam, shader, portalShader, s, recursionLevel + 1);

        //cleanup
        glDisable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glClear(GL_STENCIL_BUFFER_BIT);
    }
}




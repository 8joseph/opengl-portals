#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Scene.h"
#include "Camera.h"
#include "Portal.h"


#include "Model.h"
#include <iostream>

#include "ShaderClass.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void render(Camera camera, Shader shader, Shader portalShader, Scene s, int recursionLevel, glm::mat4 currentProjectionMatrix);
glm::mat4 ModifyProjectionMatrix(const glm::vec4& clipPlane, glm::mat4 matrix);
inline float sgn(float a);




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
    glCullFace(GL_BACK);    //cull back faces
    glFrontFace(GL_CCW);     //font faces are clockwise
    
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
    
    //Portal p2 = Portal();
    //p2.setPosition(glm::vec3(1.0f, 0.0f, -1.0f));
    //p2.setRotation(glm::vec3(0.0f, 180.0f, 0.0f));



    //p1.setLinkedPortal(&p2);
    //p2.setLinkedPortal(&p1);
    
    //scene.addPortal(&p2);
    scene.addPortal(&p1);

    glm::mat4 startProjection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 30.0f);

    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glStencilMask(0xFF);//CHECK IF THIS IS NEEDED (reset the stencil mask so the stencil bit can be cleared)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glDisable(GL_STENCIL_TEST);//CHECK IF THIS IS NEEDED AS WELL (make sure the stencil test is disabled for the first call of render, so the scene is drawn from the main camera.)

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        monkey.setRotation(glm::vec3(0.0f, monkey.getRotation().y + sin(deltaTime) * 80, 0.0f));
        monkey.setPosition(glm::vec3( monkeyStartPos.x  + sin(glfwGetTime()) * 1.0f, monkeyStartPos.y, monkeyStartPos.z));

        render(mainCamera, shader, portalShader, scene, 0, startProjection);
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

void render(Camera camera, Shader shader, Shader portalShader, Scene s, int recursionLevel, glm::mat4 currentProjectionMatrix)
{   
    glm::mat4 view;
    view = camera.getViewMatrix();
    
    glm::mat4 model = glm::mat4(1.0f);
    
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", currentProjectionMatrix);

    portalShader.use();
    portalShader.setMat4("view", view);
    portalShader.setMat4("projection", currentProjectionMatrix);


    s.draw(shader, camera);

    glfwPollEvents();

    if (recursionLevel >= maxRecursionLevel)//CHANGE TO maxRecursionLevel!!
    {
        return;
    }

    ///------------------------------------------
    for (Portal* portal : s.getPortals()) //
    {
        //write to stencil buffer so portal is only drawn where it is
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//if the stencil test passes replace the stencil value
        glStencilFunc(GL_ALWAYS, recursionLevel+1, 0xFF);//always pass the stencil test and write a 1
        glStencilMask(0xFF);//enable writing to the stenciil buffer

        //the colour and depth masks need to be turned off as we do not want to write to those for this run of drawing the portal
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        //draw the portal
        portalShader.use();
        portalShader.setMat4("view", camera.getViewMatrix());
        portalShader.setMat4("projection", currentProjectionMatrix);
        portal->draw(portalShader);

        //turn the colour + depth buffers back on
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);


        glStencilFunc(GL_EQUAL, recursionLevel+1, 0xFF);//only draw where the portal is
        glStencilMask(0x00);//dont write to stencil buffer
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);//keep the values inside the stencil buffer

        glm::vec3 newCamPos = portal->getPosition();
        Camera newCam = Camera(newCamPos);



        render(newCam, shader, portalShader, s, recursionLevel + 1, currentProjectionMatrix);
        
    }
}



/*The code below is for oblique frustrum clipping. 
Taken from: https://terathon.com/blog/oblique-clipping.html 
(and modified to work in modern OpenGL)
*/

inline float sgn(float a)
{
    if (a > 0.0F) return (1.0F);
    if (a < 0.0F) return (-1.0F);
    return (0.0F); //maybe make this 1??
}

glm::mat4 ModifyProjectionMatrix(const glm::vec4& clipPlane, glm::mat4 matrix)
{   
    glm::vec4 q;

    // Calculate the clip-space corner point opposite the clipping plane
    // as (sgn(clipPlane.x), sgn(clipPlane.y), 1, 1) and
    // transform it into camera space by multiplying it
    // by the inverse of the projection matrix

    q.x = (sgn(clipPlane.x) + matrix[2][0]) / matrix[0][0];
    q.y = (sgn(clipPlane.y) + matrix[2][1]) / matrix[1][1];
    q.z = -1.0F;
    q.w = (1.0F + matrix[2][2]) / matrix[3][2];

    // Calculate the scaled plane vector
    glm::vec4 c = clipPlane * (2.0F / glm::dot(clipPlane, q));

    // Replace the third row of the projection matrix
    matrix[0][2] = c.x;
    matrix[1][2] = c.y;
    matrix[2][2] = c.z + 1.0F;
    matrix[3][2] = c.w;


    return matrix;
}
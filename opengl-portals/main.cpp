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
void render(Camera camera, Shader shader, Shader portalShader, Scene scene, int recursionLevel, glm::mat4 currentProjectionMatrix);
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

int maxRecursionLevel = 5;

double prevTime = glfwGetTime();
int frameCount = 0;


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
    
    stbi_set_flip_vertically_on_load(false); //leave this as false.
    


    Model monkey = Model("models/monkey/monkey.obj");
    monkey.setScale(glm::vec3(0.3f));
    glm::vec3 monkeyStartPos = glm::vec3(-0.0f, 0.1f, 0.0f);
    monkey.setPosition(monkeyStartPos);
    //scene.addObject(&monkey);

    Model dust2 = Model("models/dust2/dust2.obj");
    dust2.setScale(glm::vec3(0.01, 0.01, 0.01));
    //scene.addObject(&dust2);

    Model floor = Model("models/prototype/floor.obj");
    floor.setScale(glm::vec3(0.5));
    floor.setPosition(glm::vec3(0.0f, -1.3f, 0.0f)); 
    scene.addObject(&floor);    

    Portal p1 = Portal();
    p1.setPosition(glm::vec3(1.0f, 0.5f, 0.0f));
    p1.setRotation(glm::vec3(0.0f, 0.0f, 0.0f)); 

    Model frame1 = Model("models/portal-frame/portal-frame2.obj");
    frame1.setPosition(p1.getPosition());
    frame1.setRotation(p1.getRotation());
    scene.addObject(&frame1);

    
    Portal p2 = Portal();   
    p2.setPosition(glm::vec3(1.0f, 0.5f, -2.0f));
    p2.setRotation(glm::vec3(0.0f, 180.0f, 0.0f));
    Model frame2 = Model("models/portal-frame/portal-frame2.obj");
    frame2.setPosition(p2.getPosition());
    frame2.setRotation(p2.getRotation());
    scene.addObject(&frame2);



    p1.setLinkedPortal(&p2);
    p2.setLinkedPortal(&p1);
    
    scene.addPortal(&p2);
    scene.addPortal(&p1);

    Model teapot = Model("models/teapot/teapot.obj");
    teapot.setScale(glm::vec3(0.01, 0.01, 0.01));
    glm::vec3 teapotStartPos = glm::vec3(-1.0f, -0.5f, 0.0f);
    teapot.setPosition(teapotStartPos);
    scene.addObject(&teapot);


        
    glm::mat4 startProjection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    
    while (!glfwWindowShouldClose(window))
    {
        //calculate fps
        double currentTime = glfwGetTime();
        frameCount++;
        if (currentTime - prevTime >= 1.0) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            prevTime = currentTime;
        }

        processInput(window);

        glStencilMask(0xFF);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //teapot.setRotation(glm::vec3(0.0f, teapot.getRotation().y + sin(deltaTime) * 80, 0.0f));
        //teapot.setPosition(glm::vec3( teapotStartPos.x  + sin(glfwGetTime()) * 1.0f, teapotStartPos.y, teapotStartPos.z));
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
    const float camSpeedNormal = 1.4f * deltaTime;
    const float camSpeedFast = 5.0f * deltaTime;
    float camSpeed = camSpeedNormal;
    glm::vec3 cameraPos = mainCamera.getPosition();
    glm::vec3 cameraFront = mainCamera.getFront();
    glm::vec3 cameraUp = mainCamera.getUp();
    //tab is used for 'sprint' instead of shift because glfw does not allow for the shift key to be easily polled.
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        camSpeed = camSpeedFast;
    }
    else { camSpeed = camSpeedNormal; }


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

//taken from learnopengl.com - for mouse input moving the camera
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
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; 
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    mainCamera.updatePitchAndYaw(yoffset, xoffset);
}

void render(Camera camera, Shader shader, Shader portalShader, Scene scene, int recursionLevel, glm::mat4 currentProjectionMatrix)
{   
    //STEPS ONE AND TWO: render all non-portal objects
    glm::mat4 view = camera.getViewMatrix();

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_EQUAL, recursionLevel, 0xFF);
    glStencilMask(0x00);
    
    glm::mat4 model = glm::mat4(1.0f);
    
    //maybe delete not sure if model is used anymore
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", currentProjectionMatrix);


    portalShader.use();
    portalShader.setMat4("view", view);
    portalShader.setMat4("projection", currentProjectionMatrix);


    scene.draw(shader, camera);

    glfwPollEvents();
    //STEP THREE: Return if maximum recursion depth has been reached.
    if (recursionLevel >= maxRecursionLevel)
    {
        return;
    }


    //STEP FOUR: Loop through all portals
    for (Portal* portal : scene.getPortals()) //
    {
        //STEP FIVE: Write to the stencil buffer.
        //write to the stencil buffer with recursionLevel
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
        glStencilFunc(GL_EQUAL, recursionLevel, 0xFF); 
        glStencilMask(0xFF);
        
        //disable colour + depth buffer
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);


        //draw portal so stencil buffer is set
        portalShader.use();
        portalShader.setMat4("view", view);
        portalShader.setMat4("projection", currentProjectionMatrix);
        portal->draw(portalShader);

        //turn the colour + depth buffers back on
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        //STEP SIX: Write maximum depth
        //make it so the stencil buffer will only pass fragments when equal to 1
        //and stop writing to the stencil buffer
        glStencilFunc(GL_EQUAL, recursionLevel+1, 0xFF);
        glStencilMask(0x00);


        //we cannot clear the depth buffer, so instead set the depth buffer to max distance where the stencil buffer allows
        //turn off colour mask, make sure that depth will always be written
        glDepthMask(GL_TRUE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthFunc(GL_ALWAYS);
        glDepthRange(1.0, 1.0);
        
        //draw portal AGAIN
        portalShader.use();
        portalShader.setMat4("view", view);
        portalShader.setMat4("projection", currentProjectionMatrix);
        portal->draw(portalShader);

        //reset stuff back
        glDepthFunc(GL_LESS);
        glDepthRange(0.0, 1.0);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);



        //STEP SEVEN: Calculate the exit portal camera transform and set it
        //calculate the transformation of the camera to be used for the portal view
        glm::mat4 portalMatrix = portal->getModelMatrix();
        glm::mat4 linkedPortalMatrix = portal->getLinkedPortal()->getModelMatrix();
        glm::mat4 newMat = linkedPortalMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::inverse(portalMatrix);
        glm::vec3 newCamPos = glm::vec3(newMat * glm::vec4(camera.getPosition(), 1.0f));
        glm::vec3 newCamFront = glm::normalize(glm::vec3(newMat * glm::vec4(camera.getFront(), 0.0f)));
        //convert the newCamFront into euler angles so they can be set in the camera class
        float pitch = glm::degrees(asin(newCamFront.y));
        float yaw = glm::degrees(atan2(newCamFront.z, newCamFront.x));
     
        //create this camera and set the transform variables accordingly
        Camera newCam(newCamPos);
        newCam.setPitchAndYaw(pitch, yaw);


        //STEP EIGHT: Oblique frustum culling
        // get cameras view matrix
        glm::mat4 newView = newCam.getViewMatrix();

        //get the linked portals normal + position (in world space)
        glm::vec3 linkedNormal = -glm::normalize(glm::vec3(linkedPortalMatrix[2]));
        glm::vec3 linkedPos = glm::vec3(linkedPortalMatrix[3]);

        //transform this into the cameras view space
        glm::vec3 normalView = glm::normalize(glm::vec3(newView * glm::vec4(linkedNormal, 0.0f)));
        glm::vec3 posView = glm::vec3(newView * glm::vec4(linkedPos, 1.0f));

        //get distance from origin in view space
        //move it back a bit too (makes portal less glitchy with z-fighting effect)
        float distance = -glm::dot(normalView, posView);
        distance += 0.01f; 
 

        //combine this into a plane
        glm::vec4 viewSpaceClipPlane(normalView.x, normalView.y, normalView.z, distance);

        //modify projection matrix for oblique frustrum culling
        glm::mat4 newProjection = ModifyProjectionMatrix(viewSpaceClipPlane, currentProjectionMatrix);

        //STEP NINE: Recursively call the render function
        render(newCam, shader, portalShader, scene, recursionLevel + 1, newProjection);

        //STEP 10: Cleanup, decrement the stencil buffer value 
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
        glStencilFunc(GL_EQUAL, recursionLevel + 1, 0xFF);

        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        portalShader.use();
        portalShader.setMat4("view", view);
        portalShader.setMat4("projection", currentProjectionMatrix);
        portal->draw(portalShader);

        // Fully reset for the next loop iteration
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
        glStencilMask(0x00);

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
    return (1.0F);
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
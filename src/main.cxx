#include <stdio.h>
#include <stdlib.h>
#include <../include/glad/glad.h>
#include "../include/glfw3.h"
#include "../include/myShader.h"
#include <block.h>
#include <cube.h>
#define HEIGHT 400
#define WIDTH 800
float lastX;
float lastY;
bool firstMouse;
bool isMoveMode = true;
bool r = false;
float pitch = 0.0f;
float yaw = 0.0f;
cube c;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float deltaTime = 0.0f;	// 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        isMoveMode = !isMoveMode;
    if(isMoveMode){


        float cameraSpeed = 5.0f*deltaTime; // adjust accordingly
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }else{
        if(glfwGetKey(window,GLFW_KEY_V) == GLFW_PRESS)
            r = !r;
        if(glfwGetKey(window,GLFW_KEY_R) == GLFW_PRESS)
            c.set_rotate(R,r);
        if(glfwGetKey(window,GLFW_KEY_M) == GLFW_PRESS)
            c.set_rotate(M,r);
        if(glfwGetKey(window,GLFW_KEY_L) == GLFW_PRESS)
            c.set_rotate(L,r);
        if(glfwGetKey(window,GLFW_KEY_U) == GLFW_PRESS)
            c.set_rotate(U,r);
        if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS)
            c.set_rotate(E,r);
        if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
            c.set_rotate(D,r);
        if(glfwGetKey(window,GLFW_KEY_F) == GLFW_PRESS)
            c.set_rotate(F,r);
        if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
            c.set_rotate(S,r);
        if(glfwGetKey(window,GLFW_KEY_B) == GLFW_PRESS)
            c.set_rotate(B,r);
    }
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse)
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

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "cube", NULL, NULL);
    if(!window){
        printf("load window faild\n");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("glad load faild\n");
        return -1;
    }

    Shader shader = Shader("./Shaders/vert.glsl","./Shaders/frag.glsl");
    glm::vec3 o = {-1.0f,-1.0f,0.0f}; 
    block b = block(o,&shader);
    c = cube(&shader);

    glm::mat4 view;
    glm::mat4 projection;


    projection = glm::perspective(glm::radians(45.0f),(float) WIDTH / HEIGHT, 0.1f, 100.0f);


    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // shader.setMat4("transform",view);
        shader.setMat4("view",view);

        shader.setMat4("projection",projection);
        c.action(deltaTime);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}


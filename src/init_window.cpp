#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "my/init_window.hpp"
#include "my/globals.h"

GLFWwindow* win;
GLuint program;
GLuint cameraLoc;
GLuint projLoc;
glm::mat4 camera = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.8f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
void createProgram();

void framebuffer_size_callback(GLFWwindow* window __attribute__((unused)),int w,int h){
    glViewport(0,0,w,h);
}

void mouse_callback(GLFWwindow* window __attribute__((unused)),double xpos,double ypos){
    static float lastX = 400.0f;
    static float lastY = 300.0f;
    static bool firstMouse = true;

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

    const float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    static float yaw   = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
    static float pitch =  0.0f;

    yaw   += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
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

void window(int width,int height){
    if(!glfwInit()){
        std::cout<<"glfw not initialized\n";
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);  
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    win = glfwCreateWindow(width,height,"IDNIA",NULL,NULL);
    glfwMakeContextCurrent(win);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"glad not initialized\n";
        glfwTerminate();
    }
    glViewport(0,0,width,height);
    glfwSetFramebufferSizeCallback(win,framebuffer_size_callback);
    glfwSetCursorPosCallback(win, mouse_callback);
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    createProgram();
}

std::string loadShaderFromFile(const std::string& path){
    
    std::string result;
    std::string line;
    std::ifstream myFile(path.c_str());
    if (myFile.is_open()) {
        while (std::getline(myFile, line)) {
            result += line + "\n";
        }
        myFile.close();
    } else {
        std::cerr << "Could not read file "<< path <<". File does not exist.\n";
    }
    return result;
}

GLuint compileShader(std::string type,const char* source){
  
    GLuint shader;
    int success;
    char infoLog[512];

    if (type=="vertex"){
        shader = glCreateShader(GL_VERTEX_SHADER);
    }
    else {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    glShaderSource(shader,1,&source,NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
};

void createProgram(){
    program = glCreateProgram();

    GLuint vertexShader = compileShader("vertex",loadShaderFromFile("shader/vertex.glsl").c_str());
    GLuint fragmentShader = compileShader("fragment",loadShaderFromFile("shader/fragment.glsl").c_str());
    
    glAttachShader(program,vertexShader);
    glAttachShader(program,fragmentShader);
    glLinkProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    camera = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    gIsTexLoc = glGetUniformLocation(program,"isTextured");
    gColorLoc = glGetUniformLocation(program,"color");
    gModelLoc = glGetUniformLocation(program, "model");
    cameraLoc  = glGetUniformLocation(program, "view");
    projLoc  = glGetUniformLocation(program, "projection");

}

void windowInput(){
    float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    const float cameraSpeed = 0.00002f*deltaTime; // adjust accordingly
    
    if (glfwGetKey(win,GLFW_KEY_ESCAPE)==GLFW_PRESS){
        glfwSetWindowShouldClose(win,1);
    }
    if (glfwGetKey(win,GLFW_KEY_Q)==GLFW_PRESS){
         glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (glfwGetKey(win,GLFW_KEY_W)==GLFW_PRESS){
        cameraPos += cameraSpeed * glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    }
    if (glfwGetKey(win,GLFW_KEY_S)==GLFW_PRESS){
        cameraPos -= cameraSpeed * glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    }
    if (glfwGetKey(win,GLFW_KEY_A)==GLFW_PRESS){
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(win,GLFW_KEY_D)==GLFW_PRESS){
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(win,GLFW_KEY_SPACE)==GLFW_PRESS){
        // cameraPos += cameraSpeed * cameraUp;
    }
    if (glfwGetKey(win,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS){
        // cameraPos -= cameraSpeed * cameraUp;
    }
}

bool isWindowClose(){
    return glfwWindowShouldClose(win);
}

void bgcolor(float r, float g, float b){
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glUseProgram(program);

    glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(camera));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void swapbuffer(){
    glfwSwapBuffers(win);
    glfwPollEvents();
}

void cleanWindow(){
    glDeleteProgram(program);
    glfwDestroyWindow(win);
    glfwTerminate();
    std::cout<<"cleaned"<<std::endl;
}
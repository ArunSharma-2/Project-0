#include <glad/glad.h>
#include <iostream>
#include <string>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "my/stb_image.h"
#include "my/graphic.hpp"
#include "my/globals.h"

Graphic::Graphic(float width, float height)
{
    float vertices2D[32] = {
            // positions         //texture Coord
        0.0f,  0.0f,   0.0f,       0.0f, 0.0f,
        width, 0.0f,   0.0f,       1.0f, 0.0f,
        width, height, 0.0f,       1.0f, 1.0f,
        0.0f,  height, 0.0f,       0.0f, 1.0f
    };

    unsigned int indices2D[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenTextures(1,&texture);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2D), vertices2D, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2D), indices2D, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

}

Graphic::Graphic(float width, float height, float length)
{
    float vertices3D[40] = {
            // position            //texture Coord
        0.0f,   0.0f,   0.0f,        0.0f, 0.0f,
        width,  0.0f,   0.0f,        1.0f, 0.0f,
        width,  height, 0.0f,        1.0f, 1.0f,
        0.0f,   height, 0.0f,        0.0f, 1.0f,
        0.0f,   0.0f,   length,      0.0f, 0.0f,
        width,  0.0f,   length,      1.0f, 0.0f,
        width,  height, length,      1.0f, 1.0f,
        0.0f,   height, length,      0.0f, 1.0f
    };
    
    unsigned int indices3D[36] = {
        0, 1, 2, 0, 2, 3, // front face
        4, 5, 6, 4, 6, 7, // back face
        0, 1, 5, 0, 5, 4, // bottom face
        2, 3, 7, 2, 7, 6, // top face
        0, 3, 7, 0, 7, 4, // left face
        1, 2, 6, 1, 6, 5  // right face
    };

    glGenTextures(1,&texture);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3D), vertices3D, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3D), indices3D, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
}
void Graphic::setTexture(std::string path){
    isTextured = true;
    glBindTexture(GL_TEXTURE_2D,texture);
    // texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // image data into data pointer.
    int w,h,c;
    unsigned char* data = stbi_load(path.c_str(),&w,&h,&c,0);
    // data pointer into generated bound texture 
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout<<"success to load texture\n";
    }
    else{
        std::cout<<"failed to load texture\n";
    }
    glBindTexture(GL_TEXTURE_2D,0);
    //freeing image from cpu 
    stbi_image_free(data);
}
void Graphic::setColor(float r, float g, float b){
    this->color = glm::vec3(r,g,b);
}

void Graphic::translate(float xoffset,float yoffset ,float zoffset){
    model = glm::translate(model,glm::vec3(xoffset,yoffset,zoffset));
}
void Graphic::rotate(float angle, char axis){
    switch (axis)
    {
    case 'x':
        model = glm::rotate(model,glm::radians(angle),glm::vec3(1.0f,0.0f,0.0f));
        break;
    case 'y':
        model = glm::rotate(model,glm::radians(angle),glm::vec3(0.0f,1.0f,0.0f));
        break;
    case 'z':
        model = glm::rotate(model,glm::radians(angle),glm::vec3(0.0f,0.0f,1.0f));
        break;
    }
}
void Graphic::scale(float x, float y, float z){
    model = glm::scale(model,glm::vec3(x,y,z));
}
void Graphic::Draw2D(){

    glUniform3fv(gColorLoc,1,glm::value_ptr(color));
    glUniformMatrix4fv(gModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(gIsTexLoc,isTextured);

    glBindTexture(GL_TEXTURE_2D,texture);
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);
}
void Graphic::Draw3D(){

    glUniform3fv(gColorLoc,1,glm::value_ptr(color));
    glUniformMatrix4fv(gModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(gIsTexLoc,isTextured);

    glBindTexture(GL_TEXTURE_2D,texture);
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);
}

Graphic::~Graphic(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1,&texture);
}
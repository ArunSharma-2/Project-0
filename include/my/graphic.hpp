#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP
#include <glm/mat4x4.hpp>
#include <string>
class Graphic
{
private:
    glm::vec3 color;
    glm::mat4 model = glm::mat4(1.0f);
    unsigned int VBO, VAO, EBO;
    unsigned int texture;
    bool isTextured = false;
    
public:
    Graphic(float width, float height);
    Graphic(float width, float height, float length);
    ~Graphic();
    void translate(float xoffset, float yoffset ,float zoffset);
    void rotate(float angle, char axis);
    void scale(float x, float y, float z);
    void setTexture(std::string path);
    void setColor(float r, float g, float b);
    void Draw2D();
    void Draw3D();
};
#endif
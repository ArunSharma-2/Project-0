// g++ -std=c++17 -Wall -Wextra -o app.exe src/main.cpp src/init_window.cpp src/glad.c -Iinclude -Llib -lglfw3 -lopengl32 -lgdi32

#include "my/init_window.hpp"
#include "my/graphic.hpp"
#include <iostream>

int main(){
    window(800,600);

    Graphic rect(1.0f,1.0f);
    rect.setTexture("assest/texture/platform.png");
    rect.setColor(1.0f,1.0f,1.0f);
    rect.translate(-0.5f,-0.5f,-0.5f);
    rect.rotate(90.0f,'x');

    Graphic cube(0.5f,0.5f,0.5f);
    // cube.setTexture("assest/texture/text.png");
    cube.setColor(1.0f,0.0f,0.0f);

    while(!isWindowClose()){
        windowInput();
        bgcolor(0.1f, 0.1f, 0.1f);
        //----------------------------render block--------------------------------------------
        
        cube.rotate(0.005f,'y');
        cube.Draw3D();

        //----------------------------render platform-----------------------------------------
        for (int i = 0;i<5;i++){
            rect.Draw2D();
            rect.translate(1.0f,0.0f,0.0f);
        }
        rect.translate(-5.0f,0.0f,0.0f);
        swapbuffer();
    }
    rect.~Graphic();
    cube.~Graphic();
    cleanWindow();
    return 0;
}
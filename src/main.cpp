#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

float distance = 0;
float backgroundTime = 0;
float prevBackgroundTime = 0;

#include "./../shaders/shaders.hpp"
#include "./../components/background/background1.cpp"
#include "./../components/character/character1.cpp"
#include "./../components/zapper/zapper.cpp"
#include "./../files/settings.cpp"
#include "./../files/collisions.cpp"


int collide = 0;


#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif 


int main()
{
    GLFWwindow* window = GLFWsettings();
    if(window == (GLFWwindow*)-1){
        return -1;
    }

    initializeBackground();
    initializeCharacter();
    initializeZapper();


    float startTime = glfwGetTime();
    backgroundTime = glfwGetTime();
    float prevZapper = 0;


    while (!glfwWindowShouldClose(window))
    {
        float currTime = glfwGetTime();
        backgroundTime = currTime;
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        renderBackground(currTime - startTime);
        renderCharacter(window, processInput(window), collide);
        renderZapper();

        collide = checkCollision(character_vertices, zapper_vertices);
        if (collide == 1)
        {
            glfwSetWindowShouldClose(window, true);
            std::cout << "Game Over" << std::endl;
        }
                

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    deleteBackground();
    deleteCharacter();
    deleteZapper();
    

    glfwTerminate();
    return 0;
}




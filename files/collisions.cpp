#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include <iostream>
#include <vector>

int zapperCollision(float* character_vertices, std::vector<float>& zapper_vertices,int i);

int collision = 0;
int collision_bit = 0;

int checkCollision(float* character_vertices, std::vector<float>& zapper_vertices){
    for (int i = 0 ; i <= (zapper_vertices.size()/36) ; i++)
        if(zapper_vertices[0 + i*36] >= -0.88f && zapper_vertices[18 + i*36] <= -0.77f)
        {
            if(collision_bit == 0){
                if(zapper_vertices[8 + i*36] == 1.0f)
                {
                    if(zapperCollision(character_vertices, zapper_vertices,i))
                    {
                        collision++;
                        collision_bit = 1;
                        std::cout << collision_bit << collision << std::endl;
                        return 1;
                    }
                }
            }
        }
        else
        {
            collision_bit = 0;
        }
    return 0;
}

int zapperCollision(float* character_vertices, std::vector<float>& zapper_vertices,int i){

    if((character_vertices[1] <= zapper_vertices[28+i*36] && character_vertices[1] >= zapper_vertices[19+i*36]
    || character_vertices[3] <= zapper_vertices[28+i*36] && character_vertices[3] >= zapper_vertices[19+i*36])){
        return 1;
    }

    return 0;

}
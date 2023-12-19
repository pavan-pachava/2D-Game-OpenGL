#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>

#include "./../../shaders/shaders.hpp"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

unsigned int zapper_shader;
unsigned int zapper_texture1;
unsigned int zapper_texture2;
unsigned int zapper_VAO;
std::vector<float> zapper_vertices;
std::vector<unsigned int> zapper_indices;
short count = 0;
float offset_zappers = 0.0f;
float prev_time_zappers = 0.0f;

float type_vertices[2][36] = {
    {
        0.04f, 0.33f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.04f, -0.33f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.04f, -0.33f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.04f, 0.33f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
    },
    {
        0.18f, 0.07f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.18f, -0.07f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.18f, -0.07f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.18f, 0.07f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    }
};

void initializeZapper(){

    createShader("./../shaders/vertex.shader.zapper",
    "./../shaders/fragment.shader.zapper",
    &zapper_shader);

    glGenVertexArrays(1, &zapper_VAO);

    glGenTextures(1, &zapper_texture1);
    glBindTexture(GL_TEXTURE_2D, zapper_texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./../components/zapper/zapper.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    // glGenTextures(1, &zapper_texture2);
    // glBindTexture(GL_TEXTURE_2D, zapper_texture2);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // data = stbi_load("./../src/components/zapper/zapper2.png", &width, &height, &nrChannels, 0);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);

    // stbi_image_free(data);

    srand(time(0));

}

void renderZapper(){

    short zapper_rand = rand() % 9;
    short zapper_type;

    if(zapper_rand == 0 || zapper_rand == 1 || zapper_rand == 2 || zapper_rand == 3){
        zapper_type = 0;
    }
    else if(zapper_rand == 4 || zapper_rand == 5){
        zapper_type = 1;
    }
    else if(zapper_rand == 6 || zapper_rand == 7){
        zapper_type = 0;
    }
    else if(zapper_rand == 8){
        zapper_type = 1;
    }

    short zapper_pos;
    
    zapper_pos = rand() % 7;
    if(zapper_pos == 0){
        zapper_pos = 1;
    }else if(zapper_pos < 4){
        zapper_pos = 0;
    }else{
        zapper_pos = -1;
    }
    

    unsigned int type_indices[1][6] = {
        {0, 1, 3, 1, 2, 3}
    };

    if(count == 0){
        if(zapper_vertices.size() > 288){
            for(int i = 0 ; i < 36 ; i++){
                zapper_vertices.erase(zapper_vertices.begin());
            }
        }
        for(int i = 0; i < 36; i++){
            zapper_vertices.push_back(type_vertices[zapper_type][i] + (i % 9 == 0 ? 1.2f : 0.0f) + (i % 9 == 1 ? zapper_pos * 0.5f : 0.0f));
        }

        if(zapper_indices.size() < 96){
            for(int i = 0; i < 6; i++){
                zapper_indices.push_back(type_indices[0][i] + zapper_indices.size() / 6 * 4);
            }
        }

        count += 200;
    }

    float curr_time_zappers = glfwGetTime() * 0.5f;
    if(curr_time_zappers < prev_time_zappers){
        prev_time_zappers = 0.003f;
    }

    for(int i = 0 ; i < zapper_vertices.size(); i+=9){
        zapper_vertices[i] -= curr_time_zappers - prev_time_zappers;
    }

    prev_time_zappers = curr_time_zappers;

    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(zapper_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, zapper_vertices.size() * sizeof(float), &zapper_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, zapper_indices.size() * sizeof(unsigned int), &zapper_indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, zapper_texture1);

    glUseProgram(zapper_shader);

    glUniform1i(glGetUniformLocation(zapper_shader, "texture1"), 0);
    // glUniform1i(glGetUniformLocation(zapper_shader, "texture2"), 1);

    glm::mat4 translate = glm::mat4(1.0f);
    unsigned int translateLoc = glGetUniformLocation(zapper_shader, "translate");
    glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(translate));

    glBindVertexArray(zapper_VAO);
    glDrawElements(GL_TRIANGLES, zapper_indices.size(), GL_UNSIGNED_INT, 0);

    count--;

}

void deleteZapper(){

    glDeleteVertexArrays(1, &zapper_VAO);
    glDeleteTextures(1, &zapper_texture1);
    glDeleteProgram(zapper_shader);

}
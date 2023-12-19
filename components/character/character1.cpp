#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "./../../shaders/shaders.hpp"

#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif 

unsigned int char_shader;
unsigned int char_texture;
unsigned int char_VAO;

float elevation = 0.0f;
float velocity = 0.0f;
float acceleration = 0.0026f;
float gravity = -0.0008f;
float max_elevation = 1.4f;

float character_vertices[8] = {
    -0.78f, -0.6f,
    -0.78f, -0.88f,
    -0.88f, -0.88f,
    -0.88f, -0.6f
};


void initializeCharacter()
{
    createShader("./../shaders/vertex.shader", "./../shaders/fragment.shader", &char_shader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float vertices[] = {
    -0.78f,  -0.6f, 0.0f,   1.0f, 1.0f,  // top right
    -0.78f, -0.88f, 0.0f,   1.0f, 0.0f,  // bottom right
    -0.88f, -0.88f, 0.0f,   0.0f, 0.0f,  // bottom left
    -0.88f,  -0.6f, 0.0f,   0.0f, 1.0f,   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    unsigned int VBO, EBO;
    glGenVertexArrays(1, &char_VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(char_VAO);

    // init the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // init the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // declare attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);  

    glGenTextures(1, &char_texture);
    glBindTexture(GL_TEXTURE_2D, char_texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

void renderCharacter(GLFWwindow *window, int key_press, int collide)
{
    if (elevation == 0.0f) {
        if (key_press == 1) {
            velocity += (gravity + acceleration);
            elevation += velocity;
        }
    }
    else if (elevation == max_elevation){
        if (key_press == 0) {
            velocity += gravity;
            elevation += velocity;
        }
    }
    else {
        if (key_press == 1) {
            velocity += (gravity + acceleration);
            elevation += velocity;
            if (elevation > max_elevation) {
                elevation = max_elevation;
                velocity = 0.0f;
            }
            else if (elevation < 0.0f) {
                elevation = 0.0f;
                velocity = 0.0f;
            }
        }
        else if (key_press == 0) {
            velocity += gravity;
            elevation += velocity;
            if (elevation < 0.0f) {
                elevation = 0.0f;
                velocity = 0.0f;
            }
            else if (elevation > max_elevation) {
                elevation = max_elevation;
                velocity = 0.0f;
            }
        }
    }

    // draw our first triangle
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,char_texture);
    glUseProgram(char_shader);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data;

    if (elevation > 0.0f && collide != 1) {
        data = stbi_load("./../components/character/characterfly.png", &width, &height, &nrChannels, 0);
    }
    else if (elevation == 0.0f && collide != 1) {
        data = stbi_load("./../components/character/characterstep.png", &width, &height, &nrChannels, 0);
    }
    else if (collide == 1) {
        data = stbi_load("./../components/character/charactershock.png", &width, &height, &nrChannels, 0);
    }

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    
    glUniform1i(glGetUniformLocation(char_shader, "texture1"), 0);
    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, glm::vec3(0.0f, elevation, 0.0f));

    character_vertices[1] = -0.6f + elevation;
    character_vertices[3] = -0.88f + elevation;
    character_vertices[5] = -0.88f + elevation;
    character_vertices[7] = -0.6f + elevation;

    // translate = glm::translate(translate, glm::vec3(offset, 0.0f, 0.0f));


    glUniform4fv(glGetUniformLocation(char_shader, "translate"), 1, glm::value_ptr(translate));
    unsigned int translateLoc = glGetUniformLocation(char_shader, "translate");
    glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(translate));
    
    unsigned int glowLoc = glGetUniformLocation(char_shader, "jump");
    int jump;
    if(key_press == 0){
        jump = 0;
        glUniform1i(glowLoc, jump);
    }else{
        jump = 1;
        glUniform1i(glowLoc, jump);
    }

    glBindVertexArray(char_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void deleteCharacter()
{
    glDeleteVertexArrays(1, &char_VAO);
    glDeleteProgram(char_shader);
    glDeleteTextures(1, &char_texture);
}


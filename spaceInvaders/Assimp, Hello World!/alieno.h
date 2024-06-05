#ifndef ALIENO_H
#define ALIENO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_m.h"
#include "camera.h"
#include <iostream>
#include <random>
#include <cmath>
#include <stack>
#include "model.h"


class Alieno {
private:
    //Proprietà alieni
    int map[5][6] = { {1,1,1,1,1,1},
                      {1,1,1,1,1,1},
                      {1,1,1,1,1,1},
                      {1,1,1,1,1,1},
                      {1,1,1,1,1,1} };

    glm::vec3 pos = glm::vec3(-4.77f, 0.0, -12.0f);
    float raggio = 1.0f;
    float spazio = 1.2f;
    Shader shader;
    Model model;

public:
    // Costruttore
    Alieno(){}

    int(*getmap())[6] {
        return map;
    }

    glm::vec3 getPos() const {
        return pos;
    }

    Shader getShader() const {
        return shader;
    }

    float getRaggio() const {
        return raggio;
    }

    float getSpazio() const {
        return spazio;
    }

    void setShader(Shader newShader) {
        shader = newShader;
    }

    void setModel(Model newModel) {
        model = newModel;
    }

    void renderAlieni() {

        shader.use();


        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (map[i][j] != 0)
                {

                    float x = pos.x + i * raggio * 2.0f * spazio;
                    float z = pos.z + j * raggio * 2.0f * spazio;

                    glm::mat4 modelAlieno = glm::mat4(1.0f);
                    modelAlieno = glm::translate(modelAlieno, glm::vec3(x, 0.0f, z));
                    modelAlieno = glm::scale(modelAlieno, glm::vec3(1.0f, raggio, 1.0f));
                    shader.setMat4("model", modelAlieno);
                    model.Draw(shader);
                }
            }
        }

    }
};

#endif 

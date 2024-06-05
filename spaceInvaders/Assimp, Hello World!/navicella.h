#ifndef NAVICELLA_H
#define NAVICELLA_H

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


class Navicella {
private:
    //Proprietà navicella
    glm::vec3 pos = glm::vec3(0.0f, 0.0, 6.0f);
    float raggio = 1.0f;
    float translateSpeed;
    float speed = 6;  // velocita della navicella
    float limX_pos;
    Shader shader;
    Model model;

public:
    // Costruttore
    Navicella() {}

    glm::vec3 getPos() const {
        return pos;
    }

    float getSpeed() const {
        return speed;
    }

    void setTranslateSpeed(float newTranslateSpeed) {
        translateSpeed = newTranslateSpeed;
    }

    void setLimXpos(float limite) {
        limX_pos = limite;
    }

    void setShader(Shader newShader) {
        shader = newShader;
    }

    void setModel(Model newModel) {
        model = newModel;
    }

    void renderNavicella(bool moveRight, bool moveLeft) {

        shader.use();

        glm::mat4 modelNavicella = glm::mat4(1.0f);
        modelNavicella = glm::translate(modelNavicella, glm::vec3(pos.x, 0.0f, pos.z));
        modelNavicella = glm::scale(modelNavicella, glm::vec3(1.0f, raggio, 1.0f));
        shader.setMat4("model", modelNavicella);
        model.Draw(shader);

        // Spostamento navicella laterale destro
        if (moveRight && pos.x < limX_pos)
        {
            pos = glm::vec3(pos.x + translateSpeed, pos.y, pos.z);

        }
        // Spostamento navicella laterale sinistro
        if (moveLeft && pos.x > -limX_pos)
        {
            pos = glm::vec3(pos.x - translateSpeed, pos.y, pos.z);
        }

    }


};

#endif 

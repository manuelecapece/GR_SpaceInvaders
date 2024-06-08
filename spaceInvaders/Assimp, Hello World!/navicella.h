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
#include "proiettile.h"


class Navicella {
private:
    //Proprietà navicella
    glm::vec3 pos = glm::vec3(0.0f, 0.0, 8.2f);
    float raggio = 1.0f;
    float translateSpeed;
    float speed = 6;  // velocita della navicella
    float limX_pos = 999;
    bool isHitted = false;
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

    bool getIsHitted() const {
        return isHitted;
    }

    void setPos(glm::vec3 newPos) {
        pos = newPos;
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

    void render(bool moveRight, bool moveLeft) {

        if (!isHitted) {
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

    }

    bool isPointInsideCircle(const glm::vec2& point, const glm::vec2& center) {
        // Calcola la distanza al quadrato tra il punto e il centro della circonferenza
        float distSq = (point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y);

        // Calcola il raggio al quadrato
        float radiusSq = raggio * raggio;

        // Controlla se la distanza al quadrato è minore o uguale al raggio al quadrato
        return distSq <= radiusSq;
    }

    void checkIsHitted(Proiettile& proiettile){
        for (int i = 0; i < proiettile.getColpiSparati() + 1; i++)
        {
            float proiettile_x = proiettile.getVecPos()[i].x;
            float proiettile_z = proiettile.getVecPos()[i].z;
            glm::vec2 punto = glm::vec2(proiettile_x, proiettile_z - (proiettile.getLunghezza() / 2));
            glm::vec2 centro = glm::vec2(pos.x, pos.z);
            if (isPointInsideCircle(punto, centro)) {
                proiettile.setVecPos(i, glm::vec3(proiettile_x, 0.0f, -20.0f));
                isHitted = true;
                return;

            }
        }
    }

    void inizializzaProiettile(Proiettile& proiettile) {
        proiettile.incrementaColpi();
        proiettile.inizializzaPos(pos);
        glm::vec3 proiettileAt = glm::vec3(0.0f, 0.0f, -1.0f);
        proiettile.inizializzaDir(proiettileAt);
    }


};

#endif 

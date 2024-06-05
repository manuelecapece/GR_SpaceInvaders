#ifndef PROIETTILE_H
#define PROIETTILE_H

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

const int SIZE_VECTOR_COLPI = 1000;

class Proiettile {
private:
    //Proprietà proiettile
    glm::vec3 pos = glm::vec3(0.0f, 0.0, 6.0f);
    glm::vec3 at = glm::vec3(0.0, 0.0, 0.0); // Punto in cui e diretto il proiettile
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0); // Vettore up...il proiettile è sempre parallelo al piano
    const float lunghezza = 0.6f;
    const float larghezza = 0.3f;
    const float altezza = 0.3f;
    float translateSpeed;
    float speed = 6;  // velocita della navicella
    float limZ = -20;
    float spread = 1.0f;
    std::vector<glm::vec3> vectorPos = std::vector<glm::vec3>(SIZE_VECTOR_COLPI);
    std::vector<glm::vec3> vectorDir = std::vector<glm::vec3>(SIZE_VECTOR_COLPI);
    int colpiSparati = -1;
    Shader shader;
    Model model;

public:
    // Costruttore
    Proiettile() {}

    glm::vec3 getPos() const {
        return pos;
    }

    float getSpread() {
        return spread;
    }

    float getSpeed() const {
        return speed;
    }

    int getColpiSparati() const {
        return colpiSparati;
    }

    void setTranslateSpeed(float newTranslateSpeed) {
        translateSpeed = newTranslateSpeed;
    }

    void setShader(Shader newShader) {
        shader = newShader;
    }

    void setModel(Model newModel) {
        model = newModel;
    }

    void inizializzaPos(glm::vec3 newPos) {
        vectorPos[colpiSparati] = newPos;
    }

    void inizializzaDir(glm::vec3 newDir) {
        vectorDir[colpiSparati] = newDir;
    }

    void incrementaColpi() {
        colpiSparati++;
    }

    void renderProiettile() {

        //glBindVertexArray(cubeVAO);
        shader.use();

        for (int i = 0; i < colpiSparati + 1; i++)
        {
            if (vectorPos[i].z > limZ)
            {
                glm::mat4 modelCubo = glm::mat4(1.0f);	//identity matrix
                vectorPos[i] = vectorPos[i] + translateSpeed * vectorDir[i];
                modelCubo = glm::translate(modelCubo, glm::vec3(vectorPos[i].x, vectorPos[i].y, vectorPos[i].z));
                modelCubo = glm::scale(modelCubo, glm::vec3(larghezza, altezza, lunghezza));
                shader.setMat4("model", modelCubo);
                shader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
                //glDrawArrays(GL_TRIANGLES, 0, 36);
                model.Draw(shader);
            }
        }



    }




};

#endif 
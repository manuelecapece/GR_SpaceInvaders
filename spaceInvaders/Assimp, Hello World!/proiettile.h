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

    const float lunghezza = 0.5f;
    const float larghezza = 0.15f;
    const float altezza = 0.15f;
    float translateSpeed;
    float speed = 15;  
    float limZNeg = -20;
    float limZPos = 10;
    float spread = 1.0f;
    std::vector<glm::vec3> vectorPos   = std::vector<glm::vec3>(SIZE_VECTOR_COLPI);
    std::vector<glm::vec3> vectorDir   = std::vector<glm::vec3>(SIZE_VECTOR_COLPI);
    int colpiSparati = -1;

    Shader shader;
    Model model;

public:
    // Costruttore
    Proiettile() {}

    //glm::vec3 getPos() const {
    //    return pos;
    //}

    float getSpread() {
        return spread;
    }

    float getSpeed() const {
        return speed;
    }

    int getColpiSparati() const {
        return colpiSparati;
    }

    float getLunghezza() const {
        return lunghezza;
    }

    float getLarghezza() const {
        return larghezza;
    }

    float getLimZNeg() const {
        return limZNeg;
    }

    float getLimZPos() const {
        return limZPos;
    }

    std::vector<glm::vec3> getVecPos() const {
        return vectorPos;
    }

    std::vector<glm::vec3> getVecDir() const {
        return vectorDir;
    }


    void setSpeed(float newSpeed) {
        speed = newSpeed;
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

    void setVecPos(int i, glm::vec3 newPos) {
        vectorPos[i] = newPos;
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

    void render(glm::vec3 color) {

        //glBindVertexArray(cubeVAO);
        shader.use();
        float angolo = 0;

        for (int i = 0; i < colpiSparati + 1; i++)
        {
            //if (vectorPos[i].z > limZNeg && vectorPos[i].z < limZPos)
            //{
                glm::mat4 modelCubo = glm::mat4(1.0f);	//identity matrix
                vectorPos[i] = vectorPos[i] + translateSpeed * vectorDir[i];

                if (color.x != 1.0f || color.y != 1.0f || color.z != 1.0f) {
                    angolo = calcolaAngolo(vectorDir[i],glm::vec3(0.0f, 0.0f, 1.0f));
                    if (vectorDir[i].x < 0.0f) {
                        angolo = -angolo;
                    }
                    modelCubo = glm::rotate(modelCubo, angolo, glm::vec3(0.0f, 1.0f, 0.0f));
                }

                modelCubo = glm::translate(modelCubo, glm::vec3(vectorPos[i].x, vectorPos[i].y, vectorPos[i].z));
                modelCubo = glm::scale(modelCubo, glm::vec3(larghezza, altezza, lunghezza));
                shader.setMat4("model", modelCubo);
                shader.setVec3("color", color);
                //glDrawArrays(GL_TRIANGLES, 0, 36);
                model.Draw(shader);
            //}
        }
    }

    float calcolaAngolo(glm::vec3 u, glm::vec3 v) {
        // Calcolo del prodotto scalare
        float dotProduct = glm::dot(u, v);

        // Calcolo delle norme dei vettori
        float norm_u = glm::length(u);
        float norm_v = glm::length(v);

        // Calcolo del coseno dell'angolo
        float cosTheta = dotProduct / (norm_u * norm_v);

        // Calcolo dell'angolo in radianti
        float theta = glm::acos(cosTheta);

        return theta;
    }

    bool isAllProiettiliAlienoOut() {

        for (int i = 0; i < colpiSparati + 1; i++)
        {
            if (vectorPos[i].z < limZPos)
            {
                return false;
            }
        }

        return true;

    }




};

#endif 
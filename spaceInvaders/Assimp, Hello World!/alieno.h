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
#include "proiettile.h"
#include "navicella.h"
#include "barriera.h"

class Alieno {
private:
    
    int static const righeAlieni = 6;
    int static const colonneAlieni = 5;
    int map[righeAlieni][colonneAlieni] = { {1,1,1,1,1},
                                            {1,1,1,1,1},
                                            {1,1,1,1,1},
                                            {1,1,1,1,1},
                                            {1,1,1,1,1},
                                            {1,1,1,1,1} };

    //int map[righeAlieni][colonneAlieni] = { {0,0,0,0,0},
    //                                        {0,0,0,0,0},
    //                                        {0,0,0,0,0},
    //                                        {0,0,0,0,0},
    //                                        {0,0,0,0,0},
    //                                        {0,0,0,0,0} };

    glm::vec3 pos = glm::vec3(-4.77f, 0.0, -12.0f);
    float raggio = 1.0f;
    float spazio = 1.2f;
    bool restart = false;
    float limXalieniPos = 0;
    float limXalieniNeg = -9.54;
    Shader shader;
    Model model;

    std::vector<Proiettile> vectorProiettili = std::vector<Proiettile>(righeAlieni * colonneAlieni);

public:
    // Costruttore
    Alieno(){}

    int(*getmap())[colonneAlieni] {
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

    void render(Proiettile& proiettile) {

        shader.use();

        for (int i = 0; i < righeAlieni; i++)
        {
            for (int j = 0; j < colonneAlieni; j++)
            {
                if (map[i][j] != 0)
                {

                    float x = pos.x + j * raggio * 2.0f * spazio;
                    float z = pos.z + i * raggio * 2.0f * spazio;

                    glm::mat4 modelAlieno = glm::mat4(1.0f);
                    modelAlieno = glm::translate(modelAlieno, glm::vec3(x, 0.0f, z));
                    modelAlieno = glm::scale(modelAlieno, glm::vec3(1.0f, raggio, 1.0f));
                    shader.setMat4("model", modelAlieno);
                    model.Draw(shader);

                    glm::vec3 posAlieno = glm::vec3(x,0.0f,z);

                    if (isHitted(proiettile, posAlieno)) {
                        map[i][j] = 0;
                    }

                }
            }
        }

    }

    void inizializzaProiettili(Shader proiettileShader, Model modelCubo, int i, int j) {

        int k = i * 5 + j;

        if (map[i][j] != 0 )
        {
            float x = pos.x + j * raggio * 2.0f * spazio;
            float z = pos.z + i * raggio * 2.0f * spazio;

            vectorProiettili[k].setSpeed(4);
            vectorProiettili[k].incrementaColpi();
            vectorProiettili[k].inizializzaPos(glm::vec3(x, 0.0f, z));
            glm::vec3 proiettileAt = glm::vec3(0.0f, 0.0f, 1.0f);
            vectorProiettili[k].inizializzaDir(proiettileAt);
            vectorProiettili[k].setShader(proiettileShader);
            vectorProiettili[k].setModel(modelCubo);

        }
        
    }

    void setTranslateSpeedProiettili(double deltaTime) {

        int k = 0;

        for (int i = 0; i < righeAlieni; i++)
        {
            for (int j = 0; j < colonneAlieni; j++)
            {

                if (map[i][j] != 0 || !vectorProiettili[k].isAllProiettiliAlienoOut())
                {

                    vectorProiettili[k].setTranslateSpeed(vectorProiettili[k].getSpeed() * deltaTime);

                }

                k++;
            }
        }

    }

    void renderProiettili(Navicella& navicella, Barriera& barriera) {

        int k = 0;

        for (int i = 0; i < righeAlieni; i++)
        {
            for (int j = 0; j < colonneAlieni; j++)
            {

                if (map[i][j] != 0 || !vectorProiettili[k].isAllProiettiliAlienoOut())
                {
                    if (i == 0) {
                        vectorProiettili[k].render(glm::vec3(1.0f,0.0f,0.0f));
                    }
                    if (i == 1) {
                        vectorProiettili[k].render(glm::vec3(0.0f, 1.0f, 0.0f));
                    }
                    if (i == 2) {
                        vectorProiettili[k].render(glm::vec3(0.0f, 0.0f, 1.0f));
                    }
                    if (i == 3) {
                        vectorProiettili[k].render(glm::vec3(1.0f, 0.0f, 1.0f));
                    }
                    if (i == 4) {
                        vectorProiettili[k].render(glm::vec3(0.0f, 1.0f, 1.0f));
                    }
                    if (i == 5) {
                        vectorProiettili[k].render(glm::vec3(1.0f, 1.0f, 0.0f));
                    }
                    
                }

                navicella.checkIsHitted(vectorProiettili[k]);
                barriera.render(vectorProiettili[k]);
                k++;
            }
        }

    }

    void cambiaPos() {

        if (pos.x < limXalieniPos && !restart) {
            pos = glm::vec3(pos.x + 1, 0.0f, pos.z);
        }else if(pos.x > limXalieniPos && !restart) {
            pos = glm::vec3(pos.x, 0.0f, pos.z + 1);
            restart = true;
        }else if (pos.x > limXalieniNeg && restart) {
            pos = glm::vec3(pos.x - 1, 0.0f, pos.z);
        }else if (pos.x < limXalieniNeg && restart) {
            pos = glm::vec3(pos.x, 0.0f, pos.z + 1);
            restart = false;
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



    bool isHitted(Proiettile& proiettile, glm::vec3 posAlieno) {
        for (int i = 0; i < proiettile.getColpiSparati() + 1; i++)
        {
            float proiettile_x = proiettile.getVecPos()[i].x;
            float proiettile_z = proiettile.getVecPos()[i].z;
            glm::vec2 punto = glm::vec2(proiettile_x, proiettile_z - (proiettile.getLunghezza() / 2));
            glm::vec2 centro = glm::vec2(posAlieno.x, posAlieno.z);
            if(isPointInsideCircle(punto, centro)){
                proiettile.setVecPos(i, glm::vec3(proiettile_x, 0.0f, -21.0f));
                return true;

            }
        }
        return false;
    }
};

 
#endif 
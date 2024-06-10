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

    
    float raggio = 1.0f;
    float spazio = 1.2f;
    glm::vec3 pos = glm::vec3(-(pos.x + 2 * raggio * 2.0f * spazio), 0.0, -17.0f);
    float translateSpeedx;
    float translateSpeedz;
    float speedx = 0.0f;
    float speedz = 0.0f;
    float speedProiettili = 6;
    bool restart = false;
    float limXalieniPos = (((raggio * 2 + spazio) * 3) - raggio) - (pos.x + 5 * raggio * 2.0f * spazio);
    float limXalieniNeg = -((((raggio * 2 + spazio) * 3) - raggio) + (pos.x + 4 * raggio * 2.0f * spazio)) + raggio*2;
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

    float getSpeedx() const {
        return speedx;
    }

    float getSpeedz() const {
        return speedz;
    }

    float getLimXalieniPos() const {
        return limXalieniPos;
    }

    float getLimXalieniNeg() const {
        return limXalieniNeg;
    }

    void setSpeedx(float newSpeedx) {
        speedx = newSpeedx;
    }

    void setSpeedz(float newSpeedz) {
        speedz = newSpeedz;
    }

    void setTranslateSpeedx(float newTranslateSpeedx) {
        translateSpeedx = newTranslateSpeedx;
    }

    void setTranslateSpeedz(float newTranslateSpeedz) {
        translateSpeedz = newTranslateSpeedz;
    }

    void setShader(Shader newShader) {
        shader = newShader;
    }

    void setModel(Model newModel) {
        model = newModel;
    }

    void render(Proiettile& proiettile, Navicella& navicella) {

        shader.use();

        for (int i = 0; i < righeAlieni; i++)
        {
            for (int j = 0; j < colonneAlieni; j++)
            {
                pos.x = pos.x + translateSpeedx;
                pos.z = pos.z + translateSpeedz;

                if (map[i][j] != 0)
                {

                    float x = (pos.x + j * raggio * 2.0f * spazio) ;
                    float z = (pos.z + i * raggio * 2.0f * spazio) ;

                    glm::mat4 modelAlieno = glm::mat4(1.0f);
                    modelAlieno = glm::translate(modelAlieno, glm::vec3(x, 0.0f, z));
                    modelAlieno = glm::scale(modelAlieno, glm::vec3(1.0f, raggio, 1.0f));
                    shader.setMat4("model", modelAlieno);
                    model.Draw(shader);

                    glm::vec3 posAlieno = glm::vec3(x,0.0f,z);

                    if (isHitted(proiettile, posAlieno)) {
                        map[i][j] = 0;
                    }

                    navicella.checkCollisionAlien(posAlieno, raggio);

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

            vectorProiettili[k].setSpeed(speedProiettili);
            vectorProiettili[k].incrementaColpi();
            vectorProiettili[k].inizializzaPos(glm::vec3(x, 0.0f, z));
            float random = generaNumeroCasualeFloat(-0.2f,0.2f);
            glm::vec3 proiettileAt = glm::vec3(random, 0.0f, 1.0f);
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
                barriera.renderBarriere(vectorProiettili[k]);
                k++;
            }
        }

    }

    void cambiaSpeed() {
        if (pos.x < limXalieniPos && !restart) {
            speedx = 7;
        }
        else if (pos.x > limXalieniPos && !restart) {
            speedx = 0;
            speedz = 7;
            restart = true;
        }
        else if (pos.x > limXalieniNeg && restart) {
            speedx = -7;
            speedz = 0;
        }
        else if (pos.x < limXalieniNeg && restart) {
            speedz = 7;
            speedx = 0;
            restart = false;
        }
    }

    void muovi(float speedAlieni) {
        if (pos.x < limXalieniPos && !restart && speedz == 0.0f) {
            speedx = speedAlieni;
            speedz = 0.0f;
        }
        else if (pos.x > limXalieniPos && !restart && speedx == 0.0f) {
            speedx = 0.0f;
            speedz = speedAlieni/1.5;
            restart = true;
        }
        else if (pos.x > limXalieniNeg && restart && speedz == 0.0f) {
            speedx = -speedAlieni;
            speedz = 0.0f;
        }
        else if (pos.x < limXalieniNeg && restart && speedx == 0.0f) {
            speedx = 0.0f;
            speedz = speedAlieni/1.5;
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
            if (isPointInsideCircle(punto, centro)) {
                proiettile.setVecPos(i, glm::vec3(0.0f, 0.0f, -100.0f));
                return true;
            }
        }
        return false;
    }

    float generaNumeroCasualeFloat(float estremoInferiore, float estremoSuperiore) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(estremoInferiore, estremoSuperiore);
        float random = dis(gen);
        return random;
    }
};

 
#endif 
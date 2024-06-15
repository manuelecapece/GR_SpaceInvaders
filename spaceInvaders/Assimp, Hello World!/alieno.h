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
                                            {2,2,2,2,2},
                                            {3,3,3,3,3},
                                            {4,4,4,4,4},
                                            {5,5,5,5,5},
                                            {6,6,6,6,6} };


    float raggio = 1.0f;
    float spazio = 1.2f;
    glm::vec3 pos = glm::vec3(-(pos.x + 2 * raggio * 2.0f * spazio), 0.0, -18.0f);
    float translateSpeedx;
    float translateSpeedz;
    float speedx = 0.0f;
    float speedz = 0.0f;
    float speed = 0.1;
    float speedProiettili = 6;
    int alieniEliminati = 0;
    float limXalieniPos = (((raggio * 2 + spazio) * 3) - raggio) - (pos.x + 5 * raggio * 2.0f * spazio);
    float limXalieniNeg = -((((raggio * 2 + spazio) * 3) - raggio) + (pos.x + 4 * raggio * 2.0f * spazio)) + raggio * 2;
    Shader shader;
    std::vector<Model> models =  std::vector<Model>(righeAlieni * colonneAlieni);

    bool muoviVersoDx = true;
    bool muoviVersoSx = false;
    bool muoviVersoDown = false;
    int nStepDown = 0;
    int nextAlien = 0;

    std::vector<Proiettile> vectorProiettili = std::vector<Proiettile>(righeAlieni * colonneAlieni);

public:
    // Costruttore
    Alieno() {}

    bool getMuoviVersoDx() const {
        return muoviVersoDx;
    }

    bool getMuoviVersoSx() const {
        return muoviVersoSx;
    }

    bool getMuoviVersoDown() const {
        return muoviVersoDown;
    }

    int getAlieniEliminati() {
        return alieniEliminati;
    }

    int(*getmap())[colonneAlieni] {
        return map;
        }

        int getRigheAlieni() {
        return righeAlieni;
    }

    int getColonneAlieni() {
        return colonneAlieni;
    }

    int getnStepDown() const {
        return nStepDown;
    }

    float getTranslateSpeedX() {
        return translateSpeedx;
    }

    float getTranslateSpeedZ() {
        return translateSpeedz;
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

    /*void setModel(Model newModel) {
        model = newModel;
    }*/

    void setModel(int index, Model newModel) {
        if (index >= 0 && index < 5) {
            models[index] = newModel;
        }
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

                    float x = (pos.x + j * raggio * 2.0f * spazio);
                    float z = (pos.z + i * raggio * 2.0f * spazio);

                    glm::mat4 modelAlieno = glm::mat4(1.0f);
                    modelAlieno = glm::translate(modelAlieno, glm::vec3(x, 0.0f, z));
                    modelAlieno = glm::scale(modelAlieno, glm::vec3(0.3f, 0.3, 0.3f));
                    shader.setMat4("model", modelAlieno);
                    int modelIndex = map[i][j] - 1;
                    if (modelIndex >= 0 && modelIndex < models.size()) {
                        models[modelIndex].Draw(shader);
                    }

                    glm::vec3 posAlieno = glm::vec3(x, 0.0f, z);

                    if (isHitted(proiettile, posAlieno)) {
                        map[i][j] = 0;
                        alieniEliminati++;
                    }

                    navicella.checkCollisionAlien(posAlieno, raggio);

                }
            }
        }

    }

    void inizializzaProiettili(Shader proiettileShader, Model modelCubo, int i, int j) {

        int k = i * 5 + j;

        if (map[i][j] != 0)
        {
            float x = pos.x + j * raggio * 2.0f * spazio;
            float z = pos.z + i * raggio * 2.0f * spazio;

            vectorProiettili[k].setSpeed(speedProiettili);
            vectorProiettili[k].incrementaColpi();
            vectorProiettili[k].inizializzaPos(glm::vec3(x, 0.0f, z));
            float random = generaNumeroCasualeFloat(-0.2f, 0.2f);
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
                        vectorProiettili[k].render(glm::vec3(1.0f, 0.0f, 0.0f));
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

    void stepVersoDx(int i) {
        float newPosX_dx = -(pos.x + 2 * raggio * 2.0f * spazio) + pos.x + i * raggio * 2.0f * spazio;
        float limPosX_dx = -(pos.x + 2 * raggio * 2.0f * spazio) + pos.x + 2 * raggio * 2.0f * spazio;

        if (pos.x < newPosX_dx) {
            speedx = speed;
        }
        if (pos.x >= newPosX_dx) {
            speedx = 0.0f;
        }
        if (pos.x > limPosX_dx) {
            speedx = 0.0f;
            muoviVersoDx = false;
            muoviVersoDown = true;
            nStepDown++;
        }

    }

    void stepVersoSx(int i) {
        float newPosX_sx = (pos.x + 0 * raggio * 2.0f * spazio) - pos.x - i * raggio * 2.0f * spazio;
        float limPosX_sx = (pos.x + 4 * raggio * 2.0f * spazio) - pos.x - 8 * raggio * 2.0f * spazio;

        if (pos.x > newPosX_sx) {
            speedx = -speed;
        }
        if (pos.x <= newPosX_sx) {
            speedx = 0.0f;
        }
        if (pos.x < limPosX_sx) {
            speedx = 0.0f;
            muoviVersoSx = false;
            muoviVersoDown = true;
            nStepDown++;

        }

    }

    void stepVersoDown(float& intervallo) {
        float newPosZ = -17.9f + nStepDown * raggio * 2.0f * spazio;

        if (pos.z < newPosZ) {
            speedz = speed;
        }
        if (pos.z >= newPosZ) {
            speedz = 0.0f;
            muoviVersoDown = false;
            intervallo = intervallo - 0.1f;
            speed = speed + 0.025f;
            if (nStepDown % 2 == 0) {
                muoviVersoDx = true;
            }
            else {
                muoviVersoSx = true;
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
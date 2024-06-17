#ifndef UFO_H
#define UFO_H

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

class Ufo {
private:
    glm::vec3 pos = glm::vec3(35.0f, 0.0, -22.0f);
    glm::vec3 posIniziale = glm::vec3(-15.0f, 0.0, -22.0f);
    float raggio = 2.5f;
    float translateSpeed;
    float speed = 3;  
    float speedProiettili = 6;
    float rangeSparoNeg = -10.0f;
    float rangeSparoPos = 10.0f;
    int colpiSubiti = 0;
    float rotation = 0;
    Shader shader;
    Model model;
    Model modelSfera;

public:
    // Costruttore
    Ufo() {}

    glm::vec3 getPos() const {
        return pos;
    }

    float getSpeed() const {
        return speed;
    }

    void setSpeed(float newSpeed) {
        speed = newSpeed;
    }

    void setPos(glm::vec3 newPos) {
        pos = newPos;
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

    void setModelSfera(Model newModel) {
        modelSfera = newModel;
    }

    void render() {

        if (colpiSubiti < 5) {
            shader.use();

            //Per il modello sfera
            //glm::mat4 sferaModel = glm::mat4(1.0f);
            //sferaModel = glm::translate(sferaModel, glm::vec3(pos.x, 0.0f, pos.z));
            //sferaModel = glm::scale(sferaModel, glm::vec3(raggio, raggio, raggio));
            //shader.setMat4("model", sferaModel);
            //modelSfera.Draw(shader);

            //Per il modello ufo
            glm::mat4 modelUfo = glm::mat4(1.0f);
            modelUfo = glm::translate(modelUfo, glm::vec3(pos.x, 0.0f, pos.z));
            modelUfo = glm::scale(modelUfo, glm::vec3(5.2, 5.2, 5.2));
            modelUfo = glm::rotate(modelUfo, -pigreco/2 , glm::vec3(1.0f, 0.0, 0.0f));
            rotation = rotation + translateSpeed;
            modelUfo = glm::rotate(modelUfo, rotation, glm::vec3(0.0f, 1.0, 0.0f));
            shader.setMat4("model", modelUfo);
            model.Draw(shader);

        }
        pos = glm::vec3(pos.x + translateSpeed, pos.y, pos.z);
    }

    bool isPointInsideCircle(const glm::vec2& point, const glm::vec2& center) {
        // Calcola la distanza al quadrato tra il punto e il centro della circonferenza
        float distSq = (point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y);

        // Calcola il raggio al quadrato
        float radiusSq = raggio * raggio;

        // Controlla se la distanza al quadrato è minore o uguale al raggio al quadrato
        return distSq <= radiusSq;
    }

    void checkIsHitted(Proiettile& proiettile) {
        for (int i = 0; i < proiettile.getColpiSparati() + 1; i++)
        {
            float proiettile_x = proiettile.getVecPos()[i].x;
            float proiettile_z = proiettile.getVecPos()[i].z;
            glm::vec2 punto = glm::vec2(proiettile_x, proiettile_z - (proiettile.getLunghezza() / 2));
            glm::vec2 centro = glm::vec2(pos.x, pos.z - 2.4);
            if (isPointInsideCircle(punto, centro)) {
                proiettile.setVecPos(i, glm::vec3(proiettile_x, 0.0f, -100.0f));
                colpiSubiti++;

            }
        }
    }

    bool isInRangeSparo() {
        if (pos.x > rangeSparoNeg && pos.x < rangeSparoPos) {
            return true;
        }
        return false;
    }

    void inizializzaProiettile(Proiettile& proiettile) {
        if (isInRangeSparo() && colpiSubiti < 5 && speed != 0.0f) {
            proiettile.setSpeed(speedProiettili);
            proiettile.incrementaColpi();
            proiettile.inizializzaPos(pos);
            float random = generaNumeroCasualeFloat(-0.2f, 0.2f);
            glm::vec3 proiettileAt = glm::vec3(random, 0.0f, 1.0f);
            proiettile.inizializzaDir(proiettileAt);
        }

    }

    void ripristinaPosizioneIniziale() {
        pos = posIniziale;
        colpiSubiti = 0;
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
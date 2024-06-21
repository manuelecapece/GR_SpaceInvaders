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
#include "esplosione.h"

const float pigreco = 3.14159265358979323846;
const int TIPO_NAVICELLA = 7;


class Navicella {
private:

    glm::vec3 pos = glm::vec3(0.05f, 0.0, 8.0f);
    float raggio = 1.0f;
    float rotation = 0.0f;
    float translateSpeed;
    float speed = 8;  // velocita della navicella
    float limX_pos = 999;
    bool isHitted = false;
    int vite = 3;
    double startTimeHitted;
    Shader shader;
    Model model;
    Model modelSfera;

public:
    // Costruttore
    Navicella() {}

    glm::vec3 getPos() const {
        return pos;
    }

    int getVite() const {
        return vite;
    }

    float getSpeed() const {
        return speed;
    }

    bool getIsHitted() const {
        return isHitted;
    }

    double getStartTimeHitted() const {
        return startTimeHitted;
    }

    void setPos(glm::vec3 newPos) {
        pos = newPos;
    }

    void setSpeed(float newSpeed) {
        speed = newSpeed;
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

    void setHisHitted(bool newValue) {
        isHitted = newValue;
    }

    void setModelSfera(Model newModel) {
        modelSfera = newModel;
    }

    void render(bool moveRight, bool moveLeft) {

        if (!isHitted) {
            shader.use();  
           
            if (moveRight) {
                pos = glm::vec3(pos.x + translateSpeed, pos.y, pos.z);
            }
            if (moveLeft) {
                pos = glm::vec3(pos.x - translateSpeed, pos.y, pos.z);
            }

            //Per il modello sfera
            //glm::mat4 sferaModel = glm::mat4(1.0f);
            //sferaModel = glm::translate(sferaModel, glm::vec3(pos.x, 0.0f, pos.z));
            //sferaModel = glm::scale(sferaModel, glm::vec3(raggio, raggio, raggio));
            //shader.setMat4("model", sferaModel);
            //modelSfera.Draw(shader);

            //Per il modello navicella
            glm::mat4 modelNavicella = glm::mat4(1.0f);
            modelNavicella = glm::translate(modelNavicella, glm::vec3(pos.x, 0.0f, pos.z + 0.5f));
            modelNavicella = glm::scale(modelNavicella, glm::vec3(0.25f, 0.25f, 0.25f));
            modelNavicella = glm::rotate(modelNavicella, pigreco, glm::vec3(0.0f, 1, 0.0f));
            ruotaNavicella(moveRight, moveLeft, modelNavicella);
            shader.setMat4("model", modelNavicella);
            model.Draw(shader);
        }

    }

    void ruotaNavicella(bool moveRight, bool moveLeft, glm::mat4& modelNavicella) {
        if (moveRight && rotation < (pigreco / 10)) {
            rotation = rotation + translateSpeed;
        }
        else if (moveLeft && rotation > (-pigreco / 10)) {
            rotation = rotation - translateSpeed;
        }

        if (!moveRight && rotation > 0.05f) {
            rotation = rotation - translateSpeed;
        }
        else if (!moveLeft && rotation < -0.05f) {
            rotation = rotation + translateSpeed;
        }

        modelNavicella = glm::rotate(modelNavicella, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    bool isPointInsideCircle(const glm::vec2& point, const glm::vec2& center) {
        // Calcola la distanza al quadrato tra il punto e il centro della circonferenza
        float distSq = (point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y);

        // Calcola il raggio al quadrato
        float radiusSq = raggio * raggio;

        // Controlla se la distanza al quadrato è minore o uguale al raggio al quadrato
        return distSq <= radiusSq;
    }

    void checkIsHitted(Proiettile& proiettile, Esplosione& esplosione) {
        for (int i = 0; i < proiettile.getColpiSparati() + 1; i++)
        {

            glm::vec3 centroRect = glm::vec3(proiettile.getVecPos()[i].x, 0.0f, proiettile.getVecPos()[i].z);
            glm::vec3 hitPoint;
            glm::vec2 punto;
            float angolo;
            glm::vec3 dir = proiettile.getVecDir()[i];
            glm::vec3 posBullet = proiettile.getVecPos()[i];
            float larghezza = proiettile.getLarghezza();
            float lunghezza = proiettile.getLunghezza();

            if (dir.x > 0) {
                hitPoint = glm::vec3(posBullet.x - (larghezza / 2), 0.0f, posBullet.z + (lunghezza / 2));
                angolo = calcolaAngolo(dir, glm::vec3(0.0f, 0.0f, 1.0f));

            }
            else if (dir.x < 0) {
                hitPoint = glm::vec3(posBullet.x + (larghezza / 2), 0.0f, posBullet.z + (lunghezza / 2));
                angolo = -calcolaAngolo(dir, glm::vec3(0.0f, 0.0f, 1.0f));

            }
            else {
                punto = glm::vec2(centroRect.x, centroRect.z);
            }

            if (dir.x < 0 || dir.x > 0) {

                glm::vec3 translatedHitPoint = hitPoint - centroRect;
                glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f); // Rotazione intorno all'asse y
                glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angolo, rotationAxis);
                glm::vec3 rotatedTranslatedHitPoint = glm::vec3(rotationMatrix * glm::vec4(translatedHitPoint, 1.0f));
                glm::vec3 rotatedHitPoint = rotatedTranslatedHitPoint + centroRect;
                punto = glm::vec2(rotatedHitPoint.x, rotatedHitPoint.z);
            }

            glm::vec2 centroCirconf = glm::vec2(pos.x, pos.z + 1.);
            if (isPointInsideCircle(punto, centroCirconf)) {
                startTimeHitted = glfwGetTime();
                proiettile.eliminaInPos(i);
                isHitted = true;
                esplosione.inizializza(pos, TIPO_NAVICELLA);
                vite = vite - 1;
                if (vite >= 0) {
                    ripristinaPosizioneIniziale();
                }
                return;
            }
            if (posBullet.z > 15) {
                proiettile.eliminaInPos(i);
            }

        }

    }

    void ripristinaPosizioneIniziale() {
        pos = glm::vec3(0.05f, 0.0, 8.0f);
    }

    void checkCollisionAlien(glm::vec3 alienPos, float raggioAlieno) {
        float distSq = (alienPos.x - pos.x) * (alienPos.x - pos.x) + (alienPos.z - pos.z) * (alienPos.z - pos.z);
        if (distSq <= (raggio + raggioAlieno)) {
            isHitted = true;
        }
    }

    void inizializzaProiettile(Proiettile& proiettile) {
        proiettile.incrementaColpi();
        proiettile.inizializzaPos(glm::vec3(pos.x, pos.y, pos.z - 1.));
        //proiettile.inizializzaPos(pos);
        glm::vec3 proiettileAt = glm::vec3(0.0f, 0.0f, -1.0f);
        proiettile.inizializzaDir(proiettileAt);
    }

    float calcolaAngolo(glm::vec3 u, glm::vec3 v) {

        float dotProduct = glm::dot(u, v);
        float norm_u = glm::length(u);
        float norm_v = glm::length(v);
        float cosTheta = dotProduct / (norm_u * norm_v);
        float theta = glm::acos(cosTheta);

        return theta;
    }

};

#endif 


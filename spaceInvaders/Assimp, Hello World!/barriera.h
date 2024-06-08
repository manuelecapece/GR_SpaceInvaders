#ifndef BARRIERA_H
#define BARRIERA_H

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


class Barriera {
private:

    glm::vec3 pos = glm::vec3(-1.0f, 0.0, 6.0f);
    const float lunghezza = 0.3f;
    const float larghezza = 0.3f;
    const float altezza = 0.6f;
    int static const righeCubiBarriera = 4;
    int static const colonneCubiBarriera = 8;

    int map1[righeCubiBarriera][colonneCubiBarriera] = {{1,1,1,1,1,1,1,1},
                                                        {1,1,1,1,1,1,1,1},
                                                        {1,1,0,0,0,0,1,1},
                                                        {1,1,0,0,0,0,1,1}};

    int map2[righeCubiBarriera][colonneCubiBarriera] = {{1,1,1,1,1,1,1,1},
                                                        {1,1,1,1,1,1,1,1},
                                                        {1,1,0,0,0,0,1,1},
                                                        {1,1,0,0,0,0,1,1} };

    int map3[righeCubiBarriera][colonneCubiBarriera] = {{1,1,1,1,1,1,1,1},
                                                        {1,1,1,1,1,1,1,1},
                                                        {1,1,0,0,0,0,1,1},
                                                        {1,1,0,0,0,0,1,1} };

    Shader shader;
    Model model;

public:
    // Costruttore
    Barriera() {}

    void setShader(Shader newShader) {
        shader = newShader;
    }

    void setModel(Model newModel) {
        model = newModel;
    }

    void setPos(glm::vec3 newPos) {
        pos = newPos;
    }

    void renderBarriere(Proiettile& proiettile) {

        shader.use();

        for (int k = 0; k < 3; k++) {
            pos = glm::vec3(-6.0f + k * 5.0f, 0.0f, 6.0f);

            if (k == 0) {
                renderBarriera(map1, proiettile);
            }

            if (k == 1) {
                renderBarriera(map2, proiettile);
            }

            if (k == 2) {
                renderBarriera(map3, proiettile);
            }
        }

    }

    void renderBarriera(int map[righeCubiBarriera][colonneCubiBarriera], Proiettile& proiettile) {

        for (int i = 0; i < righeCubiBarriera; i++)
        {
            for (int j = 0; j < colonneCubiBarriera; j++)
            {
                if (map[i][j] != 0)
                {

                    float x = pos.x + j * larghezza;
                    float z = pos.z + i * lunghezza;

                    glm::mat4 modelCubo = glm::mat4(1.0f);
                    modelCubo = glm::translate(modelCubo, glm::vec3(x, 0.0f, z));
                    modelCubo = glm::scale(modelCubo, glm::vec3(larghezza, altezza, lunghezza));
                    shader.setMat4("model", modelCubo);
                    model.Draw(shader);

                    glm::vec3 posCubo = glm::vec3(x, 0.0f, z);

                    if (isHitted(proiettile, posCubo)) {
                        map[i][j] = 0;
                    }

                }
            }
        }

    }

    bool isPointInsideCircle(const glm::vec2& point, const glm::vec2& center) {
        // Calcola la distanza al quadrato tra il punto e il centro della circonferenza
        float distSq = (point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y);

        // Calcola il raggio al quadrato
        float radiusSq = (larghezza/2) * (larghezza / 2);

        // Controlla se la distanza al quadrato è minore o uguale al raggio al quadrato
        return distSq <= radiusSq;
    }


    bool isHitted(Proiettile& proiettile, glm::vec3 posCubo) {
        for (int i = 0; i < proiettile.getColpiSparati() + 1; i++)
        {
            float proiettile_x = proiettile.getVecPos()[i].x;
            float proiettile_z = proiettile.getVecPos()[i].z;
            glm::vec2 punto = glm::vec2(proiettile_x, proiettile_z - (proiettile.getLunghezza() / 2));
            glm::vec2 centro = glm::vec2(posCubo.x, posCubo.z);
            if (isPointInsideCircle(punto, centro)) {
                proiettile.setVecPos(i, glm::vec3(proiettile_x, 0.0f, -21.0f));
                return true;

            }
        }
        return false;
    }

    


};

#endif 

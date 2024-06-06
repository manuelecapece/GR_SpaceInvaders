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

class Alieno {
private:
    
    int map[5][6] = { {1,1,1,1,1,1},
                      {1,1,1,1,1,1},
                      {1,1,1,1,1,1},
                      {1,1,1,1,1,1},
                      {1,1,1,1,1,1}};

    glm::vec3 pos = glm::vec3(-4.77f, 0.0, -12.0f);
    float raggio = 1.0f;
    float spazio = 1.2f;
    Shader shader;
    Model model;

    std::vector<Proiettile> vectorProiettili;

public:
    // Costruttore
    Alieno(){}

    int(*getmap())[6] {
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

    void renderAlieni(Proiettile& proiettile) {

        shader.use();

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (map[i][j] != 0)
                {

                    float x = pos.x + i * raggio * 2.0f * spazio;
                    float z = pos.z + j * raggio * 2.0f * spazio;

                    glm::mat4 modelAlieno = glm::mat4(1.0f);
                    modelAlieno = glm::translate(modelAlieno, glm::vec3(x, 0.0f, z));
                    modelAlieno = glm::scale(modelAlieno, glm::vec3(1.0f, raggio, 1.0f));
                    shader.setMat4("model", modelAlieno);
                    model.Draw(shader);

                    glm::vec3 posAlieno = glm::vec3(x,0.0f,z);

                    if (isAlienHitted(proiettile, posAlieno)) {
                        map[i][j] = 0;
                    }

                }
            }
        }

    }

    void spara(float speed, double deltaTime) {

        int k = 0;

        for (int i = 0; i < 5; i++)
        {
	        for (int j = 0; j < 6; j++)
	        {

		        if (map[i][j] != 0)
		        {
                    float x = pos.x + i * raggio * 2.0f * spazio;
                    float z = pos.z + j * raggio * 2.0f * spazio;

                    if (deltaTime > 10) {
                        vectorProiettili[k].incrementaColpi();
                        vectorProiettili[k].inizializzaPos(glm::vec3(x, 0.0f, z));
                        glm::vec3 proiettileAt = glm::vec3(0.0f, 0.0f, 20.0f);
                        proiettileAt = glm::normalize(proiettileAt);
                        vectorProiettili[k].inizializzaDir(proiettileAt);
                        vectorProiettili[k].renderProiettile();
                        vectorProiettili[k].setTranslateSpeed(speed * deltaTime);
                    }
		
		        }

                k++;
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



    bool isAlienHitted(Proiettile& proiettile, glm::vec3 posAlieno) {
        for (int i = 0; i < proiettile.getColpiSparati() + 1; i++)
        {
            float proiettile_x = proiettile.getVecPos()[i].x;
            float proiettile_z = proiettile.getVecPos()[i].z;
            glm::vec2 punto = glm::vec2(proiettile_x, proiettile_z - (proiettile.getLunghezza() / 2));
            glm::vec2 centro = glm::vec2(posAlieno.x, posAlieno.z);
            if(isPointInsideCircle(punto, centro)){
                proiettile.setVecPos(i, glm::vec3(proiettile_x, 0.0f, -20.0f));
                return true;

            }
        }
        return false;
    }
};

 
#endif 
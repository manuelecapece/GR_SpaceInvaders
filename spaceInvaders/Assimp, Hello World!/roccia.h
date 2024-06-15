#ifndef ROCCIA_H
#define ROCCIA_H

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
#include "model.h"

const int NUM_ROCCE = 500;

class Roccia {
private:
    std::vector<glm::vec3> positions;
    std::vector<float> sizes;
    std::vector<float> speeds;
    Shader shader;
    Model model;
    float screenTop = 70.0f;     // Limite superiore dal quale le rocce iniziano a scendere
    float screenBottom = -70.0f; // Limite inferiore fino al quale le rocce scendono prima di essere riportate sopra
    float screenWidth = 500.0f;  // Larghezza effettiva della finestra di rendering
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distributionX;
    std::uniform_real_distribution<float> distributionZ;
    std::uniform_real_distribution<float> distributionSize;
    std::uniform_real_distribution<float> distributionSpeed;

public:
    // Costruttore
    Roccia()
        : distributionX(-screenWidth, screenWidth),
        distributionZ(screenBottom, screenTop),
        distributionSize(0.5f, 4.0f),
        distributionSpeed(2.0f, 10.0f) { // Generiamo anche velocità diverse
        for (int i = 0; i < NUM_ROCCE; ++i) {
            positions.push_back(glm::vec3(distributionX(generator), 0.0f, distributionZ(generator)));
            sizes.push_back(distributionSize(generator));
            speeds.push_back(distributionSpeed(generator)); // Velocità casuale per ogni roccia
        }
    }

    void setShader(Shader newShader) {
        shader = newShader;
    }

    void setModel(Model newModel) {
        model = newModel;
    }

    void update(float deltaTime) {
        for (int i = 0; i < NUM_ROCCE; ++i) {
            positions[i].z += speeds[i] * deltaTime; // Muovi lungo l'asse Z verso l'alto
            if (positions[i].z > screenTop) { // Se la roccia supera il limite superiore
                positions[i].z = screenBottom; // Riportala al limite inferiore
                positions[i].x = distributionX(generator); // Nuova posizione X casuale
                speeds[i] = distributionSpeed(generator); // Nuova velocità casuale
            }
        }
    }

    void render() {
        shader.use();
        for (int i = 0; i < NUM_ROCCE; ++i) {
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, positions[i]);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(sizes[i] * 0.08f));
            shader.setMat4("model", modelMatrix);
            model.Draw(shader);
        }
    }
};

#endif


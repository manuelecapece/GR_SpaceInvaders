#ifndef BONUS_H
#define BONUS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_m.h"
#include "camera.h"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <stack>
#include "model.h"

class Bonus {
private:

    glm::vec3 pos;
    int righe;
    int colonne;
    float raggio = 1.0f;
    std::vector<std::vector<int>> map;
    float translateSpeed;
    Shader shader;
    Model model;

public:
    // Costruttore
    Bonus() {}

    void inizializza(Alieno alieno) {

        map = alieno.getMap();
        righe = alieno.getRigheAlieni();
        colonne = alieno.getColonneAlieni();

        for (int i = 0; i < alieno.getRigheAlieni(); i++)
        {
            for (int j = 0; j < alieno.getColonneAlieni(); j++)
            {
                map[i][j] = 0;
                
            }
        }

        for (int i = 0; i < alieno.getLivello(); i++)
        {

            int idRiga = generaNumeroCasualeInt(0, alieno.getRigheAlieni() - 1);
            int idColonna = generaNumeroCasualeInt(0, alieno.getColonneAlieni() - 1);
            int tipoBonus = generaNumeroCasualeInt(1, 2);

            while (map[idRiga][idColonna] != 0) {
                idRiga = generaNumeroCasualeInt(0, alieno.getRigheAlieni() - 1);
                idColonna = generaNumeroCasualeInt(0, alieno.getColonneAlieni() - 1);
            }

            map[idRiga][idColonna] = tipoBonus;
        }

        //for (const auto& row : map) {
        //    for (int value : row) {
        //        std::cout << value << " ";
        //    }
        //    std::cout << std::endl;
        //}
    }



    float generaNumeroCasualeInt(int estremoInferiore, int estremoSuperiore) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(estremoInferiore, estremoSuperiore);
        int random = dis(gen);
        return random;
    }


};

#endif 

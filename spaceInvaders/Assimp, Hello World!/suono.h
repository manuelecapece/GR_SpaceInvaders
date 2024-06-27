#ifndef SUONO_H
#define SUONO_H

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
#include <irrklang/irrKlang.h>
#include "model.h"
#include "proiettile.h"
#include "esplosione.h"

#pragma comment(lib, "irrKlang.lib") 
using namespace irrklang;

class Suono {
private:
    bool playGameStart = true;
    bool playGameOver = true;
    bool playMovimentoAlieni = true;
    bool playMovimentoUfo = true;
    ISoundEngine* soundEngine;
    ISoundSource* gameOver;
    ISoundSource* gameStart;
    ISoundSource* sparoNavicella;
    ISoundSource* sparoAlieno;
    ISoundSource* esplosioneAlieno;
    ISoundSource* esplosioneNavicella;
    ISoundSource* esplosioneUfo;
    ISoundSource* movimentoAlieni;
    ISoundSource* distruggiBarriera;
    ISoundSource* caricaBonus;
    ISoundSource* sparaColpoSpeciale;
    ISoundSource* caricaNuovoLivello;
    ISoundSource* movimentoUfo;
    ISoundSource* colpisciUfo;
    ISound* suonoUfo;
    double startTimeGameOver = 9999999999;

public:
    // Costruttore
    Suono() {}

    void inizializza() {
        soundEngine = createIrrKlangDevice();
        gameOver = soundEngine->addSoundSourceFromFile("../src/sounds/gameOver.wav");
        gameStart = soundEngine->addSoundSourceFromFile("../src/sounds/gameStart.wav");
        sparoNavicella = soundEngine->addSoundSourceFromFile("../src/sounds/sparoNavicella.wav");
        sparoAlieno = soundEngine->addSoundSourceFromFile("../src/sounds/sparoAlieno.wav");
        esplosioneAlieno = soundEngine->addSoundSourceFromFile("../src/sounds/esplosioneAlieno.wav");
        esplosioneNavicella = soundEngine->addSoundSourceFromFile("../src/sounds/esplosioneNavicella.wav");
        esplosioneUfo = soundEngine->addSoundSourceFromFile("../src/sounds/esplosioneUfo.wav");
        movimentoAlieni = soundEngine->addSoundSourceFromFile("../src/sounds/movimentoAlieni.wav");
        movimentoUfo = soundEngine->addSoundSourceFromFile("../src/sounds/movimentoUfo.wav");
        distruggiBarriera = soundEngine->addSoundSourceFromFile("../src/sounds/distruggiBarriera.wav");
        sparaColpoSpeciale = soundEngine->addSoundSourceFromFile("../src/sounds/sparaColpoSpeciale.wav");
        caricaBonus = soundEngine->addSoundSourceFromFile("../src/sounds/caricaBonus.wav");
        caricaNuovoLivello = soundEngine->addSoundSourceFromFile("../src/sounds/caricaNuovoLivello.wav");
        colpisciUfo = soundEngine->addSoundSourceFromFile("../src/sounds/colpisciUfo.wav");
    }

    void setPlayGameStart(bool val) {
        playGameStart = val;
    }

    void setPlayGameOver(bool val) {
        playGameOver = val;
    }

    void setPlayMovimentoAlieni(bool val) {
        playMovimentoAlieni = val;
    }

    void setPlayMovimentoUfo(bool val) {
        playMovimentoUfo = val;
    }

    void inizializzaStartTimeGameOver() {
        startTimeGameOver = glfwGetTime();
    }

    void soundGameStart() {
        if (playGameStart) {
            soundEngine->play2D(gameStart, false);
            playGameStart = false;
        }
    }

    void soundGameOver() {
        if (playGameOver) {
            soundEngine->play2D(gameOver, false);
            playGameOver = false;
        }
    }

    void soundSparoNavicella() {
        soundEngine->play2D(sparoNavicella, false);
    }

    void soundSparoAlieno() {
        soundEngine->play2D(sparoAlieno, false);
    }

    void soundEsplosioneAlieno() {
        soundEngine->play2D(esplosioneAlieno, false);
    }

    void soundEsplosioneNavicella() {
        soundEngine->play2D(esplosioneNavicella, false);
    }

    void soundEsplosioneUfo() {
        soundEngine->play2D(esplosioneUfo, false);
    }

    void soundDistruggiBarriera() {
        distruggiBarriera->setDefaultVolume(0.5f);
        soundEngine->play2D(distruggiBarriera, false);
    }

    void soundCaricaBonus() {
        soundEngine->play2D(caricaBonus, false);
    }

    void soundSparoProiettileSpeciale() {
        soundEngine->play2D(sparaColpoSpeciale, false);
    }

    void soundCaricaNuovoLivello() {
        soundEngine->play2D(caricaNuovoLivello, false);
    }

    void soundMovimentoAlieni() {
        if (playMovimentoAlieni) {
            soundEngine->play2D(movimentoAlieni, false);
            playMovimentoAlieni = false;
        }
    }

    void soundMovimentoAlieni2() {
        soundEngine->play2D(movimentoAlieni, false);
    }

    void soundMovimentoUfo() {
        if (playMovimentoUfo) {
            suonoUfo = soundEngine->play2D(movimentoUfo, true, false, true);
            playMovimentoUfo = false;
        }
    }

    void soundColpisciUfo() {
        soundEngine->play2D(colpisciUfo, false);
    }

    void stopSoundMovimentoUfo() {
        if (suonoUfo != nullptr) {
            suonoUfo->stop();
        }
    }

    void ripristina() {
        playGameStart = true;
    }

    void dropSoundEngine() {
        soundEngine->drop();
    }

};

#endif 

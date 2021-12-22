/*
 * Usuario.cc
 *
 *  Created on: Dec 12, 2021
 *      Author: ?
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <random>
#include "Paquete_m.h"

using namespace omnetpp;

class Usuario : public cSimpleModule
{
    private:
        double lambda = 2;
        double longitudmedia;
        int numpaquetes = 20;
        int numeroseq = 0;
        int numerrors = 0;
    protected:
        virtual void initialize() override;
        virtual std::vector<double> getDepartures(double lambda, int numpaquetes);
        virtual std::vector<double> getLengths(double mu, int numpaquetes);
        virtual Paquete* getPacket();
        virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Usuario);

void Usuario::initialize() {
    longitudmedia = (double) par("longitudmedia");
    // Tiempos de llegada: se generan los paquetes y se planifican
    std::vector<double> llegadas = getDepartures(lambda, numpaquetes);
    std::vector<double> longitudes = getLengths(longitudmedia, numpaquetes);
    for(int i = 0; i < llegadas.size(); i++) {

        Paquete *pqt = getPacket();
        pqt -> setBitLength(longitudes[i]);
        scheduleAt(llegadas[i], pqt);
    }



}

std::vector<double> Usuario::getDepartures(double lambda, int numpaquetes) {

    std::uniform_real_distribution<double> randomReal(0.0, 1.0);
    std::default_random_engine generator(time(NULL));
    std::vector<double> llegadas(numpaquetes);
    for(int i = 0; i < llegadas.size(); i++) {
        double randomNumber = randomReal(generator);
        double number = (-1/lambda) * log(randomNumber);
        if (i != 0)
            llegadas[i] = llegadas[i - 1] + number;
        else
            llegadas[i] = number;
    }
    return llegadas;
}

std::vector<double> Usuario::getLengths(double longitudmedia, int numpaquetes) {
    std::uniform_real_distribution<double> randomReal(0.0, 1.0);
    std::default_random_engine generator(time(NULL));
    std::vector<double> longitudes(numpaquetes);
    for(int i = 0; i < longitudes.size(); i++) {
        double randomNumber = randomReal(generator);
        double number = (-longitudmedia) * log(randomNumber);
        longitudes[i] = number;
    }
    return longitudes;
}

Paquete* Usuario::getPacket() {
    std::string nombrepaquete = "paquete::" + std::to_string(getId()) + "::" + std::to_string(numeroseq);
    char *charnombrepaquete = new char[nombrepaquete.length() + 1];
    strcpy(charnombrepaquete, nombrepaquete.c_str());
    Paquete *pqt = new Paquete(charnombrepaquete);
    pqt -> setKind(4);
    pqt -> setNumeroseq(numeroseq);
    pqt -> setNumerrors(numeroseq);
    numeroseq++;
    return pqt;
}

void Usuario::handleMessage(cMessage *msg) {

    Paquete *pqt = check_and_cast<Paquete *> (msg);
    send(pqt, "out");

}


/*
 * Conmutador.cc
 *
 *  Created on: Dec 22, 2021
 *      Author: ?
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "Paquete_m.h"

using namespace omnetpp;

class Conmutador : public cSimpleModule
{
    private:
    double probabilidad;
    int numeronodo;
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Conmutador);

void Conmutador::initialize() {
    probabilidad = (double) par("probabilidad");
    numeronodo = (int) par("numeronodo");

}

void Conmutador::handleMessage(cMessage *msg)
{
    Paquete *pkt = check_and_cast<Paquete *> (msg);
    pkt -> setKind(5); //indicamos que el paquete ha pasado por el conmutador
    pkt -> setUltimonodo(numeronodo);
    int puertasalida;
    double aleatorio = dblrand();

    if(aleatorio < probabilidad){
        puertasalida = 0;
        EV << "Puerta de salida 0\n";
    }
    else{
        puertasalida = 1;
        EV << "Puerta de salida 1\n";
    }
    send(pkt, "conexion$o", puertasalida);

}


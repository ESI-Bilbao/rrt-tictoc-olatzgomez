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

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;

};

Define_Module(Conmutador);

void Conmutador::initialize() {


}

void Conmutador::handleMessage(cMessage *msg)
{
    Paquete *pkt = check_and_cast<Paquete *> (msg);
    send(pkt, "out");

}

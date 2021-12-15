
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "Paquete_m.h"

using namespace omnetpp;

class Servidor : public cSimpleModule
{
    protected:
        virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Servidor);

void Servidor::handleMessage(cMessage *msg)
{
    Paquete *pkt = (Paquete*) msg;

    if (pkt -> getKind() == 1) { // 1: Packet
        if (pkt -> hasBitError()) {
            EV << "Hay error, se devuelve NACK\n";
            Paquete *nak = new Paquete("NAK");
            nak -> setKind(3);
            send(nak, "out");
        }
        else {
            EV << "No hay error, se devuelve un ACK\n";
            Paquete *ack = new Paquete("ACK");
            ack -> setKind(2);
            send(ack, "out");
            EV << "Packet it's okay!";
        }
    }
}

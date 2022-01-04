
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
    int nodo = pkt -> getUltimonodo();
    cGate *arrivalGatecgate = pkt -> getArrivalGate();
    int arrivalGate = arrivalGatecgate -> getIndex();

    EV << "Ha llegado un paquete del nodo "+std::to_string(nodo)+"al servidor\n";

    if (pkt -> getKind() == 1) { // 1: Packet
        EV << "HA LLEGADO AL SERVIDOR\n";
        if (pkt -> hasBitError()) {
            EV << "Hay error, se devuelve NACK\n";
            Paquete *nak = new Paquete("NAK");
            nak -> setKind(3);
            send(nak, "cx$o",arrivalGate);
        }
        else {
            EV << "No hay error, se devuelve un ACK\n";
            Paquete *ack = new Paquete("ACK");
            ack -> setKind(2);
            send(ack, "cx$o",arrivalGate);
            EV << "Packet is okay!";
        }
    }
}

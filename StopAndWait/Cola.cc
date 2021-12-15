/*
 * Cola.cc
 *
 *  Created on: Dec 12, 2021
 *      Author: ?
 */

/*
 * networkNode.cc
 *
 *  Created on: 4 dic. 2019
 *      Author: Adrian
 */
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "Paquete_m.h"

using namespace omnetpp;

class Cola : public cSimpleModule
{
    private:

        cQueue *queue;  // cola

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void sendNew(Paquete *pkt);
        virtual void sendNext();
        virtual void sendPacket(Paquete *pkt);
};

Define_Module(Cola);

void Cola::initialize() {

    queue = new cQueue("cola");


}

void Cola::handleMessage(cMessage *msg)
{
    Paquete *pkt = check_and_cast<Paquete *> (msg);
    if (pkt -> getKind() == 1) { // 1: paquete de la fuente
        EV << "Se envia un mensaje recibido de la fuente\n";
        sendNew(pkt);
        return;
    }

    if (pkt -> getKind() == 2) { // 2: ACK
        EV << "ACK from next node\n";
        if (queue -> isEmpty())
            EV << "Se ha recibido ACK pero no hay mas paquetes en la cola\n";
        else {
            // pop() removes queue's first packet
            queue -> pop();
            sendNext();
        }
    }
    if (pkt -> getKind() == 3) { // 3: NACK
        EV << "NAK from next node\n";
        sendNext();
    }
}

void Cola::sendNew(Paquete *pkt) {

    if (queue -> isEmpty()) {
        EV << "La cola esta vacia\n";
        // Insert in queue (it may have to be sent again)
        queue -> insert(pkt);
        sendPacket(pkt);
    } else {
        EV << "La cola no esta vacia\n";
        queue -> insert(pkt);
    }
}

void Cola::sendNext() {
    if (queue -> isEmpty())
        EV << "No hay mas paquetes en la cola\n";
    else {
        // front() gets the packet without removing it from queue
        Paquete *pkt = check_and_cast<Paquete *> (queue -> front());
        sendPacket(pkt);
    }
}

void Cola::sendPacket(Paquete *pkt) {

        // OMNeT++ can't send a packet while it is queued, must send a copy
        Paquete *newPkt = check_and_cast<Paquete *> (pkt -> dup());
        send(newPkt, "out");

}




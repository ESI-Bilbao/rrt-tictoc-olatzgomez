#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "Paquete_m.h"

using namespace omnetpp;

class Elementosaw : public cSimpleModule
{
    private:

        cQueue *queue;  // cola
        Paquete *timeoutEvent;  // holds pointer to the timeout self-message
        simtime_t timeout;  // timeout



    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void sendNew(Paquete *pkt);
        virtual void sendNext();
        virtual void sendPacket(Paquete *pkt);
};

Define_Module(Elementosaw);

void Elementosaw::initialize() {

    queue = new cQueue("cola");
    timeout = 1.0;
    timeoutEvent = new Paquete("timeoutEvent");

}

void Elementosaw::handleMessage(cMessage *msg)
{
    Paquete *pkt = check_and_cast<Paquete *> (msg);

    int nodo = pkt -> getUltimonodo();

        //EV << "Ha llegado un paquete del nodo "+std::to_string(nodo)+"\n";


    if (msg == timeoutEvent) {
            // If we receive the timeout event, that means the packet hasn't
            // arrived in time and we have to re-send it.
            EV << "Timeout, se reenvia el mensaje\n";
            sendNext();
        }

    if (pkt -> getKind() == 4) { // 4: paquete de la fuente
            // aqui estaba el error pkt -> setKind(1); //porque ahora ya no es de una fuente, sino de un nodo
            EV << "Se envia un mensaje recibido de la fuente\n";
            sendNew(pkt);
            return;
        }

    if (pkt -> getKind() == 5) { // 4: paquete del conmutador
                // aqui estaba el error pkt -> setKind(1); //porque ahora ya no es desde el conmutador, sino de un nodo
                EV << "Se envia un mensaje recibido del conmutador\n";
                sendNew(pkt);
                return;
            }

    if (pkt -> getKind() == 1) { // 1: paquete de otro nodo
        EV << "SE HA RECIBIDO UN PAQUETE DE OTRO NODO\n";
        if (pkt -> hasBitError()) {
                    EV << "Hay error, se devuelve NACK\n";
                    Paquete *nak = new Paquete("NAK");
                    nak -> setKind(3);
                    send(nak, "conexion$o",1);
                }
                else {
                    EV << "No hay error, se devuelve un ACK\n";
                    Paquete *ack = new Paquete("ACK");
                    ack -> setKind(2);
                    send(ack, "conexion$o",1);

                }
        EV << "Se envia un mensaje recibido de otro nodo\n";
        sendNew(pkt);
        return;
    }

    if (pkt -> getKind() == 2) { // 2: ACK
        EV << "Se ha recibido un ACK\n";
        cancelEvent(timeoutEvent); // se cancela el timeoutevent porque ha llegado el ACK
        if (queue -> isEmpty())
            EV << "Se ha recibido ACK pero no hay mas paquetes en la cola\n";
        else {
            // pop() removes queue's first packet
            queue -> pop();
            sendNext();
        }
    }
    if (pkt -> getKind() == 3) { // 3: NACK
        EV << "Se ha recibido un NAK\n";
        cancelEvent(timeoutEvent); // se cancela el timeout del intento anterior de envio
        sendNext();
    }
}

void Elementosaw::sendNew(Paquete *pkt) {

    if (queue -> isEmpty()) {
        //EV << "La cola esta vacia\n";
        // Insert in queue (it may have to be sent again)
        queue -> insert(pkt);
        sendPacket(pkt);
    } else {
        //EV << "La cola no esta vacia\n";
        queue -> insert(pkt);
    }
}

void Elementosaw::sendNext() {
    if (queue -> isEmpty())
        EV << "No hay mas paquetes en la cola\n";
    else {
        // front() gets the packet without removing it from queue
        Paquete *pkt = check_and_cast<Paquete *> (queue -> front());
        sendPacket(pkt);
    }
}

void Elementosaw::sendPacket(Paquete *pkt) {

        // OMNeT++ can't send a packet while it is queued, must send a copy
        Paquete *newPkt = check_and_cast<Paquete *> (pkt -> dup());

// estaba aqui el error. antes de entrar a esta funcion se hacia un setKind a 1 por lo que solo entraba al else
        if (newPkt -> getKind() == 5) {
            newPkt -> setKind(1);
            newPkt -> setMomentosalidaun(simTime().dbl()*1000);
            send(newPkt, "conexion$o",1);
        } else {
            newPkt -> setKind(1);
            send(newPkt, "conexion$o",0);
        }

        scheduleAt(simTime()+timeout, timeoutEvent);

}




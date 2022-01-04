
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "Paquete_m.h"

using namespace omnetpp;

class Servidor : public cSimpleModule
{
    private:
        double retardoee;
        double retardoenlace;
        cLongHistogram retardoeehist;
        cOutVector retardoeevec;
        cLongHistogram retardoenlacehist;
        cOutVector retardoenlacevec;
    protected:
        virtual void initialize() override;
        virtual void finish() override;
        virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Servidor);

void Servidor::initialize()
    {
        // Initialize variables
        retardoee = 0;
        retardoenlace = 0;
        WATCH(retardoee);
        WATCH(retardoenlace);

        retardoenlacehist.setName("retardoenlacehist");
        retardoeehist.setName("retardoeehist");

        retardoenlacevec.setName("retardoenlacevec");
        retardoeevec.setName("retardoeevec");
    }


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

            double tiempouno = pkt -> getMomentosalida();
            double tiempodos = pkt -> getMomentosalidaun();
            retardoee = simTime().dbl()*1000+tiempouno;
            retardoenlace = simTime().dbl()*1000+tiempodos;
            retardoeehist.collect(retardoee);
            retardoeevec.record(retardoee);

            retardoenlacehist.collect(retardoenlace);
            retardoenlacevec.record(retardoenlace);

            Paquete *ack = new Paquete("ACK");

            ack -> setKind(2);
            send(ack, "cx$o",arrivalGate);
            EV << "Packet is okay!";
        }
    }
}

void Servidor::finish()
    {
        // This function is called by OMNeT++ at the end of the simulation.
 //       EV << "Sent:     " << numSent << endl;
    //    EV << "Received: " << numReceived << endl;
    //    EV << "Hop count, min:    " << hopCountStats.getMin() << endl;
    //    EV << "Hop count, max:    " << hopCountStats.getMax() << endl;
    //    EV << "Hop count, mean:   " << hopCountStats.getMean() << endl;
    //   EV << "Hop count, stddev: " << hopCountStats.getStddev() << endl;

        EV << "Ultimo retardo enlace:     " << retardoenlace << endl;

        EV << "Media retardo end to end   " << retardoeehist.getMean() << endl;

        recordScalar("#retardoendtoend", retardoee);
        recordScalar("#retardoenlace", retardoenlace);

        retardoenlacehist.recordAs("retardoenlace");
        retardoeehist.recordAs("retardoee");
    }

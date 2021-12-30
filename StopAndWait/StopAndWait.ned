simple Usuario
{
    parameters:
        @display("i=block/source");
        double longitudmedia;
    gates:

        output out;
}


simple Cola
{
    parameters:
        @display("i=block/routing");
    gates:
        input in[];
        output out;
}

simple Servidor
{
    parameters:
        @display("i=block/process");
    gates:
        inout cx[];
}

simple Elementosaw
{
    parameters:
        @display("i=block/routing");
    gates:

        inout conexion[];
}

simple Conmutador
{
    parameters:
        @display("i=block/routing");
        double probabilidad;
    gates:
        inout conexion[];
        input in[];
}

module Nodo
{
    parameters:
        double prob;
        int numcx;
        @display("bgb=323,146");
    gates:
        input innodo[];
        inout outnodo[];

    submodules:
        protocolo[numcx]: Elementosaw {
            parameters:
                @display("p=255,70");

            gates:
                conexion[2];
        }
        conm: Conmutador {
            parameters:
                probabilidad = prob;
                @display("p=66,70");
            gates:
                conexion[numcx];
                in[sizeof(innodo)];
        }
    connections:
        
        for i=0..(numcx-1) {
            conm.conexion[i] <--> protocolo[i].conexion[0];
  			outnodo[i] <--> protocolo[i].conexion[1];
        }
        

        for i=0..sizeof(innodo)-1 {
            innodo[i] --> conm.in[i];

        }
        
	//	protocolo[0].conexion[1] <--> outnodo[0];
        


}

//
// Same as Tictoc8.
//
network StopAndWait
{
    @display("bgb=381.6,208.29001");
    types:
        channel Channel extends ned.DatarateChannel
        {
            datarate = 9600000bps;
            delay = 100ms; //tprop, para hacer pruebas de timeout se ha aumentado el valor
            per = 0.5; //packet error rate
        }

    submodules:
        fuente1: Usuario {
            parameters:
                longitudmedia = 9600/3;
                @display("p=25.44,30.210001");
        }
        fuente2: Usuario {
            parameters:
                longitudmedia = 9600/3;
                @display("p=112.89,24.645");
        }
        fuente5: Usuario {
            parameters:
                longitudmedia = 9600/3;
                @display("p=100.965004,173.31");
        }
        nodo1: Nodo {
            parameters:
                @display("i=,cyan;p=100.965004,93.015");
                prob = 1/4;
                numcx = 2;
            gates:
                innodo[1];
                outnodo[2];
        }
        nodo2: Nodo {
            parameters:
                @display("i=,cyan;p=186.825,49.29");
                prob = 1/3;
                numcx = 4;
            gates:
                innodo[1];
                outnodo[4];
        }

        nodo5: Nodo {
            parameters:
                @display("i=,cyan;p=186.03,142.30501");
                prob = 1/2;
                numcx = 3;
            gates:
                innodo[1];
                outnodo[3];
        }
        nodo3: Servidor {
            parameters:
                @display("i=,gold;p=333.105,47.7");
            gates:
                cx[1];

        }
        nodo4: Servidor {
            parameters:
                @display("i=,gold;p=338.67,142.30501");
            gates:
                cx[2];
        }
    connections:
        fuente1.out --> nodo1.innodo[0];
        nodo1.outnodo[0] <--> Channel <--> nodo2.outnodo[3];
        nodo1.outnodo[1] <--> Channel <--> nodo5.outnodo[2];

        fuente2.out --> nodo2.innodo[0];
        nodo2.outnodo[0] <--> Channel <--> nodo3.cx[0];
        nodo2.outnodo[1] <--> Channel <--> nodo4.cx[0];

        fuente5.out --> nodo5.innodo[0];
        nodo5.outnodo[0] <--> Channel <--> nodo2.outnodo[2];
        nodo5.outnodo[1] <--> Channel <--> nodo4.cx[1];


}



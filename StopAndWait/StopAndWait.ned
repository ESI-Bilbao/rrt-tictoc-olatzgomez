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
        input in;
        output out;
}

simple Elementosaw
{
    parameters:
        @display("i=block/routing");
    gates:
        input in[];
        output out[];
}

simple Conmutador
{
    parameters:
        @display("i=block/routing");
    gates:
        input in[];
        output out;
}

module Nodo
{
    gates:
        input in[];
        output out[];

    submodules:
        protocolo: Elementosaw {
            parameters:
            @display("p=106,122");
            gates:
                in[1];
                out[2];
        };
        conm: Conmutador{
            gates:
                in[2];
                out;
        };
    connections:
        in --> conm.in;
        conm.out --> protocolo.in[0];
        protocolo.out[0] --> out[0];
        protocolo.out[1] --> out[1];

}

//
// Same as Tictoc8.
//
network StopAndWait
{
    @display("bgb=304,155");
    types:
        channel Channel extends ned.DatarateChannel
        {
            datarate = 9600000bps;
            delay = 100ms; //tprop, para hacer pruebas de timeout se ha aumentado el valor
            per = 0.5; //packet error rate
        }

    submodules:
        fuente: Usuario {
            parameters:
                longitudmedia = 9600/3;
                @display("p=25.44,30.210001");
        }
        nodo1: Nodo {
            parameters:
                @display("i=,cyan;p=100.965004,61.215");
            gates:
                in;
                out[1];
        }
        nodo2: Nodo {
            parameters:
                @display("i=,cyan;p=174.90001,78.705");
            gates:
                in;
                out[2];
        }
        toc: Servidor {
            parameters:
                @display("i=,gold;p=267.915,78.705");
        }
    connections:
        fuente.out --> nodo1.in[0];
        nodo1.out[0] --> Channel --> nodo2.in[0];
        nodo2.out[1] --> Channel --> nodo1.in[1];
        nodo2.out[0] --> Channel --> toc.in;
        nodo2.in[1] <-- Channel <-- toc.out;

}



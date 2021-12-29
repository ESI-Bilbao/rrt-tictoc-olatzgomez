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
    gates:
        input innodo[];
        inout outnodo[];
		
    submodules:
        protocolo[numcx]: Elementosaw {
            parameters:
            @display("p=106,122");
            
            gates:
                conexion[2];
        };
        conm: Conmutador{
            parameters:
            probabilidad = prob;
            gates:
                conexion[numcx];
                in[sizeof(innodo)];
        };
    connections:
        
        for i=0..sizeof(innodo)-1 {
            innodo[i] --> conm.in[i];
           
       }
       
       for j=0..sizeof(numcx)-1 {
            conm.conexion[j] <--> protocolo[j].conexion[0];
           protocolo[j].conexion[1] <--> outnodo[j];
       }
 

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
                prob = 1/4;
            gates:
                innodo[1];
                outnodo[2];
        }
        nodo2: Nodo {
            parameters:
                @display("i=,cyan;p=100.965004,61.215");
                prob = 1/3;
            gates:
                innodo[3];
                outnodo[2];
        }
        
        nodo5: Nodo {
            parameters:
                @display("i=,cyan;p=100.965004,61.215");
                prob = 1/2;
            gates:
                innodo[2];
                outnodo[2];
        }
        nodo3: Servidor {
            parameters:
                @display("i=,gold;p=267.915,78.705");
            gates:
                cx[1];
                    
        }
        nodo4: Servidor {
            parameters:
                @display("i=,gold;p=267.915,78.705");
            gates:
                cx[2];
        }
    connections:
        fuente.out --> nodo1.in[0];
        nodo1.out[0] --> Channel --> nodo2.in[0];
        nodo2.out[1] --> Channel --> nodo1.in[1];
        nodo2.out[0] --> Channel --> toc.in;
        nodo2.in[1] <-- Channel <-- toc.out;

}



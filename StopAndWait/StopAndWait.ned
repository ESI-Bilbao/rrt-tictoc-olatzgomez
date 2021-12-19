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
        }
        tic: Cola {
            parameters:
                @display("i=,cyan;p=103,56");
            gates:
                in[2];
        }
        toc: Servidor {
            parameters:
                @display("i=,gold;p=233,44");
        }
    connections:
        fuente.out --> tic.in[0];
        tic.out --> Channel --> toc.in;
        tic.in[1] <-- Channel <-- toc.out;

}



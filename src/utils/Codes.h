#ifndef MYSTERIO_SRC_UTILS_CODES_H_
#define MYSTERIO_SRC_UTILS_CODES_H_

#define MAXUAVS 20

//DB CONFIG
#define HOSTNAME   "127.0.0.1"
#define USERNAME   "root"
#define PASSWORD   "root"
#define DATABASE   "mestrado"

enum CodeMessage {
    //STATUS
    SOLICITAR_LOCALIZACAO = 10,
    SOLICITAR_VELOCIDADE,

    //Tarefas

    //UAV STATUS
    RESPONDER_LOCALIZACAO = 30,
    RESPONDER_VELOCIDADE


};

#endif

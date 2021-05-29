#include "RepositoryMySQL.h"
#include "../../src/utils/Codes.h"
#include "../../src/utils/Coordinate.h"
#include <stdio.h>
#include <string>
#include <string.h>

RepositoryMySQL::RepositoryMySQL() {
    this->createConnection();
}

bool RepositoryMySQL::createConnection(){
    mysql_init(&connection);
    if (mysql_real_connect(&connection,HOSTNAME,USERNAME,PASSWORD,DATABASE,0,NULL,0)){
        printf("Conectado com o banco de dados com sucesso!\n");
    }else{
        printf("Falha na conexão com o banco de dados!\n");
        if (mysql_errno(&connection))
            printf("Erro %d : %s\n", mysql_errno(&connection), mysql_error(&connection));
    }
}

void RepositoryMySQL::saveStatusInformation(int params){

}

int RepositoryMySQL::requestStatusInformation(){
    MYSQL_RES *resp;
    MYSQL_ROW linhas;
    MYSQL_FIELD *campos;
    char query[]="SELECT * FROM uav_history;";
    int conta; //Contador comum
    if (mysql_query(&connection,query))
        printf("Erro: %s\n",mysql_error(&connection));
    else{
        resp = mysql_store_result(&connection);//recebe a consulta
        if (resp) { //se houver consulta
            //passa os dados dos campos para a variável campos
            //escreve na tela os nomes dos campos dando
            //um tab somente
            campos = mysql_fetch_fields(resp);
            for (conta=0;conta<mysql_num_fields(resp);conta++) {
                printf("%s",(campos[conta]).name);
                if (mysql_num_fields(resp)>1)
                    printf("\t");
            }
            printf("\n");
            //enquanto retonrnar registros, conta até o
            //número de colunas que a tabela tem e escreve na
            //tela com um tab, depois pula a linha e tenta
            //pegar outro registro
            while ((linhas=mysql_fetch_row(resp)) != NULL){
                for (conta=0;conta<mysql_num_fields(resp);conta++)
                    printf("%s\t",linhas[conta]);
                printf("\n");
            }
        }
        mysql_free_result(resp);//limpa a variável do resultado: resp
    }
}

void RepositoryMySQL::destroyConnection(){
    mysql_close(&connection);
    printf("Conexão finalizada com o banco de dados!\n");
}

Coordinate RepositoryMySQL::getUAVLocation(int idUAV){
    MYSQL_RES *resp;
    MYSQL_ROW linhas;
    MYSQL_FIELD *campos;
    char query[]="";
    std::string txt = "SELECT * FROM uav_location where id_uav = '" + std::to_string(idUAV)+"';";
    strcpy(query, txt.c_str());

    int conta;
    if (mysql_query(&connection,query))
        printf("Erro: %s\n",mysql_error(&connection));
    else{
        resp = mysql_store_result(&connection);
        if (resp) {
            campos = mysql_fetch_fields(resp);
            for (conta=0;conta<mysql_num_fields(resp);conta++) {
                printf("%s",(campos[conta]).name);
                if (mysql_num_fields(resp)>1)
                    printf("\t");
            }
            printf("\n");
            while ((linhas=mysql_fetch_row(resp)) != NULL){
                for (conta=0;conta<mysql_num_fields(resp);conta++)
                    printf("%s\t",linhas[conta]);
                printf("\n");
            }
        }
        mysql_free_result(resp);
    }

    Coordinate c;
    return c;
}

void RepositoryMySQL::setUAVLocation(int idUAV, Coordinate coord){

    char snd[150];
    std::string txt = "INSERT INTO uav_location(id_uav, location_x, location_y, location_z, time) values('" + std::to_string(idUAV);
    txt += "', '" + std::to_string(coord.getX());
    txt += "', '" + std::to_string(coord.getY());
    txt += "', '" + std::to_string(coord.getZ());
    txt += "', '" + std::to_string(1) + "');";
    strcpy(snd, txt.c_str());

    int res;
    res = mysql_query(&connection,snd);

    if (!res)
        printf("Registro inserido %llu\n", mysql_affected_rows(&connection));
    else
        printf("Erro na inserção %d : %s\n", mysql_errno(&connection), mysql_error(&connection));
}

double RepositoryMySQL::getVelocity(int idUAV){
    MYSQL_RES *resp;
    MYSQL_ROW linhas;
    MYSQL_FIELD *campos;
    char query[]="SELECT * FROM uav_velocity where id_uav = '1';";
    int conta;
    if (mysql_query(&connection,query))
        printf("Erro: %s\n",mysql_error(&connection));
    else{
        resp = mysql_store_result(&connection);
        if (resp) {
            campos = mysql_fetch_fields(resp);
            for (conta=0;conta<mysql_num_fields(resp);conta++) {
                printf("%s",(campos[conta]).name);
                if (mysql_num_fields(resp)>1)
                    printf("\t");
            }
            printf("\n");
            while ((linhas=mysql_fetch_row(resp)) != NULL){
                for (conta=0;conta<mysql_num_fields(resp);conta++)
                    printf("%s\t",linhas[conta]);
                printf("\n");
            }
        }
        mysql_free_result(resp);
    }

    double c;
    return c;
}

void RepositoryMySQL::setVelocity(int idUAV, double velocity){
    char snd[150];
    std::string txt = "INSERT INTO uav_velocity(id_uav, velocity, time) values('" + std::to_string(idUAV);
    txt += "', '" + std::to_string(velocity);
    txt += "', '" + std::to_string(1) + "');";
    strcpy(snd, txt.c_str());

    int res;
    res = mysql_query(&connection,snd);

    if (!res)
        printf("Registro inserido %llu\n", mysql_affected_rows(&connection));
    else
        printf("Erro na inserção %d : %s\n", mysql_errno(&connection), mysql_error(&connection));
}

float RepositoryMySQL::getBattery(int idUAV){
    MYSQL_RES *resp;
    MYSQL_ROW linhas;
    MYSQL_FIELD *campos;
    char query[]="SELECT * FROM uav_battery where id_uav = '1';";
    int conta;
    if (mysql_query(&connection,query))
        printf("Erro: %s\n",mysql_error(&connection));
    else{
        resp = mysql_store_result(&connection);
        if (resp) {
            campos = mysql_fetch_fields(resp);
            for (conta=0;conta<mysql_num_fields(resp);conta++) {
                printf("%s",(campos[conta]).name);
                if (mysql_num_fields(resp)>1)
                    printf("\t");
            }
            printf("\n");
            while ((linhas=mysql_fetch_row(resp)) != NULL){
                for (conta=0;conta<mysql_num_fields(resp);conta++)
                    printf("%s\t",linhas[conta]);
                printf("\n");
            }
        }
        mysql_free_result(resp);
    }

    float c;
    return c;
}

void RepositoryMySQL::setBattery(int idUAV, float battery){
    char snd[150];
    std::string txt = "INSERT INTO uav_battery(id_uav, battery, time) values('" + std::to_string(idUAV);
    txt += "', '" + std::to_string(battery);
    txt += "', '" + std::to_string(1) + "');";
    strcpy(snd, txt.c_str());

    int res;
    res = mysql_query(&connection,snd);

    if (!res)
        printf("Registro inserido %llu\n", mysql_affected_rows(&connection));
    else
        printf("Erro na inserção %d : %s\n", mysql_errno(&connection), mysql_error(&connection));
}

int RepositoryMySQL::getFlightTime(int idUAV){
    MYSQL_RES *resp;
    MYSQL_ROW linhas;
    MYSQL_FIELD *campos;
    char query[]="SELECT * FROM uav_flight_time where id_uav = '1';";
    int conta;
    if (mysql_query(&connection,query))
        printf("Erro: %s\n",mysql_error(&connection));
    else{
        resp = mysql_store_result(&connection);
        if (resp) {
            campos = mysql_fetch_fields(resp);
            for (conta=0;conta<mysql_num_fields(resp);conta++) {
                printf("%s",(campos[conta]).name);
                if (mysql_num_fields(resp)>1)
                    printf("\t");
            }
            printf("\n");
            while ((linhas=mysql_fetch_row(resp)) != NULL){
                for (conta=0;conta<mysql_num_fields(resp);conta++)
                    printf("%s\t",linhas[conta]);
                printf("\n");
            }
        }
        mysql_free_result(resp);
    }

    int c;
    return c;
}

void RepositoryMySQL::setFlightTime(int idUAV, int flightTime){
    char snd[150];
    std::string txt = "INSERT INTO uav_flight_time(id_uav, flight_time, time) values('" + std::to_string(idUAV);
    txt += "', '" + std::to_string(flightTime);
    txt += "', '" + std::to_string(1) + "');";
    strcpy(snd, txt.c_str());

    int res;
    res = mysql_query(&connection,snd);

    if (!res)
        printf("Registro inserido %llu\n", mysql_affected_rows(&connection));
    else
        printf("Erro na inserção %d : %s\n", mysql_errno(&connection), mysql_error(&connection));
}

RepositoryMySQL::~RepositoryMySQL() { }

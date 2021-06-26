#include "RepositoryMySQL.h"
#include "../../src/utils/Coordinate.h"
#include <stdio.h>
#include <string>
#include <string.h>

#include "../../src/utils/Codes.h"

RepositoryMySQL::RepositoryMySQL() {
    this->createConnection();
}

//Reconnect nos tratamentos de exceção

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

void RepositoryMySQL::destroyConnection(){
    mysql_close(&connection);
    printf("Conexão finalizada com o banco de dados!\n");
}

Coordinate RepositoryMySQL::requestUAVLocation(int idUAV){
    MYSQL_RES *resp;
    MYSQL_ROW linhas;
    MYSQL_FIELD *campos;
    char query[180];
    std::string txt = "SELECT * FROM uav_location where id_uav = '" + std::to_string(idUAV)+"' ORDER BY id desc LIMIT 1;";
    strcpy(query, txt.c_str());
    int conta;
    Coordinate c;
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
                c.setX(std::stof(linhas[0]));
                c.setY(std::stof(linhas[1]));
                c.setZ(std::stof(linhas[2]));
                for (conta=0;conta<mysql_num_fields(resp);conta++)
                    printf("%s\t",linhas[conta]);
                printf("\n");
            }
        }
        mysql_free_result(resp);
    }

    return c;
}

void RepositoryMySQL::saveUAVLocation(int idUAV, Coordinate coord){

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
    char query[180];
    std::string txt = "SELECT * FROM uav_velocity where id_uav = '" + std::to_string(idUAV)+"' ORDER BY id desc LIMIT 1;";
    strcpy(query, txt.c_str());
    int conta;
    double velocity;
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
                velocity = std::stod(linhas[0]);
                for (conta=0;conta<mysql_num_fields(resp);conta++)
                    printf("%lf\t",std::stod(linhas[conta]));
                printf("\n");
            }
        }
        mysql_free_result(resp);
    }

    return velocity;
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
    char query[180];
    std::string txt = "SELECT * FROM uav_battery where id_uav = '" + std::to_string(idUAV)+"' ORDER BY id desc LIMIT 1;";
    strcpy(query, txt.c_str());
    int conta;
    float battery;
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
                battery = std::stof(linhas[0]);
                for (conta=0;conta<mysql_num_fields(resp);conta++)
                    printf("%s\t",linhas[conta]);
                printf("\n");
            }
        }
        mysql_free_result(resp);
    }

    return battery;
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
    char query[180];
    std::string txt = "SELECT * FROM uav_flight_time where id_uav = '" + std::to_string(idUAV)+"' ORDER BY id desc LIMIT 1;";
    strcpy(query, txt.c_str());
    int conta;
    int flightTime;
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
                flightTime = std::stoi(linhas[0]);
                for (conta=0;conta<mysql_num_fields(resp);conta++)
                    printf("%s\t",linhas[conta]);
                printf("\n");
            }
        }
        mysql_free_result(resp);
    }

    return flightTime;
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

RepositoryMySQL::~RepositoryMySQL() { this->destroyConnection(); }

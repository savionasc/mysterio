#include "RepositoryMySQL.h"
#include "../../src/utils/Coordinate.h"
#include <ctime>
#include <iostream>
#include <string.h>

#define HOSTNAME "127.0.0.1"
#define USERNAME "root"
#define PASSWORD "root"
#define DATABASE "mestrado"

RepositoryMySQL::RepositoryMySQL() {
    this->createConnection();
}

RepositoryMySQL::RepositoryMySQL(bool prints) {
    this->prints = prints;
    this->createConnection();
}

//Reconnect nos tratamentos de exceção

//Depois trocar printf por cout

bool RepositoryMySQL::createConnection(){
    mysql_init(&connection);
    if (mysql_real_connect(&connection,HOSTNAME,USERNAME,PASSWORD,DATABASE,0,NULL,0)){
        if(this->prints)
            std::cout << "[DB] Conectado com o banco de dados com sucesso!" << std::endl;
    }else{
        if(this->prints){
            std::cout << "[DB] Falha na conexão com o banco de dados!" << std::endl;
            if (mysql_errno(&connection))
                std::cout << "[DB] Erro " << mysql_errno(&connection) << " : " << mysql_error(&connection) << std::endl;
        }
    }
}

void RepositoryMySQL::destroyConnection(){
    mysql_close(&connection);
    if(this->prints)
        std::cout << "[DB] Conexão finalizada com o banco de dados!" << std::endl;
}

int RepositoryMySQL::createExecutionID(){
    char datatime[150];
    std::string txt = "INSERT INTO execution(datatime) values('" + getCurrentDate() + "');";
    strcpy(datatime, txt.c_str());

    int res;
    res = mysql_query(&connection,datatime);

    if(this->prints){
        if (!res)
            std::cout << "[DB] Registro inserido " << mysql_affected_rows(&connection) << std::endl;
        else
            std::cout << "[DB] Erro na inserção " << mysql_errno(&connection) << " : " << mysql_error(&connection) << std::endl;
    }

    return this->getExecutionID();
}

int RepositoryMySQL::getExecutionID(){
    MYSQL_RES *resp;
    MYSQL_ROW linhas;
    MYSQL_FIELD *campos;
    char query[180];
    std::string txt = "SELECT * FROM execution ORDER BY id desc LIMIT 1;";
    strcpy(query, txt.c_str());
    int conta;
    int exID;
    if (mysql_query(&connection,query))
        printf("[DB] Erro: %s\n",mysql_error(&connection));
    else{
        resp = mysql_store_result(&connection);
        if (resp) {
            campos = mysql_fetch_fields(resp);
            //for (conta=0;conta<mysql_num_fields(resp);conta++) {
            //    printf("%s",(campos[conta]).name);
            //    if (mysql_num_fields(resp)>1)
            //        printf("\t");
            //}
            //printf("\n[DB] ");
            while ((linhas=mysql_fetch_row(resp)) != NULL){
                exID = std::stoi(linhas[0]);
                //for (conta=0;conta<mysql_num_fields(resp);conta++)
                //    printf("%lf\t",std::stod(linhas[conta]));
                //printf("\n");
            }
        }
        mysql_free_result(resp);
    }

    return exID;
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
        if(this->prints)
            std::cout << "Erro: " << mysql_error(&connection)<< std::endl;
    else{
        resp = mysql_store_result(&connection);
        if (resp) {
            campos = mysql_fetch_fields(resp);
            for (conta=0;conta<mysql_num_fields(resp);conta++) {
                printf("%s",(campos[conta]).name);
                if (mysql_num_fields(resp)>1)
                    printf("\t");
            }
            printf("\n[DB] ");
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

    if(this->prints){
        if (!res)
            std::cout << "[DB] Registro inserido " << mysql_affected_rows(&connection) << std::endl;
        else
            std::cout << "[DB] Erro na inserção " << mysql_errno(&connection) << " : " << mysql_error(&connection) << std::endl;
    }
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
        printf("[DB] Erro: %s\n",mysql_error(&connection));
    else{
        resp = mysql_store_result(&connection);
        if (resp) {
            campos = mysql_fetch_fields(resp);
            for (conta=0;conta<mysql_num_fields(resp);conta++) {
                printf("%s",(campos[conta]).name);
                if (mysql_num_fields(resp)>1)
                    printf("\t");
            }
            printf("\n[DB] ");
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
        printf("[DB] Registro inserido %llu\n", mysql_affected_rows(&connection));
    else
        printf("[DB] Erro na inserção %d : %s\n", mysql_errno(&connection), mysql_error(&connection));
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
            printf("\n[DB] ");
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
        printf("[DB] Registro inserido %llu\n", mysql_affected_rows(&connection));
    else
        printf("[DB] Erro na inserção %d : %s\n", mysql_errno(&connection), mysql_error(&connection));
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
            printf("\n[DB] ");
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
        printf("[DB] Registro inserido %llu\n", mysql_affected_rows(&connection));
    else
        printf("[DB] Erro na inserção %d : %s\n", mysql_errno(&connection), mysql_error(&connection));
}

void RepositoryMySQL::enablePrints(){ this->prints = true; }
void RepositoryMySQL::disablePrints(){ this->prints = false; }

RepositoryMySQL::~RepositoryMySQL() { this->destroyConnection(); }
std::string RepositoryMySQL::getCurrentDate(){
    time_t timetoday;
    time(&timetoday);
    return std::string(asctime(localtime(&timetoday)));
}

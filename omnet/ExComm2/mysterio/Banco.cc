#include "../../ExComm2/mysterio/Banco.h"

#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>
#include <string>
#include <iostream>


namespace inet {

Banco::Banco() { }

Banco::~Banco() { }

void Banco::testeConexao(){
    MYSQL conexao;

    mysql_init(&conexao);
    if ( mysql_real_connect(&conexao, "localhost", "root", "root", "mestrado", 0, NULL, 0) ){
          printf("conectado com sucesso!\n");
          mysql_close(&conexao);
    }else{
          printf("Falha de conexao\n");
          printf("Erro %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));
    }
}

void Banco::consultarDadosDoBanco(){
    MYSQL conexao;
    MYSQL_RES *resp;
    MYSQL_ROW linhas;
    MYSQL_FIELD *campos;
    char query[]="SELECT * FROM uav_history;";
    int conta; //Contador comum

    mysql_init(&conexao);
    if (mysql_real_connect(&conexao,HOST,USER,PASS,DB,0,NULL,0)){
        printf("Conectado com Sucesso!\n");
        if (mysql_query(&conexao,query))
            printf("Erro: %s\n",mysql_error(&conexao));
        else{
            resp = mysql_store_result(&conexao);//recebe a consulta
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
        mysql_close(&conexao);
    }else{
        printf("Conexao Falhou\n");
        if (mysql_errno(&conexao))
        printf("Erro %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));
    }

}

void Banco::inserirDadosNoBanco(){
    MYSQL conexao;
    int res;

    mysql_init(&conexao);
    if ( mysql_real_connect(&conexao, HOST, USER, PASS, DB, 0, NULL, 0) ){
    //if ( mysql_real_connect(&conexao, "localhost", "root", "root", "mestrado", 0, NULL, 0) ){
     printf("conectado com sucesso!\n");

     res = mysql_query(&conexao,"INSERT INTO aprendendo(id_uav, location_x, location_y, location_z) values('1', '1', '2', '3');");

     if (!res)
        printf("Registros inseridos %llu\n", mysql_affected_rows(&conexao));
     else
        printf("Erro na inserção %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));

     mysql_close(&conexao);
    }else{
     printf("Falha de conexao\n");
     printf("Erro %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));
    }
}
void Banco::insertLocation(Coordinate coord, const char idUAV){
    MYSQL conexao;
        int res;

        mysql_init(&conexao);
        if ( mysql_real_connect(&conexao, HOST, USER, PASS, DB, 0, NULL, 0) ){
            printf("conectado com sucesso!\n");
            char result[200];   // array to hold the result.

            const char *query = "INSERT INTO uav_history(id_uav, location_x, location_y, location_z, time) values('";

            strcpy(result,query);
            std::string s = std::to_string(idUAV);
            strcat( result, s.c_str() );
            strcat(result, "','");
            s = std::to_string(coord.x);
            strcat( result, s.c_str() );
            strcat(result, "','");
            s = std::to_string(coord.y);
            strcat( result, s.c_str() );
            strcat(result, "','");
            s = std::to_string(coord.z);
            strcat( result, s.c_str());
            strcat(result, "', 'H');");
            //printf("SSSSSSSSS: %s", result);

         res = mysql_query(&conexao, result);

         if (!res)
            printf("Registros inseridos %llu\n", mysql_affected_rows(&conexao));
         else
            printf("Erro na inserção %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));

         mysql_close(&conexao);
        }else{
         printf("Falha de conexao\n");
         printf("Erro %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));
        }
}
void Banco::alterarDadosDoBanco(){
    MYSQL conexao;
    int res;

    mysql_init(&conexao);
    if ( mysql_real_connect(&conexao, HOST, USER, PASS, DB, 0, NULL, 0) ){
     printf("Conectado com sucesso!\n");

     res = mysql_query(&conexao,"UPDATE uav_history SET location_x = '1' WHERE id_uav = '1';");

     if (!res)
        printf("Registros alterados %llu\n", mysql_affected_rows(&conexao));
     else
        printf("Erro na alteração %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));

     mysql_close(&conexao);
    }else{
     printf("Falha de conexao\n");
     printf("Erro %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));
    }
}

void Banco::deletarDadosDoBanco(){
    MYSQL conexao;
    int res;

    mysql_init(&conexao);
    if ( mysql_real_connect(&conexao, HOST, USER, PASS, DB, 0, NULL, 0) ){
        printf("Conectado com sucesso!\n");

        res = mysql_query(&conexao,"DELETE FROM uav_history WHERE id_uav = '1';");

        if (!res)
        printf("Registros deletados %llu\n", mysql_affected_rows(&conexao));
        else
        printf("Erro na deleção %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));

        mysql_close(&conexao);
    }else{
        printf("Falha de conexao\n");
        printf("Erro %d : %s\n", mysql_errno(&conexao), mysql_error(&conexao));
    }
}

}

#include "RepositoryMySQL.h"
#include "../../src/utils/Codes.h"
#include <stdio.h>

RepositoryMySQL::RepositoryMySQL() { }

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

RepositoryMySQL::~RepositoryMySQL() { }

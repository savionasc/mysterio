#include "RepositoryMySQL.h"
#include "../../src/utils/Codes.h"
#include <stdio.h>

RepositoryMySQL::RepositoryMySQL() { }

bool RepositoryMySQL::createConnection(){
    MYSQL_RES *resp;
    MYSQL_ROW linhas;
    MYSQL_FIELD *campos;
    char query[]="SELECT * FROM uav_history;";
    int conta; //Contador comum

    mysql_init(&conexao);
    if (mysql_real_connect(&conexao,HOSTNAME,USERNAME,PASSWORD,DATABASE,0,NULL,0)){
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

void RepositoryMySQL::saveStatusInformation(int params){

}

int RepositoryMySQL::requestStatusInformation(){

}

void RepositoryMySQL::destroyConnection(){

}

RepositoryMySQL::~RepositoryMySQL() { }

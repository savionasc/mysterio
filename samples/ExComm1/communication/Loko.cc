#include "Loko.h"
#include <stdio.h>

Loko::Loko() { }

Loko::~Loko() { }

void Loko::onMessageReceive(int a){
    printf("Entrou!\n");

    if(a == 0){
        printf("Igual a zero!\n");
    }else{
        printf("Diferente de zero!\n");
    }

}

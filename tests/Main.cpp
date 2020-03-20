//
// Created by Athanasios Filippidis on 2/25/20.
//
#include <iostream>
#include "./include/Tuple.h"
//using namespace std;

int main(void){
    Tuple *t1 = new Tuple(1,2);
    t1->PrintTuple();
    t1->SetKey(3);
    t1->SetValue(4);
    t1->PrintTuple();
}
#ifndef STRUCTSANDFUNCTIONS
#define STRUCTSANDFUNCTIONS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*Constantes*/

#define MAXNAME 1024
#define MAXEMAIL 512
#define MAXPHONE 64
#define MAXINPUT MAXNAME+ MAXEMAIL+ MAXPHONE
#define HASHSIZE 1013
#define HASHDOMAINSIZE 503

/*Lista Duplamente ligada*/

typedef struct node {
    char* name;
    char* email;
    char* phone;
    char* domain;
    char* workmail;
    char* workdomain;
    struct node* next;
    struct node* previous;
    struct node* bucket_hash_next;
     
}node;

typedef struct list {
    node* head;
    node* last;
    node** hashtable;
}list;

/*Estrutura para Hash Table do dominio*/

typedef struct domainstruct{
    struct domainstruct* bucket_domain_next;
    char *domain_copy;
    int domaincounter;
}domainstruct;


/*Declaracoes de Prototipos*/
list* add_person(list* l,domainstruct** domaintable,char *argv);
list* delete_person(list* l,domainstruct** domaintable,char* argv);
void seek_person(list* l,char* argv);
list* change_email(list* l,domainstruct** domaintable,char* argv);
void howmany_domain(domainstruct** domaintable,char* argv);
void destroylist(list* l);
list* makelist();
void printlist(list* l);
node* seek_person_hash(list* l,char* argv);
unsigned int calculate_hash(char *v);
domainstruct** make_domain_table();
void add_domain_table(domainstruct** domaintable,char*token);
unsigned int calculate_domain_hash(char *v);
void remove_domain_table(domainstruct** domaintable,char* token);
int return_counter_domain(domainstruct** domaintable,char* token);
void destroy_domain_hash(domainstruct** domaintable);
void seek_person_phone(list*l,char*argv);
#endif
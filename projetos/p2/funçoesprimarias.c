#include "structsANDfunctions.h"



/*-------------------------------------------*/
/*             Funcoes Primarias             */
/*-------------------------------------------*/

/* Funcao primaria que recebe a lista, a tabela de dominio e 
todas as caracteristicas do contacto, e consoante as condicoes 
insere e retorna a lista ou emite erro devido as mesmas.*/

list* add_person(list* l,domainstruct** domaintable,char *argv){
    int index;
    char *token;
    node* new_node;
    token=strtok(argv," ");
    
    if(seek_person_hash(l,token)!=NULL) {
        printf("Nome existente.\n");
        return l;
    }
    /*aloco memoria para o novo contacto a ser inserido
    juntamente com cada caracteristica individualmente*/
   
    new_node=malloc(sizeof(node));


    new_node->name=malloc((strlen(token)+1)*sizeof(char));
    strcpy(new_node->name,token);
    
    /*fica com a parte antes do arroba*/
    token=strtok(NULL,"@");
    
    new_node->email=malloc((strlen(token)+1)*sizeof(char));
    strcpy(new_node->email,token);


    token=strtok(NULL," ");
    new_node->domain=malloc((strlen(token)+1)*sizeof(char));
    strcpy(new_node->domain,token);
    
    /*incremento contador do dominio*/
    add_domain_table(domaintable,token);
    
    token=strtok(NULL," ");
    new_node->phone=malloc((strlen(token)+1)*sizeof(char));
    strcpy(new_node->phone,token);

    token=strtok(NULL,"@");
    new_node->workmail=malloc((strlen(token)+1)*sizeof(char));
    strcpy(new_node->workmail,token);

    token=strtok(NULL," ");
    new_node->workdomain=malloc((strlen(token)+1)*sizeof(char));
    strcpy(new_node->workdomain,token);
    
    add_domain_table(domaintable,token);
    
    /*calculo a hash para o nome*/
    index=calculate_hash(new_node->name);
    /*caso de o indice estar vazio*/
    if(*(l->hashtable+index) == NULL){
        *(l->hashtable+index)=new_node;/*fica como head naquele index*/
        new_node->bucket_hash_next=NULL;
    }
    
    /*caso de o indice ja tiver um ou mais nomes diferentes mas com 
    a mesma hash,o novo passa a ser a cabeca no indice. */
    else{
        new_node->bucket_hash_next=*(l->hashtable+index);
        *(l->hashtable+index)=new_node;

    }
    
    /*Acrescenta no fim da lista*/
    new_node->previous=l->last;
    new_node->next= NULL;
    l->last=new_node;
    /*caso de ser o primeiro elemento*/
    if(l->head == NULL){
        l->head= new_node;
    }
    if(new_node != l->head){
        new_node->previous->next= new_node;
    }
   
    
    return l;
    
}

/* Funcao primaria que recebe a lista, a tabela do dominio e nome do contacto
a remover, e consoante as condicoes, ou permite remover o contacto e 
decrementar o contador ou emite erro.*/ 

list* delete_person(list* l,domainstruct** domaintable,char* argv){
    char *token;
    int index;
    node* auxiliar;
    node* auxiliar_hash;
    token=strtok(argv,"\n");
    
    if ((auxiliar = seek_person_hash(l,token))) {
        remove_domain_table(domaintable,auxiliar->domain);
        /*caso de ser a cabeça mas nao unico*/
        if( auxiliar->next != NULL && auxiliar->previous== NULL){
            l->head=auxiliar->next;
            auxiliar->next->previous= NULL;

        }
        /*caso de ser a cabeça e unico*/
        else if(auxiliar->next ==NULL && auxiliar->previous==NULL){
            l->head= NULL;
            l->last= NULL;
        }
        /*caso de se encontrar la no meio*/
        else if(auxiliar->next != NULL && auxiliar->previous != NULL ){
            auxiliar->next->previous=auxiliar->previous;
            auxiliar->previous->next=auxiliar->next;
        }
        /*caso de ser o ultimo elemento e nao unico*/
        else if(auxiliar->next == NULL  && auxiliar->previous != NULL){
            auxiliar->previous->next= NULL;
            l->last=auxiliar->previous;
        }

        index=calculate_hash(token);
        auxiliar_hash=*(l->hashtable+index);
        /*caso de ser a head do bucket*/
        if (strcmp(auxiliar_hash->name,token)==0){
            *(l->hashtable+index)=auxiliar_hash->bucket_hash_next;
        }
        else{
            for(auxiliar_hash=*(l->hashtable+index);strcmp(auxiliar_hash->name,token) != 0;auxiliar_hash=auxiliar_hash->bucket_hash_next);
            if(auxiliar->bucket_hash_next !=NULL){
                auxiliar_hash->next=auxiliar_hash->bucket_hash_next->bucket_hash_next;
            }
        }
        free(auxiliar->workdomain);
        free(auxiliar->workmail);
        free(auxiliar->phone);
        free(auxiliar->domain);
        free(auxiliar->email);
        free(auxiliar->name);
        free(auxiliar);
        return l;
        
    }
    printf("Nome inexistente.\n");
        
    return l;
}

/* Funcao primaria que recebe a lista e imprime 
todos os contatos pela ordem que foram inseridos.*/

void printlist(list* l){
    node* auxiliar;
    auxiliar=l->head;
    
    while(auxiliar != NULL){
        printf("%s %s@%s %s",auxiliar->name,auxiliar->email,auxiliar->domain,auxiliar->phone);
        auxiliar=auxiliar->next;
    }
    return;
}

/* Funcao primaria que recebe a lista e um nome como 
argumentos e retorna as caracteristicas do contacto
caso este exista na lista, senao emite erro.*/

void seek_person(list* l ,char*argv){
    char* token;
    node* auxiliar;
    token=strtok(argv,"\n");

    if((auxiliar=seek_person_hash(l,token))!=0){
        printf("%s %s@%s %s",auxiliar->name,auxiliar->email,auxiliar->domain,auxiliar->phone);
            return;
    }
    printf("Nome inexistente.\n");
    return;
}

/* Funcao primaria que recebe a tabela dos dominios
e um dominio e imprime o numero de vezes que o 
dominio se encontra na lista.*/

void howmany_domain(domainstruct** domaintable, char* argv){
    char*token;
    token=strtok(argv,"\n");
    
    printf("%s:%d\n",token,return_counter_domain(domaintable,token));
    return;

}

/* Funcao primaria que recebe a lista, a tabela dos dominios
e o novo email por inseir e so emite erro caso o contacto nao
exista.*/

list* change_email(list* l,domainstruct** domaintable,char* argv){
    char* token;
    node* auxiliar;
    token=strtok(argv," ");

    if((auxiliar=seek_person_hash(l,token))!=0){
        
        
        /*decremento o contador do dominio antigo*/
        remove_domain_table(domaintable,auxiliar->domain);
        
        /*liberto a memoria alocada da string antiga do domino*/
        free(auxiliar->domain);
        
        /*liberto a memoria alocada para a string antiga do email*/
        free(auxiliar->email);
        
        /*separo o novo email a introduzir,alocando memoria para 
        cada caracteristica individualmente*/
        token=strtok(NULL,"@");
        auxiliar->email=malloc((strlen(token)+1)*sizeof(char));
        strcpy(auxiliar->email,token);
        
        token=strtok(NULL,"\n");
        auxiliar->domain=malloc((strlen(token)+1)*sizeof(char));
        strcpy(auxiliar->domain,token);
        
        /*incremento o contador do novo dominio inserido*/
        add_domain_table(domaintable,token);
        return l;

    }
    printf("Nome inexistente.\n");
    return l;
}

void seek_person_phone(list* l , char* argv){
    char *token;
    node*auxiliar;
    token=strtok(argv," ");
    auxiliar=l->head;
    while(strcmp(auxiliar->phone,token) !=0){
        auxiliar=auxiliar->next;
        continue;
    }
    printf("%s %s@%s %s",auxiliar->name,auxiliar->email,auxiliar->domain,auxiliar->phone);
    return;

}
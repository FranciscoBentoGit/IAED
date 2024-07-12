#include "structsANDfunctions.h"

/*-------------------------------------------*/
/*             Funcoes Auxiliares            */
/*-------------------------------------------*/


/*Funcao auxiliar que calcula o indice de um
determinado nome do contacto e retorna um numero inteiro
maior ou igual a 0. */ 

unsigned int calculate_hash(char *v){
    int h=0,a=31415,b=27183;
    
    for(h=0;*v != '\0';v++,a=a*b%(HASHSIZE-1)){
        h=(a*h + *v) % HASHSIZE;
    }
    return h;
}

/*Funcao auxiliar que calcula o indice de um
determinado dominio do contacto e retorna um numero inteiro
maior ou igual a 0. */ 

unsigned int calculate_domain_hash(char *v){
    int h=0,a=31415,b=27183;
    
    for(h=0;*v != '\0';v++,a=a*b%(HASHDOMAINSIZE-1)){
        h=(a*h + *v) % HASHDOMAINSIZE;
    }
    return h;
}

/* Funcao auxiliar que nao recebe nada como input e 
retorna a lista que será utilizada para gravar os 
contactos juntamente com a sua Hash Table.*/

list* makelist(){
    int i;
    list* l=malloc(sizeof(list));
    l->head=NULL;
    l->last=NULL;
    
    /*aloco espaço para toda a Hash Table por criar*/
    l->hashtable = malloc(sizeof(node)*HASHSIZE);
    
    /*coloca-se todos os indices a NULL ao inicio*/
    for(i=0;i<HASHSIZE;i++){
        *(l->hashtable+i)= NULL;

    }
    return l;
}

/* Funcao auxiliar que nao recebe nada como input e retorna 
a Hash Table destinada para o dominio, uma vez que nao se
consegues associar diretamente uma variavel inteira ao dominio.*/

domainstruct** make_domain_table(){
    int j;
    /*aloca-se espaço para toda a Hash Table do dominio*/
    domainstruct **domaintable=malloc(sizeof(domainstruct)*HASHDOMAINSIZE);
    
    /*coloca-se todos os indices a NULL ao inicio*/
    for (j=0;j<HASHDOMAINSIZE;j++){
        *(domaintable +j)=NULL;
    }
    return domaintable;
}

/* Funcao auxiliar que recebe como argumentos a tabela do dominio e o dominio
e procura adiciona-lo consoante a sua hash calculada, incrementando
juntamente o contador daquele dominio se ja la estiver ou 
inicializa-lo. */

void add_domain_table(domainstruct** domaintable,char*token){
    domainstruct* pointer;
    int domain_index=calculate_domain_hash(token);
    
    /*Ciclo que so pode ser interrompido pelo ponteiro ser NULL
    ou o dominio a ser inserido corresponder a um dominio ja existente
    no determinado indice da tabela calculado previamente*/
    for(pointer=*(domaintable + domain_index);pointer != NULL && strcmp(token,pointer->domain_copy)!=0;pointer=pointer->bucket_domain_next);
    
    /*Caso do dominio ainda nao existir, insere-se alocando memoria para o mesmo
    e para a string dominio e inicializa-se a variavel de contagem a 1. */
    if (pointer == NULL){
        pointer=malloc(sizeof(domainstruct));
        pointer->domain_copy=malloc((strlen(token)+1)*sizeof(char));
        strcpy(pointer->domain_copy,token);
        pointer->domaincounter=1;
        pointer->bucket_domain_next=*(domaintable + domain_index);
        *(domaintable + domain_index)=pointer;
        return;
    }
    
    /*Caso do dominio já estar inserido*/
    if(pointer != NULL && strcmp(token,pointer->domain_copy)==0){
        pointer->domaincounter ++;
        return;
    }
}

/* Funcao auxiliar que recebe como argumentos a tabela do dominio 
e o dominio e procura decrementar o contador desse dominio. */

void remove_domain_table(domainstruct** domaintable,char* token){
    domainstruct* pointer,*aux;
    int domain_index=calculate_domain_hash(token);
    
    /*Ciclo que so pode ser interrompido pelo ponteiro ser NULL
    ou o dominio a ser inserido corresponder a um dominio ja existente
    no determinado indice da tabela calculado previamente*/
 for(pointer=*(domaintable + domain_index);pointer != NULL ;pointer=pointer->bucket_domain_next){
        if( strcmp(pointer->domain_copy,token)==0){
            if(pointer->domaincounter>1){
                pointer->domaincounter--;
                return;
            }
            if(pointer->domaincounter==1){
                aux=*(domaintable + domain_index);
                if (strcmp(aux->domain_copy,token)==0 && aux->bucket_domain_next==NULL){
                    *(domaintable+ domain_index)=aux->bucket_domain_next;
                    free(aux->domain_copy);
                    free(aux);
                    return;
                }
                if (strcmp(aux->domain_copy,token)==0 && aux->bucket_domain_next!=NULL){
                    *(domaintable+ domain_index)=aux->bucket_domain_next;
                    free(aux->domain_copy);
                    return;
                }
                else{
                    for(;strcmp(aux->domain_copy,token)!=0;aux=aux->bucket_domain_next);
                    aux->bucket_domain_next=aux->bucket_domain_next->bucket_domain_next;
                    free(aux->domain_copy);
                    return;
                }
            }
        }
    }
}

/* Funcao auxiliar que recebe a tabela do dominio e o dominio
e retorna o contador daquele dominio,a 0 se este nao existir na
tabela. */

int return_counter_domain(domainstruct** domaintable,char* token){
    domainstruct* pointer;
    int domain_index=calculate_domain_hash(token);
    
    for(pointer=*(domaintable + domain_index);pointer != NULL && strcmp(token,pointer->domain_copy) != 0;pointer=pointer->bucket_domain_next);
    if(pointer == NULL){
        return 0;
    }
   
    return pointer->domaincounter;

}

/* Funcao auxiliar que recebe a lista e liberta toda a memoria
alocada para cada caracteristica do contacto, o contacto em si,
a lista e hash table criada para procura mais eficaz de um contacto. */

void destroylist(list* l){
    while(l->head != NULL){
        /*metemos um  ponteiro auxiliar no seguinte a cabeça*/
        node* temp=l->head->next;
        free(l->head->phone);
        free(l->head->domain);
        free(l->head->email);
        free(l->head->name);
        free(l->head);
        
        /*avançamos o ponteiro para o seguinte*/
        l->head=temp;
    }
    free(l->hashtable);
    free(l);
}
/*Funcao auxiliar que recebe a Hash Table do dominio
e liberta toda a memoria alocada para cada dominio;*/

void destroy_domain_hash(domainstruct** domaintable){
    int i;
    domainstruct* temp,* aux_temp;
    
    for(i=0;i<HASHDOMAINSIZE;i++){
        temp=*(domaintable+i);
        while(temp != NULL){
            /*uso uma variavel temporaria para ficar a apontar para o next do pointer*/
            aux_temp=temp->bucket_domain_next;
            free(temp->domain_copy);
            free(temp);
            /*avançamos para o dominio seguinte naquele indice da tabela*/
            temp=aux_temp;
        }
    }
    free(domaintable);
}

/* Funcao auxiliar que recebe a lista e o nome
e retorna o indice da tabela onde se encontra o
especifico contacto.*/

node* seek_person_hash(list* l,char* token){
    node* auxiliar;
    int index_aux;
    index_aux=calculate_hash(token);
    
    for(auxiliar= *(l->hashtable+index_aux);auxiliar!=NULL && strcmp(auxiliar->name,token)!= 0;auxiliar=auxiliar->bucket_hash_next);
    return auxiliar;
}

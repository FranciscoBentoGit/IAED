/*------------------Projeto 2 IAED------------------*/
/*                                                  */
/*          Francisco Bento- 93581 LETI             */
/*                                                  */
/*--------------------------------------------------*/

/*Bibliotecas*/

#include "structsANDfunctions.h"





/* Funcao que recebe um contacto como input, separa o seu comando do conteudo 
em si e permite executar uma determinada funcao dependendo do comando.*/

int main(){
    char argv[MAXINPUT];
    char command;
    
    list* l = makelist();
    domainstruct** domaintable=make_domain_table();
    while((command = getchar()) != 'x'){
        if(command != 'l'){
            getchar();
        }
        fgets(argv, MAXINPUT , stdin);
        switch (command) {
            case 'a':
                add_person(l,domaintable, argv);
                break;
            case 'l':
                printlist(l);
                break;
            case 'p':
                seek_person(l,argv);
                break;
            case 'r':
                delete_person(l,domaintable,argv);
                break;
            case 'e':
                change_email(l,domaintable,argv);
                break;
            case 'c':
                howmany_domain(domaintable,argv);
                break;
            case 'P':
                seek_person_phone(l,argv);
                break;
        }

    }
    destroy_domain_hash(domaintable);
    destroylist(l);
   
    return 0;
}
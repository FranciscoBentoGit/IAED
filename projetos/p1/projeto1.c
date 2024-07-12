/*------------------Projeto 1 IAED------------------*/
/*                                                  */
/*          Francisco Bento- 93581 LETI             */
/*                                                  */
/*--------------------------------------------------*/

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Constantes*/
#define MAXSALAS 11
#define MAXEVENTOSPORSALA 100
#define STRMAX 64
#define DURACAOMAX 1440
#define MAXINPUT 10
#define MAXPART 4
#define MAXSTRDATA 8
#define MAXINICIO 4
#define MINPART 2
#define TOTAL 1000

/*Estruturas de Dados*/
typedef struct {
  char descricao[STRMAX];
  int data;
  int inicio;
  int duracao;
  int numero_sala;
  char responsavel[STRMAX];
  char lista_participantes[MAXPART][STRMAX];
  char stringdata[MAXSTRDATA];
  char stringinicio[MAXINICIO];
  int nr_participantes;
  int horas, minutos, ano, mes, dia, hora_comeco, comeco_em_minutos, fim;
  unsigned long int total;
} evento;

evento tabela[MAXSALAS][MAXEVENTOSPORSALA]; /* Vetor onde sao guardados os elementos do tipo evento*/
int j=0;/*variavel global que ajuda a contar o numero de participantes inseridos*/       
int conta_eventos_por_sala[MAXSALAS] = {0,0,0,0,0,0,0,0,0,0}; /*inicializa todos contadores das salas a 0*/

/*Prototipos*/
void adiciona_evento(char input[10][STRMAX]);
void listagem();
void lista_sala(char input[10][STRMAX]);
void apaga_evento(char input[10][STRMAX]);
void altera_inicio(char input[10][STRMAX]);
void altera_duracao(char input[10][STRMAX]);
void altera_sala(char input[10][STRMAX]);
void adiciona_par(char input[10][STRMAX]);
void apaga_par(char input[10][STRMAX]);
void evento_mais_longo();
/*Codigo*/


/* Funcao que recebe um evento como input, separa o seu comando do evento 
em si e permite executar uma determinada funcao dependendo do comando.*/

int main() {
  int i;
  char str[342]; /* numero de caracteres maximo que pudemos receber no input*/
  char comando, k, input[MAXINPUT][STRMAX];
  char *token;
  comando = getchar();
  while (comando != 'x') {
    i = 0;
    j = 0;
    if (comando != 'l' && comando != 'Z') {
    getchar();
    }
    while ((k = getchar()) != '\n') {
      str[i] = k;
      i++;
    }
    str[i] = '\0';

    token = strtok(str, ":");
    while (token != NULL) {
      strcpy(input[j], token);
      token = strtok(NULL, ":");
      j++;
    }
     
    switch (comando) {
    case 'a':
      adiciona_evento(input);
      break;
    case 'l':
      listagem();
      break;
    case 's':
      lista_sala(input);
      break;
    case 'r':
      apaga_evento(input);
      break;
    case 'i':
      altera_inicio(input);
      break;
    case 't':
      altera_duracao(input);
      break;
    case 'm':
      altera_sala(input);
      break;
    case 'A':
      adiciona_par(input);
      break;
    case 'Z':
      evento_mais_longo();
      break; 
    case 'R':
      apaga_par(input);
      break;
    }
    comando = getchar();
  }
  return 0;
  }


/* Funcao que recebe todo o tipo de  argumentos(descricao:data:inicio:duracao:sala:
responsavel:participantes), e consoante as condicoes, podera adicionar um evento,
ou emitir uma mensagem de erro consoante o mesmo(conflito de horarios,salas ocupadas).
Output:Nada exceto erro. */ 

void adiciona_evento(char input[10][STRMAX]) {
  int i, e, sala_real, b, c, d, repete_erro;
  /*Toda esta variavel evento_aux serve para guardar temporiamente o evento que
    poderemos vir a colocar, no caso de respeitar todas as condicoes
    necessarias*/
  evento evento_aux;
  repete_erro=0;
  strcpy(evento_aux.descricao, input[0]);
  strcpy(evento_aux.stringdata, input[1]);
  evento_aux.data = atoi(input[1]);
  evento_aux.inicio = atoi(input[2]);
  evento_aux.duracao = atoi(input[3]);
  evento_aux.numero_sala = atoi(input[4]);
  for (i = 5; i < j; i++) {
    strcpy(evento_aux.lista_participantes[i - 5], input[i]);
    continue;
    }
  evento_aux.nr_participantes =i - 5; /*numero de participantes,inclusive o responsavel*/
  evento_aux.ano = evento_aux.data % 10000; /*ano do evento*/
  evento_aux.mes = (evento_aux.data / 10000) % 100; /*mes do evento*/
  evento_aux.dia = (evento_aux.data / 10000) / 100; /*dia do evento*/
  evento_aux.hora_comeco = evento_aux.inicio / 100; /*hora de comeco*/
  evento_aux.minutos = evento_aux.inicio % 100; /*minutos da hora de comeco*/
  evento_aux.comeco_em_minutos =60 * evento_aux.hora_comeco +evento_aux.minutos; /*comeco colocado em minutos*/
  evento_aux.fim = evento_aux.comeco_em_minutos + evento_aux.duracao; /*fim em minutos*/
  evento_aux.total=  (evento_aux.ano*365*24*60) +(evento_aux.mes*60*24*30)+(evento_aux.dia*24*60);
  sala_real = evento_aux.numero_sala;
  /*ciclo que serve apenas para comparar o inicio em
  minutos,assumindo que os participantes nao causam
  problemas*/
  for (e = 0; e < conta_eventos_por_sala[sala_real-1]; e++) {
    if (strcmp(tabela[sala_real-1][e].stringdata, evento_aux.stringdata) ==0){ 
      if ((evento_aux.comeco_em_minutos <=tabela[sala_real-1][e].comeco_em_minutos &&
          tabela[sala_real-1][e].comeco_em_minutos <evento_aux.fim) ||
          (evento_aux.comeco_em_minutos < tabela[sala_real-1][e].fim && 
          tabela[sala_real-1][e].fim <= evento_aux.fim) ||
          (tabela[sala_real-1][e].comeco_em_minutos <=evento_aux.comeco_em_minutos && 
          evento_aux.comeco_em_minutos < tabela[sala_real-1][e].fim)) {
          printf("Impossivel agendar evento %s. Sala%d ocupada.\n",evento_aux.descricao, evento_aux.numero_sala);
          return;
      }
    }
  }
  for (b = 0; b < MAXSALAS; b++) {
    if (b == sala_real - 1) {
      continue;
    }
    for (e = 0; e < conta_eventos_por_sala[b]; e++) {
      if (strcmp(tabela[b][e].stringdata, evento_aux.stringdata) == 0) {
        if (((evento_aux.comeco_em_minutos <= tabela[b][e].comeco_em_minutos &&
            tabela[b][e].comeco_em_minutos < evento_aux.fim) ||
            (evento_aux.comeco_em_minutos < tabela[b][e].fim &&
            tabela[b][e].fim <= evento_aux.fim) ||
            (tabela[b][e].comeco_em_minutos <= evento_aux.comeco_em_minutos && 
            evento_aux.comeco_em_minutos < tabela[b][e].fim))) {
          for (c = 0; c < evento_aux.nr_participantes; c++) {
            for (d = 0; d < tabela[b][e].nr_participantes; d++) {
              if (strcmp(evento_aux.lista_participantes[c],tabela[b][e].lista_participantes[d]) == 0) {
                printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n",
                evento_aux.descricao, evento_aux.lista_participantes[c]);
                repete_erro = 1;
              }
            }
          }
        }
      }
    }
  }
  if (repete_erro == 1) {
    return;
  }
  tabela[sala_real - 1][conta_eventos_por_sala[sala_real - 1]] = evento_aux;
  conta_eventos_por_sala[sala_real - 1]++;
}


/* Funcao que recebe uma nova hora de começo para um certo evento 
e dependendo se as condicoes assim o permitem, podera ou nao emitir
erro(conflito de horarios,salas ocupadas ou eventos nao existentes).
Output: Nada exceto erro. */ 

void altera_inicio(char input[10][STRMAX]) {
  evento evento_aux;
  int a, b, c, d, e, f, g, novo_inicio, repete_erro;
  char descricao_input[STRMAX];
  repete_erro=0;
  strcpy(descricao_input, input[0]);
  novo_inicio = atoi(input[1]);
  strcpy(evento_aux.descricao, descricao_input);
  for (a = 0; a < MAXSALAS; a++) {
    for (b = 0; b < conta_eventos_por_sala[a]; b++) {
      if (strcmp(evento_aux.descricao, tabela[a][b].descricao) == 0) {
        evento_aux = tabela[a][b]; /*copia do evento com a mesma descricao,porem alteramos o inicio*/
        evento_aux.hora_comeco = novo_inicio / 100;
        evento_aux.minutos = novo_inicio % 100;
        evento_aux.comeco_em_minutos =evento_aux.hora_comeco * 60 + evento_aux.minutos;
        evento_aux.fim = evento_aux.comeco_em_minutos + evento_aux.duracao;
        evento_aux.inicio=novo_inicio;
          /*ciclo que serve apenas para comparar o inicio em
          minutos,assumindo que os participantes nao causam
          problemas*/
        for (c = 0; c < conta_eventos_por_sala[a];c++) { 
          if (strcmp(evento_aux.descricao, tabela[a][c].descricao) == 0) {
            continue;
          }
          if (strcmp(tabela[a][c].stringdata, evento_aux.stringdata) == 0) {
            if (((evento_aux.comeco_em_minutos <=
                      tabela[a][c].comeco_em_minutos &&
                  tabela[a][c].comeco_em_minutos < evento_aux.fim) ||
                 (evento_aux.comeco_em_minutos < tabela[a][c].fim &&
                  tabela[a][c].fim <= evento_aux.fim) ||
                 (tabela[a][c].comeco_em_minutos <
                      evento_aux.comeco_em_minutos &&
                  evento_aux.comeco_em_minutos < tabela[a][c].fim))) {
              printf("Impossivel agendar evento %s. Sala%d ocupada.\n",
                     evento_aux.descricao, evento_aux.numero_sala);
              return;
            }
          }
        }
        /*neste ciclo ja vai comparar com os participantes*/
        for (d = 0; d < MAXSALAS;d++) { 
          if (a == d) {
            continue; /* so no caso de estarmos na mesma sala em que a descricao e igual*/
          }
          for (e = 0; e < conta_eventos_por_sala[d]; e++) {
            if (strcmp(evento_aux.descricao, tabela[d][e].descricao) == 0) {
              continue;
            }
            if (strcmp(tabela[d][e].stringdata, evento_aux.stringdata) == 0) {
              if  (((evento_aux.comeco_em_minutos <=
                  tabela[d][e].comeco_em_minutos &&
                  tabela[d][e].comeco_em_minutos < evento_aux.fim) ||
                  (evento_aux.comeco_em_minutos < tabela[d][e].fim &&
                  tabela[d][e].fim <= evento_aux.fim) ||
                  (tabela[d][e].comeco_em_minutos <
                  evento_aux.comeco_em_minutos &&
                  evento_aux.comeco_em_minutos < tabela[d][e].fim))) {
                  /* No caso de um dos participantes(por ordem)
                  tambem for igual a um dos participantes que
                  esteja nas salas=f e evento=g,imprime erro.*/
                for (f = 0; f < evento_aux.nr_participantes; f++) {
                  for (g = 0; g < tabela[d][e].nr_participantes; g++) {
                    if (strcmp(evento_aux.lista_participantes[f],
                               tabela[d][e].lista_participantes[g]) == 0) { 
                      printf("Impossivel agendar evento %s. Participante %s "
                             "tem um evento sobreposto.\n",evento_aux.descricao,
                             evento_aux.lista_participantes[f]);
                      /*variavel local que permite imprimir erro para cada 
                      participante com horas em conflito.*/              
                      repete_erro = 1;
                    }
                  }
                }
              }
            }
          }
        }
        if (repete_erro ==1) { 
          return;
        }
        tabela[a][b] = evento_aux;
        return;
      }
    }
  }
  printf("Evento %s inexistente.\n", descricao_input);
}


/* Funcao que recebe uma nova duracao para um certo evento 
e dependendo se as condicoes assim o permitem, podera ou nao emitir
erro(conflito de horarios,salas ocupadas ou eventos nao existentes).
Output: Nada exceto erro. */ 

void altera_duracao(char input[10][STRMAX]) {
  evento evento_aux;
  int a, b, c, d, e, f, g, nova_duracao, repete_erro;
  char descricao_input[STRMAX];
  repete_erro=0;
  strcpy(descricao_input, input[0]);
  nova_duracao = atoi(input[1]);
  strcpy(evento_aux.descricao, descricao_input);
  for (a = 0; a < MAXSALAS; a++) {
    for (b = 0; b < conta_eventos_por_sala[a]; b++) {
      if (strcmp(evento_aux.descricao, tabela[a][b].descricao) == 0) {
        /*copia do evento com a mesma descricao,
        porem alteramos a duracao.*/
        evento_aux = tabela[a][b]; 
        evento_aux.hora_comeco = evento_aux.inicio / 100;
        evento_aux.minutos = evento_aux.inicio % 100;
        evento_aux.comeco_em_minutos =evento_aux.hora_comeco * 60 + evento_aux.minutos;
        evento_aux.fim = evento_aux.comeco_em_minutos + nova_duracao;
        evento_aux.duracao=nova_duracao;
        for (c = 0; c < conta_eventos_por_sala[a];c++) {
                     
          if (strcmp(evento_aux.descricao, tabela[a][c].descricao) == 0) {
            continue;
          }
          if (strcmp(tabela[a][c].stringdata, evento_aux.stringdata) == 0) {
            if  (((evento_aux.comeco_em_minutos <=
                tabela[a][c].comeco_em_minutos &&
                tabela[a][c].comeco_em_minutos < evento_aux.fim) ||
                (evento_aux.comeco_em_minutos < tabela[a][c].fim &&
                tabela[a][c].fim <= evento_aux.fim) ||
                (tabela[a][c].comeco_em_minutos <
                evento_aux.comeco_em_minutos &&
                evento_aux.comeco_em_minutos < tabela[a][c].fim))) {
              printf("Impossivel agendar evento %s. Sala%d ocupada.\n",
                     evento_aux.descricao, evento_aux.numero_sala);
              return;
            }
          }
        }
        for (d = 0; d < MAXSALAS;d++) { 
          if (a == d) {
            continue; 
          }
          for (e = 0; e < conta_eventos_por_sala[d]; e++) {
            if (strcmp(evento_aux.descricao, tabela[d][e].descricao) == 0) {
              continue;
            }
            if (strcmp(tabela[d][e].stringdata, evento_aux.stringdata) == 0) {
              if    (((evento_aux.comeco_em_minutos <=
                    tabela[d][e].comeco_em_minutos &&
                    tabela[d][e].comeco_em_minutos < evento_aux.fim) ||
                    (evento_aux.comeco_em_minutos < tabela[d][e].fim &&
                    tabela[d][e].fim <= evento_aux.fim) ||
                    (tabela[d][e].comeco_em_minutos <
                    evento_aux.comeco_em_minutos &&
                    evento_aux.comeco_em_minutos < tabela[d][e].fim))) {
                      for (f = 0; f < evento_aux.nr_participantes; f++) {
                  for (g = 0; g < tabela[d][e].nr_participantes; g++) {
                    if (strcmp(evento_aux.lista_participantes[f],
                      tabela[d][e].lista_participantes[g]) == 0) { 
                      printf("Impossivel agendar evento %s. Participante %s "
                          "tem um evento sobreposto.\n",
                          evento_aux.descricao,
                          evento_aux.lista_participantes[f]);
                      repete_erro = 1;
                    }
                  }
                }
              }
            }
          }
        }
        if (repete_erro ==1) { 
          return;
        }
        tabela[a][b] = evento_aux;
        return;
      }
    }
  }
  printf("Evento %s inexistente.\n", descricao_input);
}

/* Funcao que recebe a descricao de um certo evento e apaga-lo
da lista a nao ser que o proprio nao contenha.
Output: Nada exceto erro. */ 

void apaga_evento(char input[10][STRMAX]) {
  int a, b;
  char descricao_input[STRMAX];
  strcpy(descricao_input, input[0]);

  for (a = 0; a < MAXSALAS; a++) {
    for (b = 0; b < conta_eventos_por_sala[a]; b++) {
      /*aqui faz-se uma copia do ultimo elemento para cima do evento que queremos eliminar,
      destruindo esse.Agora basta elminar o ultimo elemento.*/
      if (strcmp(descricao_input, tabela[a][b].descricao) == 0) {
          tabela[a][b] =tabela[a][conta_eventos_por_sala[a]-1]; 
          conta_eventos_por_sala[a]--; /*a respetiva sala fica com menos um elemento, o ultimo*/
          return;
      }
    }
  }
  printf("Evento %s inexistente.\n", descricao_input);
}


/* Funcao que recebe uma descricao e um numero de uma sala para
onde se pretende deslocar o evento, emite erro no caso de
o evento ser inexistente ou a sala estar ocupada.
Output: Nada exceto erro. */  

void altera_sala(char input[10][STRMAX]) {
  evento evento_aux;
  int a, b, c, nova_sala;
  char descricao_input[STRMAX];
  strcpy(descricao_input, input[0]);
  nova_sala = atoi(input[1]);
  for (a = 0; a < MAXSALAS; a++) {
    for (b = 0; b < conta_eventos_por_sala[a]; b++) {
      if (strcmp(descricao_input, tabela[a][b].descricao) == 0) {
        /*copia do evento com a mesma descricao,
        porem alteramos a sala.*/
        evento_aux = tabela[a][b];
        evento_aux.numero_sala = nova_sala;
        /*se a sala for a mesma, apenas decrementa os contadores,
        pois ele  adicinado e retirado*/
        if (nova_sala == tabela[a][b].numero_sala) {
          return;
        }
        for (c = 0; c < conta_eventos_por_sala[nova_sala-1]; c++) {
          if (strcmp(tabela[nova_sala-1][c].stringdata, evento_aux.stringdata) == 0) { 
            if (((evento_aux.comeco_em_minutos <=
                tabela[nova_sala-1][c].comeco_em_minutos &&
                tabela[nova_sala-1][c].comeco_em_minutos < evento_aux.fim) ||
                (tabela[nova_sala-1][c].comeco_em_minutos <
                evento_aux.comeco_em_minutos &&
                evento_aux.comeco_em_minutos < tabela[nova_sala-1][c].fim) ||
                (evento_aux.comeco_em_minutos <
                tabela[nova_sala-1][c].comeco_em_minutos &&
                tabela[nova_sala-1][c].fim <= evento_aux.fim))) {

                printf("Impossivel agendar evento %s. Sala%d ocupada.\n",
                descricao_input, nova_sala);
                return;
            }
          }
        } 
       
        /*a respetiva sala tem mais um evento,na posicao seguinte
        o proximo elemento e adicionado no fim .*/
        
        tabela[nova_sala-1][conta_eventos_por_sala[nova_sala-1]] = evento_aux;
        conta_eventos_por_sala[nova_sala-1]++;
        /*elimina o evento que vamos mudar, copiado o ultimo para la
        ,os contadores sao decrementados.*/
        tabela[a][b]=tabela[a][conta_eventos_por_sala[a]-1];
        conta_eventos_por_sala[a]--;
        return;
        
     }
    }
  }
  printf("Evento %s inexistente.\n", descricao_input);
}


/* Funcao que recebe uma descricao e um participante por ser 
adicionado,dependo das situacoes(evento nao existente,Evento
cheio com 3 participantes,conflito de horarios ou participante
ja reside no evento).
Output: Nada exceto erro. */    

void adiciona_par(char input[10][STRMAX]) {
  evento evento_aux;
  int a, b,c,e,d,f;
  char descricao_input[STRMAX];
  char novo_par[STRMAX];
  strcpy(descricao_input, input[0]);
  strcpy(novo_par, input[1]);
  for (a = 0; a < MAXSALAS; a++) {
    for (b = 0; b < conta_eventos_por_sala[a]; b++) {
      if (strcmp(descricao_input, tabela[a][b].descricao) == 0) {
        evento_aux = tabela[a][b];
            if (evento_aux.nr_participantes != MAXPART) {/*se o numero de participantes
              for 4,automaticamente emite a mensagem de erro:evento cheio.*/
                 for(e=0;e<evento_aux.nr_participantes;e++){
                    if (strcmp(evento_aux.lista_participantes[e],novo_par)==0){
                      return;
                      /*no caso de se encontrar na lista,nada se faz*/
                      }
                   }
                    /*Havendo a possibilidade de ser inserido, vamos
                    agora testar os conflitos com horarios.*/
                   for(d=0;d<MAXSALAS-1;d++){
                     for(f=0;f<conta_eventos_por_sala[d];f++){
                        if (strcmp(tabela[d][f].stringdata, evento_aux.stringdata) == 0) {
                            for(c=0;c<tabela[d][f].nr_participantes;c++){
                               if(strcmp(novo_par,tabela[d][f].lista_participantes[c])==0){
                                 
                                  if  (((evento_aux.comeco_em_minutos <=
                                    tabela[d][f].comeco_em_minutos &&
                                    tabela[d][f].comeco_em_minutos < evento_aux.fim) ||
                                    (evento_aux.comeco_em_minutos < tabela[d][f].fim &&
                                    tabela[d][f].fim <= evento_aux.fim) ||
                                    (tabela[d][f].comeco_em_minutos <
                                    evento_aux.comeco_em_minutos &&
                                    evento_aux.comeco_em_minutos < tabela[d][f].fim))){
                                        printf("Impossivel adicionar participante. Participante %s tem "
                                        "um evento sobreposto.\n",novo_par);
                                        return;
                                    }
                          
                                    }
                              }
                          }
                      }
                    }
                      /*Ha permissao para ser adicionado.*/
                      if (evento_aux.nr_participantes == 2) {
                      strcpy(tabela[a][b].lista_participantes[2], novo_par);
                      tabela[a][b].nr_participantes++;
                      return;
                    }
                    else if (evento_aux.nr_participantes == 3) {
                      strcpy(tabela[a][b].lista_participantes[3], novo_par);
                      tabela[a][b].nr_participantes++;
                      return;
                  
                  }
               }
          printf("Impossivel adicionar participante. Evento %s ja tem 3 "
          "participantes.\n",descricao_input);
          return;
        }
       
      }
    }
    printf("Evento %s inexistente.\n", descricao_input);
}


/* Funcao que recebe uma descricao e um nome de um participante por 
remover, este sera removido a nao ser que respeite as condicoes(unico
participante no evento, evento nao existente ou participante nao se encontra
na lista do evento).
Output: Nada exceto erro. */ 

void apaga_par(char input[10][STRMAX]){
  int a, b,c,d,e,f;
  char descricao_input[STRMAX], delete_par[STRMAX];
  strcpy(descricao_input, input[0]);
  strcpy(delete_par, input[1]);
  for (a = 0; a < MAXSALAS; a++){
    for (b = 0; b < conta_eventos_por_sala[a]; b++){
      if (strcmp(descricao_input, tabela[a][b].descricao) == 0) {
        /*se apenas tiver 1 participante, nao posso remover
        origina erro,*/
        if (tabela[a][b].nr_participantes != MINPART){
          for(c=0;c<MAXPART;c++){
             if(strcmp(delete_par,tabela[a][b].lista_participantes[c])!=0){
               continue;
             }
          }
          /*caso de eliminar participantes quando o evento tem 2.*/
          if(tabela[a][b].nr_participantes==MAXPART-1){
            for(e=1;e<tabela[a][b].nr_participantes;e++){
              if(strcmp(tabela[a][b].lista_participantes[e],delete_par)==0){
                if(e==1){
                  /*troco o participante da ultima posicao com o da penultima eliminado-o ao mesmo tempo*/
                  strcpy(tabela[a][b].lista_participantes[e],tabela[a][b].lista_participantes[e+1]);
                  tabela[a][b].nr_participantes--;
                  return;
                }
                else if(e==2){
                  tabela[a][b].nr_participantes--;
                  return;
                }
              }
            }
          }
          /*caso de eliminar participantes quando o evento tem 3.*/
          else if(tabela[a][b].nr_participantes==MAXPART){
            for(d=1;d<tabela[a][b].nr_participantes;d++){
              if(strcmp(tabela[a][b].lista_participantes[d],delete_par)==0){
                if(d==1){
                  strcpy(tabela[a][b].lista_participantes[d],tabela[a][b].lista_participantes[d+1]);
                  strcpy(tabela[a][b].lista_participantes[d+1],tabela[a][b].lista_participantes[d+2]);
                  tabela[a][b].nr_participantes--;
                  return;
                }
                if(d==2){
                  strcpy(tabela[a][b].lista_participantes[d],tabela[a][b].lista_participantes[d+1]);
                  tabela[a][b].nr_participantes--;
                  return;
                }
                else if(d==3){
                  tabela[a][b].nr_participantes--;
                  return;
                }
              }
            }
          }
        }
        for(f=1;f<tabela[a][b].nr_participantes;f++){
          if(strcmp(tabela[a][b].lista_participantes[f],delete_par)==0){
            printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n",delete_par,descricao_input);
            return;
          }
          return;
        }
        
      }
    }
  }
  printf("Evento %s inexistente.\n", descricao_input);
}

/* Funcao que recebe o numero de uma das salas e emite a lista
por ordem cronologica de eventos da mesma.
Output: <descricao> <data> <inicio> <duracao> Sala<sala> <responsavel>
         <participante1> <participante2> <participante3>. */ 

void lista_sala(char input[10][STRMAX]){
  evento evento_aux;
  int a,b,i,sala,c;
  sala=atoi(input[0]);
   /*Para cada evento que haja, comparo a data cronologica
    se o da primeira for maior, troco de posicao com o seguinte
    de modo a acabar ultimamente o primeiro elemento como aquele
    com menor cronologia.*/
  for(a=0;a<conta_eventos_por_sala[sala-1]-1;a++){
    for(c=0;c<conta_eventos_por_sala[sala-1]-a-1;c++){
        /*Aplico os criterios de ordenacao necessarios para a cronologia.
        Algoritmo de Bubble Sort.*/
      if(tabela[sala-1][c].total>tabela[sala-1][c+1].total){
        evento_aux=tabela[sala-1][c];
        tabela[sala-1][c]=tabela[sala-1][c+1];
        tabela[sala-1][c+1]=evento_aux;
      }
      else if(tabela[sala-1][c].total==tabela[sala-1][c+1].total){
        if(tabela[sala-1][c].comeco_em_minutos>tabela[sala-1][c+1].comeco_em_minutos){
          evento_aux=tabela[sala-1][c];
          tabela[sala-1][c]=tabela[sala-1][c+1];
          tabela[sala-1][c+1]=evento_aux;
        }
      }
    }
  }
  for(b=0;b<conta_eventos_por_sala[sala-1];b++){
      printf("%s %.8d %.4d %d Sala%d %s\n*", tabela[sala-1][b].descricao,
      tabela[sala-1][b].data, tabela[sala-1][b].inicio, tabela[sala-1][b].duracao,
      tabela[sala-1][b].numero_sala, tabela[sala-1][b].lista_participantes[0]);
      for(i=1;i<tabela[sala-1][b].nr_participantes;i++){
          printf(" %s",tabela[sala-1][b].lista_participantes[i]);
        }
       printf("\n");
      }
}


/* Funcao que recebe nada como argumento e emite a lista por ordem 
cronologica de eventos, no caso de haver varios eventos a mesma hora,
imprime por ordem crescente de salas.
Output: <descricao> <data> <inicio> <duracao> Sala<sala> <responsavel>
        <participante1> <participante2> <participante3>. */ 

void listagem(){
  evento evento_aux,vetor[TOTAL];
  int i,k,f,g,x,y,contador;
  contador=0,i=0;
  /*copio para um vetor unidimensional
  todos os eventos de todas as salas, de
  modo a iterar e organizar mais facilmente.*/
  for(f=0;f<MAXSALAS;f++){
    for(g=0;g<conta_eventos_por_sala[f];g++){
      vetor[contador]=tabela[f][g];
      contador++;
    }

  }
  /*Aplico os criterios de ordenacao necessarios para a cronologia.
   Algoritmo de Bubble Sort.*/
  for(x=0;x<contador-1;x++){
    for(y=0;y<contador-x-1;y++){
      if(vetor[y].total>vetor[y+1].total){
        evento_aux=vetor[y];
        vetor[y]=vetor[y+1];
        vetor[y+1]=evento_aux;
      }
      else if(vetor[y].total==vetor[y+1].total){
        if(vetor[y].comeco_em_minutos>vetor[y+1].comeco_em_minutos){
          evento_aux=vetor[y];
          vetor[y]=vetor[y+1];
          vetor[y+1]=evento_aux;
        } 
        else if(vetor[y].comeco_em_minutos==vetor[y+1].comeco_em_minutos){
            if(vetor[y].numero_sala>vetor[y+1].numero_sala){
              evento_aux=vetor[y];
              vetor[y]=vetor[y+1];
              vetor[y+1]=evento_aux;
           }
         }
       }
     }
   }
  for(i=0;i<contador;i++){
    printf("%s %.08d %.04d %d Sala%d %s\n",vetor[i].descricao,vetor[i].data,
    vetor[i].inicio,vetor[i].duracao,vetor[i].numero_sala,vetor[i].lista_participantes[0]);
    printf("*");
    for(k=1;k<vetor[i].nr_participantes;k++){
      printf(" %s",vetor[i].lista_participantes[k]);
    }
    printf("\n");
  }
}


void evento_mais_longo(){
  int a,b;
  int auxiliar;
  int maior=0;
  for(a=0;a<MAXSALAS;a++){
    for(b=0;conta_eventos_por_sala[a-1];b++){
        auxiliar=tabela[a][b].duracao;
        if (auxiliar>maior){
          maior=tabela[a][b].duracao;
        }
    }
  }
  printf("%d\n",maior);
  return;
}
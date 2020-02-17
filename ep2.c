/* ************************************************************************** */
/* Nome: Giovana Oshiro da Silva                                              */
/* Numero USP: 8022103                                                        */
/*                                                                            */
/* Nome: Lucas Freitas Bastos                                                 */
/* Numero USP: 9783118                                                        */
/*                                                                            */
/* Exercício-programa 2                                                       */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

/* A estrutura dicio abaixo armazena um vetor de char e um ponteiro tipo dicio
que guarda o endereço da próxima celula_dic, definida pela estrutura dicio.
Essa estrutura vai ser criada, em uma lista encadeada, para armazenar apenas as
palavras do dicionário que possuem o mesmo tamanho que a palavra de entrada. */

struct dicio{
  char palavra [MAX];
  struct dicio *prox;
};

typedef struct dicio celula_dic;

/* A estrutura registro, que define o tipo-de-dados celula, armazena um vetor de
char e dois ponteiros tipo registro que guardam dois endereços diferentes. Essa
estrutura vai ser criada para que seja feita uma busca em largura em uma árvore. */

struct registro{
  char palavra [MAX];
  struct registro *pai, *prox;
};

typedef struct registro celula;

/* A função insere coloca uma palavra do dicionário entre duas células da lista
encadeada. Ela recebe uma palavra lida do dicionário e o endereço da primeira
célula da lista. */

void insere (char v [], celula_dic *p){
   celula_dic *nova;
   nova = malloc (sizeof(celula_dic));
   strcpy (nova->palavra, v);
   nova->prox = p->prox;
   p->prox = nova;
}

/* A função *criadicionario lê o dicionário e coloca apenas as palavras de mesmo
tamanho que a palavra de entrada em uma lista encadeada, utilizando a função
insere. A palavra de entrada não é inserida no dicionário para que ela não seja
repetida na cadeia do word ladder. Ela retorna um ponteiro que guarda o endereço
da primeira célula da lista. Essa lista possui cabeça (para ser mais fácil de
remover um elemento dela quando for preciso). A palavra mais próxima à cabeça é
a última do dicionário, ou seja, ele está na ordem inversa à alfabética. */

celula_dic *cria_dicionario (int n, char palavra_inicial []){
  FILE *dicionario;
  int leitura = 0, i;
  char v [MAX], caractere;
  celula_dic *p;

  p = malloc (sizeof(celula_dic));
  p->prox = NULL;

  dicionario = fopen ("dicionario.txt", "r");

  while (leitura != EOF){
    leitura = fscanf (dicionario, "%c", &caractere);

    for (i = 0; caractere != '\n' && leitura != EOF; i++){
      v [i] = caractere;
      leitura = fscanf (dicionario, "%c", &caractere);
    }
    v [i] = '\0';

    if (i == n && strcmp (palavra_inicial, v) != 0) insere (v, p);
                    /* a palavra de entrada não é colocada no dicionário, assim
                     como todas as palavras adjacentes serão removidas da lista
                     ao serem encontradas posteriormente */
  }

  fclose (dicionario);
  return p;
}

/* A função remove_palavra remove uma palavra da lista encadeada que contém o
dicionário. Ela recebe o endereço da primeira celula_dic do dicionário e a
palavra que se deseja excluir.
A função declara dois ponteiros tipo celula_dic, *aux e *lixo, de forma que *aux
vai sempre estar atrás de *lixo ao percorrer a lista encadeada. Pensamos isso
dessa forma porque a função remove dada em aula remove apenas elementos que
estejam entre duas células, nunca o primeiro ou o último elemento da lista. */

void remove_palavra (celula_dic *p, char palavra[]){
  celula_dic *aux, *lixo;
  aux = p;
  lixo = p->prox;

  while (strcmp (lixo->palavra, palavra) != 0){
    aux = aux->prox;
    lixo = lixo->prox;
  }

  aux->prox = lixo->prox;
  free (lixo);
}

/* A função adjacencia recebe duas palavras de mesmo tamanho e o tamanho delas,
compara cada um de seus caracteres e retorna se elas são adjacentes ou não, isto
é, se diferem em apenas um caractere. Se elas forem adjacentes, a função retorna
0. */

int adjacencia (char x[], char y[], int n){
  int j, i;
  j = -1;

  for (i = 0; i < n; i++){
    if (x[i] != y[i]) j++;
  }

  return j;
}

/* A função enfileira coloca um elemento tipo celula em uma lista circular com
cabeça. A inserção na fila é feita apenas na direção prox.
Ela recebe a palavra a ser inserida, a cabeça da fila e o endereço do pai do
elemento que está sendo inserido e retorna o endereço da nova cabeça. */

celula *enfileira (char y[], celula *fi, celula *palavra_pai){
   celula *nova;
   nova = malloc (sizeof(celula));
   strcpy (fi->palavra, y);

   nova->prox = fi->prox;
   fi->prox = nova;
   nova->pai = NULL;
   fi->pai = palavra_pai; /* garante que o elemento inserido esteja apontando
                           para o seu pai direto */
   return nova;
}

/* A função desenfileira remove um elemento da fila. Ela recebe o endereço da
cabeça da fila e retorna a cabeça da fila. */

celula *desenfileira (celula *fi){
   celula *p;
   p = fi->prox;
   fi->prox = p->prox;
   return fi;
}

/* A função imprime imprime na tela as palavras da lista encadeada apenas da
direção pai. */

void imprime (celula *le){
  celula *p;
  for (p = le; p != NULL; p = p -> pai) printf ("%s ", p->palavra);
}

int main (){

  int n = 0, m = 0, achei = -1, adjacente, tem_mais_dados = -1;
  char entrada[MAX], saida[MAX];
  celula *palavra_pai, *fi_head;
  celula_dic *dic, *busca_dic;

  /* O nosso programa monta um tipo de árvore em que os filhos apontam para o pai,
  ao contrário do normal. Isso facilita porque conseguimos trabalhar com uma árvore
  binária. No entanto, ao encontrar o menor caminho, é preciso que seja feita a
  volta para imprimi-lo. Como a nossa árvore aponta do filho para o pai, o caminho
  impresso está ao contrário. Para que isso não aconteça, invertemos a ordem das
  palavras de entrada e saída. Como o menor caminho é o mesmo tanto para a ida,
  quanto para a volta, ao fazer o código com a palavra de saída no lugar da entrada,
  e a de entrada no lugar da saída, o programa devolve o menor caminho e não é
  necessário inverter a ordem da lista. */

  printf ("\n");
  printf ("Insira uma palavra inicial que consta no dicionário:\n");
  scanf ("%s", saida); /* no lugar de entrada */
  printf ("\n");

  while (saida[n] != '\0') n++; /* n é o tamanho da saida */

  printf ("Insira uma palavra final uma palavra no dicionário:\n");
  scanf ("%s", entrada); /* no lugar de saída */
  printf ("\n");

  while (entrada[m] != '\0') m++; /* m é o tamanho da entrada */

  if (m != n){
    printf ("Não existem caminhos entre as palavras.\n");
    printf ("\n");
    return 0;
  }

  /* O ponteiro *palavra_pai armazena o endereço da celula que é pai dos filhos
  que estão sendo verificados no momento. Ou seja, ele é o pai da palavra que
  está sendo "visitada" no momento.
  Ele é inicializado com a palavra de entrada, que é, então, a raiz principal
  da árvore. */

  palavra_pai = malloc (sizeof(celula));
  palavra_pai->prox = NULL;
  palavra_pai->pai = NULL;
  strcpy (palavra_pai->palavra, entrada);

  /* fi_head é o ponteiro que armazena o endereço da cabeça da fila. A fila cresce/
  descresce apenas na direção prox. A lista vai ser circular. No entanto, ela é
  inicializada como fi_head->prox =! fi_head porque a condição de ela estar
  vazia vai entrar no laço maior que mantém o programa. Quando ela estiver vazia,
  não há caminhos entre a palavra de entrada e a palavra de saída e o programa
  acaba. */

  fi_head = malloc (sizeof(celula));
  fi_head->prox = NULL;
  fi_head->pai = NULL;

  /* A função cria_dicionario é chamada e o dicionário é lido e apenas as palavras
  com o tamanho n (tamanho da palavra de entrada e saída) são encadeadas.
  O ponteiro dic, então, aponta para o começo dessa lista. */

  dic = cria_dicionario (n, entrada);

  /* Depois de criadas a lista encadeada do dicionário e de os ponteiros terem sido
  devidamente inicializados, o programa entra no laço maior. Ele sai desse laço
  apenas quando alguma palavra adjacente ao pai atual (isto é, à palavra_pai) é
  a palavra de saída (ou seja, o worldladder foi resolvido), ou se a fila está
  vazia, isto é, se a palavra final não foi encontrada trocando-se as letras uma
  por vez.
  Ao entrar nesse laço, o programa entra em um laço menor que percorre a lista
  encadeada do dicionário. Cada palavra da lista (isto é, cada palavra do dicio-
  nário com o tamanho n de letras) é comparada com palavra_pai->palavra. Se elas
  forem adjacentes, a palavra da lista encadeada do dicionário entra na fila (e a
  direção pai aponta para palavra_pai). Se a palavra recém-enfileirada e a saída
  forem as mesmas, significa que o word ladder foi solucionado. Se não, a palavra
  recém-enfileirada é removida do dicionário (para que não seja lida novamente)
  e passa a ser a palavra_pai. */

  while (achei != 0 && (tem_mais_dados == -1)){
    if (fi_head->prox == fi_head) tem_mais_dados = 0; /* indica que essa deve ser
                                                      a última passada pelo while,
                                                      caso não ache mais adjacentes */
    busca_dic = dic; /* o ponteiro busca_dic percorre o dicionário e retorna ao
                      início dele toda vez que a leitura do dicionário acaba */

    if (fi_head->prox == NULL) fi_head->prox = fi_head; /* essa condição torna a
                                                         lista circular */

    while (busca_dic != NULL && achei != 0){

      adjacente = adjacencia (palavra_pai->palavra, busca_dic->palavra, n);
                          /* verifica se cada palavra do dicionário é adjacente
                           à palavra_pai->palavra */

      if (adjacente == 0){ /* se adjacente == 0, as palavras são adjacentes */
        tem_mais_dados = -1; /* encontrou uma adjacencia, então tem dados */
        fi_head = enfileira (busca_dic->palavra, fi_head, palavra_pai);
                          /* palavra do dicionário é enfileirada e a nova
                           cabeça é retornada */

        if (strcmp (busca_dic->palavra, saida) == 0) achei = 0;
        else remove_palavra (dic, busca_dic->palavra);
      }

      busca_dic = busca_dic->prox; /* a próxima palavra do dicionário
                                    vai ser comparada */

    } /* while (busca_dic != NULL && achei != 0) */

    if (achei != 0 && fi_head != fi_head->prox){

      palavra_pai = fi_head->prox; /* atualiza o pai */
      fi_head = desenfileira (fi_head); /* desenfileira a primeira palavra da
                                         lista, que é o novo pai */
      palavra_pai->prox = NULL; /* corta a ligação prox da palavra_pai com
                                 a fila */


    }

  } /* while (achei != 0 && (fi_head->prox != fi_head)) */

  if (achei == 0){

    printf("A menor cadeia de palavras é: %s ", saida); /* a última palavra */
    imprime (palavra_pai);                      /* (que é a palavra de saída) */
    printf ("\n");                              /* não está na lista palavra_pai
                                                pois acabou de entrar em fi_head
                                                e, então, não é a próxima a ser
                                                desenfileirada. Por isso ela é
                                                impressa separadamente */
  }

  else printf ("Não existem caminhos entre as palavras.\n");
  printf ("\n");

  return 0;

}

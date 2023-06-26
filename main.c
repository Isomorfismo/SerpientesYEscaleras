#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RESET_COLOR "\x1b[0m"
#define NEGRO "\x1b[30m"
#define ROJO "\x1b[31m"
#define VERDE "\x1b[32m"
#define AMARILLO "\x1b[33m"
#define AZUL "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define BLANCO "\x1b[37m"

#define tamTablero 10

typedef char element;

typedef struct node {
  element e[3]; // Elemento de 4 caracteres
  element type; // Tipo de 1 caracter
  int number;
  struct node *prev;
  struct node *next;
} Node;

typedef struct list {
  Node *head;
  Node *tail;
} List;

void createList(List *list) {
  list->head = NULL;
  list->tail = NULL;
}

void insertAtHead(List *list, element *e, element type, int number) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  strcpy(newNode->e, e);
  newNode->type = type;
  newNode->number = number;
  newNode->next = NULL;
  newNode->prev = NULL;
  if (list->head == NULL) {
    list->head = newNode;
    list->tail = newNode;
  } else {
    list->head->prev = newNode;
    newNode->next = list->head;
    list->head = newNode;
  }
}

void fillList(List *list) {
  for (int i = 0; i < tamTablero * tamTablero; i++) {
    insertAtHead(list, "\0", '\0', tamTablero * tamTablero - i);
  }
}

Node *searchNode(List *l, int pos) {
  Node *current = l->head;
  int count = 1; // Modifique esto
  while (current != NULL) {
    if (count == pos) {
      return current;
    }
    count++;
    current = current->next;
  }
  return NULL; // Nodo no encontrado
}

void *posSnakeOrLadder(List *l) {
  int arr[20] = {3,  39, 38, 6,  15, 10, 18, 46, 49, 12,
                 60, 66, 63, 26, 75, 88, 45, 80, 90, 76};
  for (int i = 0; i < 20; i += 2) {
    Node *temp1 = searchNode(l, arr[i]);
    Node *temp2 = searchNode(l, arr[i + 1]);
    if (arr[i] > arr[i + 1]) {
      temp1->type = 'S';
      temp1->prev = temp2;
      temp1->number = temp2->number;
    } else if (arr[i + 1] > arr[i]) { // agregue esto
      temp1->type = 'E';
      temp1->prev = temp2;
      temp1->number = temp2->number;
    }
  }
}

void changeNodeValue(List *list, int pos, char value) {
  Node *current = list->head;
  int count = 1;

  while (current != NULL) {
    if (count == pos) {
      current->e[0] = value;
      break;
    }

    count++;
    current = current->next;
  }
}

void moverJugador(List *list, int *posJugador, int numeroDado, char jugador) {
  int nuevaPosicion = *posJugador + numeroDado;
  Node *current = searchNode(list, nuevaPosicion);
  if (*posJugador + numeroDado > 100) { // agregue esto tambien
    int exceso = (*posJugador + numeroDado) - 100;
    nuevaPosicion = 100 - exceso;
  } else if (*posJugador + numeroDado == 100) {
    printf("Jugador %c ha ganado!!!\n", jugador);
  }
  if (current != NULL) {
    if (current->type == 'S' || current->type == 'E') {
      printf("El jugador %c cayó en la casilla %d y se movió a la casilla %d "
             "porque era %c\n",
             jugador, nuevaPosicion, current->prev->number, current->type);
      nuevaPosicion = current->prev->number;
    } else {
      printf("El jugador %c cayó en la casilla %d\n", jugador, nuevaPosicion);
    }
  }
  changeNodeValue(list, *posJugador,
                  ' '); // Eliminar el jugador de la posición actual
  changeNodeValue(list, nuevaPosicion,
                  jugador); // Colocar el jugador en la nueva posición

  *posJugador = nuevaPosicion; // Actualizar la posición del jugador
  // Verificar si los jugadores están en la misma posición
}

void printGrid(List *list, int posJugador1, int posJugador2) {
  int position = 1;
  Node *current = list->head;

  for (int i = 0; i < tamTablero; i++) {
    Node *aux = current;

    for (int j = 0; j < tamTablero; j++) {
      char type = current->type;

      if (type != '\0') {
        printf(AZUL "." VERDE "%c%d" AZUL "----.\t" RESET_COLOR, type,
               current->number);
      } else {
        printf(AZUL ".-------.\t" RESET_COLOR);
      }

      current = current->next;
    }

    printf("\n");

    for (int j = 0; j < tamTablero; j++) {
      char playerChar = '\0';

      if (aux->number == posJugador1) {
        playerChar = 'A';
      } else if (aux->number == posJugador2) {
        playerChar = 'B';
      }

      if (playerChar != '\0' && aux->type == '\0') {
        if (posJugador1 == posJugador2) {
          printf(AZUL "|   " BLANCO "%s" AZUL "  |\t" RESET_COLOR, "AB");
        } else {
          printf(AZUL "|   " BLANCO "%c" AZUL "   |\t" RESET_COLOR, playerChar);
        }
      } else {
        printf(AZUL "|       |\t" RESET_COLOR);
      }

      aux = aux->next;
    }

    printf("\n");

    for (int k = 0; k < tamTablero; k++) {
      if (position >= tamTablero&&position<100) {
        printf(AZUL "!_____" ROJO "%d" AZUL "!\t" RESET_COLOR, position);
      }else if(position>=100){
        printf(AZUL "!____" ROJO "%d" AZUL "!\t" RESET_COLOR, position);
      } 
      else {
        printf(AZUL "!______" ROJO "%d" AZUL "!\t" RESET_COLOR, position);
      }
      position++;
    }
    printf("\n");
  }
}

int tirarDados() {
  srand(time(NULL));
  int aleatorio = (rand() % 6) + 1; // Modifique para que fuera de un dado
  printf("\nDados: %d \n", aleatorio);
  return aleatorio;
}


int main() {
  List l;
  createList(&l);
  fillList(&l);
  posSnakeOrLadder(&l);
  
  /*Node *aux = l.head;
  while(aux != NULL){
    printf("Node: %d\n", aux->number);
    printf("Type: %c\n", aux->type);
    aux = aux->next;
   }*/
  
  // Juego
  char j1 = 'A', j2 = 'B';
  int modo;
  puts(" .|'''.|                     ||                .                                                   '||                           ");
  puts(" ||..  '  .... ... ..... ......  .... .. ... .||. ....  ....   .... ...    ....  ....   .... ....   ||  .... ... .. ....   ....  ");
  puts("  ''|||..|...|| ||' ''||'  ||||.|...|| ||  || ||.|...||||. '    '|.  |   .|...||||. ' .|   ''' .||  ||.|...|| ||' ''' .|| ||. '  ");
  puts(".     '|||      ||    ||    |||||      ||  || ||||     . '|..    '|.|    ||     . '|..||    .|' ||  ||||      ||   .|' || . '|.. ");
  puts("|'....|' '|...'.||.   ||...'.||.'|...'.||. ||.'|.'|...'|'..|'     '|      '|...'|'..|' '|...'|..'|'.||.'|...'.||.  '|..'|'|'..|' ");
  puts("                      ||                                       .. |                                                              ");
  puts("                     ''''                                       ''                                                               \n\n");
  puts("                                            888888  .dP'Y8   dP''b8   dPOYb   8b    d8 ");
  puts("                                            88__    `Ybo.'  dP   `R  dP   Yb  88b  d88 ");
  puts("                                            88--    o.`Y8b  Yb       Yb   dP  88YbdP88 ");
  puts("                                            888888  8bodP'   YboodP   YbodP   88 YY 88 ");

  printf("1. Jugar\n");
  printf("2. Salir\n");
  scanf("%d", &modo);
  system("clear");
  if (modo == 1) {
    // posicionar ambos jugadores en la casilla 1
    int posJ1 = 1, posJ2 = 1;
    moverJugador(&l, &posJ1, 0, j1);
    moverJugador(&l, &posJ2, 0, j2);
    printGrid(&l, posJ1, posJ2);
    printf("Presione enter para iniciar\n");
    getchar();
    // elegir al azar quien empieza
    int turno = (rand() % 2) + 1;
    int dado;
    while (posJ1 < 100 && posJ2 < 100) {
      if (posJ1 == posJ2) { //Esto tambien lo agregue
        printf("El jugador %c y %c estan en la misma posicion!\n", j1, j2);
      }
      if (turno == 1) {
        printf("Turno del jugador %c\n", j1);
        printf("Presione enter para tirar dados\n");
        getchar();
        system("clear");
        dado = tirarDados();
        moverJugador(&l, &posJ1, dado, j1);
        turno = 2;
      } else {
        printf("Turno del jugador %c\n", j2);
        printf("Presione enter para tirar dados\n");
        getchar();
        system("clear");
        dado = tirarDados();
        moverJugador(&l, &posJ2, dado, j2);
        turno = 1;
      }

      printGrid(&l, posJ1, posJ2);
    }
  }

  // Liberar memoria
  Node *current = l.head;
  while (current != NULL) {
    Node *next = current->next;
    free(current);
    current = next;
  }

  return 0;
}

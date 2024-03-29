#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char character;
    struct node *next, *prev;
} Node, *TList;

typedef struct {
    TList sentinel, wagon, mechanic;
} List;

typedef struct queueNode {
    char ins[30];
    struct queueNode *next, *prev;
} QNode;

typedef struct queue {
    QNode *first, *last;
} Queue;

void initTrain(List *t) {
    t->sentinel = (TList) malloc(sizeof(Node));
    t->wagon = (TList) malloc(sizeof(Node));
    if(t->sentinel == NULL || t->wagon == NULL) {
        printf("Malloc Failed!\n");
        return ;
    }
    t->sentinel->next = t->wagon;
    t->sentinel->prev = t->wagon;
    t->wagon->next = t->sentinel;
    t->wagon->prev = t->sentinel;
    t->mechanic = t->wagon;
    t->wagon->character = '#';
}

void initQueue(Queue *q) {
    q->first = (QNode*) malloc(sizeof(QNode));
    q->last = (QNode*) malloc(sizeof(QNode));
    if(q->first == NULL) {
        printf("Malloc Failed!\n");
        return ;
    }
    q->first->next = q->last;
    q->first->prev = NULL;
    q->last->next = NULL;
    q->last->prev = q->first;
}

void write(TList *mechanic, char c) {
    (*mechanic)->character = c;
}

void moveRight(List *t) {
    if(t->mechanic->next == t->sentinel) {
        TList new = (TList) malloc(sizeof(Node));
        new->next = t->sentinel;
        new->prev = t->mechanic;
        t->mechanic->next = new;
        t->sentinel->prev = new;
        t->mechanic = new;
        t->mechanic->character = '#';
    }
    else {
        t->mechanic = t->mechanic->next;
    }
}

void moveLeft(List *t) {
    if(t->mechanic->prev == t->sentinel) {
        t->mechanic = t->sentinel->prev;
    }
    else {
        t->mechanic = t->mechanic->prev;
    }
}

void clearCell(List *t) {
    if(t->mechanic == t->wagon) {
        // t->mechanic->character = '#';
        TList aux = t->mechanic;
        t->mechanic->prev->next = t->mechanic->next;
        t->mechanic->next->prev = t->mechanic->prev;
        t->mechanic = t->sentinel->prev;
        t->wagon = t->sentinel->next;
        free(aux);
    }
    // caz vagon unic in tren
    // caz vagon oarecare
    else {
        
    }
}

void push(Queue *q, char s[]) {
    QNode *new_instr = (QNode*) malloc(sizeof(QNode));
    strcpy(new_instr->ins, s);
    q->last->prev->next = new_instr;
    new_instr->prev = q->last->prev;
    q->last->prev = new_instr;
    new_instr->next = q->last;
}

void pop(Queue *q, List *train) {
    QNode *aux = q->first->next;
    q->first->next = aux->next;
    aux->next->prev = q->first;
    if(strstr(aux->ins, "WRITE")) {
        write(&(*train).mechanic, aux->ins[6]);
    }
    else if(strstr(aux->ins, "MOVE_RIGHT")) {
        moveRight(&(*train));
    }
        else if(strstr(aux->ins, "MOVE_LEFT")) {
            moveLeft(&(*train));
        }
            else if(strstr(aux->ins, "CLEAR_CELL")) {
                clearCell(&(*train));
            }
    free(aux);
}

void show_current(List t, FILE *out) {
    fprintf(out,"%c", t.mechanic->character);
}

void show(List t, FILE *out) {
    TList p;
    for(p = t.wagon; p != t.sentinel; p = p->next) {
        if(p == t.mechanic) {
            fprintf(out, "|");
            fprintf(out, "%c", p->character);
            fprintf(out, "|");
        }
        else {
            fprintf(out, "%c", p->character);
        }
    }
}

int main(){
    List train;
    Queue q;
    FILE *f, *out;
    int nr_instr, i;
    char instr[30];
    f = fopen("tema1.in", "r");
    out = fopen("tema1.out", "w");
    if(f == NULL || out == NULL) {
        printf("Cannot open file!\n");
        return 0;
    }
    initTrain(&train);
    initQueue(&q);
    fscanf(f, "%d", &nr_instr);
    fgetc(f);
    for(i = 0; i < nr_instr; i++) {
        fgets(instr, 30, f);
        if(strstr(instr, "MOVE") || strstr(instr, "WRITE") || strstr(instr, "CLEAR") || strstr(instr, "INSERT")) {
            push(&q, instr);
            //printf(out, "%s", q.first->next->ins);   
        }
        else if(strcmp(instr, "SHOW") == 0) {
            show(train, out);
        }
            else if(strcmp(instr, "SHOW-CURRENT") == 0) {
                show_current(train, out);
            }
                else if(strstr(instr, "EXECUTE")) {
                    pop(&q, &train);
                }
    }
    // moveRight(&train);
    fclose(f);
    fclose(out);
    return 0;
}
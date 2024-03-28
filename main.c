#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char c;
    struct node *next, *prev;
} Node, *TList;

typedef struct {
    TList sentinel, wagon, mechanic;
    int len;
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
    t->wagon->c = '#';
    t->len = 1;
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
            QNode *new_instr = (QNode*) malloc(sizeof(QNode));
            q.last->prev->next = new_instr;
            new_instr->prev = q.last->prev;
            q.last->prev = new_instr;
            new_instr->next = q.last;
            strcpy(new_instr->ins, instr);
        }
    }
    QNode *current = q.first->next;
    // for (current; current->next != NULL; current = current->next) {
    //     fprintf(out, "%s", current->ins);
    // }
    fclose(f);
    fclose(out);
    return 0;
}
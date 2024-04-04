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
    // t->mechanic->character = '#';
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

void write(List *t, char c) {
    t->mechanic->character = c;
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
    if(t->mechanic->prev == t->sentinel && t->mechanic->next == t->sentinel) {
        t->mechanic->character = '#';
    }
    else if(t->mechanic == t->wagon && t->mechanic->next != t->sentinel) {
        TList aux = t->mechanic;
        t->mechanic->prev->next = t->mechanic->next;
        t->mechanic->next->prev = t->mechanic->prev;
        t->mechanic = t->sentinel->prev;
        t->wagon = t->sentinel->next;
        free(aux);
    }
        else {
            TList aux = t->mechanic;
            t->mechanic->prev->next = t->mechanic->next;
            t->mechanic->next->prev = t->mechanic->prev;
            t->mechanic = t->mechanic->prev;
            free(aux);
        }
}

void clearAll(List *t) {
    TList p = t->wagon->next;
    for(p; p != t->sentinel; p = p->next) {
        free(p->prev);
    }
    t->wagon->next = t->sentinel;
    t->mechanic = t->wagon;
    t->wagon->character = '#';
}

void insertRight(List *t, char c) {
    TList new_wagon = (TList) malloc(sizeof(Node));
    new_wagon->character = c;
    new_wagon->next = t->mechanic->next;
    new_wagon->prev = t->mechanic;
    t->mechanic->next->prev = new_wagon;
    t->mechanic->next = new_wagon;
    t->mechanic = t->mechanic->next;
}

void insertLeft(List *t, char c, FILE *out) {
    TList new_wagon = (TList) malloc(sizeof(Node));
    new_wagon->character = c;
    if(t->mechanic == t->wagon) {
        fprintf(out, "ERROR\n");
    }
    else {
        new_wagon->next = t->mechanic;
        new_wagon->prev = t->mechanic->prev;
        t->mechanic->prev->next = new_wagon;
        t->mechanic->prev = new_wagon;
        t->mechanic = t->mechanic->prev;
    }
}

void search(List *t, char word[], FILE *out) {
    char aux[100], s[100];
    int k = 1, ok = 0, i, found = 0;
    aux[0] = t->mechanic->character;
    if(aux[0] == word[0]) {
        ok = 1;
        aux[1] = '\0';
    }
    else if(strcmp(aux, word) == 0) {
        found = 1;
    }
    else if(ok == 0) {
        k = 0;
        strcpy(aux, "");
    }
    TList p;
    for(p = t->mechanic->next; p != t->mechanic && found == 0; p = p->next) {
        if(p == t->sentinel) {
            p = t->wagon;
            if(p == t->mechanic) {
                break;
            }
        }
        aux[k++] = p->character;
        aux[k] = '\0';
        if(strcmp(aux, word) == 0) {
            found = 1;
        }
        else if(aux[k - 1] != word[k - 1]) {
            ok = 0;
            for (i = 0; i < k && ok == 0; i++) {
                if(strstr(word, aux + i) != NULL) {
                    if(strcmp(strstr(word, aux + i), word) == 0) {
                        ok = 1;
                        if (strlen(aux) >= i) {
                            strcpy(s, aux + i);
                            strcpy(aux, s);
                            k = k - i;
                        }
                    }
                    else {
                        ok = 0;
                    }
                }
            }
            if(!ok) {
                k = 0;
                strcpy(aux, "");
            }
        }
    }
    if(found) {
        for (i = 0; i < k; i++) {
            p = p->prev;
            if(p == t->sentinel) {
                p = t->sentinel->prev;
            }
        }
        t->mechanic = p;
    }
    else {
        fprintf(out, "ERROR\n");
    }
}

void searchRight(List *t, char word[], FILE *out) {
    char aux[50], s[50];
    int k = 1, ok = 0, i, found = 0;
    aux[0] = t->mechanic->character;
    if(aux[0] == word[0]) {
        ok = 1;
    }
    if(ok == 0) {
        k = 0;
        strcpy(aux, "");
    }
    if(strcmp(aux, word) == 0) {
        found = 1;
    }
    TList p;
    for(p = t->mechanic->next; p != t->sentinel && found == 0; p = p->next) {
        aux[k++] = p->character;
        aux[k] = '\0';
        if(strcmp(aux, word) == 0) {
            found = 1;
        }
        else if(aux[k - 1] != word[k - 1]) {
            ok = 0;
            for (i = 0; i < k && ok == 0; i++) {
                if(strstr(word, aux + i) != NULL) {
                    if(strcmp(strstr(word, aux + i), word) == 0) {
                        ok = 1;
                        if (strlen(aux) >= i) {
                            strcpy(s, aux + i);
                            strcpy(aux, s);
                            k = k - i;
                        }
                    }
                    else {
                        ok = 0;
                    }
                }
            }
            if(!ok) {
                k = 0;
                strcpy(aux, "");
            }
        }
    }
    if(found) {
        t->mechanic = p->prev;
    }
    else {
        fprintf(out, "ERROR\n");
    }
}

void searchLeft(List *t, char word[], FILE *out) {
    char aux[50], s[50];
    int k = 1, ok = 0, i, found = 0;
    aux[0] = t->mechanic->character;
    if(aux[0] == word[0]) {
        ok = 1;
    }
    if(ok == 0) {
        k = 0;
        strcpy(aux, "");
    }
    if(strcmp(aux, word) == 0) {
        found = 1;
    }
    TList p;
    for(p = t->mechanic->prev; p != t->sentinel && found == 0; p = p->prev) {
        aux[k++] = p->character;
        aux[k] = '\0';
        if(strcmp(aux, word) == 0) {
            found = 1;
        }
        else if(aux[k - 1] != word[k - 1]) {
            ok = 0;
            for (i = 0; i < k && ok == 0; i++) {
                if(strstr(word, aux + i) != NULL) {
                    if(strcmp(strstr(word, aux + i), word) == 0) {
                        ok = 1;
                        if (strlen(aux) >= i) {
                            strcpy(s, aux + i);
                            strcpy(aux, s);
                            k = k - i;
                        }
                    }
                    else {
                        ok = 0;
                    }
                }
            }
            if(!ok) {
                k = 0;
                strcpy(aux, "");
            }
        }
    }
    if(found) {
        t->mechanic = p->next;
    }
    else {
        fprintf(out, "ERROR\n");
    }
}

void switchQueue(Queue *q) {
    QNode *aux = q->first;
    q->first = q->last;
    q->last = aux;
    q->first->next = q->first->prev;
    q->first->prev = NULL;
    q->last->prev = q->last->next;
    q->last->next = NULL;
    for(aux = q->first->next; aux != q->last; aux = aux->next) {
        QNode *aux2 = aux->next;
        aux->next = aux->prev;
        aux->prev = aux2;
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

void pop(Queue *q, List *train, FILE *out) {
    char c, word[50];
    QNode *aux = q->first->next;
    q->first->next = aux->next;
    aux->next->prev = q->first;
    if(strstr(aux->ins, "WRITE")) {
        c = (strchr(aux->ins, ' ') + 1)[0];
        write(&(*train), c);
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
                else if(strstr(aux->ins, "CLEAR_ALL")) {
                    clearAll(&(*train));
                }
                    else if(strstr(aux->ins, "INSERT_RIGHT")) {
                        c = (strchr(aux->ins, ' ') + 1)[0];
                        insertRight(&(*train), c);
                    }
                        else if(strstr(aux->ins, "INSERT_LEFT")) {
                            c = (strchr(aux->ins, ' ') + 1)[0];
                            insertLeft(&(*train), c, out);
                        }
                            else if(strstr(aux->ins, "SEARCH ")) {
                                strcpy(word, strchr(aux->ins, ' ') + 1);
                                // printf("%s\n", word);
                                search(&(*train), word, out);
                            }
                                else if(strstr(aux->ins, "SEARCH_RIGHT ")) {
                                    strcpy(word, strchr(aux->ins, ' ') + 1);
                                    searchRight(&(*train), word, out);
                                }
                                    else if(strstr(aux->ins, "SEARCH_LEFT ")) {
                                        strcpy(word, strchr(aux->ins, ' ') + 1);
                                        searchLeft(&(*train), word, out);
                                    }
    free(aux);
}

void show_current(List t, FILE *out) {
    fprintf(out,"%c", t.mechanic->character);
    fprintf(out, "\n");
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
    fprintf(out, "\n");
}

int main(){
    List train;
    Queue q;
    FILE *f, *out;
    int nr_instr, i;
    char instr[101];
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
        fgets(instr, 101, f);
        if(instr[strlen(instr) - 1] == '\n') {
            instr[strlen(instr) - 1] = '\0';
        }
        // if(i >= 50) printf("%s\n", instr);
        if(strstr(instr, "MOVE") || strstr(instr, "WRITE") || strstr(instr, "CLEAR") || strstr(instr, "INSERT") || strstr(instr, "SEARCH")) {
            push(&q, instr);
            // printf("%s", q.first->next->ins);   
        }
        else if(strcmp(instr, "SHOW") == 0) {
            show(train, out);
        }
            else if(strcmp(instr, "SHOW_CURRENT") == 0) {
                show_current(train, out);
                // printf("%c", train.wagon->character);
            }
                else if(strstr(instr, "EXECUTE")) {
                    pop(&q, &train, out);
                }
                    else if(strcmp(instr, "SWITCH") == 0) {
                        switchQueue(&q);
                    }
        // show(train, out);
        
    }
    // moveRight(&train);
    fclose(f);
    fclose(out);
    return 0;
}
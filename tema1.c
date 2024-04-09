// ABĂHNENCEI Alin Andrei - 312CC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structura pentru vagon
typedef struct node
{
    char character;
    struct node *next, *prev;
} Node, *TList;

//structura pentru tren
typedef struct
{
    TList sentinel, wagon, mechanic;
} List;

//structura pentru un element din coada
typedef struct queueNode
{
    char ins[101];
    struct queueNode *next, *prev;
} QNode;


//structura pentru coada
typedef struct queue
{
    QNode *first, *last;
} Queue;


//initializarea trenului
void initTrain(List *t)
{
    t->sentinel = (TList)malloc(sizeof(Node));
    t->wagon = (TList)malloc(sizeof(Node));
    if (t->sentinel == NULL || t->wagon == NULL)
    {
        printf("Malloc Failed!\n");
        return;
    }
    t->sentinel->next = t->wagon;
    t->sentinel->prev = t->wagon;
    t->wagon->next = t->sentinel;
    t->wagon->prev = t->sentinel;
    t->mechanic = t->wagon;
    t->wagon->character = '#';
}

//initializarea cozii
void initQueue(Queue *q)
{
    q->first = (QNode *)malloc(sizeof(QNode));
    q->last = (QNode *)malloc(sizeof(QNode));
    if (q->first == NULL)
    {
        printf("Malloc Failed!\n");
        return;
    }
    q->first->next = q->last;
    q->first->prev = NULL;
    q->last->next = NULL;
    q->last->prev = q->first;
}

//functie pentru modificarea inscriptiei de pe vagon
void write(List *t, char c)
{
    t->mechanic->character = c;
}

//functie pentru deplasarea mecanicului la dreapta
void moveRight(List *t)
{
    if (t->mechanic->next == t->sentinel)
    {
        TList new = (TList)malloc(sizeof(Node));
        if (new == NULL)
        {
            printf("Malloc Failed!\n");
            return;
        }
        new->next = t->sentinel;
        new->prev = t->mechanic;
        t->mechanic->next = new;
        t->sentinel->prev = new;
        t->mechanic = new;
        t->mechanic->character = '#';
    }
    else
    {
        t->mechanic = t->mechanic->next;
    }
}

//functie pentru deplasarea mecanicului la stanga
void moveLeft(List *t)
{
    if (t->mechanic->prev == t->sentinel)
    {
        t->mechanic = t->sentinel->prev;
    }
    else
    {
        t->mechanic = t->mechanic->prev;
    }
}

//functie pentru stergerea unui vagon
void clearCell(List *t)
{
    //daca e un singur vagon in tren
    if (t->mechanic->prev == t->sentinel && t->mechanic->next == t->sentinel)
    {
        t->mechanic->character = '#';
    }
    //daca mecanicul e pe primul vagon
    else if (t->mechanic == t->wagon && t->mechanic->next != t->sentinel)
    {
        TList aux = t->mechanic;
        t->mechanic->prev->next = t->mechanic->next;
        t->mechanic->next->prev = t->mechanic->prev;
        t->mechanic = t->sentinel->prev;
        t->wagon = t->sentinel->next;
        free(aux);
    }
    //daca mecanicul e pe alta pozitie
    else
    {
        TList aux = t->mechanic;
        t->mechanic->prev->next = t->mechanic->next;
        t->mechanic->next->prev = t->mechanic->prev;
        t->mechanic = t->mechanic->prev;
        free(aux);
    }
}

//functie pentru stergerea tuturor vagoanelor

void clearAll(List *t)
{
    TList p = t->wagon->next;
    while (p != t->sentinel)
    {
        TList next = p->next;
        free(p);
        p = next;
    }
    t->wagon->next = t->sentinel;
    t->mechanic = t->wagon;
    t->wagon->character = '#';
}

//functie pentru inserarea unui vagon la dreapta
void insertRight(List *t, char c)
{
    //mecanicul e pe ultimul vagon
    if (t->mechanic == t->sentinel->prev)
    {
        TList new_wagon = (TList)malloc(sizeof(Node));
        if (new_wagon == NULL)
        {
            printf("Malloc Failed!\n");
            free(new_wagon);
            return;
        }
        t->mechanic->next = new_wagon;
        new_wagon->prev = t->mechanic;
        new_wagon->next = t->sentinel;
        t->sentinel->prev = new_wagon;
        t->mechanic = new_wagon;
        new_wagon->character = c;
    }
    //mecanicul e pe alta pozitie
    else
    {
        TList new_wagon = (TList)malloc(sizeof(Node));
        if (new_wagon == NULL)
        {
            printf("Malloc Failed!\n");
            free(new_wagon);
            return;
        }
        new_wagon->next = t->mechanic->next;
        new_wagon->prev = t->mechanic;
        t->mechanic->next->prev = new_wagon;
        t->mechanic->next = new_wagon;
        t->mechanic = t->mechanic->next;
        new_wagon->character = c;
    }
}

//functie pentru inserarea unui vagon la stanga
void insertLeft(List *t, char c, FILE *out)
{
    TList new_wagon = (TList)malloc(sizeof(Node));
    if (new_wagon == NULL)
    {
        printf("Malloc Failed!\n");
        return;
    }
    //mecanicul e pe primul vagon
    new_wagon->character = c;
    if (t->mechanic == t->wagon)
    {
        fprintf(out, "ERROR\n");
        free(new_wagon);
    }
    //mecanicul e pe alta pozitie
    else
    {
        new_wagon->next = t->mechanic;
        new_wagon->prev = t->mechanic->prev;
        t->mechanic->prev->next = new_wagon;
        t->mechanic->prev = new_wagon;
        t->mechanic = t->mechanic->prev;
    }
}

//functie pentru cautarea unui cuvant in tot trenul
void search(List *t, char word[], FILE *out)
{
    char aux[101] = "", s[101] = "", *cmp = NULL;
    int k = 1, ok = 0, i = 0, found = 0;
    aux[0] = t->mechanic->character;
    //primul caracter e cel din cuvant
    if (aux[0] == word[0])
    {
        ok = 1;
        aux[1] = '\0';
    }
    //daca a fost gasit cuvantul
    else if (strcmp(aux, word) == 0)
    {
        found = 1;
    }
    //daca nu a fost gasit cuvantul se reinitializeaza aux
    else if (ok == 0)
    {
        k = 0;
        strcpy(aux, "");
    }
    TList p;
    for (p = t->mechanic->next; p != t->mechanic && found == 0; p = p->next)
    {
        //daca s-a ajuns la locomoitva se sare peste ea
        if (p == t->sentinel)
        {
            p = t->wagon;
            //daca s-a ajuns de unde a plecat se iese din for
            if (p == t->mechanic)
            {
                break;
            }
        }
        aux[k++] = p->character;
        aux[k] = '\0';
        if (strcmp(aux, word) == 0)
        {
            found = 1;
        }
        //daca ultimul caracter din aux e diferit de caracterul corespunzator
        //din cuvant se incearca sa se gaseasca un sufix al cuvantului in aux
        else if (aux[k - 1] != word[k - 1])
        {
            ok = 0;
            for (i = 0; i < k && ok == 0; i++)
            {
                cmp = strstr(word, aux + i);
                if (cmp != NULL)
                {
                    if (strcmp(cmp, word) == 0)
                    {
                        ok = 1;
                        //strlen returneaza date de tip size_t, asa ca se face
                        //cast
                        if (strlen(aux) >= (size_t)i)
                        {
                            strcpy(s, aux + i);
                            strcpy(aux, s);
                            k = k - i;
                        }
                    }
                    else
                    {
                        ok = 0;
                    }
                }
            }
            if (!ok)
            {
                k = 0;
                strcpy(aux, "");
            }
        }
    }
    //daca cuvantul a fost gasit se muta mecanicul pe primul caracter
    if (found)
    {
        for (i = 0; i < k; i++)
        {
            p = p->prev;
            if (p == t->sentinel)
            {
                p = t->sentinel->prev;
            }
        }
        t->mechanic = p;
    }
    else
    {
        fprintf(out, "ERROR\n");
    }
    p = NULL;
    free(p);
    cmp = NULL;
    free(cmp);
}

//functie pentru cautarea unui cuvant in dreapta
void searchRight(List *t, char word[], FILE *out)
{
    char aux[101] = "", s[101] = "", *cmp = NULL;
    int k = 1, ok = 0, i, found = 0;
    aux[0] = t->mechanic->character;
    if (aux[0] == word[0])
    {
        ok = 1;
    }
    if (ok == 0)
    {
        k = 0;
        strcpy(aux, "");
    }
    if (strcmp(aux, word) == 0)
    {
        found = 1;
    }
    TList p;
    for (p = t->mechanic->next; p != t->sentinel && found == 0; p = p->next)
    {
        aux[k++] = p->character;
        aux[k] = '\0';
        if (strcmp(aux, word) == 0)
        {
            found = 1;
        }
        //daca ultimul caracter din aux e diferit de caracterul corespunzator
        //din cuvant se incearca sa se gaseasca un sufix al cuvantului in aux
        else if (aux[k - 1] != word[k - 1])
        {
            ok = 0;
            for (i = 0; i < k && ok == 0; i++)
            {
                cmp = strstr(word, aux + i);
                if (cmp != NULL)
                {
                    if (strcmp(cmp, word) == 0)
                    {
                        ok = 1;
                        if (strlen(aux) >= (size_t)i)
                        {
                            strcpy(s, aux + i);
                            strcpy(aux, s);
                            k = k - i;
                        }
                    }
                    else
                    {
                        ok = 0;
                    }
                }
            }
            if (!ok)
            {
                k = 0;
                strcpy(aux, "");
            }
        }
    }
    //daca cuvantul a fost gasit se muta mecanicul pe ultimul caracter
    if (found)
    {
        t->mechanic = p->prev;
    }
    else
    {
        fprintf(out, "ERROR\n");
    }
    p = NULL;
    free(p);
    cmp = NULL;
    free(cmp);
}

//functie pentru cautarea unui cuvant in stanga
void searchLeft(List *t, char word[], FILE *out)
{
    char aux[101] = "", s[101] = "", *cmp = NULL;
    int k = 1, ok = 0, i, found = 0;
    aux[0] = t->mechanic->character;
    if (aux[0] == word[0])
    {
        ok = 1;
    }
    if (ok == 0)
    {
        k = 0;
        strcpy(aux, "");
    }
    if (strcmp(aux, word) == 0)
    {
        found = 1;
    }
    TList p;
    for (p = t->mechanic->prev; p != t->sentinel && found == 0; p = p->prev)
    {
        aux[k++] = p->character;
        aux[k] = '\0';
        if (strcmp(aux, word) == 0)
        {
            found = 1;
        }
        //daca ultimul caracter din aux e diferit de caracterul corespunzator
        //din cuvant se incearca sa se gaseasca un sufix al cuvantului in aux
        else if (aux[k - 1] != word[k - 1])
        {
            ok = 0;
            for (i = 0; i < k && ok == 0; i++)
            {
                cmp = strstr(word, aux + i);
                if (cmp != NULL)
                {
                    if (strcmp(cmp, word) == 0)
                    {
                        ok = 1;
                        if (strlen(aux) >= (size_t)i)
                        {
                            strcpy(s, aux + i);
                            strcpy(aux, s);
                            k = k - i;
                        }
                    }
                    else
                    {
                        ok = 0;
                    }
                }
            }
            if (!ok)
            {
                k = 0;
                strcpy(aux, "");
            }
        }
    }
    //daca cuvantul a fost gasit se muta mecanicul pe ultimul caracter
    if (found)
    {
        t->mechanic = p->next;
    }
    else
    {
        fprintf(out, "ERROR\n");
    }
    p = NULL;
    free(p);
    cmp = NULL;
    free(cmp);
}

//functie pentru interschimbarea elementelor cozii
void switchQueue(Queue *q)
{
    QNode *aux = q->first;
    q->first = q->last;
    q->last = aux;
    q->first->next = q->first->prev;
    q->first->prev = NULL;
    q->last->prev = q->last->next;
    q->last->next = NULL;
    for (aux = q->first->next; aux != q->last; aux = aux->next)
    {
        QNode *aux2 = aux->next;
        aux->next = aux->prev;
        aux->prev = aux2;
    }
    aux = NULL;
    free(aux);
}

//functie pentru adaugarea unui element in coada
void push(Queue *q, char s[])
{
    QNode *new_instr = (QNode *)malloc(sizeof(QNode));
    if (new_instr == NULL)
    {
        printf("Malloc Failed!\n");
        return;
    }
    strcpy(new_instr->ins, s);
    q->last->prev->next = new_instr;
    new_instr->prev = q->last->prev;
    q->last->prev = new_instr;
    new_instr->next = q->last;
}

//functie pentru eliminarea unui element din coada
void pop(Queue *q, List *train, FILE *out)
{
    char c, word[101];
    QNode *aux = q->first->next;
    q->first->next = aux->next;
    aux->next->prev = q->first;
    if (strstr(aux->ins, "WRITE"))
    {
        c = (strchr(aux->ins, ' ') + 1)[0];
        write(&(*train), c);
    }
    else if (strstr(aux->ins, "MOVE_RIGHT"))
    {
        moveRight(&(*train));
    }
    else if (strstr(aux->ins, "MOVE_LEFT"))
    {
        moveLeft(&(*train));
    }
    else if (strstr(aux->ins, "CLEAR_CELL"))
    {
        clearCell(&(*train));
    }
    else if (strstr(aux->ins, "CLEAR_ALL"))
    {
        clearAll(&(*train));
    }
    else if (strstr(aux->ins, "INSERT_RIGHT"))
    {
        c = (strchr(aux->ins, ' ') + 1)[0];
        insertRight(&(*train), c);
    }
    else if (strstr(aux->ins, "INSERT_LEFT"))
    {
        c = (strchr(aux->ins, ' ') + 1)[0];
        insertLeft(&(*train), c, out);
    }
    else if (strstr(aux->ins, "SEARCH "))
    {
        strcpy(word, strchr(aux->ins, ' ') + 1);
        search(&(*train), word, out);
    }
    else if (strstr(aux->ins, "SEARCH_RIGHT "))
    {
        strcpy(word, strchr(aux->ins, ' ') + 1);
        searchRight(&(*train), word, out);
    }
    else if (strstr(aux->ins, "SEARCH_LEFT "))
    {
        strcpy(word, strchr(aux->ins, ' ') + 1);
        searchLeft(&(*train), word, out);
    }
    free(aux);
}

//functie pentru afisarea caracterului de pe vagonul pe care se afla mecanicul
void show_current(List t, FILE *out)
{
    fprintf(out, "%c", t.mechanic->character);
    fprintf(out, "\n");
}

//functie pentru afisarea trenului
void show(List t, FILE *out)
{
    TList p;
    for (p = t.wagon; p != t.sentinel; p = p->next)
    {
        if (p == t.mechanic)
        {
            fprintf(out, "|");
            fprintf(out, "%c", p->character);
            fprintf(out, "|");
        }
        else
        {
            fprintf(out, "%c", p->character);
        }
    }
    fprintf(out, "\n");
    p = NULL;
    free(p);
}

//functie pentru eliberarea memoriei
void freeMemory(List *t, Queue *q)
{
    //eliberarea memoriei pentru tren
    TList current = t->sentinel->next;
    while (current != t->sentinel)
    {
        TList temp = current;
        current = current->next;
        free(temp);
    }
    free(t->sentinel);

    //eliberarea memoriei pentru coada
    QNode *currentQ = q->first->next;
    while (currentQ != q->last)
    {
        QNode *tempQ = currentQ;
        currentQ = currentQ->next;
        free(tempQ);
    }
    free(q->first);
    free(q->last);
}

int main()
{
    List train;
    Queue q;
    FILE *f, *out;
    int nr_instr, i;
    char instr[101];
    f = fopen("tema1.in", "r");
    out = fopen("tema1.out", "w");
    if (f == NULL || out == NULL)
    {
        printf("Cannot open file!\n");
        return 0;
    }
    //initializarea trenului si a cozii
    initTrain(&train);
    initQueue(&q);
    fscanf(f, "%d", &nr_instr);
    fgetc(f);
    for (i = 0; i < nr_instr; i++)
    {
        fgets(instr, 101, f);
        //eliminarea caracterului '\n' de la finalul instructiunii
        if (instr[strlen(instr) - 1] == '\n')
        {
            instr[strlen(instr) - 1] = '\0';
        }
        //adaugare in coada
        if (strstr(instr, "MOVE") || strstr(instr, "WRITE") ||
            strstr(instr, "CLEAR") || strstr(instr, "INSERT") ||
            strstr(instr, "SEARCH"))
        {
            push(&q, instr);
        }
        else if (strcmp(instr, "SHOW") == 0)
        {
            show(train, out);
        }
        else if (strcmp(instr, "SHOW_CURRENT") == 0)
        {
            show_current(train, out);
        }
        else if (strstr(instr, "EXECUTE"))
        {
            pop(&q, &train, out);
        }
        else if (strcmp(instr, "SWITCH") == 0)
        {
            switchQueue(&q);
        }
    }
    //eliberarea memoriei si inchiderea fisierelor
    freeMemory(&train, &q);
    fclose(f);
    fclose(out);
    return 0;
}
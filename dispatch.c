// BOBOC Ana Maria Mihaela - 315CC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct unit {
    int id;
    char type;
    int availability;
};

struct incident {
    int id;
    char priority[7];
    char *description;
    char status[11];
    struct incident *next, *prev;
};

struct intervention {
    struct incident *incident;
    struct unit *unit;
    struct intervention *next, *prev;
};

struct nod_queue_incidente{
    struct incident *adresa;
    struct nod_queue_incidente *next;
};

struct nod_queue_unitati{
    struct unit *adresa;
    struct nod_queue_unitati *next;
};

struct queue_incidente{
    struct nod_queue_incidente *head, *tail;
};

struct queue_unitati{
    struct nod_queue_unitati *head, *tail;
};

struct nod_stack_interventii{
    struct intervention *adresa;
    struct nod_stack_interventii *next;
};

struct stack_interventii{
    struct nod_stack_interventii *top;
};

struct system {
    struct unit *units;
    struct incident *incidents, *santinela;
    struct intervention *interventions;
    struct queue_incidente *queue_high, *queue_medium, *queue_low;
    struct queue_unitati *queue_available_units;
    struct stack_interventii *istoric;
};

//functie pt a extrage un echipaj din coada
struct nod_queue_unitati *remove_nod_unitati(struct queue_unitati *q)
{
    struct nod_queue_unitati *del = q->head;
    q->head = q->head->next;
    del->next = NULL;

    if (q->head == NULL){
        q->tail = NULL;
    }
    return del;
}

//functie pt a extrage un incident din coada de prioritate
struct nod_queue_incidente *remove_nod_incidente(struct queue_incidente *q)
{
    struct nod_queue_incidente *del = q->head;
    q->head = q->head->next;
    del->next = NULL;

    if (q->head == NULL){
        q->tail = NULL;
    }
    return del;
}

//functie pt a adauga un incident la finalul cozii
void add_incidente(struct queue_incidente *q, struct incident *incident)
{
    struct nod_queue_incidente *nou;
    nou = (struct nod_queue_incidente*)malloc(sizeof(struct nod_queue_incidente));
    nou->adresa = incident;
    nou->next = NULL;

    if (q->head == NULL){
        q->head = nou;
        q->tail = nou;
    } else {
        q->tail->next = nou;
        q->tail = nou;
    }
}

//functie pt a adauga un echipaj la finalul cozii de disponibilitate
void add_unitati(struct queue_unitati *q, struct unit *unit)
{
    struct nod_queue_unitati *nou;
    nou = (struct nod_queue_unitati*)malloc(sizeof(struct nod_queue_unitati));
    nou->adresa = unit;
    nou->next = NULL;

    if (q->head == NULL){
        q->head = nou;
        q->tail = nou;
    } else {
        q->tail->next = nou;
        q->tail = nou;
    }
}

void ADD_INCIDENT(struct system *list, int ID_incident, char *priority, char *description)
{
    struct incident *nou;
    nou = (struct incident*)malloc(sizeof(struct incident));

    nou->id = ID_incident;
    strcpy(nou->priority, priority);

    //alocare de memorie (+1 pt \0)
    nou->description=(char*)malloc(strlen(description)+1);
    strcpy(nou->description, description);
    strcpy(nou->status, "queued");
    
    //inserare la finalul listei dublu inlantuite
    struct incident *aux;
    aux = list->santinela->prev;
    aux->next = nou;
    nou->prev = aux;
    nou->next = list->santinela;
    list->santinela->prev = nou;

    //adaugare in functie de gravitate
    if (strcmp(nou->priority, "high") == 0){
        add_incidente(list->queue_high, nou);
    } else if (strcmp(nou->priority, "medium") == 0){
        add_incidente(list->queue_medium, nou);
    } else if (strcmp(nou->priority, "low") == 0){
        add_incidente(list->queue_low, nou);
    }

}

void CHECK_UNITS_AVAILABILITY(struct queue_unitati *q, FILE *out)
{
    int nr = 0;
    struct nod_queue_unitati *curent = q->head;
    while (curent != NULL){
        nr++;
        curent = curent->next;
    }
    fprintf(out, "Number of available units: %d\n", nr);

}

void DISPATCH(struct system *list, FILE *out)
{
    struct nod_queue_incidente *incident = NULL;
    struct nod_queue_unitati *unitate = NULL;

    //extragere in functie de gravitate
    if (list->queue_available_units->head != NULL){
        if (list->queue_high->head != NULL){
            incident = remove_nod_incidente(list->queue_high);
        } else if (list->queue_medium->head != NULL){
                incident = remove_nod_incidente(list->queue_medium);
        } else if (list->queue_low->head != NULL){
                incident = remove_nod_incidente(list->queue_low);
        } else {
                fprintf(out, "INVALID OPERATION! ERROR 404\n");
        }

    } else {
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
    }

    //daca s-a gasit si incident si echipaj disponibil
    if (incident != NULL && list->queue_available_units->head != NULL){
        unitate = remove_nod_unitati(list->queue_available_units);

        //echipajul devine indisponibil
        strcpy(incident->adresa->status, "intervened");
        unitate->adresa->availability = 0; 

        //interventie noua care leaga incidentul de echipaj
        struct intervention *interventie = (struct intervention*)malloc(sizeof(struct intervention));
        interventie->incident = incident->adresa;
        interventie->unit = unitate->adresa;
        interventie->next = NULL;

        free(incident);
        free(unitate);

        //adaugare in lista dublu inlantuita
        struct intervention *aux = list->interventions->prev;
        aux->next = interventie;
        interventie->prev = aux;
        interventie->next = list->interventions;
        list->interventions->prev = interventie;

        //salvare interventie in stiva
        struct nod_stack_interventii *interventie_stack = (struct nod_stack_interventii*)malloc(sizeof(struct nod_stack_interventii));
        interventie_stack->adresa = interventie;
        interventie_stack->next = list->istoric->top;
        list->istoric->top = interventie_stack;

    }    
}

void UNDO_LAST_DISPATCH(struct system *list, FILE *out)
{
    struct nod_stack_interventii *undo = list->istoric->top;

    //scoaterea de interventii finalizate
    while (undo != NULL && strcmp(undo->adresa->incident->status, "solved") == 0){
        list->istoric->top = undo->next;
        free(undo);
        undo = list->istoric->top;
    }

    if (undo ==NULL){
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
        return;
    }

    //anularea interventiei extrase
    if (strcmp(undo->adresa->incident->status, "intervened") == 0){
        strcpy(undo->adresa->incident->status, "queued");

        struct nod_queue_incidente *nou = (struct nod_queue_incidente*)malloc(sizeof(struct nod_queue_incidente));
        nou->adresa = undo->adresa->incident;

        //adaugare incident la inceputul cozii
        if (strcmp(undo->adresa->incident->priority, "high") == 0){
            nou->next = list->queue_high->head;
            list->queue_high->head = nou;
            if(list->queue_high->tail == NULL) list->queue_high->tail = nou;
        } else if(strcmp(undo->adresa->incident->priority, "medium") == 0){
                nou->next = list->queue_medium->head;
                list->queue_medium->head = nou;
                if(list->queue_medium->tail == NULL) { 
                    list->queue_medium->tail = nou;
                }
        } else if(strcmp(undo->adresa->incident->priority, "low") == 0){
                nou->next = list->queue_low->head;
                list->queue_low->head = nou;
                if(list->queue_low->tail == NULL) {
                    list->queue_low->tail = nou;
                }
        }

        //adaugare echipaj la finalul cozii de disponibilitate
        undo->adresa->unit->availability = 1;
        add_unitati(list->queue_available_units, undo->adresa->unit);

        //stergerea din lista dublu inlantuita a interventiilor
        struct intervention *del = undo->adresa;
        del->prev->next = del->next;
        del->next->prev = del->prev;
        free(del);

        list->istoric->top = undo->next;
        //scoatere din stiva
        free(undo);
    }
}


void SOLVED_INCIDENT(struct system *list, int id, FILE *out)
{
    struct intervention *curent = list->interventions->next;
    int ok = 0;
    while (ok == 0 && curent != list->interventions){
        if (curent->incident->id == id){
            if (strcmp(curent->incident->status, "intervened") == 0){
                strcpy(curent->incident->status, "solved");
                curent->unit->availability = 1;
                add_unitati(list->queue_available_units, curent->unit);
                ok = 1;
            } else {
                fprintf(out, "INVALID OPERATION! ERROR 404\n");
                ok = 1;
            }
        }
        curent = curent->next;
    }

    if (ok == 0){
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
        return;
    }
}

void SHOW_UNIT(struct system *list, int id, int nr, FILE *out)
{
    int ok = 0;
    for (int i = 0; i < nr && ok == 0; i++){
        if (list->units[i].id == id){
            char availability[12];
            if (list->units[i].availability == 0){
                strcpy(availability, "unavailable");
            } else {
                strcpy(availability, "available");
            }
            fprintf(out, "Unit %d is type %c and is %s\n", id, list->units[i].type, availability);
            ok = 1;
        }
    }

    if (ok == 0){
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
    }
}

void SHOW_INCIDENT(struct system *list, int id, FILE *out)
{
    struct incident *curent = list->santinela->next;
    int ok = 0;
    while (ok == 0 && curent != list->santinela){
        if (curent->id == id){
            fprintf(out, "Incident %d has %s priority, the following description: \"%s\" and is %s\n", id, curent->priority, curent->description, curent->status);
            ok = 1;
        }
        curent = curent->next;
    }

    if (ok == 0){
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
    }
}

void SHOW_INTERVENTIONS(struct system *list, FILE *out)
{
    struct intervention *curent = list->interventions->next;

    if (list->interventions->next == list->interventions){
        fprintf(out, "No intervention has been initiated\n");
    } else {
        while (curent != list->interventions){
            fprintf(out, "Incident %d was assigned to unit %d, and has the following status: \"%s\"\n", curent->incident->id, curent->unit->id, curent->incident->status);
            curent = curent->next;
        }
    }
}

//eliberare de memorie
void free_all(struct system *list)
{
    //istoric
    struct nod_stack_interventii *curent1 = list->istoric->top;
    struct nod_stack_interventii *aux1 = NULL;
    while (curent1 != NULL){
        aux1 = curent1;
        curent1 = curent1->next;
        free(aux1);
    }
    free(list->istoric);
    
    //available units
    struct nod_queue_unitati *curent2 = list->queue_available_units->head;
    struct nod_queue_unitati *aux2 = NULL;
    while (curent2 != NULL){
        aux2 = curent2;
        curent2 = curent2->next;
        free(aux2);
    }
    free(list->queue_available_units);
    
    //high
    struct nod_queue_incidente *curent3 = list->queue_high->head;
    struct nod_queue_incidente *aux3 = NULL;
    while (curent3 != NULL){
        aux3 = curent3;
        curent3 = curent3->next;
        free(aux3);
    }
    free(list->queue_high);
    
    //medium
    struct nod_queue_incidente *curent4 = list->queue_medium->head;
    struct nod_queue_incidente *aux4 = NULL;
    while (curent4 != NULL){
        aux4 = curent4;
        curent4 = curent4->next;
        free(aux4);
    }
    free(list->queue_medium);
    
    //low
    struct nod_queue_incidente *curent5 = list->queue_low->head;
    struct nod_queue_incidente *aux5 = NULL;
    while (curent5 != NULL){
        aux5 = curent5;
        curent5 = curent5->next;
        free(aux5);
    }
    free(list->queue_low);

    //interventions
    struct intervention *curent6 = list->interventions->next;
    struct intervention *aux6 = NULL;
    while (curent6 != list->interventions){
        aux6 = curent6;
        curent6 = curent6->next;
        free(aux6);
    }
    free(list->interventions);

    //incidents
    struct incident *curent7 = list->santinela->next;
    struct incident *aux7 = NULL;
    while (curent7 != list->santinela){
        aux7 = curent7;
        curent7 = curent7->next;
        free(aux7->description); //pt ca am folosit malloc in functie
        free(aux7);
    }
    free(list->santinela->description);
    free(list->santinela);
    free(list);

}

int main()
{
    int i, n, id, nr=0;
    char type, priority[7], *description, aux[101];
    struct unit unitati[51], unitate;
    struct system *list;

    FILE *in = fopen("tema1.in", "r");
    FILE *out = fopen("tema1.out", "w");

    list = (struct system*)malloc(sizeof(struct system));

    //construire santinelei pt incidente
    list->santinela = (struct incident*)malloc(sizeof(struct incident));
    list->santinela->id = 0;
    strcpy(list->santinela->priority, "low");
    list->santinela->description = (char*)malloc(strlen("test incident") + 1);
    strcpy(list->santinela->description, "test incident");
    strcpy(list->santinela->status, "solved");
    list->santinela->next = list->santinela;
    list->santinela->prev = list->santinela;
    list->incidents = list->santinela;

    //construirea santinelei pentru interventii
    list->interventions = (struct intervention *)malloc(sizeof(struct intervention));
    list->interventions->incident = NULL;
    list->interventions->unit = NULL;
    list->interventions->next = list->interventions;
    list->interventions->prev = list->interventions;

    //alocare memorie
    list->queue_high = (struct queue_incidente*)malloc(sizeof(struct queue_incidente));
    list->queue_medium = (struct queue_incidente*)malloc(sizeof(struct queue_incidente));
    list->queue_low = (struct queue_incidente*)malloc(sizeof(struct queue_incidente));

    list->queue_high->head = NULL;
    list->queue_high->tail = NULL;

    list->queue_medium->head = NULL;
    list->queue_medium->tail = NULL;

    list->queue_low->head = NULL;
    list->queue_low->tail = NULL;

    list->istoric = (struct stack_interventii*)malloc(sizeof(struct stack_interventii));
    list->istoric->top = NULL;

    fscanf(in, "%d", &n);
    
    list->queue_available_units = (struct queue_unitati*)malloc(sizeof(struct queue_unitati));
    list->queue_available_units->head = NULL;
    list->queue_available_units->tail = NULL;

    //initializare vector
    for (i = 0; i < n; i++){
        fscanf(in, "%d", &id);
        fgetc(in);
        fscanf(in, "%c", &type);
        unitate.id = id;
        unitate.type = type;
        unitate.availability = 1;
        unitati[nr] = unitate;
        add_unitati(list->queue_available_units, &unitati[nr]);
        nr++;
        
    }
    list->units = unitati;
    char command[41];
    fscanf(in, "%d", &n);

    for (i = 0; i < n; i++){
        fscanf(in, "%s", command);
        if (strcmp(command, "ADD_INCIDENT") == 0){
            fscanf(in, "%d", &id);
            fscanf(in, "%s", priority);
            fgets(aux, 101, in);

            //sterge ghilimelele si spatiile libere
            int len = strlen(aux);
            for (int j = 0; j < len; j++) {
                aux[j] = aux[j+2]; 
            }
            aux[strlen(aux)-2]='\0';
            ADD_INCIDENT(list, id, priority, aux);
        } else if(strcmp(command, "CHECK_UNITS_AVAILABILITY") == 0){
            CHECK_UNITS_AVAILABILITY(list->queue_available_units, out);
        } else if(strcmp(command, "DISPATCH") == 0){
            DISPATCH(list, out);
        } else if(strcmp(command, "UNDO_LAST_DISPATCH") == 0){
            UNDO_LAST_DISPATCH(list, out);
        } else if(strcmp(command, "SOLVED_INCIDENT") == 0){
            fscanf(in, "%d", &id);
            SOLVED_INCIDENT(list, id, out);
        } else if(strcmp(command, "SHOW_UNIT") == 0){
            fscanf(in, "%d", &id);
            SHOW_UNIT(list, id, nr, out);
        } else if(strcmp(command, "SHOW_INCIDENT") == 0){
            fscanf(in, "%d", &id);
            SHOW_INCIDENT(list, id, out);
        } else if(strcmp(command, "SHOW_INTERVENTIONS") == 0){
            SHOW_INTERVENTIONS(list, out);
        }
    }
    fclose(in);
    fclose(out);
    free_all(list);
    return 0;
}
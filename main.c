#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Definizione dei limiti degli array
#define MAX_AUTOMOBILI 512

// Definizione dei colori dei nodi
enum Color {RED, BLACK};

// Definizione della struttura del nodo
struct Stazione {
    int distanza;
    enum Color color;
    int automobili[MAX_AUTOMOBILI];
    struct Stazione* parent;
    struct Stazione* left;
    struct Stazione* right;
};

typedef struct Stazione Stazione;

/*
/ PROTOTIPI DELLE FUNZIONI
*/

Stazione* nuovaStazione(const int parameters[]);
void aggiungiStazione(Stazione** root, int parameters[]);
void rotazioneSinistra(Stazione** root, Stazione* nodo);
void rotazioneDestra(Stazione** root, Stazione* nodo);
void insertFixup(Stazione** root, Stazione* z);
void demolisciStazione(Stazione** root, int parameters[]);
void cancellaFixUp(Stazione** root, Stazione* nodo, Stazione* padre, int figlioSinistro);
void traspostaNodi(Stazione** root, Stazione* u, Stazione* v);
Stazione* minimoNodo(Stazione* node);
Stazione* nodoSuccessivo(Stazione* x);
Stazione* stazioneVicinaMaggiore(Stazione* root, int distanza);
Stazione* nodoPrecedente(Stazione** root, Stazione* nodoAttuale);
Stazione* ricercaNodo(Stazione* root, int distanza);
int calcolaDistanza(Stazione* stazione1, Stazione* stazione2);
void pianificaPercorso(Stazione** root, const int parameters[]);

void ordinaArray(Stazione** root, int distanza);
int massimoArray(const int array[], int dimensione);
int aggiungiTappa(int arr[], int kmTappa);
void aggiungiAuto(Stazione** root, int distanza, int autonomia, bool stampa);
void rottamaAuto(Stazione** root, int kmStazione, int autonomia, bool stampa);


// Creazione di un nuovo nodo (= nuova stazione)
Stazione* nuovaStazione(const int parameters[]) {
    Stazione* nuovaStazione = (Stazione*)malloc(sizeof(Stazione));
    nuovaStazione->distanza = parameters[0];
    nuovaStazione->color = RED; // I nuovi nodi vengono inizializzati come rossi
    // Controllo sia che i sia minore della lunghezza dell'array
    // sia che vengano inserite esattamente le auto indicate
    // es: se ho 1 auto da inserire leggerò fino a i = 1+2 = 3
    for(int i=0; i< MAX_AUTOMOBILI; i++){
        // Se l'autonomia è >= 0 e devo ancora inserirne inserisco
        if(i < parameters[1] && parameters[i+2] >= 0){
            nuovaStazione->automobili[i] = parameters[i+2];
        }
        // Se sono oltre riempio il resto dell'array con -1
        else if(i >= parameters[1]) {
            nuovaStazione->automobili[i] = -1;
        }
        // Altrimenti c'è qualcosa che non va, es non ho abbastanza
        // autonomie (valide) per il numero di auto che dovrei inserire
        // -> cancello il nodo e ritorno NULL
        else{
            free(nuovaStazione);
            return NULL;
        }
    }
    nuovaStazione->parent = NULL;
    nuovaStazione->left = NULL;
    nuovaStazione->right = NULL;
    return nuovaStazione;
}

// Inserimento di un nodo nell'albero rosso-nero
void aggiungiStazione(Stazione** root, int parameters[]) {
    // Cerca se già presente
    Stazione* found = ricercaNodo(*root,parameters[0]);
    // Se già presente stampa "non aggiunta"
    if(!(found == NULL)) {
        printf("non aggiunta\n");
        return;
    }

    // Se ci sono errori nella formattazione non aggiungere
    Stazione *newNode = nuovaStazione(parameters);
    if(newNode == NULL){
        printf("non aggiunta \n");
        return;
    }

    // Altrimenti aggiungi la stazione:
    // Se la radice è NULL, rendi il nodo radice
    if (*root == NULL) {
        newNode->color = BLACK;
        (*root) = newNode;
    } else{
        Stazione* y = NULL;
        Stazione* x = *root;

        while (x != NULL) {
            y = x;
            if (newNode->distanza < x->distanza) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        newNode->parent = y;

        if (y == NULL) {
            *root = newNode;
        } else if (newNode->distanza < y->distanza) {
            y->left = newNode;
        } else {
            y->right = newNode;
        }
        insertFixup(root, newNode);}
    printf("aggiunta\n");
}

// Rotazione sinistra del nodo passato come argomento
void rotazioneSinistra(Stazione** root, Stazione* nodo) {
    Stazione* y = nodo->right;
    nodo->right = y->left;
    if (y->left != NULL) {
        y->left->parent = nodo;
    }
    y->parent = nodo->parent;
    if (nodo->parent == NULL) {
        *root = y;
    } else if (nodo == nodo->parent->left) {
        nodo->parent->left = y;
    } else {
        nodo->parent->right = y;
    }
    y->left = nodo;
    nodo->parent = y;
}

// Rotazione destra del nodo passato come argomento
void rotazioneDestra(Stazione** root, Stazione* nodo) {
    Stazione* x = nodo->left;
    nodo->left = x->right;
    if (x->right != NULL) {
        x->right->parent = nodo;
    }
    x->parent = nodo->parent;
    if (nodo->parent == NULL) {
        *root = x;
    } else if (nodo == nodo->parent->left) {
        nodo->parent->left = x;
    } else {
        nodo->parent->right = x;
    }
    x->right = nodo;
    nodo->parent = x;
}

// Funzione per riordinare l'albero dopo l'inserimento
void insertFixup(Stazione** root, Stazione* z) {
    while (z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Stazione* y = z->parent->parent->right;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotazioneSinistra(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotazioneDestra(root, z->parent->parent);
            }
        } else {
            Stazione* y = z->parent->parent->left; // Simmetrico: left <-> right
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotazioneDestra(root, z); // Simmetrico: right <-> left
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotazioneSinistra(root, z->parent->parent); // Simmetrico: rotazioneSinistra <-> rotazioneDestra
            }
        }
    }
    (*root)->color = BLACK;
}

// Funzione per rimuovere una stazione (nodo) dall'albero
void demolisciStazione(Stazione** root, int parameters[]) {
    Stazione* z = ricercaNodo(*root, parameters[0]);
    if (z == NULL) {
        printf("non demolita\n");
        return;  // Node not found
    }
    Stazione* y = z;
    enum Color yOriginalColor = y->color;
    Stazione* x;

    if (z->left == NULL) {
        x = z->right;
        traspostaNodi(root, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        traspostaNodi(root, z, z->left);
    } else {
        y = minimoNodo(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x != NULL) {
                x->parent = y;
            }
        } else {
            traspostaNodi(root, y, y->right);
            y->right = z->right;
            if (y->right != NULL) {
                y->right->parent = y;
            }
        }
        traspostaNodi(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    free(z);
    if (yOriginalColor == BLACK) {
        cancellaFixUp(root, x, x != NULL ? x->parent : NULL, x == y->left);
    }
    printf("demolita\n");
}

// Funzione di fixup per sistemare i colori dopo la cancellazione
void cancellaFixUp(Stazione** root, Stazione* nodo, Stazione* padre, int figlioSinistro) {
    Stazione* w;

    while (nodo != *root && (nodo == NULL || nodo->color == BLACK) && padre != NULL) {
        if (figlioSinistro) {
            w = padre->right;
            if(w == NULL){
                break;
            }
            if (w->color == RED) {
                w->color = BLACK;
                padre->color = RED;
                rotazioneSinistra(root, padre);
                w = padre->right;
                if(w == NULL){
                    break;
                }
            }
            if ((w->left == NULL || w->left->color == BLACK) && (w->right == NULL || w->right->color == BLACK)) {
                w->color = RED;
                nodo = padre;
                padre = nodo->parent;
                figlioSinistro = (nodo == NULL) ? 0 : (nodo == padre->left);
            } else {
                if (w->right == NULL || w->right->color == BLACK) {
                    if (w->left != NULL) {
                        w->left->color = BLACK;
                    }
                    w->color = RED;
                    rotazioneDestra(root, w);
                    w = padre->right;
                }
                w->color = padre->color;
                padre->color = BLACK;
                if (w->right != NULL) {
                    w->right->color = BLACK;
                }
                rotazioneSinistra(root, padre);
                nodo = *root;
                padre = NULL;
            }
        } else {
            w = padre->left; // Symmetric case
            if(w == NULL){
                break;
            }
            if (w->color == RED) {
                w->color = BLACK;
                padre->color = RED;
                rotazioneDestra(root, padre);
                w = padre->left;
                if(w == NULL){
                    break;
                }
            }
            if ((w->right == NULL || w->right->color == BLACK) && (w->left == NULL || w->left->color == BLACK)) {
                w->color = RED;
                nodo = padre;
                padre = nodo->parent;
                figlioSinistro = (nodo == NULL) ? 0 : (nodo == padre->left);
            } else {
                if (w->left == NULL || w->left->color == BLACK) {
                    if (w->right != NULL) {
                        w->right->color = BLACK;
                    }
                    w->color = RED;
                    rotazioneSinistra(root, w);
                    w = padre->left;
                }
                w->color = padre->color;
                padre->color = BLACK;
                if (w->left != NULL) {
                    w->left->color = BLACK;
                }
                rotazioneDestra(root, padre);
                nodo = *root;
                padre = NULL;
            }
        }
    }
    if (nodo != NULL) {
        nodo->color = BLACK;
    }
}

// Funzione di "trapianto" dei nodi durante la cancellazione
void traspostaNodi(Stazione** root, Stazione* u, Stazione* v) {
    if (u->parent == NULL) {
        *root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}

// Funzione di ricerca del nodo minimo nell'albero
Stazione* minimoNodo(Stazione* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Funzione per trovare la stazione successiva
Stazione* nodoSuccessivo(Stazione* x) {
    if (x->right != NULL) {
        return minimoNodo(x->right);
    }
    Stazione* y = x->parent;
    while (y != NULL && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y;
}

// Funzione per trovare il nodo con il valore più vicino (maggiore)
Stazione* stazioneVicinaMaggiore(Stazione* root, int distanza) {
    Stazione* nodoVicino = NULL;
    Stazione* nodoCorrente = root;

    while (nodoCorrente != NULL) {
        if(nodoCorrente->distanza == distanza){
            nodoVicino = nodoCorrente;
            return nodoVicino;
        } else if (nodoCorrente->distanza > distanza) {
            nodoVicino = nodoCorrente;
            nodoCorrente = nodoCorrente->left;
        } else {
            nodoCorrente = nodoCorrente->right;
        }
    }
    return nodoVicino;
}

// Funzione per trovare la stazione precedente
Stazione* nodoPrecedente(Stazione** root, Stazione* nodoAttuale) {
    Stazione* current = *root;
    Stazione* result = NULL;

    while (current != NULL) {
        if (current->distanza < nodoAttuale->distanza) {
            result = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }
    return result;
}

// Funzione per cercare il nodo corrispondente alla distanza passata come parametro
Stazione* ricercaNodo(Stazione* root, int distanza) {
    while (root != NULL && root->distanza != distanza) {
        if (distanza < root->distanza)
            root = root->left;
        else
            root = root->right;
    }
    return root; // Restituirà NULL se il valore cercato non è stato trovato
}

// Calcolo della distanza (in valore assoluto) tra due stazioni
int calcolaDistanza(Stazione* stazione1, Stazione* stazione2) {
    return abs(stazione1->distanza - stazione2->distanza);
}

// Funzione per pianificare il percorso tra due stazioni
void pianificaPercorso(Stazione** root, const int parameters[]) {
    int kmPartenza = parameters[0];
    int kmArrivo = parameters[1];
    int tappe[100];
    for(int i=0;i<100;i++){
        tappe[i] = -1;
    }
    int autonomiaMassima;
    bool forward;

    // Cerca le stazioni di partenza e arrivo
    struct Stazione* stazionePartenza = ricercaNodo(*root, kmPartenza);
    struct Stazione* stazioneArrivo = ricercaNodo(*root, kmArrivo);

    // Se la stazione di partenza o quella di arrivo non esistono non ho percorsi
    if (stazionePartenza == NULL || stazioneArrivo == NULL) {
        printf("nessun percorso\n");
        return;
    }

    // Calcolo la distanzaTotale da percorrere
    int distanzaTotale = calcolaDistanza(stazionePartenza, stazioneArrivo);

    // CASO SEMPLICE: Posso arrivare senza soste:
    autonomiaMassima = massimoArray(stazionePartenza->automobili,MAX_AUTOMOBILI);
    if(autonomiaMassima >= distanzaTotale){
        tappe[0] = kmPartenza;
        tappe[1] = kmArrivo;
        printf("%d %d\n",tappe[0],tappe[1]);    // Stampo al contrario le tappe
        return;
    }
    else{
        // CASO MENO SEMPLICE: Calcolo a ritroso se ho autonomia a sufficienza
        Stazione* stazioneAttuale = NULL;
        Stazione* ultimaSosta = stazioneArrivo; // Ultima stazione in cui ho fatto una sosta
        // IN AVANTI
        if (stazioneArrivo->distanza > stazionePartenza->distanza) {
            forward = true;
            tappe[0] = kmArrivo;
            stazioneAttuale = stazioneArrivo;   // Parto dalla stazione di arrivo
            while (stazioneAttuale->distanza > stazionePartenza->distanza && stazioneAttuale != NULL) {
                int distanza;
                int autonomia;

                // Cerco la stazione la stazione più lontana dall'arrivo con autonomia sufficiente (se esiste)
                Stazione* stazioneProva = ultimaSosta;
                while(stazioneProva->distanza > stazionePartenza->distanza) {
                    // Calcolo l'autonomia da questa stazione all'ultima sosta
                    autonomia = massimoArray(stazioneProva->automobili, MAX_AUTOMOBILI);
                    distanza = calcolaDistanza(stazioneProva, ultimaSosta);
                    // Se ho autonomia sufficiente inizio a salvare la stazione
                    if(autonomia >= distanza){
                        stazioneAttuale = stazioneProva;
                    }
                    // Controllo anche col nodo precedente
                    stazioneProva = nodoPrecedente(root, stazioneProva);
                    if(stazioneProva == stazionePartenza){
                        // Calcolo l'autonomia da questa stazione all'ultima sosta
                        autonomia = massimoArray(stazioneProva->automobili, MAX_AUTOMOBILI);
                        distanza = calcolaDistanza(stazioneProva, ultimaSosta);
                        // Se ho autonomia sufficiente inizio a salvare la stazione
                        if(autonomia >= distanza){
                            stazioneAttuale = stazioneProva;
                        }
                    }
                }
                // Se la stazione attuale è ancora quella dell'ultima sosta significa che non ho più autonomia
                if(stazioneAttuale == ultimaSosta){
                    printf("nessun percorso\n");
                    return;
                }
                    // Se ho scansionato tutto fino alla partenza faccio il controllo specifico
                else if(stazioneAttuale == stazionePartenza){
                    autonomia = massimoArray(stazioneAttuale->automobili, MAX_AUTOMOBILI);
                    distanza = calcolaDistanza(stazioneAttuale, ultimaSosta);
                    // Se l'autonomia dalla stazione di partenza non basta, non ho percorsi
                    if (!(autonomia >= distanza)) {
                        printf("nessun percorso\n");
                        return;
                    } else {
                        // Aggiungi la tappa all'array
                        aggiungiTappa(tappe, stazioneAttuale->distanza);
                    }
                }else{
                    // Aggiungi la tappa all'array
                    aggiungiTappa(tappe, stazioneAttuale->distanza);
                    ultimaSosta = stazioneAttuale;
                }
            }
        }
        // ALL'INDIETRO
        else if (stazioneArrivo->distanza < stazionePartenza->distanza) {
            forward = false;
            tappe[0] = stazionePartenza->distanza;
            stazioneAttuale = stazionePartenza;   // Parto dalla stazione di partenza
            ultimaSosta = stazionePartenza;
            while (stazioneAttuale->distanza > stazioneArrivo->distanza && stazioneAttuale != NULL) {
                int distanza;
                int autonomia;

                // Cerco la stazione la stazione più lontana dall'arrivo con autonomia sufficiente (se esiste)
                Stazione* stazioneProva = ultimaSosta;
                autonomia = massimoArray(ultimaSosta->automobili,MAX_AUTOMOBILI);
                distanza = abs(stazioneProva->distanza - ultimaSosta->distanza);
                while(autonomia >= distanza && stazioneProva != stazioneArrivo) {
                    // Fino a quale stazione arrivo attualmente?
                    int distanzaRaggiuntaAttuale = stazioneAttuale->distanza - massimoArray(stazioneAttuale->automobili, MAX_AUTOMOBILI);
                    Stazione* stazioneRaggiuntaAttuale = stazioneVicinaMaggiore(*root, distanzaRaggiuntaAttuale);
                    // Fino a quale stazione arrivo con la stazione che provo?
                    int nuovaDistanzaRaggiunta = stazioneProva->distanza - massimoArray(stazioneProva->automobili, MAX_AUTOMOBILI);
                    Stazione* nuovaStazioneRaggiunta = stazioneVicinaMaggiore(*root, nuovaDistanzaRaggiunta);
                    // Se riesco già ad arrivare alla stazione di arrivo scelgo la stazione con la distanza minore
                    if(stazioneRaggiuntaAttuale->distanza <= stazioneArrivo->distanza && nuovaStazioneRaggiunta->distanza <= stazioneArrivo->distanza){
                        stazioneAttuale = stazioneProva;
                    }
                    // Altrimenti scelgo la stazione che mi garantisce l'autonomia maggiore
                    else if(nuovaStazioneRaggiunta->distanza <= stazioneRaggiuntaAttuale->distanza) {
                        stazioneAttuale = stazioneProva;
                    }
                    // Passo al nodo precedente e aggiorno autonomia e distanza
                    stazioneProva = nodoPrecedente(root,stazioneProva);
                    autonomia = massimoArray(ultimaSosta->automobili,MAX_AUTOMOBILI);
                    distanza = abs(stazioneProva->distanza - ultimaSosta->distanza);
                    if(stazioneProva == stazioneArrivo){
                        // Se ci arrivo con l'autonomia salvo la stazione
                        if(autonomia >= distanza){
                            stazioneAttuale = stazioneProva;
                        }
                    }
                }
                // Se la stazione attuale è ancora quella dell'ultima sosta significa che non ho più autonomia
                if(stazioneAttuale == ultimaSosta){
                    printf("nessun percorso\n");
                    return;
                }else if(stazioneAttuale != ultimaSosta){
                    // Aggiungi la tappa all'array
                    aggiungiTappa(tappe, stazioneAttuale->distanza);
                    ultimaSosta = stazioneAttuale;
                }
            }
            for(int indiceTappa = 99; indiceTappa > 1; indiceTappa--){
                if (tappe[indiceTappa] != -1) {
                    Stazione *stazionePrePrecedente = ricercaNodo(*root, tappe[indiceTappa - 2]);
                    int autonomiaPrePrecedente = massimoArray(stazionePrePrecedente->automobili, MAX_AUTOMOBILI);
                    int distanzaRaggiuntaPrecedente = stazionePrePrecedente->distanza - autonomiaPrePrecedente;
                    Stazione *stazioneRaggiuntaPrecedente = stazioneVicinaMaggiore(*root, distanzaRaggiuntaPrecedente);
                    // Prima non ho scelto la stazione più lontana
                    stazioneAttuale = ricercaNodo(*root, tappe[indiceTappa]);
                    while (tappe[indiceTappa - 1] > stazioneRaggiuntaPrecedente->distanza) {
                        int autonomiaPrecedente = massimoArray(stazioneRaggiuntaPrecedente->automobili, MAX_AUTOMOBILI);
                        int distanzaPrecedente = calcolaDistanza(stazioneRaggiuntaPrecedente, stazioneAttuale);
                        if (distanzaPrecedente < autonomiaPrecedente) {
                            tappe[indiceTappa - 1] = stazioneRaggiuntaPrecedente->distanza;
                        } else {
                            stazioneRaggiuntaPrecedente = nodoSuccessivo(stazioneRaggiuntaPrecedente);
                        }
                    }
                }
            }
        }


        // Se ho solo una stazione non stampo
        if(tappe[1] == -1){
            printf("nessun percorso\n");
        }
        else {
            if (forward) {
                // Stampo a ritroso le tappe
                for (int i = 99; i >= 0; i--) {
                    if (tappe[i] != -1) {
                        printf("%d", tappe[i]);
                        if (i != 0) {
                            printf(" ");
                        }
                    }
                }
            } else {
                // Stampo in ordine le tappe
                for (int i = 0; i < 100; i++) {
                    if (tappe[i] != -1) {
                        printf("%d", tappe[i]);
                        if (tappe[i + 1] != -1) {
                            printf(" ");
                        }
                    }
                }
            }
            printf("\n");
        }
    }
}

////

// Funzione per riordinare l'array e avere i valori diversi da -1 davanti
void ordinaArray(Stazione** root, int distanza) {
    int count = 0;
    struct Stazione* stazione = ricercaNodo(*root,distanza);

    for (int i = 0; i < MAX_AUTOMOBILI; i++) {
        if (stazione->automobili[i] != -1) {
            int temp = stazione->automobili[count];
            stazione->automobili[count] = stazione->automobili[i];
            stazione->automobili[i] = temp;
            count++;
        }
    }
}

// Funzione che ritorna il valore massimo di un array
int massimoArray(const int array[], int dimensione) {
    int max = array[0]; // Assume il primo elemento come massimo iniziale

    for (int i = 1; i < dimensione; i++) {
        if (array[i] > max) {
            max = array[i]; // Aggiorna il massimo se trovi un valore maggiore
        }
    }

    return max;
}

// Funzione per aggiungere una tappa al relativo array
int aggiungiTappa(int arr[], int kmTappa) {
    int i;
    for (i = 0; i < 100; i++) {
        if (arr[i] == -1) { // Trova il primo slot libero (assumendo che 0 rappresenti uno slot libero)
            arr[i] = kmTappa; // Inserisci il nuovo elemento
            return i;
        }
    }

    if (i == 100) {
        printf("L'array è pieno, impossibile aggiungere l'elemento.\n");
        return -2;
    }
    return -1;
}

// Funzione per aggiungere un'auto a una stazione
void aggiungiAuto(Stazione** root, int distanza, int autonomia, bool stampa){
    struct Stazione* stazione = ricercaNodo(*root,distanza);
    // Se non esiste la stazione non si può aggiungere
    if(stazione == NULL){
        if(stampa) {
            printf("non aggiunta\n");
            return;
        }
    } else {
        for (int i = 0; i < MAX_AUTOMOBILI; i++) {
            // Inserisci nel primo slot libero nell'array (vuoto o con autonomia nulla)
            if (stazione->automobili[i] == -1 || stazione->automobili[i] == 0) {
                stazione->automobili[i] = autonomia;
                if(stampa)
                    printf("aggiunta\n");
                return;
            }
        }
    }
    // Se eccedo i limiti dell'array non posso aggiungere
    printf("Stazione piena!!\n");
}

// Funzione per aggiungere un'auto a una stazione
void rottamaAuto(Stazione** root, int kmStazione, int autonomia, bool stampa){
    struct Stazione* stazione = ricercaNodo(*root,kmStazione);
    // Se non esiste la stazione non si può rottamare
    if(stazione == NULL) {
        if (stampa){
            printf("non rottamata\n");
            return;
        }
    }
    for(int i=0; i<MAX_AUTOMOBILI;i++){
        // Trova il primo valore corrispondente nell'array e
        // sostituiscilo con -1, poi riordina l'array
        if(stazione->automobili[i] == autonomia){
            stazione->automobili[i] = -1;
            ordinaArray(root,kmStazione);
            if(stampa)
                printf("rottamata\n");
            return;
        }
    }
    // Se nessuno dei valori dell'array corrisponde non si può rottamare
    if(stampa)
        printf("non rottamata\n");
}



int main() {
    struct Stazione* root = NULL;

    while(1){
        // Dichiarazione variabili
        char input[(MAX_AUTOMOBILI + 2 + 50) * 10];         // Stringa contenente tutto l'input
        char command[50];                                   // Stringa con il comando
        int parameters[MAX_AUTOMOBILI + 2];                 // Array contenente i soli parametri (interi)
        char delimiters[] = " \t";                          // Delimitatori dei token (spazi e tabulazioni)
        char *token;
        int valore;                                         // Valore del singolo parametro
        int i = 0;                                          // Variabile generica per i cicli for

        // Inizializzazione array
        for(int i_array = 0; i_array<MAX_AUTOMOBILI+2;i_array++){
            if(i_array < 100)
                input[i_array] = '\0';
            parameters[i_array] = -1;
        }

        // Scansione dell'input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // Primo token (tutto prima del primo spazio) salvato come comando
        token = strtok(input, delimiters);
        strcpy(command,token);

        // Scansione token per token e salvataggio
        while (token != NULL) {
            token = strtok(NULL, delimiters);           // Passaggio al prossimo token (= primo parametro)
            // Se il token non è nullo, viene convertito in intero e salvato nell'array parameters
            while (token != NULL && sscanf(token, "%d", &valore) == 1) {
                parameters[i] = valore;
                i++;
                token = strtok(NULL, delimiters);       // Passaggio al prossimo token
            }
            token = strtok(NULL, delimiters);           // Passaggio al prossimo token
        }

        // Chiamata a funzione a seconda del comando richiesto
        if(strcmp(command, "aggiungi-stazione") == 0){
            aggiungiStazione(&root,parameters);
        } else if(strcmp(command, "demolisci-stazione") == 0){
            demolisciStazione(&root, parameters);
        } else if(strcmp(command, "aggiungi-auto") == 0){
            aggiungiAuto(&root, parameters[0],parameters[1], true);
        } else if(strcmp(command, "rottama-auto") == 0){
            rottamaAuto(&root, parameters[0],parameters[1],true);
        } else if(strcmp(command, "pianifica-percorso") == 0){
            pianificaPercorso(&root, parameters);
        } else{}
    }
}

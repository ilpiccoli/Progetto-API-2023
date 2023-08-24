
# Progetto di API - AA 2022/23
Progetto di Algoritmi e Strutture Dati realizzato per il corso "PROVA FINALE (PROGETTO DI ALGORITMI E STRUTTURE DATI)" del Politecnico di Milano nell'anno accademico 2022/2023. 
Voto: 30 e lode.

### Prestazioni

<table>
  <tr>
    <th></th>
    <th>Tempo impiegato [s]</th>
    <th>Tempo massimo per passare il task [s]</th>
    <th>Memoria occupata [MB]</th>
    <th>Limite memoria per passare il task [MB]</th>
  </tr>
  <tr>
    <th>UPTO18</th>
    <th>0,799</th>
    <th>19</th>
    <th>40</th>
    <th>128</th>
  </tr>
  <tr>
    <th>UPTO21</th>
    <th>0,813</th>
    <th>15</th>
    <th>40,1</th>
    <th>118</th>
  </tr>
  <tr>
    <th>UPTO24</th>
    <th>0,788</th>
    <th>10</th>
    <th>40</th>
    <th>108</th>
  </tr>
  <tr>
    <th>UPTO27</th>
    <th>0,811</th>
    <th>6</th>
    <th>40</th>
    <th>98</th>
  </tr>
  <tr>
    <th>UPTO30</th>
    <th>0,812</th>
    <th>4</th>
    <th>40</th>
    <th>88</th>
  </tr>
  <tr>
    <th>CUMLAUDE</th>
    <th>0,811</th>
    <th>1</th>
    <th>40</th>
    <th>78</th>
  </tr>
</table>


### Strutture dati utilizzate

```c
struct Stazione {
    int distanza;
    enum Color color;
    int automobili[MAX_AUTOMOBILI];
    struct Stazione* parent;
    struct Stazione* left;
    struct Stazione* right;
};

typedef struct Stazione Stazione;
```
<p>
  Per ottimizzare i tempi di ricerca e calcolo dei percorsi, le stazioni vengono salvate come nodi all'interno di un albero rosso/nero.
  Al suo interno è contenuto un array di interi che contiene le autonomie delle automobili.
</p>

### Specifiche e note
Nel file Specifiche.pdf è possibile trovare le specifiche e il funzionamento del progetto.
Nella cartella "Test" è possibile trovare tutti i test forniti dal verificatore per testare il funzionamento corretto del programma.
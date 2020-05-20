# CALCOLARE PI GRECO MEDIANTE I DUE METODI TRAPEZOIDE E MONTECARLO CON ISTANZE T2.XLARGE 


#### TRAPEZIOIDE

Per il calcolo di Pi Greco con il metodo trapezoide, ho sviluppato due soluzioni distinte. 
La prima soluzione con nome "trapezioPackUnpack.c" ho utilizzato le operazioni di base point-to-point Send, Receive e Pack,Unpack.
Nel main, quindi, ho utilizzato le chiamate send e receive, ho chiamato il metodo countTrapezio in cui ho fatto calcolare il valore di result, tali valori
li ho inviati al master tramite buffer con Pack.
Il master ricevuto il buffer e spacchettato tramite Unpack ha effettuato la somma e ha richiamato il metodo CalcolaPi, il quale calcolerà il valore finale 
di Pi Greco.
Mentre nel metodo countTrapezio verranno definiti gli intervalli in cui i vari processori dovranno operare in maniera parallela e dove ognuno calcolerà la sua parte.
 
In questa soluzione ho sviluppato anche un DEBUG per verificare gli intervalli e i valori calcolati dai vari processi. Basta settare la variabile globale DEBUG a TRUE.

Mentre nella seconda soluzione "trapezioReduce.c" ho utilizzato l'operazione di comunicazione collettiva MPI Reduce. 
Il codice risulta essere molto simile alla soluzione precedente, l'unica cosa che cambia è che non vengono più utilizzate le operazioni Send, Receive e Pack,Unpack, ma l'unica operazione Reduce. Anche qui ho sviluppato un DEBUG, in cui basta settare la variabile globale DEBUG a true. 

Tra le due soluzioni sceglierò la più veloce, la quale poi verrà utilizzata per i test finali e per il confronto con Montecarlo. 
Come definito dalla traccia ho utilizzato istanze t2.xlarge. 
In questo caso ho utilizzato 3 istanze totali. 

#### MONTECARLO
Nella soluzione "montecarlo.c" ho utilizzato l'operazione MPI_Reduce, ci sono le funzioni
CountMontecarlo il cui calcola il lavoro per ogni processore e provvederà a calcolare i numeri random, mentre la funzione calcolaPi in questo caso
calcolerà il valore finale di Pi Greco . Anche qui ho sviluppato un DEBUG, in cui basta settare la variabile globale DEBUG a true. 


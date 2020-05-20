~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TRACCIA~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CALCOLARE PI GRECO MEDIANTE I DUE METODI TRAPEZOIDE E MONTECARLO CON ISTANZE T2.XLARGE

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
------------------------------------------------------------------------------------------------------------------------------------------------------------
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TRAPEZIOIDE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Per il calcolo di Pi Greco con il metodo trapezoide, ho sviluppato due soluzioni distinte. 
La prima soluzione con nome "trapezioPackUnpack.c" ho utilizzato le operazioni di base point-to-point Send, Receive e Pack,Unpack.
Nel main, quindi, ho utilizzato le chiamate send e receive, ho chiamato il metodo countTrapezio in cui ho fatto calcolare il valore di result, tali valori
li ho inviati al master tramite buffer con Pack.
Il master ricevuto il buffer e spacchettato tramite Unpack ha effettuato la somma e ha richiamato il metodo CalcolaPi, il quale calcolerà il valore finale 
di Pi Greco.
Mentre nel metodo countTrapezio verranno definiti gli intervalli in cui i vari processori dovranno operare in maniera parallela e dove ognuno calcolerà la sua parte.
 
In questa soluzione ho sviluppato anche un DEBUG per verificare gli intervalli e i valori calcolati dai vari processi.
Basta settare la variabile globale DEBUG a TRUE.

Mentre nella seconda soluzione "trapezioReduce.c" ho utilizzato l'operazione di comunicazione collettiva MPI Reduce. 
Il codice risulta essere molto simile alla soluzione precedente, l'unica cosa che cambia è che non vengono più utilizzate le operazioni Send, Receive e Pack,Unpack, ma
l'unica operazione Reduce. Anche qui ho sviluppato un DEBUG, in cui basta settare la variabile globale DEBUG a true. 

Tra le due soluzioni sceglierò la più veloce, la quale poi verrà utilizzata per i test finali e per il confronto con Montecarlo. 

Come definito dalla traccia ho utilizzato istanze t2.xlarge. 
In questo caso ho utilizzato 3 istanze totali. 

-----------------------------------------------VALUTAZIONE TEMPO DI ESECUZIONE 1 SOLUZIONE , "trapezioPackUnpack"-----------------------------------------------


Per definire il tempo di esecuzione, farò un grafico dove sull'asse delle x denoterò il numero di Processori MPI e sull'asse delle y il tempo di esecuzione in
millisecondi.

Questa prima soluzione verrà compilata ed eseguita in questo modo:

 		- mpicc -fopenmp trapezio.c -o trapezioPackUnpack
		- mpirun -np numeridiprocessori[1 a 12] --hostfile machinefile ./trapezioPackUnpack

Il file machinefile è così definito:
-------------------------MACHINEFILE---------------------------------
		       localhost slots=4
		       172.31.46.127 slots=4
		       172.31.44.15 slots=4
---------------------------------------------------------------------

In questo caso ho utilizzato 3 istanze.

Prima però, per ogni np ho effettuato 5 esecuzioni e ho calcolato la media.

-  https://ibb.co/jZVMZC7


Riportando la media su un grafico risulta che : 

-  https://ibb.co/Tws1Z5j

Ovviamente come da suggerimento con numero di processori uguale al numero di core risulta essere più veloce. 

Il file trapezioPackUnpack.c è contenuto nella cartella trapezoide/TrapezioPackUnpack

-----------------------------------------------VALUTAZIONE TEMPO DI ESECUZIONE 1 SOLUZIONE , "TRAPEZIOREDUCE.C" -----------------------------------------------

Anche in questo caso ho costruito un grafico con asse x e y con i valori di media. 

Questa seconda soluzione verrà compilata ed eseguita in questo modo:

 		- mpicc -fopenmp trapezioReduce.c -o trapezioReduce
		- mpirun -np numeridiprocessori[1 a 12] --hostfile machinefile ./trapezioReduce

IL FILE MACHINEFILE E' DEFINITO SOPRA.

Le 5 esecuzioni e la media.

-  https://ibb.co/Z6tbfXY

Riportando la media su un grafico: 

-  (1) https://ibb.co/VBF9S4J


Ovviamente con numero di processori uguale al numero di core risulta essere più veloce. 
Il file trapezioReduce.c è contenuto nella cartella trapezoide/TrapezioReduce

---------------------------------------------------CONSIDERAZIONI FINALI--------------------------------------------------------------------

Quindi la soluzione trapezioReduce.c sebbene è molto simile a trapezioPackUnpack risulta essere leggermente più veloce,
per i test finali verrà presa essa in considerazione.

--------------------------------------------------------------------------------------------------------------------------------------------

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~MONTECARLO~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 

Nella soluzione "montecarlo.c" ho utilizzato l'operazione MPI_Reduce, ci sono le funzioni
CountMontecarlo il cui calcola il lavoro per ogni processore e provvederà a calcolare i numeri random, mentre la funzione calcolaPi in questo caso
calcolerà il valore finale di Pi Greco . Anche qui ho sviluppato un DEBUG, in cui basta settare la variabile globale DEBUG a true. 

Questa soluzione verrà compilata ed eseguita in questo modo:

 		- mpicc -fopenmp montecarlo.c -o montecarlo
		- mpirun -np numeridiprocessori[1 a 12] --hostfile machinefile ./montecarlo 10000000

(*10000000 per confrontare montecarlo e trapezioReduce con lo stesso numero di iterazioni)

Il file machinefile è così definito:
-------------------------MACHINEFILE---------------------------------
		       localhost slots=4
		       172.31.46.127 slots=4
		       172.31.44.15 slots=4
---------------------------------------------------------------------

Per ogni np ho effettuato 5 esecuzioni e ho calcolato la media.

-  https://ibb.co/NjNPmFV

Riportando la media su un grafico risulta che : 

-  (2) https://ibb.co/3MBZzQ1

Il file montecarlo.c è contenuto nella cartella Montecarlo

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
---------------------------------------------------CONSIDERAZIONI FINALI--------------------------------------------------------------------

La soluzione trapezioReduce.c risulta essere più veloce rispetto a montecarlo.c

Ora calcolo lo SPEEDUP, in cui considero il tempo di esecuzione in sequenziale / tempo di esecuzione in parallelo di trapezioReduce.c e montecarlo.c.


La media del tempo di esecuzione in sequenziale del metodo trapezioide è: 130,846 ms

La media del tempo di esecuzione in sequenziale del metodo montecarlo è: 261,37 ms


Ora calcolo lo speedup per entrambi i metodi

 - (3) https://ibb.co/Xjn67DY

Valutando i tempo di esecuzione di entrambi i metodi: 
- Dal Grafico 1 valutando i tempi di esecuzione il metodo trapezoide è più veloce e riesce a calcolare tutte le cifre decimali di Pi Greco.
- Dal Grafico 2 si evince che il metodo di montecarlo è più lento a causa dei grandi numeri casuali che deve sviluppare
- Dal Grafico 3 si nota che il metodo di montecarlo scala meglio all'aumentare del numero di processori. Inoltre per tale input e per 
numeri di processori più grandi lo speedup risulta non essere ottimale.

--------------------------------------------------------------------------------------------------------------------------------------------



---------------------------------------------------BENCHMARKS--------------------------------------------------------------------

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~STRONG SCALABILITY TRAPEZIOREDUCE.C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Per effettuare questo benchamrks ho utilizzato 5 istanze, quindi utilizzerò da 1 a 20 processi (4 VCPU x 5)

Ho dovuto utilizzare massimo 5 istanze (oltre ai problemi di credito), a causa di un problema che mi dava AWS, in cui non mi faceva
creare più di 5 istanze.

 - https://ibb.co/x8TfknX

Questa soluzione verrà compilata ed eseguita in questo modo:

 		- mpicc -fopenmp trapezioReduce.c -o trapezioReduce
		- mpirun -np numeridiprocessori[1 a 20] --hostfile machinefile ./trapezioReduce

Il file machinefile è così definito:
-------------------------MACHINEFILE---------------------------------
		       localhost slots=4
		       172.31.44.242 slots=4
                       172.31.46.186 slots=4
                       172.31.42.73 slots=4
                       172.31.45.120 slots=4
---------------------------------------------------------------------


Ho testato il tempo di esecuzione fino a 20 processi e li ho riportati in questa tabella:
 
 -  https://ibb.co/vHdpKnq

Ho riportato i valori su un grafico :

 - https://ibb.co/T0HQwqb

---------------------------------------------------CONSIDERAZIONI--------------------------------------------------------------------

Il grafico mostra che all'aumentare del numero di processori il tempo di esecuzione calcolato in ms risulta tendere al basso.

Si nota che il tempo di esecuzione, per renderlo più affidabile, è stato calcolato facendo la media di 5 esecuzioni distinte

Inoltre si evince che da 10 a 20 processori il tempo rimane più o meno costante, ciò denota l'inutilità di utilizzare da 10 a 20 processori . 
 

--------------------------------------------------------------------------------------------------------------------------------------------

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~STRONG SCALABILITY MONTECARLO.C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Anche in questo caso ho utilizzato 5 istanze 

Questa soluzione verrà compilata ed eseguita in questo modo:

 		- mpicc -fopenmp montecarlo.c -o montecarlo
		- mpirun -np numeridiprocessori[1 a 20] --hostfile machinefile ./montecarlo 10000000


Il file machinefile è così definito:
-------------------------MACHINEFILE---------------------------------
		       localhost slots=4
		       172.31.44.242 slots=4
                       172.31.46.186 slots=4
                       172.31.42.73 slots=4
                       172.31.45.120 slots=4
---------------------------------------------------------------------


Ho testato il tempo di esecuzione fino a 20 processi e li ho riportati in questa tabella:

 - https://ibb.co/qyByDyR

Ho riportato i valori su un grafico :

 - https://ibb.co/7KZbQyS

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
---------------------------------------------------CONSIDERAZIONI--------------------------------------------------------------------

Anche in questo caso, come il metodo trapezoide, all'aumentare del numero di processori il tempo di esecuzione diminuisce.

Però, da 9 a 20 processori il tempo non migliora in maniera significativa, ma risulta essere regolare, ciò evidenza il fatto che non sia produttivo l'utilizzo
di tali processori.


--------------------------------------------------------------------------------------------------------------------------------------------
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

WEAK SCALABILITY SUL METODO TRAPEDOIDE NON E' POSSIBILE EFFETTUARLO PERCHE' LA SOLUZIONE NON HA INPUT, LE ITERAZIONI SONO FISSATE. 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~WEAK SCALABILITY MONTECARLO.C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
La taglia per un processore è 1000000. 
Per 2 processori è 2000000.
Per 3 processori è 3000000 e così via.


Questa soluzione verrà compilata ed eseguita in questo modo (per 1 processore):

 		- mpicc -fopenmp montecarlo.c -o montecarlo
		- mpirun -np numeridiprocessori 1 --hostfile machinefile ./montecarlo 1000000 


Il file machinefile è così definito:
-------------------------MACHINEFILE---------------------------------
		       localhost slots=4
		       172.31.44.242 slots=4
                       172.31.46.186 slots=4
                       172.31.42.73 slots=4
                       172.31.45.120 slots=4
---------------------------------------------------------------------

Ho testato il tempo di esecuzione fino a 20 processi e li ho riportati in questa tabella:

 - https://ibb.co/60s2Xzr

Ho riportato i valori su un grafico :

 - https://ibb.co/TcDPVtS


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
---------------------------------------------------CONSIDERAZIONI--------------------------------------------------------------------

Il grafico, sebbene leggermente altalenante, evidenza un buon andamento del tempo di esecuzione (calcolato effettuando la media tra 5 esecuzione) 
all'aumentare la taglia dell'input e del numero di processori.
 
In generale, i tempi risultano rimanere costanti e variare di poco,
ciò significa che è possibile risolvere problemi più grandi utilizzando più processori, con un tempo non elevato. 

--------------------------------------------------------------------------------------------------------------------------------------------

---------------------------------------------------------------FINE BENCHMARK------------------------------------------------------------------------

NEL CASO IN CUI LE IMMAGINI NON VENISSERO APERTE SONO PRESENTI NELLA CARTELLA SUDDIVISE PER OGNI CASO. 
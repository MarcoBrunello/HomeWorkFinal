Per compilare il file homework: "gcc -Wall -o homework main.c communication.c output.c linkingList.c threads.c  -lpthread"
Per compilare il dispositivo remoto "gcc -Wall -o remoteDevice remoteDevice.c"

Il software dispone di 6 thread, model, controller, viewer e due interfacce (una per leggere i dati dal dispositivo locale e l'altra per ricevere dal dispositivo remoto),
pi� la thread per ascoltare il sigint. Avviare prima l'homework da linea di comando con i parametri come da specifica (altrimenti va in errore e termina), i tempi sono
calcolati in secondi (� stato usato uno sleep) quindi evitare di dare tempi cicli troppo lunghi per avere un output ragionevole (da 1 a 5 va benissimo, se infatti � troppo 
alto ho impostato che quando terminano entrambe le letture venga inizializzata la chiusura, e se le thread dormono non vengono visualizzati tutti i dati).
All'avvio il file homework comincer� a leggere i dati dal file device.txt mostrando in maniera approssimativa la posizione rispetto le due pareti date, se 
la posizione � fuori le pareti mostrer� la X sulla parete (la posizione reale viene scritta accompagnata dal tempo).  L'output del controller invece sar� visualizzato 
nel file output.txt nella directory del file compilato. Se tutto � andato bene all'avvio comparir� "Server Ready" ad indicare che si � in grado di ricevere posizioni da 
un dispositivo connesso in remoto, in quel caso � possibile avviare da un altro terminale il file remoteDevice che manda le sue posizioni lette dal file remoteDevice.txt, 
in questo caso le posizioni del dispositivo locale non sono mostrate e nemmeno controllate (supponendo che sia il fisioterapista a correggere il movimento e permetta ad 
un ulteriore dispositivo remoto di essere controllato), mentre vengono mostrate (e corrette) solo le posizioni remote (N.B. le posizioni locali, nonch� i tempi, 
sono comunque salvati dal model e se si disconnette il dispositivo remoto si prosegue a visualizzare le posizioni locali dal tempo raggiunto in remoto) . 
Per avviare il file remoto aprire un altro terminale e avviare "./remoteDevice". In assenza di parametri prover� a connettersi a localhost (127.0.0.1), 
altrimenti si pu� inserire un indirizzo IP (es ./remoteDevice 55.66.77.88). Da quel momento verranno visualizzate le posizioni (e i tempi) dal dispositivo remoto,
 il cui tempo 0 � quando si � iniziato a trasmettere. Quando termina la lettura da remoto, la thread si chiude e tornano ad essere visualizzate le posizioni locali 
(supponendo che a questo punto il fisioterapista non controlli pi� e lasci fare alla macchina, avendo disconnesso un dispositivo remoto). 
Il programma terminer� autonomamente se � stato connesso un dispositivo remoto e la lettura locale � terminata. In caso di sola lettura locale il programma 
non termina, ma rimane in attesa di una connessione remota da mostrare, in quel caso basta un Sigint per terminare il tutto, 
(ho supposto il sigint come tasto di spegnimento). Durante l'esecuzione � sempre possibile mandare un sigint, tutte le thread si chiuderanno e i buffer (liste linkate)
verranno distrutte e deallocate,  il programma terminer�. 
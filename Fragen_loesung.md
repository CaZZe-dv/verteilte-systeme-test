1. Analysieren sie folgendes C-Promm. Welche Fehler können Sie identifizieren (Zeilennummer + Fehlerbeschreibung)? Sie müssen den Code nicht ändern oder erweitern, sondern nur die Fehler beschreiben und die resultierenden möglichen Probleme aufzeigen.

``` cpp
#include "myheader.h" 
#define BUF 1024
#define PORT 6543

int main(void){
    int cs, ns;
    socklen_t addrlen;
    char buffer[BUF];
    struct sockaddr_in addr, cl;
    pid_t pid;

    cs = socket(AF_INET, SOCK_STREAM,0);

    addr.sin_family = AF_INET;

    addr.sin_addr.s_addr = inet_addr("127.0.0.1");//addr.sin_addr.s_addr = "127.0.0.1";
    
    addr.sin_port = htons(PORT);//addr.sin_port = PORT;

    bind(cs,(struct sockaddr*)&addr,sizeof(addr));//Error handling? 

    listen(cs,5);//Error handling? 

    addrlen = sizeof(struct sockaddr_in);

    while(1){//Endlosschleife while(1)?
        printf("Waiting for connections\n");
        ns = accept(cs,(struct sockaddr*)&cl, &addrlen);
        if((pid=fork()) == 0){
        do{
            recv(ns,buffer,BUF-1,0);//recv(ns,buffer,BUF,0);
            buffer[BUF-1] = '\0';//buffer[BUF-1] = '\0';
            printf("Message received: %s",buffer);
        }while(strncmp(buffer, "quit",4) != 0);
        close(ns);//close(cs);
        }
    }
    close(cs);
    return EXIT_SUCCESS;
}
```
Die gegebene C++-Zeile versucht, einen String-Literal "127.0.0.1" einer Variable des falschen Typs zuzuweisen. Um eine IP-Adresse in einer sockaddr_in-Struktur zu setzen, sollte inet_addr oder inet_pton verwendet werden, um die Zeichenkette in das korrekte numerische Format umzuwandeln.

``` cpp
15 addr.sin_addr.s_addr = "127.0.0.1";
15 addr.sin_addr.s_addr = inet_addr("127.0.0.1");
```
Die Codezeile addr.sin_port = PORT;, wenn #define PORT 23123 verwendet wird, könnte aufgrund einer möglichen Überlaufsituation scheitern, da der Portwert außerhalb des gültigen Bereichs für einen 16-Bit-Port liegen könnte. Es wird empfohlen, htons(PORT) zu verwenden, um sicherzustellen, dass der Portwert im richtigen Netzwerkformat liegt und innerhalb des gültigen Bereichs für einen Port liegt.

``` cpp
16 addr.sin_port = PORT;
16 addr.sin_port = htons(PORT);
```
Hier sollte eventuell Error Handling eingebaut werden, bei einem Fehlschlag wird -1 zurückgegeben und das Programm soll abgebrochen werden ansonsten kann Programm weiterlaufen.

``` cpp
18 bind(cs,(struct sockaddr*)&addr,sizeof(addr));

if(bind(cs,(struct sockaddr*)&addr,sizeof(addr)) == -1){
    fprintf(stderr,"Error occoured while binding serversocket");
}
```
Auch hier wird kein Error Handling betrieben.

``` cpp
19 listen(cs,5);

if(listen(cs,5); == -1){
    fprintf(stderr,"Error occoured while listening for clients");
}
```
Endlosschleife macht in diesem Fall vielleicht wenig Sinn, hier würde ich Vorschlagen, einen Index zu erstellen, der bis 5 Clients hochzählt und dann, mit waitpid() warten bis alle Clients terminiert sind und dann serversocket schließen und mit EXIT_SUCCESS Programm beenden.

``` cpp
23 while(1){}
int i = 0;
while(i < 5){
    .
    .
    .
    i++;
}
int status;
pid_t child_pid;
while ((child_pid = waitpid(-1, &status, 0)) > 0) {
    if (WIFEXITED(status)) {
        std::cout << "Child process " << child_pid << " terminated with status: " << WEXITSTATUS(status) << std::endl;
    } else if (WIFSIGNALED(status)) {
        std::cout << "Child process " << child_pid << " terminated by signal: " << WTERMSIG(status) << std::endl;
    }
}
```
Das letzte Element enthält keinen Null Terminator, es könnte also zu undefiniertem Verhalten kommen, da auf Speicheradressen zugegriffen wird, auf die nicht zugegriffen werden sollte. Daher muss 1 vom Buffer abgezogen werden und das letzte Element mit einem NUll Terminator versehen werden.

``` cpp
28 recv(ns,buffer,BUF,0);
28 recv(ns,buffer,BUF-1,0);
29 buffer[BUF-1] = '\0';
```
Hier wird der Serversocket geschlossen obwohl man hier eigentlich den Client schließen möchte.

``` cpp
31 close(cs);
close(ns);
```

2. Klassifizieren Sie den Servertyp des untenstehenden C-Programms anhand der folgenden KAtegorien und begründen Sie Ihre Entscheidung:
- stateful/stateless
- connection oriented/connectionless
- iterative/concurrent

Stateful/Stateless:
The server maintains state by accepting multiple connections in a loop and creating a child process for each connection using fork(). Each child process handles its connection independently. The child processes share the same file descriptors, so they may share some state.
It's more accurate to say that each connection handled by a child process is stateful for the duration of that connection, but the overall server can be considered stateless between different connections.
Connection 

Oriented/Connectionless:
The server uses TCP (SOCK_STREAM), which is a connection-oriented protocol. This means that a reliable, two-way communication channel is established between the server and the client.

Iterative/Concurrent:
The server is concurrent because it uses fork() to create a new process for each incoming connection. This allows the server to handle multiple connections simultaneously.

In summary:
Stateful/Stateless: Stateless overall but stateful during each connection.
Connection Oriented/Connectionless: Connection-oriented (uses TCP).
Iterative/Concurrent: Concurrent (uses fork for handling multiple connections simultaneously).

---

NTP
Reorganisiren Sie das folgende NTP Netzwerk, sodass der Ausfall der Verbindungen (markiert mit einem dicken Querstrich) bestmöglich kompensiert wird:
- Kreis mit ZIffer: Knoten mit entsprechendem Stratum
- Pfeil: verbundener Peer
- Linie: mögliche Netzwerkverbindung

![NTP](./image.png)






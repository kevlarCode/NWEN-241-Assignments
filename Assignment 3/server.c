// Name : Kevin Lee
#include<stdio.h>
#include<string.h> //copy string 
#include<stdlib.h> //exit()
#include<sys/socket.h> //socket
#include<netinet/in.h> // socket address struct sockaddr_in 
#include<unistd.h> //read and write

/* Prints out the Errors
*/
void error(const char *msg)
{
    printf("Error: %s\n", msg);
    exit(1);
}


/* Sends the message to Client and handles send errors
*/
void sendToClient(const char *msg, int sockfd, int clientfd, const void *buf) 
{
    ssize_t toSend = send(clientfd, buf, strlen(buf), 0);
    if(toSend < 0) { // Failed to Send
        error(msg);
        close(clientfd);
        close(sockfd);
        exit(0);
    }
}

/* Reading Files and displaying the content in the Files
*/ 
void readFile (char fileName[], int clientfd, int sockfd)
{
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL) { // failed to Open
        sendToClient("No File Found", sockfd, clientfd, "SERVER 404 Not Found\n"); 
        return;
    }
    
    fseek(file, 0L, SEEK_END); // point to the end to find size of file
    long int fileSize = ftell(file); // determine file size for content Array
    fseek(file,0,SEEK_SET);// reset pointer back
    char ch; 
    char content[fileSize / sizeof(char)]; // set length for file contents
    memset(content, '\0', sizeof(content));
    int i = 0;
    while (!feof(file)) // grab all the contents of the file
    {
        ch = fgetc(file);
        content[i] = ch; 
        i++;
    }
    content[i-1] = '\0';
    // Sending the Contents of the file back to the client in correct format.
    sendToClient("", sockfd,clientfd, "SERVER 200 OK\n \n");
    sendToClient("", sockfd,clientfd, content);
    sendToClient("", sockfd,clientfd, "\n \n \n");
    fclose(file);
}

/* Main Function Recieves Portnum as arguement
*/
int main(int argc, char *argv[])
{
    if(argc != 2){
        error("Invalid input - please enter one valid port Number");
        return -1;
    }
    int portNumber;
    sscanf(argv[1], "%d",&portNumber); // set port number to be an int 
    if(portNumber < 1024){
        error("Access to port Denied");
        return -1;
    }
    // Step 1 - create a socket
    int sockfd;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        error("Error creating socket");
    }
    printf("Socket Created\n");
    
    //Step 2 - bind socket to an address + Define IP and Port
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(portNumber);
    printf("Address created\n");
    
    //bind address to socket
    int br;
    br = bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    if(br < 0){
        error("Error Binding");
    }
    printf("Bind successful\n");
    
    
    if(listen(sockfd,5)<0) {
        error("Litening error");
        return -1;
    }
    printf("Success listening\n");
    
    while(1) // Server Runs Forever
    {
        struct sockaddr_in clientaddr;
        int clienlen = sizeof(clientaddr);
        int clientfd = accept(sockfd,(struct sockaddr*)&clientaddr,(socklen_t*)&clientaddr);
        if(clientfd < 0){
            error("Error accepting client");
        }
        printf("Client connection accepted\n");
        // write to client
        char buffer[100];
        memset(buffer,0,100);
        strncpy(buffer,"HELLO FROM SERVER \n",strlen("HELLO FROM SERVER \n"));
        
        int s = write(clientfd,buffer,100);
        if( s< 0)
        error("Eror Writing to Client");
        printf("Write successful\n");
        
        //read from client
        char incoming[100]; 
        int terminate = 1;
        while (terminate) 
        {
            memset(incoming, '\0', sizeof(incoming));
            int r = recv(clientfd, incoming, 100, 0);
            if(r <= 0) {
                printf("Error receiving message");
                close(clientfd);
                exit(0);
            }
            incoming[strlen(incoming) - 1] = '\0';
            
            // client says Bye:
            printf("Received message: %s\n", incoming);
            if ((incoming[0] == 'b' || incoming[0] == 'B') &&
            (incoming[1] == 'y' || incoming[1] == 'Y' )&&
            (incoming[2] == 'e' || incoming[2] == 'E')) {
                printf("Closing connection with client\n");
                printf ("Listening\n");
                terminate = 0;
                close(clientfd);
            }
            
            else if ((incoming[0] == 'g' || incoming[0] == 'G') &&
                    (incoming[1] == 'e' || incoming[1] == 'E') &&
                    (incoming[2] == 't' || incoming[2] == 'T')) {
                if (incoming[3] == ' ') {
                    char readName[100];
                    memset(readName, '\0', sizeof(readName));
                    int i = 4, j = 0;
                    while (i < strlen(incoming)) {
                        readName[j] = incoming[i];
                        i++;
                        j++;
                    }
                    readName[j] = '\0'; // set last character to null
                    if (strlen(readName) < 1){ // check for empty file Name
                        sendToClient("File Name Empty", sockfd, clientfd, "SERVER 500 Get Error\n");
                    } else {
                        readFile(readName, clientfd, sockfd);
                    }
                } else { // error  
                    sendToClient("Invalid call", sockfd, clientfd, "SERVER 500 Get Error\n");
                }
            }   
            
            //If client puts 
            else if ((incoming[0] == 'p' || incoming[0] == 'P') &&
                     (incoming[1] == 'u' || incoming[1] == 'U') &&
                     (incoming[2] == 't' || incoming[2] == 'T')) {
                if (incoming[3] == ' ') {
                char writeName[100]; // Name of File to write to
                memset(writeName, '\0', sizeof(writeName));
                int i = 4, j = 0; 
                while (i < strlen(incoming)) {
                    writeName[j] = incoming[i];
                    i++;
                    j++;
                }
                writeName[j] = '\0'; // set Last Character to Null
                FILE *out = fopen(writeName, "w");
                if (out == NULL) {
                    printf("Error Creating File");
                } else {
                    int counter = 0; 
                    int terminatePut = 0;
                    while(terminatePut == 0) {
                        memset(incoming, '\0', sizeof(incoming));
                        recv(clientfd, incoming, 100, 0);
                        for (int i = 0; i<sizeof(incoming); i++){
                            if (incoming[i] != '\0') { 
                                if (incoming[0] == '\n') {
                                    counter ++; // seen one empty Line
                                } else {
                                    counter = 0;// Only stop when 2 consecutive lines
                                }
                                fprintf(out, "%c", incoming[i]);
                                if (counter == 2){
                                    fclose(out);
                                    sendToClient("DONE", sockfd, clientfd, "SERVER 201 OK\n");
                                    printf("EXIT Here \n");
                                    terminatePut = 1; // End Put call
                                }
                            }
                        }
                    }
                }
            } else { // error  
                sendToClient("Invalid call", sockfd, clientfd, "SERVER 500 Get Error\n");
            }
        }
    }
}
}


#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "kernel/syscall.h"



int main(void){
    static char buf[128];
    
    while(1){
        //Print >>> into the terminal
        write(2, ">>> ", 4); //4 because 4 char
        memset(buf, 0, sizeof(buf)); // Clearing buf, and set to 0, alternative for memset
        // fgets, note that we need to specify that we read our input from stdin
        gets(buf, sizeof(buf));
        if(buf[0] == 0){//EOF
            break;
        }

        int n = 0;
        while (buf[n] == ' ') {
            n++;
        }
        if(buf[n] == 'c' && buf[n+1] == 'd' && buf[n+2] == ' '){ //(user/sh.c:161)
            buf[strlen(buf)-1] = '\0';  // chop \n
            if(chdir(buf+3+n) < 0)
                fprintf(2, "cd %s not working\n", buf+3);
            
            continue; // skip to next iteration
        }
        

        char *flag = "exit";
        char *e = buf;
        int ie = 0;
        int je = 0;
        while(ie < 5){
            if(*flag == *e){
                je++;
                flag++;
                e++;
            }
            else if (je == 4) {
                exit(0);
            } else{
                break;
            }
            ie++;
        }


        /*for (int i = 0; i < sizeof(buf); i++) {
        printf("%c ", buf[i]); // Print each character in the array
        }*/
        for (int i = 0; i< sizeof(buf); i++) {
            if (buf[i] == '\n'){
                buf[i] = '\0'; //terminate
                break;
            }
        //printf("%c ", buf[i]); // Print each character
        }

        char *argv[20];
        char *argv2[20];
        char *word = buf;
        int i = 0;
        int j = 0;
        int k = 0;
        //int pipe_counter = 0;
        int redir = 0;
        int new_pipe = 0;
        int p[2];
        char filename[128];
        memset(filename, 0, sizeof(filename));
        memset(argv, 0, sizeof(argv));
        memset(argv2, 0, sizeof(argv2));
        while(*word != '\0'){
            while(*word == ' '){ //skip the spaces (ChatGpt, 3.5), used to skip any spaces & string tokenization
                word++;
            }
            if(*word != '\0' && *word != ' '){
                if (*word == '>'){ // I/O redirection
                redir = 1;
                word++;
                } else if(*word == '<'){
                    redir = 2;
                    word++;
                } if( *word == '|'){
                    new_pipe = 1;
                    //pipe_counter = i;
                    pipe(p);
                    *word = '\0';
                    word++;
                    word++;
                }
                if(redir == 0 && new_pipe == 0){
                    argv[i++] = word;// word start
                    //printf("word start argv:%c\n", *word);
                } if (redir == 1 || redir == 2){
                    word++;
                    filename[j++] = *word;
                    
                } if (new_pipe == 1){
                    //word++;
                    argv2[k++] = word;
                    //printf("word start argv2:%c\n", *word);
                    word++;
                }
            } 
            while(*word != '\0' && *word != ' '){
                word++;
                if(redir == 1 || redir == 2){
                  filename[j++] = *word;
                }
                
            }

            if(*word != '\0'){
                *word = '\0';
                word++;
            }
        }

        filename[j] = '\0';
        argv[i] = '\0';
        argv2[k] = '\0';

        int pid = fork();
        if (fork < 0){
            printf("%s", "Fork failed");
        } else if (pid == 0){

            if(new_pipe == 1){
                close(0); 
                dup(p[0]); //(2022) in xv6: a simple, Unix-like teaching operating system, p. 16. 
                close(p[0]);
                close(p[1]);
                exec(argv2[0], argv2);
                printf("Pipeline failed\n");
                exit(0);
            }

            if(redir == 1){
            int file = open(filename, O_CREATE | O_RDWR | O_TRUNC);
            if(file < 0){
                printf("open file failed successfully");
            }
                close(1);
                dup(file);
                close(file);
        } else if(redir == 2){
            int fd0 = open(filename, O_RDONLY); // Open the input file
                        if(fd0 < 0){
                printf("open file failed successfully");
            }
                close(0);
                dup(fd0);
                close(fd0);
        }

            exec(argv[0], argv);
            printf("%s", "Command not found\n");
            exit(0);

        } else{
            if(new_pipe == 1){    
                int pid2 = fork();
                    if(pid2 == 0){
                        close(1);//(2022) in xv6: a simple, Unix-like teaching operating system, p. 16. 
                        //close(p[0]);
                        dup(p[1]);
                        close(p[0]);
                        close(p[1]);//
                        exec(argv[0], argv);
                        exit(0);
                        } else{                        
                        close(p[1]);
                        close(p[0]);}
 
                }else{
                    wait(0);
                    }
            
                


        }

    }
    exit(0);
}

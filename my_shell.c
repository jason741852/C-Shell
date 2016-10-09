#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

#define tokenDelim " \n\t"
#define bufferSize 128

char* readCmd();


char** tokenizer(char*);// Split commands into tokens
bool exitCheck(char**);// Check for exit()
bool cdCheck(char**);// Check for cd
int cmdCount(char**);// Return the number of commands (for piping)
void execution(char*[bufferSize][bufferSize], int[2], int, int);


int main(void){
  // only two pipes is need at the same time,
  // regardless of the number of commands(???)
  int fdP[2];
  int fdC[2];



  bool terminate = false;
  while(!terminate){
    char* cmd;
    char* cwd;
    char** cmdToken;
    int i =0;

    int cmd_counter = 0; // count the number of pipes needed
    pid_t pid;

    cmd = readCmd();
    cmdToken = tokenizer(cmd);


    if(exitCheck(cmdToken)==true){
      printf("exiting shell..\n");
      terminate = true;
      exit(0);
    }
    else if(cdCheck(cmdToken)==true){
      cwd = getcwd(NULL, 0);

      //printf("%s/%s \n", cwd, cmdToken[1]);
      strncat(cwd, "/", bufferSize);
      strncat(cwd, cmdToken[1], bufferSize);
      //printf("%s\n",cwd);
      if(cmdToken[1]==NULL){
        perror("error: missing directory path\n");
        //terminate = true;
        //exit(0);
      }
      else{
        if(chdir(cwd)==-1){
          perror("error: changing directory not successful\n");
          //terminate = true;
          //exit(0);
        }
        else{
          printf("changing directory..\n");
        }
      }
    }
    else{
      /////////////////////////////////////////////
      /*forking if command not calling build-in's*/
      /////////////////////////////////////////////

      // Slpit commands for piping here
      // char cmdArrays[bufferSize][bufferSize][bufferSize];
      char* cmdArrays[bufferSize][bufferSize];

      int i=0;
      int j=0;
      int k=0;
      int n=0;

      // clear the array for every iteration
      for(i=0; i<bufferSize;i++){
        for (j=0; j<bufferSize; j++){
          cmdArrays[i][j]="";
        }
      }
      i=0;
      j=0;


      while(cmdToken[n]!=NULL){
        n++;
      }
      //printf("n= %i\n",n);

      while( k<n){
        if((strcmp(cmdToken[k],"|"))==0){
          cmdArrays[i][j] = NULL;
          i++;
          j=0;
          k++;// skip the pipe symbol
          printf("\n");
        }
        else{
          //printf("bug here %s\n",cmdArrays[0][0]);
          cmdArrays[i][j]= cmdToken[k];
          //printf("%i.%i: %s ",i,j,cmdArrays[i][j]);
          k++;
          j++;
        }
      }
      cmdArrays[i][j] = NULL;
      //exit(0);
      /////////////////////////////////////////////////////////////
      // forking and piping
      ///////////////////////////////////////////
      int fd[2];

      pipe(fd);

      int ii = 0;
      while(ii<=i){
        pid = fork();
        //printf("pid = %d \n",pid);
        if(pid<0){
          perror("forking error\n");
          exit(1);
        }
        else if(pid==0){
          printf("ii= %i\n",ii);
          if (ii==i){
            printf("------in CHILD--------\n");
            //close(STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            dup2(fd[0],STDIN_FILENO);
            dup2(STDOUT_FILENO,fd[1]);
            close(fd[1]);

            if(execvp(cmdArrays[ii][0],cmdArrays[ii])==-1){
              perror("EXECUTION ERROR");
              exit(1);
            }
          }
          else if (ii==0){
            //close(STDOUT_FILENO);
            //close(fd[0]);
            close(fd[1]);
            dup2(fd[0],STDOUT_FILENO);

            if(execvp(cmdArrays[ii][0],cmdArrays[ii])==-1){
              perror("EXECUTION ERROR");
              exit(1);
            }
          }
          else{
            close(fd[0]);
            close(fd[1]);
            dup2(fd[0],STDIN_FILENO);
            dup2(fd[1],STDOUT_FILENO);

            if(execvp(cmdArrays[ii][0],cmdArrays[ii])==-1){
              perror("EXECUTION ERROR");
              exit(1);
            }
          }
        }
        else{
          wait(NULL);
          close(fd[0]);
          close(fd[1]);
          //printf("parent exiting\n");
          //exit(0);
        }
        ii++;
    }

  }
}
  return 0;
}

void execution(char*cmdArrays[bufferSize][bufferSize], int fd[2], int ii, int i);


/// Print out a prompt then read a command from user
char* readCmd(){
  printf("Please enter a command:\n");
  char* line = NULL;
  ssize_t buf = 0;
  getline(&line, &buf, stdin);
  return line;
}

/// Split command arguement into tokens
char** tokenizer(char* line){
  char** tokens = malloc(bufferSize);
  //char* token;
  int i = 0; //array position counter

  tokens[i] = strtok(line,tokenDelim);

  while(tokens[i]!=NULL)
  {
    tokens[++i] = strtok(NULL,tokenDelim);
  }
  tokens[++i] = NULL;
  return tokens;
}


/// Check for "exit" command, if so exit
bool exitCheck(char** cmdTok){
  if(strcmp("exit", cmdTok[0])==0){
    //printf("true \n");
    return true;
  }
  else{
    //printf("false \n");
    return false;
  }
 }


/// Check for "exit" command, if so return true for main to change directory
 bool cdCheck(char** cmdTok){
   if(strcmp("cd", cmdTok[0])==0){
     //printf("true \n");
     return true;
   }
   else{
     //printf("false \n");
     return false;
   }
 }

 int cmdCount(char** cmdTok){
   int n = 0;//tokens counter
   while(cmdTok[n]!=NULL){
     n++;
   }
   int cmd_counter=1;

   int i = 0;//loop counter
   for(i; i<n; i++){
     //printf("cmp result: %d  \n", strcmp("|", cmdTok[i]));
     if((strcmp("|", cmdTok[i]))==0){
       cmd_counter++;
     }
   }
   //printf("Number of commands: %d\n", cmd_counter);
   return cmd_counter;
 }

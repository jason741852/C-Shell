#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

#define tokenDelim " \n "
#define bufferSize 100

char* readCmd();
char* readCmd();
char** tokenizer(char*);
bool exitCheck(char**);
bool cdCheck(char**);

//bool execute(char**);


int main(void){
  bool terminate = false;
  while(!terminate){
    char* cmd;
    char** cmdToken;
    char* cwd;
    int i = 0;
    pid_t pid;


    cmd = readCmd();
    cmdToken = tokenizer(cmd);
    //  for(i = 0; i<3; i++){
    //    printf("%s | ", cmdToken[i]);
    //  }

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

      pid = fork();
      if(pid<0){
        perror("error");
        //exit(1);
      }
      else if(pid==0){
        if(execvp(cmdToken[0],cmdToken)==-1){
          //perror("command not found");
        }
      }
      else{
        wait(NULL);
        printf("parent exiting\n");
        //exit(0);
      }
    }
  }
  return 0;
}


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
  char* token;
  int i = 0; //array position counter

  tokens[i] = strtok(line,tokenDelim);

  while(tokens[i]!=NULL)
  {
    tokens[++i] = strtok(NULL,tokenDelim);
  }
  return tokens;
}

// Check for build-in system calls
// int buildinCheck(char** cmdTok){
//   if(strcmp("cd", cmdTok[0])==0){
//     return 1;
//   }
//   else if(strcmp("exit", cmdTok[0])==0){
//     //printf("true \n");
//     return 2;
//   }
// }

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

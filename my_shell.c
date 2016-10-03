#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(void){


  printf("Please enter a command:\n");

  char* line = NULL;
  ssize_t bufferSize = 0;
  getline(&line, &bufferSize, stdin);


  char *tokenizer(char* line, ssize_t bufferSize){
    char* tokens[bufferSize];
    int i = 0;
    tokens[i] = strtok(line," ");

    while(tokens[i]!=NULL)
    {
      tokens[++i] = strtok(NULL," ");
    }


  //printf("%s",line);



  return 0;
}
//
//
// char *tokenizer(char* line, ssize_t bufferSize){
//   char* tokens[bufferSize];
//   int i = 0;
//   tokens[i] = strtok(line," ");
//
//   while(tokens[i]!=NULL)
//   {
//     tokens[++i] = strtok(NULL," ");
//   }
//
//   return tokens;
}

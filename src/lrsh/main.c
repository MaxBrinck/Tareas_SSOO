#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../input_manager/manager.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



void comand_hello () {
  pid_t pid  = fork();
  /*En caso de que haya un error*/
  if ( pid > 0) {
    /*Creo que cuando es mayor a cero es porque hay un proceso hijo*/
    /*La idea de waitpid es que el proceso padre espere que el hijo termine, si no lo ponemos significa que el proceso padre puede seguir 
    Haciendo otras cosas y en algun momento le llegara la respuesta del proceso hijo*/
    waitpid(pid, NULL, 0);
    /*Con esta linea se espera a que termine el proceso hijo*/
  }

  else if ( pid == 0) {
    printf("Hello world!\n");
    exit(0);
  }

  else {
    /*En caso de que haya un error*/
    /*Lo otro que vi es que en general se usa "perror" en vez de printf para cuando es error*/
    printf("Error al crear proceso hijo\n");
  }
}

void command_sum(float n1, float n2){
  pid_t pid = fork();
  if (pid == 0){
    printf("Sum: %f\n", n1 + n2);
    exit(0);

  }

  else if (pid > 0){
    waitpid(pid, NULL, 0);
  }

  else {
    printf("Error al crear proceso hijo\n");
  }
}

bool primo (int num){
  if (num <= 1){
    return false;
  }
  /*Creo que podemos mejorar la funcion, si el numero es muy grande podria demorarse mucho, puede ser hasta la raiz del numero o hasta la mitad*/
  for (int i = 2; i<= num - 1; i++){
    if (num % i == 0){
      return false;
    }
  }
  return true;

}

void command_prime (int numero){
  pid_t pid = fork();
  if (pid == 0){
    if (primo(numero)){
      printf("%d Es un número primo", numero);
    }
    else {
      printf("%d No es un número primo", numero);
    }
  exit(0);
  }
  

  else if (pid > 0){
    waitpid(pid, NULL, 0);
  }
  
  else{
    printf("Error al crear el proces hijo");
  }
}






int main(int argc, char const *argv[])
{
  char** input = read_user_input();
  printf("%s\n", input[0]);
  free_user_input(input);
}

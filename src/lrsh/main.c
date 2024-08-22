#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../input_manager/manager.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


void command_hello () {
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


/*Para responder a suma*/
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

/*Funcion para evaluar si es primo*/
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


/*Para responder a si es primo o no*/
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


/*Para responder a irexec*/
void command_irexec (char *archivo, char **args){
  pid_t pid = fork();
  if (pid == 0){
    execvp(archivo, args);
    printf("Fallo en exec");
    exit(1);
  }

  else if (pid > 0){
    waitpid(pid, NULL, 0);

  }

  else{
    printf("Error al crear proceso hijo";)
  }
}

/*Creo que tenemos que crear Proceso para poder guardar los procesos que se llaman y de esa forma poder entregarlos*/
typedef struct {
  pid_t pid;
  char* nombre;
  time_t tiempo;
  int exit_code;  /*Este nose exactamente que es, lo puse porque lo lei*/
} Proceso;

/*Tenemos que crear un arreglo que contenga cada proceso pero*/
/*no se cual es la cantidad de procesos a guardar, como para guardar espacio en memoria*/

/*Creamos un int que tenga la cantidad de procesos que se han ejecutado, se tiene que ir actualizando*/
int cantidad_procesos = 0;


/*Aca tenemos que hacer una funcion que vaya agregando los porocesos cada vez que se ejecuta uno nuevo, no entiendo si
como tenemos que hacer para guardar nombre del ejecutable y todo eso, lo deje a medias*/
void Nuevo_proceso (pid_t pid, char *nombre_proceso){
  Procesos[cantidad_procesos].pid = Proceso
}

/*Hay que hace command_irexit*/









int main(int argc, char const *argv[])
{
  /*Esto hace un loop infinito para estar recibiendo comandos todo el rato*/
  while (1) {

    char** input = read_user_input();
    printf("%s\n", input[0]);
    /*Revisamos que no sea un input vacío, si es asi se sigue en el loop*/
    if (input[0] == NULL){
      continue;
    }

    /*Entramos si no es vacío, por eso hay que procesar aca adentro cada caso*/
    else {
      if (strcmp(input[0], "hello") ==0 ){
      command_hello();
      }

      else if (strcmp(input[0], "sum") == 0){
      float n1 = atof (input[1]);
      float n2 = atof (input[2]);
      command_sum(n1,n2);
      }

      else if (strcmp(input[0], "is_prime") == 0){
        int numero = atof(input[1]);
        command_prime(numero);
      }
      /*Hay que seguir con el resto de los comandos que se pueden entregar, falta terminar aun los otros*/
    }
  }


  free_user_input(input);
}

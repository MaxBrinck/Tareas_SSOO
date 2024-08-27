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
#define MAX_PROCESOS 16



void command_hello () {
  pid_t pid  = fork();
  /*En caso de que haya un error*/
  if ( pid > 0) {
    /*Creo que cuando es mayor a cero es porque hay un proceso hijo*/
    /*La idea de waitpid es que el proceso padre espere que el hijo termine, si no lo ponemos significa que el proceso padre puede seguir 
    Haciendo otras cosas y en algun momento le llegara la respuesta del proceso hijo*/
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
    sleep(10);
    printf("%f + %f = %f: \n",n1, n2, n1 + n2);
    exit(0);

  }

  else if (pid > 0){
    

  }

  else {
    printf("Error al crear proceso hijo\n");
  }
}

/*Funcion para evaluar si es primo*/
bool primo(int num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

/*Para responder a si es primo o no*/
void command_prime (int numero){
  pid_t pid = fork();
  if (pid == 0){
    if (primo(numero)){
      printf("%d Es un número primo\n", numero);
    }
    else {
      printf("%d No es un número primo\n", numero);
    }
  exit(0);
  }
  

  else if (pid > 0){

  }
  
  else{
    printf("Error al crear el proces hijo");
  }
}


/*Para responder a irexec*/


/*Creo que tenemos que crear Proceso para poder guardar los procesos que se llaman y de esa forma poder entregarlos*/
typedef struct {
  pid_t pid;
  char* nombre;
  time_t tiempo;
  int exit_code;  /*Este nose exactamente que es, lo puse porque lo lei*/
} Proceso;

/*Tenemos que crear un arreglo que contenga cada proceso pero*/
/*no se cual es la cantidad de procesos a guardar, como para guardar espacio en memoria*/ // son 16 creo xd

/*Creamos un int que tenga la cantidad de procesos que se han ejecutado, se tiene que ir actualizando*/
int cantidad_procesos = 0;


/*Aca tenemos que hacer una funcion que vaya agregando los porocesos cada vez que se ejecuta uno nuevo, no entiendo si
como tenemos que hacer para guardar nombre del ejecutable y todo eso, lo deje a medias*/

Proceso* procesos[MAX_PROCESOS];

void command_lrlist() {
    printf("Lista de procesos corriendo \n");
    printf("\n");

    // Recorrer todos los procesos
    for (int i = 0; i < cantidad_procesos; i++) {
        Proceso* p = procesos[i];  

        // Calcular el tiempo de ejecucion
        time_t tiempo_ejecucion = time(NULL) - p->tiempo;

        // Imprimir la info del proceso:
        printf("PID: %d, Name: %s, Time running: %ld seconds, Exit Code: %d\n", 
               p->pid,        
               p->nombre,     
               tiempo_ejecucion,  
               p->exit_code   
        );
    }
    printf("\n");
}
void Nuevo_proceso(pid_t pid, char *nombre_proceso) {
    // Manejo memoria proceso
    Proceso* nuevo_proceso = malloc(sizeof(Proceso));
    if (!nuevo_proceso) {
        perror("Error al asignar memoria para el nuevo proceso");
        exit(1);
    }

    // Asignar pid
    nuevo_proceso->pid = pid;

    // Guardar nombre proceso y liberar meemoria
    nuevo_proceso->nombre = strdup(nombre_proceso);
    if (!nuevo_proceso->nombre) {
        perror("Error al duplicar el nombre del proceso");
        free(nuevo_proceso);  
        exit(1);
    }

    // Guardar tiempo de inicio del proceso
    nuevo_proceso->tiempo = time(NULL);
    if (nuevo_proceso->tiempo == ((time_t) - 1)) {
        perror("Error al obtener el tiempo actual");
        free(nuevo_proceso->nombre);  
        free(nuevo_proceso);
        exit(1);
    }

    // El codigo de salida es -1 ya que proceso no ha empezado
    nuevo_proceso->exit_code = -1;

    // Guardar el nuevo proceso en el array de procesos
    procesos[cantidad_procesos++] = nuevo_proceso;

    // No pasarse de la cantidad maxima
    if (cantidad_procesos > MAX_PROCESOS) {
        fprintf(stderr, "Se ha alcanzado el número máximo de procesos permitidos\n");
    }
}

void command_lrexec (char *archivo, char **args){
  pid_t pid = fork();
  if (pid == 0){
    execvp(archivo, args);
    printf("Fallo en exec");
    exit(1);
  }

  else if (pid > 0){
    // waitpid(pid, NULL, 0); // saqué esto no se para que servia xd
    Nuevo_proceso(pid, archivo);
  }

  else{
    // printf("Error al crear proceso hijo"); // tambien saque esto pq el perror creo q hace un exit 
    perror("Error al crear proceso hijo");
  }
}

void command_lrexit() {
    // Iterar sobre todos los procesos, encontrar su posicion y enviar señal para terminar
    for (int i = 0; i < cantidad_procesos; i++) {
        Proceso* p = procesos[i];  
        if (kill(p->pid, SIGINT) == -1) {
            perror("Error al enviar SIGINT al proceso");
        }
    }

    // Maximo 10 sec para forzar termino
    for (int i = 0; i < 10; i++) {
        int todos_terminados = 1;

        // Recorrer para ver si alguno sigue activo
        for (int j = 0; j < cantidad_procesos; j++) {
            Proceso* p = procesos[j];
            int status;

            // Revisamos si el proceso termino
            pid_t result = waitpid(p->pid, &status, WNOHANG);
            if (result == 0) {  // Si el proceso sigue vivo
                todos_terminados = 0;
            } else if (result > 0) {  // Si el proceso ha terminado
                if (WIFEXITED(status)) {
                    p->exit_code = WEXITSTATUS(status);  // Guardamos el código de salida del proceso
                }
            }
        }

        // Si todos los procesos han terminado, salimos del loop
        if (todos_terminados) {
            break;
        }

        // Si aún hay procesos vivos, esperamos un segundo antes de revisar nuevamente
        sleep(1);
    }

    // Después de 10 segundos, se fuerzzan para terminar
    for (int i = 0; i < cantidad_procesos; i++) {
        Proceso* p = procesos[i];
        int status;

        // Revisamos si el proceso sigue en ejecución
        pid_t result = waitpid(p->pid, &status, WNOHANG);
        if (result == 0) {
            // Si el proceso sigue vivo después de 10 segundos, lo matamos
            if (kill(p->pid, SIGKILL) == -1) {
                perror("Error al enviar SIGKILL al proceso");
            } else {
                printf("Proceso con PID %d fue forzado a terminar.\n", p->pid);
            }
        }
    }

    // memoria
    for (int i = 0; i < cantidad_procesos; i++) {
        free(procesos[i]->nombre);  
        free(procesos[i]);         
    }

    printf("Cerrando lrsh. Muchas gracias \n");
    exit(0);  
}




int main(int argc, char const *argv[])
{
  char** input;
  /*Esto hace un loop infinito para estar recibiendo comandos todo el rato*/
  while (1) {

    input = read_user_input();
    printf("%s\n", input[0]);
    /*Revisamos que no sea un input vacío, si es asi se sigue en el loop*/
    if (input == NULL || input[0] == NULL){
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
       else if (strcmp(input[0], "lrexec") == 0) {
            command_lrexec(input[1], &input[1]);  
        }

  
        else if (strcmp(input[0], "lrlist") == 0) {
            command_lrlist();  
        }

        // Comando "lrexit"
        else if (strcmp(input[0], "lrexit") == 0) {
            command_lrexit();  
            break;  
        }

        
        else {
            printf("Comando no reconocido: %s\n", input[0]);  
        }

    }
  }


  free_user_input(input);
}

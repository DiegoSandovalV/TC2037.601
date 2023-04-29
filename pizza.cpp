/**
 * @file pizza.cpp
 * @author Diego Sandoval
 * @brief 
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <condition_variable>

#define SLICES 5

std::mutex mtx;
int students = 6;


//Condition to deliver pizza

pthread_cond_t pizzaDelivering;
pthread_cond_t orderPizza;

//Mutex to eat pizza
pthread_mutex_t pizzaEating;

//Mutex to check if there is pizza available
pthread_mutex_t pizzaAvailable;

int rebPizza = SLICES;



void* estudiante(void* params){
    int id = * (int*) params;
    
 while (true) {

        pthread_mutex_lock(&pizzaAvailable);
        pthread_mutex_lock(&pizzaEating);
        
        if (rebPizza <= 0) {
            std::cout << "Estudiante " << id << " esta ordenando pizza."  << std::endl;
            pthread_cond_signal(&orderPizza);
            pthread_cond_wait(&pizzaDelivering, &pizzaEating);
            
       
        }

        pthread_mutex_unlock(&pizzaAvailable);
        rebPizza--;
        std::cout << "Estudiante " << id << " tomó una rebanada de pizza. Quedan: " << rebPizza << std::endl;
        
        
        pthread_mutex_unlock(&pizzaEating);
        sleep(3);
        
    
    }

}

void* PizzaShop(void* params){


    while(true){
        pthread_cond_wait(&orderPizza,&pizzaEating);
        std::cout << "PizzaShop: Se creo la orden de pizza" << std::endl;
        sleep(6);
        rebPizza = SLICES;

        std::cout << "PizzaShop: la pizza esta siendo entregada" << std::endl;
        pthread_cond_signal(&pizzaDelivering);
        pthread_mutex_unlock(&pizzaEating);
    }
}

int main(){

    pthread_cond_init (&pizzaDelivering, NULL);
    pthread_cond_init (&orderPizza, NULL);
    pthread_mutex_init (&pizzaEating, NULL);

    pthread_t threadPizzaShop;
    pthread_create(&threadPizzaShop, NULL, PizzaShop, NULL);

    pthread_t threadStudent[students];

    for(int i = 0; i < students; i++){
        pthread_create(&threadStudent[i], NULL, estudiante, new int(i));
    }

    pthread_join(threadPizzaShop, NULL);
    for (int i = 0; i < students; i++) {
        pthread_join(threadStudent[i], NULL);
    }

    return 0;


}
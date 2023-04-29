/**
 * @file Brigde.cpp
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
#include <semaphore>


//Mutex to cross the bridge
pthread_mutex_t crossBridge;

//Condition to check if theres space in the bridge
pthread_cond_t spaceInBridge;

//Condition to check if the brigde is empty
pthread_cond_t bridgeEmpty;


//Starting direction
std::string currDirection;

//Current number of vehicles in the bridge
int currVehicles = 0;

//Quantiny of vehicles waiting to cross the bridge in the same direction
int northWaiting = 0;
int southWaiting = 0;

//Quantity of vehicles
const int VEHICLES = 50;

typedef struct{
    int id;
    std::string direction;
} Vehicle;



void ArrieveBridge(int id, std::string direction){
    pthread_mutex_lock(&crossBridge);

    std::cout << "Vehicle " << id << " arrived to the bridge in: " << direction << " direction" << std::endl;

    //Sets the starting direction
    if(currVehicles == 0){
        currDirection = direction;
    }

    //Checks if the bridge has less than 3 vehicles and if the direction is the same
    else if(currVehicles < 3 && direction == currDirection){
        std::cout << "Vehicle " << id << " is checking the direction of the bridge" << std::endl;

    //Wait until the bridge has space to cross
    }else if(direction == currDirection){
        std::cout << "Vehicle " << id << " is waiting to cross the bridge in: " << direction << " direction" << std::endl;
        if(direction == "north"){
                northWaiting++;
                pthread_cond_wait(&spaceInBridge, &crossBridge);
                northWaiting--;
            }else{
                southWaiting++;
                pthread_cond_wait(&spaceInBridge, &crossBridge);
                southWaiting--;
    }

    //Waits the bridge to be empty to change direction  
    }else{
        std::cout << "Vehicle " << id << " is waiting for the bridge to change direction" << std::endl;
        if(direction == "north"){
                northWaiting++;
                pthread_cond_wait(&bridgeEmpty, &crossBridge);
                northWaiting--;
            }else{
                southWaiting++;
                pthread_cond_wait(&bridgeEmpty, &crossBridge);
                southWaiting--;
            }
    }


    currVehicles++;
    pthread_mutex_unlock(&crossBridge);
    
    return;
}


void CrossBridge(int id, std::string direction){
    std::cout << "Vehicle " << id << " is crossing the bridge in: " << direction << " direction" << std::endl;
    usleep(50000);
    return;
}

void ExitBridge(int id, std::string direction){
    pthread_mutex_lock(&crossBridge);
   currVehicles--;

   std::cout << "Vehicle " << id << " exited" << std::endl;

    //Checks if the bridge is empty and changes the direction
   if(currVehicles == 0 && (currDirection == "north" && northWaiting == 0)){
    
    currDirection = "south";
    std::cout << "Bridge is empty, changing direction to: " << currDirection << std::endl;
    pthread_cond_broadcast(&bridgeEmpty);

    } else if(currVehicles == 0 && (currDirection == "south" && southWaiting == 0)){

    currDirection = "north";
    std::cout << "Bridge is empty, changing direction to: " << currDirection << std::endl;
    pthread_cond_broadcast(&bridgeEmpty);

    }else{
    pthread_cond_signal(&spaceInBridge);

    }

    pthread_mutex_unlock(&crossBridge);


    return;   
}

void* vehicle(void* params){
    Vehicle* vehicle = (Vehicle*) params;
    ArrieveBridge(vehicle->id, vehicle->direction);
    CrossBridge(vehicle->id, vehicle->direction);
    ExitBridge(vehicle->id, vehicle->direction);
    
}


int main(){

    //creating the threads and vehicles
    pthread_t threads[VEHICLES];
    Vehicle vehicles[VEHICLES];

    //initializing the mutex
    pthread_mutex_init(&crossBridge, NULL);

    //initializing the conditions
    pthread_cond_init(&spaceInBridge, NULL);
    pthread_cond_init(&bridgeEmpty, NULL);

    //creating the vehicles with random directions
    for(int i = 0; i < VEHICLES; i++){
        vehicles[i].id = i;
        if(rand() % 2 == 0){
            vehicles[i].direction = "north";
        }else{
            vehicles[i].direction = "south";
        }
        pthread_create(&threads[i], NULL, vehicle, (void*) &vehicles[i]);
        usleep(10000);
    }

    //joining the threads
    for(int i = 0; i < VEHICLES; i++){
        pthread_join(threads[i], NULL);
    }

    //destroying the mutex
    pthread_mutex_destroy(&crossBridge);

    //destroying the conditions
    pthread_cond_destroy(&spaceInBridge);
    pthread_cond_destroy(&bridgeEmpty);

    return 0;
    
    
    
}
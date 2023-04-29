/**
 * @file MonteCarlo.cpp
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
#include <chrono>
#include <random>
#include <mutex>




const long int points = 100000000;


/**
 * @brief 
 * Generates randoms points within a square
 * @param {int} points
 * @param {int} *circle_points
 * @return {void} 
 * 
 */

void generateRandomPoints(int points, double &pointsInCircle){
    
    //Number generator

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    //Generate points and counter the number of points within the circle

    for(int i=0; i < points; i++){
        double x = dis(gen);
        double y = dis(gen);
        if((x*x + y*y) <= 1){
            pointsInCircle++;
        }
    }

}

/**
 * @brief 
 * Calculates the value of pi using threads
 * @param {void} 
 * @return {void} 
 * 
 */

void piAprox(){
//Get the number of threads
    int numThreads = 8;

    //Create an array of threads
    std::thread* threads = new std::thread[numThreads];

    //Array to store the number of points within the circle
    double* pointsInCircle = new double[numThreads];

    //Global variable for the points within the circle
    double totalPointsInCircle = 0;

    //Start the timer
    auto start_time = std::chrono::high_resolution_clock::now();

    //Create the threads
    for(int i = 0; i < numThreads; i++){
        //Create threads and use mutex to lock the global variable
        threads[i] = std::thread(generateRandomPoints, points/numThreads, std::ref(pointsInCircle[i]));
        
    }

    //Join the threads
    for(int i = 0; i < numThreads; i++){
        threads[i].join();
    }

    //Calculate the total number of points within the circle
    for(int i = 0; i < numThreads; i++){
        totalPointsInCircle += pointsInCircle[i];
    }

    //Calculate the value of pi
    double pi = 4 * totalPointsInCircle / points;

    //Stop the timer
    auto end_time = std::chrono::high_resolution_clock::now();

    //Print the value of pi
    double seconds =1000;
    std::cout << "The value of pi is: " << pi << std::endl;
    std::cout << "The time taken was: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()/seconds << " seconds" << std::endl;

    //Delete the arrays
    delete[] threads;
    delete[] pointsInCircle;
}




int main(){

    piAprox();

    return 0;

}
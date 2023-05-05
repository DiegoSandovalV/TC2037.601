/**
 * @file numerosPrimos.cpp
 * @author Diego Sandoval
 * @brief 
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include <iostream>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"

long int n = 5000000;


//Threads structure
struct PrimeThreadArgs  {
  int start;
  int end;
  long long* sum;
};



//Function to check if a number is prime
bool isPrime(int n) {
  if (n <= 2) {
    return false;
  }

  for (int i = 2; i <= sqrt(n); i++) {
    if (n % i == 0) {
      return false;
    }
  }

  return true;
}

//Function to sum the prime numbers in a range using pthreads
void* sumPrimesInRange(void* threadArgs) {
  PrimeThreadArgs* args = (PrimeThreadArgs*) threadArgs;

  long long sum = 0;

  for (int i = args->start; i <= args->end; i++) {
    if (isPrime(i)) {
      sum += i;
    }
  }

  *(args->sum) += sum;

  pthread_exit(NULL);
}

//Function to sum the prime numbers in a range using pthreads
long long sumPrimes(int n, int numThreads) {
  long long sum = 0;

  // Creates an array of threads
  PrimeThreadArgs threadArgs[numThreads];

  // Creates threads
  pthread_t threads[numThreads];

  // Calculate the range size for each thread
  int rangeSize = ceil((double) n / numThreads);

  // Initialize threads
  for (int i = 0; i < numThreads; i++) {
    int start = i * rangeSize;
    int end = std::min((i + 1) * rangeSize - 1, n);

    threadArgs[i].start = start;
    threadArgs[i].end = end;
    threadArgs[i].sum = &sum;

    pthread_create(&threads[i], NULL, sumPrimesInRange, (void*) &threadArgs[i]);
  }

  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  return sum;
}


int main(){
    double ms, ms2;
    long long int sum = 0;

  //secuencial

  ms = 0;


  for(int i = 0; i<=n; i++){
    start_timer();
    if(isPrime(i) == true){
        sum += i;
    }
    ms += stop_timer();
  }

   std::cout << "Secuencial acum: " << sum << std::endl;

   std::cout << "time =  " << (ms / 1000) << "ms" << std::endl;

   std::cout << "----------------------------------" << std::endl;

    //Multithread

    int numThreads = 8;

    ms2 = 0;

    start_timer();
    sum = sumPrimes(n, numThreads);
    ms2 += stop_timer();


    std::cout << "Multithread acum: " << sum << std::endl;

   std::cout << "time =  " << (ms2 / 1000) << "ms" << std::endl;




  return 0;
}
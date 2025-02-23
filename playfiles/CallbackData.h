#pragma once
#include <vector>
#include <iostream>
#include <deque>

class CallbackData {
    public:
      unsigned int nb_channels = 2;
      double sample_rate = 22050;
      // a vector of int, that correspond to the number 
      // of elements of each of the array of double below
      std::vector<int> samples_size;
      // a vector of array of double
      std::vector<std::deque<double*>> samples;
      int frameCounter = 0;
    
      void print();

      void queue(double* e, size_t i);
    
      double dequeue(size_t i);

      bool isEmpty(size_t i);
    
};    
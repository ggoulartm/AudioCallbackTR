#include "CallbackData.h"
#include <unistd.h>


void CallbackData::print(){
    std::cout << "nb_channels : " << nb_channels << std::endl;
    std::cout << "sample_rate : " << sample_rate << std::endl;
    std::cout << "frameCounter : " << frameCounter << std::endl;
    for(size_t i = 0; i < samples_size.size(); i++){
      std::cout << "samples_size[" << i << "] : " << samples_size[i] << std::endl;
      std::cout << "samples[" << i << "] size : " << samples[i].size() << std::endl;
    }

    std::cout << "---------------------------------" << std::endl;
}
  
void CallbackData::queue(double* e, size_t i){
  if(samples.size() <= i){
    samples.push_back(std::deque<double*>());
  }
  if(samples[i].size() == 0){
    samples[i] = std::deque<double*>();
  }
  samples[i].push_back(e);
}
  
double CallbackData::dequeue(size_t i){
  while(samples[i].empty()){
    usleep(10000);
  }
  double* e = samples[i].front();
  samples[i].pop_front();
  if(samples[i].size() == 0){
    samples[i] = std::deque<double*>();
  }
  if(e == NULL){
    return NULL;
  }
  return *e;
}

bool CallbackData::isEmpty(size_t i){
  return samples[i].size() == 0;
}
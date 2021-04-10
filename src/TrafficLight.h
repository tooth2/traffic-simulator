#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

template <class T>
class MessageQueue
{
public:
   void send(T &&msg); // take an rvalue reference of type TrafficLightPhase 
   T receive();   // return the type TrafficLightPhase 
private:
    
    std::deque<T> _queue ;  // queue stores objects of type TrafficLightPhase. 
    std::condition_variable _condition;
    std::mutex _mutex;    
    
};

enum class TrafficLightPhase {
  red,
  green
};

class TrafficLight: public TrafficObject  //  a child class of TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();
    //~TrafficLight();
    // getters / setters
    TrafficLightPhase getCurrentPhase();
    // typical behaviour methods
    void waitForGreen();
    void simulate();  //override : TrafficObejct


private:
    // typical behaviour methods
    void cycleThroughPhases();

    MessageQueue<TrafficLightPhase> _messageQueue;  //  for messages of type TrafficLightPhase 
    TrafficLightPhase _currentPhase; // „red“ or „green“ value
    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif
#include <iostream>
#include <random>
#include "TrafficLight.h"

#include <queue>
#include <future>
#include <chrono>

/* Implementation of class "MessageQueue" */
template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> lck(_mutex);
    _condition.wait(lck, [this] { return !_queue.empty(); });

    // get the latest new messages and remove it from the queue using move semantics
    T msg = std::move(_queue.back());
    _queue.pop_back();
    // The received object should then be returned by the receive function
    return msg;

}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // add a new message to the queue and afterwards send a notification
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push_back(std::move(msg));
    _condition.notify_one();
   
}


/* Implementation of class "TrafficLight" */
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while(true) {       
        // repeatedly calls the receive function on the message queue 
        auto currentPhase = _messageQueue.receive(); 
        std::string light = currentPhase == TrafficLightPhase::green ? "green" : "red";
        std::cout << "Current Phase is " << light << " light." << std::endl;
        // when receives TrafficLightPhase::green, the method returns.
        if (currentPhase == TrafficLightPhase::green) {     
            return;
        }
    }
   
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // "cycleThroughPhases“ will be started in a thread when this is called
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
  
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // The cycle duration is a random value between 4 and 6 seconds.
    std::random_device rd;  
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(4000, 6000); 
  
    double cycleDuration = dist(gen); // duration of a cycle in sec
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    // stop watch
    lastUpdate = std::chrono::system_clock::now();
  
    while (true) {
        // compute the time difference between two loop cycles
        long timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();      
     
        if (timeDiff >= cycleDuration) {

            std::cout << "timeDiff= " << timeDiff << " , cycleDuration= " <<  cycleDuration << std::endl;
 
            // toggles the current phase of the traffic light between red and green
            _currentPhase = _currentPhase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;
            // sends an update to the message queue using move semantics.
            _messageQueue.send(std::move(_currentPhase));
            // set a new random cycle duration between 4000ms and 6000ms
            cycleDuration = dist(gen);
            // reset stop watch for next cycle
            lastUpdate = std::chrono::system_clock::now();

        }      
        //  sleep 1ms between two cycles at every iteration
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  
}


# traffic-simulation

### Code Structure 
- `TrafficLight`
    - `TrafficLight` is a child class of `TrafficObject`.
    - `void waitForGreen()` and `void simulate()` as well as `TrafficLightPhase getCurrentPhase()`, where TrafficLightPhase is an enum that can be either red or green.  
    - a private method `void cycleThroughPhases()` and a private member `_currentPhase` which can take red or green as its value.
    - cycleThroughPhases() method: 
        - measures the time between two loop cycles 
        - toggles the current phase of the traffic light between red and green
        - The cycle duration should be a random value between 4 and 6 seconds, 
        - while-loop uses std::this_thread::sleep_for to wait 1ms between two cycles
        - it is started in a thread when the public method simulate is called from a thread queue
- `MessageQueue`
    - A MessageQueue class is defined in the header of class TrafficLight which has the public methods send and receive.
    - `send` should take an rvalue reference of type `TrafficLightPhase` whereas `receive` should return this type. 
    - defines a `std::dequeue` called `_queue`, which stores objects of type TrafficLightPhase. 
    - variable `std::condition_variable` , an `std::mutex` as private members.

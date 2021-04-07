# traffic-simulation

### Code Structure 
```
src
|- Graphics.h, Graphics.cpp 
|- vehicle.h, vehicle.cpp
|- street.h, street.cpp
|- intersection.h, intersection.cpp 
|- TrafficLight.h, TrafficLight.cpp
|- TrafficObject.h, TrafficObject.cpp
|- TrafficSimulator.cpp
```
- `TrafficLight`
    - `TrafficLight` is a child class of `TrafficObject`.
    - public methods: `void waitForGreen()` , `void simulate()` , `TrafficLightPhase getCurrentPhase()`
        - `TrafficLightPhase getCurrentPhase()` : the return is enumType `TrafficLightPhase` that can be either red or green 
    - a private method: `void cycleThroughPhases()` 
        - measures the time between two loop cycles 
        - toggles the current phase of the traffic light between red and green
        - sends an update method to the message queue using move semantics
        - The cycle duration should be a random value between 4 and 6 seconds, 
        - while-loop uses `std::this_thread::sleep_` for to wait 1ms between two cycles
        - it is started in a thread when the public method `simulate` is called in a thread queue
    - private member 
        - `_currentPhase` which can take red or green as its value.
        - `_trafficLight` a Type of TrafficLight

- `MessageQueue`
    - A MessageQueue class is defined in the header of class TrafficLight which has the public methods send and receive.
    - `send` should take an rvalue reference of type `TrafficLightPhase` whereas `receive` should return this type. 
    - defines a `std::dequeue` called `_queue`, which stores objects of type TrafficLightPhase. 
    - private members variables:  `std::condition_variable` ,  `std::mutex` 
    - public method: `send` , `receive`
    - `send()`: 
        - use the mechanisms `std::lock_guard<std::mutex>` to add a new message to the queue
        - take an rvalue reference of type `TrafficLightPhase`
        - `_condition.notify_one()` to send a notification.
        - In the class `TrafficLight`, a private member of type `MessageQueue` is created and used within the infinite loop to push each new `TrafficLightPhase` into it by calling `send` in conjunction with move semantics. 
    - `receive`
        - use `std::unique_lock<std::mutex>` and `_condition.wait()` to wait for and receive new messages  
        - pull messages from the queue using move semantics
        - The received object is returned by the `receive` function
    - `waitForGreen()`
        - The method `waitForGreen` is completed, in which an infinite while loop runs 
        - repeatedly calls the `receive` function on the message queue. 
        - Once it receives `TrafficLightPhase::green`, the method returns.
- message exchange
    - a private member `_trafficLight` of type `TrafficLight` 
    - The method `Intersection::simulate()` starts the simulation of `_trafficLight`
    - The method `Intersection::addVehicleToQueue` use the methods `TrafficLight::getCurrentPhase` and` TrafficLight::waitForGreen` to block the execution until the traffic light turns green.


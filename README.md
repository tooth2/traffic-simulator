# concurrent traffic simulator

A traffic simulator implementation in which vehicles are moving along streets and are crossing intersections.
With increasing traffic in the city, traffic lights are needed for road safety. Each intersection will therefore be equipped with a traffic light. This project implemented thread-safe communication protocol between vehicles and intersections to complete the simulation and concurrent programming (such as mutexes, locks and message queues) the traffic lights and integrate them properly.

### Project Result 
Traffic simulation runs with red lights controlling traffic, just as in the .gif file below. There are two simulation : one is Paris street and the other one is NYC street simulation. 
| Paris | New York| 
|--| --| 
| ![Paris](paris.gif)  |![New York](nyc.gif)    |

### Code Structure 
```
src
|--
|  Graphics.h, Graphics.cpp
|  Vehicle.h, Vehicle.cpp
|  Street.h, Street.cpp
|  Intersection.h, Intersection.cpp
|  TrafficLight.h, TrafficLight.cpp
|  TrafficObject.h, TrafficObject.cpp
|  TrafficSimulator-Final.cpp
```

- `TrafficLight`
    - `TrafficLight` is a child class of `TrafficObject`.
    - public methods: `void waitForGreen()` , `void simulate()` , `TrafficLightPhase getCurrentPhase()`
        - `TrafficLightPhase TrafficLight::getCurrentPhase(): the return is enumType `TrafficLightPhase` that can be either red or green 
        - `void TrafficLight::simulate()`
        - `void TrafficLight::waitForGreen()`
            - The method `waitForGreen` is completed, in which an infinite while loop runs 
            - repeatedly calls the `receive` function on the message queue. 
            - Once it receives `TrafficLightPhase::green`, the method returns.
    - a private method: `void TrafficLight::cycleThroughPhases()` 
        - measures the time between two loop cycles 
        - toggles the current phase of the traffic light between red and green
        - sends an update method to the message queue using move semantics
        - The cycle duration should be a random value between 4 and 6 seconds, 
        - while-loop uses `std::this_thread::sleep_for` to wait 1ms between two cycles
        - it is started in a thread when the public method `simulate()` is called in a thread queue
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
- `Intersection`
    - a private member `_trafficLight` of type `TrafficLight` 
    - The method `Intersection::simulate()` starts the simulation of `_trafficLight`
    - The method `Intersection::addVehicleToQueue` use the methods `TrafficLight::getCurrentPhase` and` TrafficLight::waitForGreen` to block the execution until the traffic light turns green.

### Run-time environment 
- cmake >= 3.7
- make >= 4.1 (Linux, Mac), 3.81 (Windows)
- gcc/g++ >= 5.4
- openCV >=4.1

### Build instruction
- Make a build directory in the top level directory: mkdir build && cd build
- Compile: cmake .. && make
- Run : ./traffic_simulation

```
cmake_minimum_required(VERSION 3.7)
add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(traffic_simulation)

add_executable(traffic_simulation src/TrafficSimulator-Final.cpp)
```
### reference 
- [threads](https://hackernoon.com/learn-c-multi-threading-in-5-minutes-8b881c92941f)
- [C++14 threads/mutex](https://hackernoon.com/learn-c-multi-threading-in-5-minutes-8b881c92941f)
- [Message Queue](https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_message_queues.htm)
- [C++ Standard template library](https://www.modernescpp.com/index.php/c-core-guidelines-rules-for-concurrency-and-parallelism)
- [C++ tips](https://www.acodersjourney.com/top-20-cplusplus-multithreading-mistakes/)

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

const int NUM_PHILOSOPHERS = 5;
mutex forks[NUM_PHILOSOPHERS];
mutex consoleMutex; // Mutex for synchronizing console output

/*
philosopher 0 needs left_fork 0 and right_fork 1
philosopher 1 needs left_fork 1 and right_fork 2
philosopher 2 needs left_fork 2 and right_fork 3
philosopher 3 needs left_fork 3 and right_fork 4
philosopher 4 needs left_fork 4 and right_fork 0
*/

void philosopher(int id) {
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    // Simulating philosopher's actions
    while (true) {
        // Thinking
        this_thread::sleep_for(chrono::milliseconds(2000));

        // Hungry, trying to pick up forks
        if (forks[left_fork].try_lock()) {
            if (forks[right_fork].try_lock()) {
                // Eating
                {
                    lock_guard<mutex> lock(consoleMutex); // Blocking access to the console
                    cout << "Philosopher " << id << " start eating." << endl;
                }
                this_thread::sleep_for(chrono::milliseconds(1000));

                // Done eating, release forks
                {
                    lock_guard<mutex> lock(consoleMutex); // Blocking access to the console
                    cout << "Philosopher " << id << " is done eating." << endl;
                }
                forks[left_fork].unlock();
                forks[right_fork].unlock(); 
            }
            else {
                // Could not find the right fork - returns the left one
                forks[left_fork].unlock();
            }
        }

        // Repeat
    }
}

int main() {
    thread philosophers[NUM_PHILOSOPHERS];

    // Start philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosophers[i] = thread(philosopher, i);
    }

    // Join threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosophers[i].join();
    }

    return 0;
}

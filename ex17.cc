// Copyright 2025 Landon Davidson
// landond@uw.edu

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <string>
#include <ctime>

#include "SimpleQueue.h"

using std::cout;
using std::endl;
using std::string;
using std::thread;

const int NUM_PIES = 6;                 // # pies created by each producer
static SimpleQueue queue;               // queue of pies
static unsigned int seed = time(NULL);  // initialize random sleep time
static pthread_mutex_t write_lock;      // mutex for cout

// Thread safe print that prints the given str on a line
void thread_safe_print(string str) {
  pthread_mutex_lock(&write_lock);
  // Only one thread can hold the lock at a time, making it safe to
  // use cout. If we didn't lock before using cout, the order of things
  // put into the stream could be mixed up.
  cout << str << endl;
  pthread_mutex_unlock(&write_lock);
}

// Produces NUM_PIES pies of the given type
// You should NOT modify this method at all
void producer(string pie_type) {
  for (int i = 0; i < NUM_PIES; i++) {
    queue.Enqueue(pie_type);
    thread_safe_print(pie_type + " pie ready!");
    int sleep_time = rand_r(&seed) % 500 + 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
  }
}

// Eats 2 * NUM_PIES pies
// You should NOT modify this method at all
void consumer() {
  for (int i = 0; i < NUM_PIES * 2; i++) {
    bool successful = false;
    string pie_type;
    while (!successful) {
      while (queue.IsEmpty()) {
        // Sleep for a bit and then check again
        int sleep_time = rand_r(&seed) % 800 + 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
      }
      successful = queue.Dequeue(&pie_type);
    }
    thread_safe_print(pie_type + " pie eaten!");
  }
}

void *concurrent_producer(void *arg) {
  string pie_type = *static_cast<string*>(arg);
  producer(pie_type);
  return arg;
}

void *concurrent_consumer(void *arg) {
  consumer();
  return arg;
}

int main(int argc, char **argv) {
  pthread_mutex_init(&write_lock, NULL);
  // Your task: Make the two producers and the single consumer
  // all run concurrently (hint: use pthreads)

  // Attempt to create Apple pie producer thread
  pthread_t apple_thr;
  string apple_arg = "Apple";
  if (pthread_create(&apple_thr, nullptr,
                     concurrent_producer, &apple_arg) != 0) {
    std::cerr << "Failed to create Apple pie thread" << std::endl;
    pthread_mutex_destroy(&write_lock);
    return EXIT_FAILURE;
  }

  // Attempt to create Blackberry pie producer thread
  pthread_t blackberry_thr;
  string blackberry_arg = "Blackberry";
  if (pthread_create(&blackberry_thr, nullptr,
                     concurrent_producer, &blackberry_arg) != 0) {
    std::cerr << "Failed to create Blackberry pie thread" << std::endl;
    pthread_mutex_destroy(&write_lock);
    return EXIT_FAILURE;
  }

  // Attempt to create consumer thread
  pthread_t consumer_thr;
  if (pthread_create(&consumer_thr, nullptr, concurrent_consumer, nullptr) != 0) {
    std::cerr << "Failed to create consumer thread" << std::endl;
    pthread_mutex_destroy(&write_lock);
    return EXIT_FAILURE;
  }

  // Join the threads and exit
  pthread_join(apple_thr, nullptr);
  pthread_join(blackberry_thr, nullptr);
  pthread_join(consumer_thr, nullptr);

  pthread_mutex_destroy(&write_lock);
  return EXIT_SUCCESS;
}

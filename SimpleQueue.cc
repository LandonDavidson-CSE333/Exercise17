// Copyright 2025 Landon Davidson
// landond@uw.edu

#include "SimpleQueue.h"

SimpleQueue::SimpleQueue() {
  size = 0;
  front = nullptr;
  end = nullptr;
  // Initialize lock
  pthread_mutex_init(&lock, nullptr);
}

SimpleQueue::~SimpleQueue() {
  while (front != nullptr) {
    node *next = front->next;
    delete front;
    front = next;
  }
  // Delete lock
  pthread_mutex_destroy(&lock);
}

void SimpleQueue::Enqueue(string item) {
  // Lock critical section
  pthread_mutex_lock(&lock);
  node *new_node = new node();
  new_node->next = nullptr;
  new_node->item = item;
  if (end != nullptr) {
    end->next = new_node;
  } else  {
    front = new_node;
  }
  end = new_node;
  size++;
  // Unlock critical section
  pthread_mutex_unlock(&lock);
}

bool SimpleQueue::Dequeue(string *result) {
  // Lock critical section
  pthread_mutex_lock(&lock);
  if (size == 0) {
    return false;
  }
  *result = front->item;
  node *next = front->next;
  delete front;
  if (end == front) {
    end = front = next;
  } else {
    front = next;
  }
  size--;
  // Unlock critical section
  pthread_mutex_unlock(&lock);
  return true;
}

int SimpleQueue::Size() const {
  // Lock critical section (prevents data race with en/dequeue)
  pthread_mutex_lock(&lock);
  int res_size = size;
  // Unlock critical section
  pthread_mutex_unlock(&lock);
  return res_size;
}

bool SimpleQueue::IsEmpty() const {
  // Lock critical section (prevents data race with en/dequeue)
  pthread_mutex_lock(&lock);
  int res_size = size;
  // Unlock critical section
  pthread_mutex_unlock(&lock);
  return res_size == 0;
}

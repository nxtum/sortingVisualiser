#include "../include/Algorithms.h"
#include <thread>

void Algorithms::bubbleSort(SharedData &sharedData, std::atomic<bool> &stopFlag,
                            std::mutex &mtx) {
  // Bubble sort works by repeatedly swapping adjacent elements if they are in
  // the wrong order
  // The sharedData object is used to access the array and barsToHighlight


  std::vector<int> arr = sharedData.arr;
  int n = arr.size();
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (stopFlag) { // This means user has clicked the stop button
        mtx.lock();
        sharedData.barsToHighlight.clear();
        mtx.unlock();
        return;
      }
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]); // Swapping the elements
        std::this_thread::sleep_for(
            std::chrono::milliseconds(10)); // Delay for visual purposes
      }
      // Update the shareddata with the current state of the
      // algorithm
      mtx.lock();
      sharedData.arr = arr;
      sharedData.barsToHighlight = {j, j + 1};
      mtx.unlock();
    }
  }
  mtx.lock();
  sharedData.barsToHighlight.clear();
  mtx.unlock();
}

void Algorithms::selectionSort(SharedData &sharedData,
                               std::atomic<bool> &stopFlag, std::mutex &mtx) {
  // Selection sort works by repeatedly finding the minimum element from the
  // unsorted part of the array and moving it to the beginning


  std::vector<int> arr = sharedData.arr;
  int n = arr.size();
  for (int i = 0; i < n - 1; i++) {
    int minIndex = i;
    for (int j = i + 1; j < n; j++) {
      if (stopFlag) { // User has clicked the stop button
        mtx.lock();
        sharedData.barsToHighlight.clear();
        mtx.unlock();
        return;
      }
      if (arr[j] < arr[minIndex]) {
        minIndex = j;
      }
      // Update the shareddata with the current state of the
      // algorithm
      mtx.lock();
      sharedData.arr = arr;
      sharedData.barsToHighlight = {i, j, minIndex};
      mtx.unlock();
    }
    std::swap(arr[i], arr[minIndex]);
    std::this_thread::sleep_for(
        std::chrono::milliseconds(10));
  }
  mtx.lock();
  sharedData.barsToHighlight.clear();
  mtx.unlock();
}

void Algorithms::insertionSort(SharedData &sharedData,
                               std::atomic<bool> &stopFlag, std::mutex &mtx) {
  // Insertion sort works by building a sorted array one element at a time by
  // comparing each element with the rest of the array


  std::vector<int> arr = sharedData.arr;
  int n = arr.size();
  for (int i = 1; i < n; i++) {
    int key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j] > key) {
      if (stopFlag) {
        mtx.lock();
        sharedData.barsToHighlight.clear();
        mtx.unlock();
        return;
      }
      arr[j + 1] = arr[j];
      j = j - 1;

      mtx.lock();
      sharedData.arr = arr;
      sharedData.barsToHighlight = {i, j + 1};
      mtx.unlock();
    }
    arr[j + 1] = key;
    std::this_thread::sleep_for(
        std::chrono::milliseconds(10));
  }
  mtx.lock();
  sharedData.barsToHighlight.clear();
  mtx.unlock();
}

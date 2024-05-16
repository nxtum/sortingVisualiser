#ifndef SORTINGVISUALISER_ALGORITHMS_H
#define SORTINGVISUALISER_ALGORITHMS_H

#include "SharedData.h"
#include <atomic>
#include <mutex>
#include <vector>

class Algorithms {
public:
  static void bubbleSort(SharedData &sharedData, std::atomic<bool> &stopFlag,
                         std::mutex &mtx);
  static void insertionSort(SharedData &sharedData, std::atomic<bool> &stopFlag,
                            std::mutex &mtx);
  static void selectionSort(SharedData &sharedData, std::atomic<bool> &stopFlag,
                            std::mutex &mtx);
};

#endif // SORTINGVISUALISER_ALGORITHMS_H
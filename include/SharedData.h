
#ifndef DEMO3_SHAREDDATA_H
#define DEMO3_SHAREDDATA_H

#include <vector>

// Shared data between the Interface and Algorithms

struct SharedData {
  std::vector<int> arr;
  std::vector<int> barsToHighlight;
};

#endif // DEMO3_SHAREDDATA_H

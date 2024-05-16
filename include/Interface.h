#ifndef SORTINGVISUALISER_INTERFACE_H
#define SORTINGVISUALISER_INTERFACE_H

#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"
#include "../imgui/imgui.h"
#include "Algorithms.h"
#include "SharedData.h"
#include <GLFW/glfw3.h>
#include <atomic>
#include <mutex>
#include <random>
#include <unordered_set>
#include <vector>

class Interface {
public:
  Interface();
  void MainMenu();
  void DrawSorting(std::vector<int> &arr);

  void StartAction();
  void StopAction();
  void ResetAction();
  void ShuffleAction();

private:
  ImVec2 pos;
  ImVec2 windowSize;
  ImVec2 buttonSize;
  int numElements;
  std::vector<std::string> items;
  int selectedAlgorithm;
  std::vector<int> emptyVector;
  std::mutex mtx;
  SharedData sharedData;
  std::atomic<bool> stopFlag;
  std::vector<int> arrCopy;

  void DrawAlgorithmSelection();
  void DrawControlButtons();
  void DrawNumElementsSlider();
  void DrawBars(std::vector<int> &nums, std::vector<int> &barsToHighlight);
};

#endif // SORTINGVISUALISER_INTERFACE_H

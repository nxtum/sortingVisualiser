#include "../include/Interface.h"
#include "../include/Algorithms.h"
#include <algorithm>
#include <thread>

Interface::Interface() {
  pos = ImVec2(0, 0);
  windowSize = ImVec2(960, 50);
  buttonSize = ImVec2(125, 30);
  numElements = 50;
  selectedAlgorithm = 0;
  stopFlag = false;
  arrCopy = emptyVector;
  sharedData = {emptyVector, emptyVector};
  items = {"Bubble Sort", "Insertion Sort", "Selection Sort"};
}

void Interface::MainMenu() {
  if (sharedData.arr.empty()) {
    ShuffleAction(); // Initialize the array with random values of 50 elements
  }

  ImGui::SetNextWindowPos(pos);
  ImGui::SetNextWindowSize(windowSize);
  ImGui::Begin("##MainMenu", nullptr,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

  // Background color for the main menu
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));

  // Main menu icons + components
  DrawAlgorithmSelection();
  DrawControlButtons();
  DrawNumElementsSlider();

  ImGui::PopStyleColor();
  ImGui::End();
}

void Interface::DrawSorting(std::vector<int> &nums) {
  // Sorting window visualization of bars
  ImGui::SetNextWindowPos(ImVec2(0, windowSize.y));
  ImGui::SetNextWindowSize(ImVec2(960, 870));

  ImGui::Begin("##SortingWindow", nullptr,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

  // Background color for the sorting window
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));

  // Calls function that draws the bars
  DrawBars(sharedData.arr, sharedData.barsToHighlight);

  ImGui::PopStyleColor();
  ImGui::End();
}

void Interface::StartAction() {
  // Called when user clicks the start button


  // Copy the array for resetting
  arrCopy = sharedData.arr;

  // Thread for sorting algorithm
  // This is so that the sorting algorithm can run independently
  // of the main thread
  // Each case corresponds to a different sorting algorithm called by static functions

  std::thread sortingThread;
  switch (selectedAlgorithm) {
  case 0:
    sortingThread = std::thread(
        [&]() { Algorithms::bubbleSort(sharedData, stopFlag, mtx); });
    sortingThread
        .detach();
    break;
  case 1:
    sortingThread = std::thread(
        [&]() { Algorithms::insertionSort(sharedData, stopFlag, mtx); });
    sortingThread.detach();
    break;
  case 2:
    sortingThread = std::thread(
        [&]() { Algorithms::selectionSort(sharedData, stopFlag, mtx); });
    sortingThread.detach();
    break;

  default:
    // nothing
    break;
  }
}

void Interface::ResetAction() {

  // Reset the array to its original state

  mtx.lock();
  sharedData.arr = arrCopy;
  sharedData.barsToHighlight.clear();
  mtx.unlock();
}

void Interface::ShuffleAction() {

  // Shuffle the array with random values
  // Numbers from 1-200, no duplicates allowed hence find

  sharedData.arr.clear();
  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_int_distribution<int> distribution(1, 200);

  while (sharedData.arr.size() < numElements) {
    int randomNumber = distribution(g);
    if (std::find(sharedData.arr.begin(), sharedData.arr.end(), randomNumber) ==
        sharedData.arr.end()) {
      sharedData.arr.push_back(randomNumber);
    }
  }
}

void Interface::StopAction() {

  // Stop the sorting algorithm
  // Set true for a brief moment to stop the sorting algorithm

  stopFlag = true;
  std::this_thread::sleep_for(
      std::chrono::milliseconds(100));
  stopFlag = false;
}

void Interface::DrawBars(std::vector<int> &nums,
                         std::vector<int> &barsToHighlight) {


  // Draws the actual bars in the sorting window


  ImGui::SetNextWindowPos(ImVec2(0, windowSize.y));
  ImGui::SetNextWindowSize(ImVec2(960, 870));

  ImGui::Begin("##SortingWindow", nullptr,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

  // Background color
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));

  // I use this as a scale
  int maxNum = 65;

  // Calculations based on window size + num of elements
  float availableWidth = 960.0f;
  float totalSpacing = (numElements) * 8.08f;
  float barWidth = (availableWidth - totalSpacing) / numElements;
  float scaleFactor = (maxNum > 0) ? 200.0f / maxNum : 1.0f;

  // Drawing
  for (int i = 0; i < numElements; i++) {
    ImGui::SetCursorPosX(i * (barWidth + 8.1f));
    if (std::find(barsToHighlight.begin(), barsToHighlight.end(), i) !=
        barsToHighlight.end()) {
      ImGui::PushStyleColor(
          ImGuiCol_Button,
          ImVec4(1.0f, 1.0f, 1.0f, 1.00f));
    } else {
      ImGui::PushStyleColor(
          ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
    }
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.35f, 0.35f, 0.35f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(0.45f, 0.45f, 0.45f, 1.00f));
    ImGui::Button(
        "##Bar", ImVec2(barWidth,
                        sharedData.arr[i] *
                            scaleFactor));
    ImGui::PopStyleColor(3);
    if (i < numElements - 1) {
      ImGui::SameLine();
    }
  }

  ImGui::PopStyleColor();
  ImGui::End();
}

void Interface::DrawAlgorithmSelection() {

  // Algorithm selection combo box


  ImGui::SetCursorPosY(
      ImGui::GetCursorPosY() +
      ImGui::GetStyle().ItemSpacing.y);
  ImGui::PushItemWidth(200);

  if (ImGui::BeginCombo("##SelectCombo", items[selectedAlgorithm].c_str())) {
    for (int i = 0; i < items.size(); i++) {
      bool isSelected = (selectedAlgorithm == i);
      if (ImGui::Selectable(items[i].c_str(), isSelected)) {
        selectedAlgorithm = i; // Updates selected algorithm
      }
      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
  ImGui::PopItemWidth();
}

void Interface::DrawControlButtons() {
  ImGui::SameLine();
  if (ImGui::Button("Start", buttonSize)) {
    StartAction();
  }

  ImGui::SameLine();
  if (ImGui::Button("Stop", buttonSize)) {
    StopAction();
  }

  ImGui::SameLine();
  if (ImGui::Button("Reset", buttonSize)) {
    ResetAction();
  }

  ImGui::SameLine();
  if (ImGui::Button("Shuffle", buttonSize)) {
    ShuffleAction();
  }
}

void Interface::DrawNumElementsSlider() {
  // Sliders for number of elements


  float sliderWidth = windowSize.x - (4 * buttonSize.x) - 240 -
                      ImGui::GetStyle().ItemSpacing.x * 3;

  ImGui::SameLine();


  ImGui::SetCursorPosX(
      ImGui::GetCursorPosX() +
      ImGui::GetStyle()
          .ItemSpacing.x);
  ImGui::SetNextItemWidth(sliderWidth);
  if (ImGui::SliderInt("##NumElementsSlider", &numElements, 20, 100,
                       "NumElements: %d")) {
    ShuffleAction(); // Calls shuffle whenever slider value changes
  }
}

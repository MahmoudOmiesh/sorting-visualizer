# Sorting Algorithm Visualizer

A sorting visualizer built using **c++** and **qt widgets** to demonstrate the step-by-step execution of popular sorting algorithms.

## Key Features
- **Supoorted Algorithms:**:
    * Bubble Sort
    * Selection Sort
    * Insertion Sort
    * Merge Sort
    * Quick Sort
    * Heap Sort
- **Real-Time Visualization:** Dynamically display the sorting process, with columns representing the array the is being sorted.
- **Visual Feedback:** The app provides visual feedback by highlight columns being compared or swapped, and changes their color to indicate their state.
- **Threading:** Implemented multi-threading to ensure the UI remains responsive during the sorting process.  The algorithms run in a separate thread, allowing users to stop at any time.
- **Customizable:** Users can adjust the number of columns and the delay between steps to control the speed and complexity of the visualization.


## Project Structure

- **main.cpp:** Entry point of the application, initializes the Qt application and displays the main window.
- **sorter.cpp:** Implements the main logic for  the GUI, include column generation, sorting algorithm selection, and interaction with the visualizer thread.
- **visualizer.cpp:** Contains the implementation of the sorting algorithms, and handles the visualization logic, including columns swapping and highlighting.
- **sorter.ui:** XML-based UI design file for the main window, created using Qt Designer. 

## Installation
Download the release folder and run `sorting-visualizer.exe`

## Demo
### Bubble Sort
![](https://github.com/MahmoudOmiesh/sorting-visualizer/blob/main/demos/bubble%20sort.gif)
### Selection Sort
![](https://github.com/MahmoudOmiesh/sorting-visualizer/blob/main/demos/selection%20sort.gif)
### Insertion Sort
![](https://github.com/MahmoudOmiesh/sorting-visualizer/blob/main/demos/insertion%20sort.gif)
### Merge Sort
![](https://github.com/MahmoudOmiesh/sorting-visualizer/blob/main/demos/merge%20sort.gif)
### Quick Sort
![](https://github.com/MahmoudOmiesh/sorting-visualizer/blob/main/demos/quick%20sort.gif)
### Heap Sort
![](https://github.com/MahmoudOmiesh/sorting-visualizer/blob/main/demos/heap%20sort.gif)

# Memory Management System

A **C++-based Memory Management Simulator** developed as an Operating Systems Lab project. This project demonstrates how different page replacement algorithms work and how they manage memory frames during page requests.

It provides a step-by-step simulation of page replacement processes, allowing users to understand page hits, page faults, and the performance differences between algorithms.

## Features

- **Multiple Page Replacement Algorithms:** Implements FIFO, LRU, Optimal, and Clock (Second Chance).
- **Step-by-Step Simulation:** Displays the page reference sequence, memory frames, and whether each request results in a hit or fault.
- **Performance Analysis:** Calculates total page references, page hits, page faults, hit ratio, and fault ratio.
- **Algorithm Comparison:** Compares all four algorithms using the same reference string.
- **Belady's Anomaly Demonstration:** Demonstrates how increasing the number of frames can sometimes increase page faults in FIFO.
- **Frame-Size Comparison:** Compares page faults for different numbers of memory frames using a text-based chart.
- **Random Reference String Generation:** Generates random page references with locality of reference.
- **Working-Set View:** Displays the distinct pages within a specified reference window.
- **File Input:** Allows users to load page reference strings from a text file.
- **Input Validation:** Handles invalid inputs to ensure valid simulation data.

## Technologies Used

- **Programming Language:** C++
- **Compiler:** GCC (g++)
- **Libraries:** iostream, vector, string, fstream, sstream, iomanip, cstdlib, ctime, set

## Page Replacement Algorithms

| Algorithm | Description |
|---|---|
| FIFO | Replaces the page that entered memory first. |
| LRU | Replaces the page that has not been used for the longest time. |
| Optimal | Replaces the page that will not be needed for the longest time in the future. |
| Clock (Second Chance) | Uses reference bits to give recently used pages a second chance before replacement. |

## How to Run

### 1. Clone the Repository

```bash
git clone https://github.com/tay-e-bah/Memory-Management-System.git
```

### 2. Navigate to the Project Directory

```bash
cd Memory-Management-System
```

### 3. Compile the Source Code

```bash
g++ memory_simulator.cpp -o memory_simulator
```

### 4. Run the Program

**Windows:**

```bash
memory_simulator.exe
```

**Linux/macOS:**

```bash
./memory_simulator
```

## How to Use

1. Run the program.
2. Enter a page reference string manually, load it from a file, or generate one randomly.
3. Set the number of memory frames.
4. Select a page replacement algorithm to view its step-by-step simulation.
5. Compare the performance of different algorithms.
6. Explore the additional features to understand memory management concepts.

## Learning Objectives

- Understand the concept of virtual memory and page replacement.
- Learn how different page replacement algorithms operate.
- Analyze page hits and page faults.
- Compare the performance of different replacement strategies.
- Understand Belady's Anomaly and locality of reference.
- Explore the working-set concept in memory management.

## Project Information

**Project Type:** Operating Systems Lab Project  
**Language:** C++  
**Repository:** [Memory Management System](https://github.com/tay-e-bah/Memory-Management-System)

---

Developed as an academic project to explore and understand memory management concepts in Operating Systems.
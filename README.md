# Memory Management Simulator

This project was developed as part of our **Operating Systems course**.
It is a simple simulator that helps demonstrate how page replacement
works when a process needs a page that is not currently available in
physical memory.

The project has two parts: a C++ console application and a browser-based
graphical interface. Both allow users to work with page reference
strings, choose the number of memory frames, and observe how different
page replacement algorithms behave.

## About the Project

In an operating system, memory is divided into fixed-size frames, while
a process is divided into pages. When a requested page is not found in
the available frames, a page fault occurs. If all frames are already
occupied, the system needs to decide which page should be removed to
make room for the new one.

This simulator demonstrates that process using four commonly studied
page replacement algorithms. It also shows how their results can differ
for the same reference string and number of frames.

## Algorithms Included

The project includes four page replacement algorithms:

-   **FIFO (First-In, First-Out):** Replaces the page that entered
    memory first.
-   **LRU (Least Recently Used):** Replaces the page that has not been
    used for the longest time.
-   **Optimal:** Replaces the page that will be needed farthest in the
    future. This algorithm is mainly used as a benchmark because it
    requires knowledge of future page references.
-   **Clock (Second Chance):** Uses a reference bit and a circular
    pointer to give recently used pages another chance before replacing
    them.

## What Can Be Done With This Simulator?

Users can:

-   Enter a page reference string manually.
-   Load a reference string from a text file.
-   Generate a random reference string with locality of reference.
-   Choose the number of memory frames.
-   Run any of the four algorithms and follow the frame changes step by
    step.
-   See which references result in a page hit or a page fault.
-   Compare the number of hits, faults, and their ratios for all four
    algorithms.
-   Try the Belady's Anomaly demonstration.
-   Compare page faults with different numbers of frames.
-   View the working set for a selected window size.

The graphical interface also has controls to move through the simulation
one step at a time, play it automatically, or show all steps together.
It includes separate sections for algorithm comparison, Belady's
Anomaly, Working Set, and short algorithm descriptions. Users can also
switch between light and dark themes.

## Project Files

  -----------------------------------------------------------------------
  File                                What it contains
  ----------------------------------- -----------------------------------
  `memory_simulator.cpp`              The C++ console application,
                                      including the four algorithms and
                                      the additional simulation features.

  `page_replacement_gui.html`         The graphical version, built with
                                      HTML, CSS, and JavaScript.
  -----------------------------------------------------------------------

## Technologies Used

-   C++
-   HTML
-   CSS
-   JavaScript

The graphical interface is contained in a single HTML file, so no
external libraries or frameworks are needed.

## Running the Project

### C++ Console Application

You need a C++ compiler such as `g++`.

First, open a terminal in the project folder and compile the file:

``` bash
g++ memory_simulator.cpp -o memory_simulator
```

Then run it.

**Linux or macOS:**

``` bash
./memory_simulator
```

**Windows:**

``` bash
memory_simulator.exe
```

After launching the program, follow the menu to enter a reference
string, set the number of frames, and choose an algorithm or one of the
extra features.

### Graphical Interface

1.  Download or clone this repository.
2.  Open the project folder.
3.  Open `page_replacement_gui.html` in a web browser.
4.  Enter a reference string and choose an algorithm and number of
    frames.
5.  Click **Start Visualization**.

No server or additional installation is required. The interface runs
directly in the browser.

## Input Example

A page reference string can be entered using spaces or commas:

``` text
7 0 1 2 0 3 0 4 2 3 0 3 2
```

For the C++ application, page numbers must be non-negative whole
numbers. The number of frames can be set from 1 to 10.

The graphical interface also accepts a compact sequence such as `70120`
for single-digit page references. If you want to use multi-digit page
numbers, separate them with spaces or commas.

You can also load a `.txt` file containing the reference string in
either version.

## Operating Systems Concepts

While working with the project, the following topics can be explored:

-   Paging and memory frames
-   Page hits and page faults
-   Page replacement
-   FIFO, LRU, Optimal, and Clock algorithms
-   Locality of reference
-   Working set
-   Belady's Anomaly
-   Comparing page replacement performance

## A Few Notes

-   The C++ application and the browser interface are separate
    implementations. For a fair comparison, use the same reference
    string, algorithm, and number of frames in both.
-   Optimal uses future page references to decide which page to replace,
    so it is a theoretical benchmark rather than a practical algorithm
    for normal online use.
-   Randomly generated reference strings can be different each time.
-   In the graphical interface, a continuous sequence of three or more
    digits is read as individual single-digit page numbers. Use spaces
    or commas for multi-digit page numbers.

## Project Information

**Project Name:** Memory Management Simulator\
**Course:** Operating Systems\
**Academic Level:** Undergraduate, 3rd Year

This project was made to put the page replacement topics covered in the
Operating Systems course into practice and make the working process
easier to understand through both a console program and a graphical
visualization.

// ============================================================
//  Memory Management Simulator  (Page Replacement Algorithms)
//  Operating Systems Lab Project
//
//  Algorithms : FIFO, LRU, Optimal, Clock (Second Chance)
//  Extras     : Belady's Anomaly demo, frame-size comparison chart,
//               random reference string with locality, working-set view
//
//  Compile    : g++ memory_simulator.cpp -o memory_simulator
//  Run        : ./memory_simulator        (Windows: memory_simulator.exe)
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <set>
using namespace std;

// ------------------------------------------------------------
//  A small structure to store the result of one algorithm run
// ------------------------------------------------------------
struct Result {
    string name;
    int total;    // total page references
    int hits;
    int faults;
};

// ------------------------------------------------------------
//  Global data (the current reference string and frame count)
// ------------------------------------------------------------
vector<int> pages;      // page reference string
int numFrames = 0;      // number of memory frames (0 = not set yet)

// ============================================================
//  PART 1: Small helper functions
// ============================================================

// Returns the index of 'value' inside 'frames', or -1 if not found.
// (We store -1 in a frame to mean "empty", so findPage(frames, -1)
//  finds an empty frame.)
int findPage(const vector<int>& frames, int value) {
    for (int i = 0; i < (int)frames.size(); i++) {
        if (frames[i] == value) return i;
    }
    return -1;
}

// Reads an integer from the user and keeps asking until it is valid
// and between minValue and maxValue.
int readInt(const string& prompt, int minValue, int maxValue) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);

        istringstream ss(line);
        int number;
        string extra;
        if ((ss >> number) && !(ss >> extra)) {
            if (number >= minValue && number <= maxValue) return number;
        }
        cout << "  Invalid input! Please enter a number between "
             << minValue << " and " << maxValue << ".\n";
    }
}

// Converts a line of text like "7 0 1, 2 0" into numbers.
// Returns false if something is not a valid non-negative number.
bool parsePages(string text, vector<int>& result) {
    // treat commas as spaces
    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] == ',') text[i] = ' ';
    }
    istringstream ss(text);
    string token;
    result.clear();
    while (ss >> token) {
        for (int i = 0; i < (int)token.size(); i++) {
            if (token[i] < '0' || token[i] > '9') return false;
        }
        if (token.size() > 6) return false;   // too big
        result.push_back(atoi(token.c_str()));
    }
    return !result.empty();
}

void printLine(int length = 60, char c = '=') {
    cout << string(length, c) << "\n";
}

void showPages() {
    for (int i = 0; i < (int)pages.size(); i++) cout << pages[i] << " ";
    cout << "\n";
}

// ============================================================
//  PART 2: Printing the step-by-step table
// ============================================================

void printHeader(const string& name, int n) {
    cout << "\n";
    printLine();
    cout << "  " << name << " Page Replacement  (" << n << " frames)\n";
    printLine();
    cout << left << setw(6) << "Step" << setw(6) << "Page"
         << setw(n * 4 + 2) << "Frames" << "Result\n";
    printLine(60, '-');
}

void printStep(int step, int page, const vector<int>& frames, bool hit) {
    cout << left << setw(6) << step << setw(6) << page;
    for (int i = 0; i < (int)frames.size(); i++) {
        if (frames[i] == -1) cout << setw(4) << "-";
        else cout << setw(4) << frames[i];
    }
    cout << "  " << (hit ? "H (Hit)" : "F (Fault)") << "\n";
}

Result makeResult(const string& name, int total, int hits) {
    Result r;
    r.name = name;
    r.total = total;
    r.hits = hits;
    r.faults = total - hits;
    return r;
}

void printResult(const Result& r) {
    double hitRatio = 100.0 * r.hits / r.total;
    double faultRatio = 100.0 * r.faults / r.total;
    printLine(60, '-');
    cout << "Results for " << r.name << ":\n";
    cout << "  Total references : " << r.total << "\n";
    cout << "  Total hits       : " << r.hits << "\n";
    cout << "  Total page faults: " << r.faults << "\n";
    cout << fixed << setprecision(2);
    cout << "  Hit ratio        : " << hitRatio << " %\n";
    cout << "  Fault ratio      : " << faultRatio << " %\n";
}

// ============================================================
//  PART 3: The page replacement algorithms
//  Each function returns a Result. If show == true, it prints
//  the step-by-step table, otherwise it runs silently
//  (used by the comparison features).
// ============================================================

// ---------------- FIFO ----------------
// The page that has been in memory the longest is replaced.
Result fifo(const vector<int>& refs, int n, bool show) {
    vector<int> frames(n, -1);   // -1 means empty
    int nextToReplace = 0;       // points to the oldest page
    int hits = 0;

    if (show) printHeader("FIFO", n);

    for (int i = 0; i < (int)refs.size(); i++) {
        int page = refs[i];
        bool hit = (findPage(frames, page) != -1);

        if (hit) {
            hits++;
        } else {
            frames[nextToReplace] = page;
            nextToReplace = (nextToReplace + 1) % n;   // move to next
        }
        if (show) printStep(i + 1, page, frames, hit);
    }

    Result r = makeResult("FIFO", refs.size(), hits);
    if (show) printResult(r);
    return r;
}

// ---------------- LRU ----------------
// The page that has not been used for the longest time is replaced.
Result lru(const vector<int>& refs, int n, bool show) {
    vector<int> frames(n, -1);
    vector<int> lastUsed(n, 0);   // time when each frame was last used
    int hits = 0;

    if (show) printHeader("LRU", n);

    for (int i = 0; i < (int)refs.size(); i++) {
        int page = refs[i];
        int pos = findPage(frames, page);
        bool hit = (pos != -1);

        if (hit) {
            hits++;
            lastUsed[pos] = i;                 // update usage time
        } else {
            int empty = findPage(frames, -1);
            if (empty != -1) {
                frames[empty] = page;          // use an empty frame
                lastUsed[empty] = i;
            } else {
                // find the frame with the smallest lastUsed value
                int victim = 0;
                for (int f = 1; f < n; f++) {
                    if (lastUsed[f] < lastUsed[victim]) victim = f;
                }
                frames[victim] = page;
                lastUsed[victim] = i;
            }
        }
        if (show) printStep(i + 1, page, frames, hit);
    }

    Result r = makeResult("LRU", refs.size(), hits);
    if (show) printResult(r);
    return r;
}

// ---------------- Optimal ----------------
// Replace the page that will not be used for the longest time in
// the future. (Needs the future, so it is only for comparison.)
Result optimal(const vector<int>& refs, int n, bool show) {
    vector<int> frames(n, -1);
    int hits = 0;
    int total = refs.size();

    if (show) printHeader("Optimal", n);

    for (int i = 0; i < total; i++) {
        int page = refs[i];
        bool hit = (findPage(frames, page) != -1);

        if (hit) {
            hits++;
        } else {
            int empty = findPage(frames, -1);
            if (empty != -1) {
                frames[empty] = page;
            } else {
                int victim = 0;
                int farthest = -1;
                for (int f = 0; f < n; f++) {
                    // look into the future for the next use of this page
                    int nextUse = -1;
                    for (int j = i + 1; j < total; j++) {
                        if (refs[j] == frames[f]) {
                            nextUse = j;
                            break;
                        }
                    }
                    if (nextUse == -1) {       // never used again: best choice
                        victim = f;
                        break;
                    }
                    if (nextUse > farthest) {
                        farthest = nextUse;
                        victim = f;
                    }
                }
                frames[victim] = page;
            }
        }
        if (show) printStep(i + 1, page, frames, hit);
    }

    Result r = makeResult("Optimal", total, hits);
    if (show) printResult(r);
    return r;
}

// ---------------- Clock (Second Chance) ----------------
// Like FIFO, but each page has a reference bit. If the bit is 1,
// the page gets a "second chance" (bit set to 0) and we move on.
Result clockAlgo(const vector<int>& refs, int n, bool show) {
    vector<int> frames(n, -1);
    vector<int> refBit(n, 0);
    int hand = 0;                 // the clock hand
    int hits = 0;

    if (show) {
        printHeader("Clock (Second Chance)", n);
        cout << "(Reference bit of each frame is shown in brackets)\n";
    }

    for (int i = 0; i < (int)refs.size(); i++) {
        int page = refs[i];
        int pos = findPage(frames, page);
        bool hit = (pos != -1);

        if (hit) {
            hits++;
            refBit[pos] = 1;     // page was used again
        } else {
            // move the hand until we find a frame to replace
            while (true) {
                if (frames[hand] == -1 || refBit[hand] == 0) {
                    frames[hand] = page;
                    refBit[hand] = 1;
                    hand = (hand + 1) % n;
                    break;
                } else {
                    refBit[hand] = 0;           // give a second chance
                    hand = (hand + 1) % n;
                }
            }
        }

        if (show) {
            cout << left << setw(6) << i + 1 << setw(6) << page;
            for (int f = 0; f < n; f++) {
                string cell;
                if (frames[f] == -1) cell = "-";
                else cell = to_string(frames[f]) + "[" + to_string(refBit[f]) + "]";
                cout << setw(7) << cell;
            }
            cout << "  " << (hit ? "H (Hit)" : "F (Fault)") << "\n";
        }
    }

    Result r = makeResult("Clock", refs.size(), hits);
    if (show) printResult(r);
    return r;
}

// ============================================================
//  PART 4: Menu actions
// ============================================================

// Menu 1: enter the reference string by hand
void inputManually() {
    cout << "\nEnter the page reference string (numbers separated by spaces).\n";
    cout << "Example: 7 0 1 2 0 3 0 4 2 3 0 3 2\n> ";
    string line;
    getline(cin, line);

    vector<int> temp;
    if (parsePages(line, temp)) {
        pages = temp;
        cout << "Reference string saved (" << pages.size() << " pages).\n";
    } else {
        cout << "Invalid input! Use only non-negative whole numbers.\n";
    }
}

// Menu 2: load the reference string from a text file
void inputFromFile() {
    cout << "\nEnter the file name (example: sample_input.txt): ";
    string filename;
    getline(cin, filename);

    ifstream file(filename.c_str());
    if (!file) {
        cout << "Could not open the file '" << filename << "'.\n";
        return;
    }

    string content, line;
    while (getline(file, line)) content += line + " ";
    file.close();

    vector<int> temp;
    if (parsePages(content, temp)) {
        pages = temp;
        cout << "Loaded " << pages.size() << " pages from the file.\n";
    } else {
        cout << "The file is empty or contains invalid data.\n";
    }
}

// Menu 3: random reference string with locality of reference
// Most references come from a small "locality" set of pages,
// and sometimes the program moves to a new locality.
void generateRandom() {
    int length = readInt("\nHow many page references (5-100)? ", 5, 100);
    int maxPage = readInt("Highest page number (1-20)? ", 1, 20);

    vector<int> locality;                 // current set of "hot" pages
    int localSize = 3;
    for (int i = 0; i < localSize; i++) locality.push_back(rand() % (maxPage + 1));

    pages.clear();
    for (int i = 0; i < length; i++) {
        if (rand() % 100 < 80) {
            // 80%: pick a page from the locality set
            pages.push_back(locality[rand() % localSize]);
        } else {
            // 20%: pick a random page and make it part of the locality
            int p = rand() % (maxPage + 1);
            locality[rand() % localSize] = p;
            pages.push_back(p);
        }
    }
    cout << "Generated reference string:\n";
    showPages();
}

// Menu 4: set the number of frames
void setFrames() {
    numFrames = readInt("\nEnter number of memory frames (1-10): ", 1, 10);
    cout << "Number of frames set to " << numFrames << ".\n";
}

// Checks that we have data before running an algorithm
bool readyToRun() {
    if (pages.empty()) {
        cout << "\nPlease enter a page reference string first (options 1, 2 or 3).\n";
        return false;
    }
    if (numFrames == 0) {
        cout << "\nPlease set the number of frames first (option 4).\n";
        return false;
    }
    return true;
}

// Menu 9: compare all algorithms on the same string
void compareAll() {
    Result r[4];
    r[0] = fifo(pages, numFrames, false);
    r[1] = lru(pages, numFrames, false);
    r[2] = optimal(pages, numFrames, false);
    r[3] = clockAlgo(pages, numFrames, false);

    cout << "\n";
    printLine();
    cout << "  Comparison  (" << pages.size() << " references, "
         << numFrames << " frames)\n";
    printLine();
    cout << left << setw(12) << "Algorithm" << setw(8) << "Faults"
         << setw(8) << "Hits" << setw(12) << "Hit Ratio" << "Fault Ratio\n";
    printLine(60, '-');
    cout << fixed << setprecision(2);
    int best = 0;
    for (int i = 0; i < 4; i++) {
        double hr = 100.0 * r[i].hits / r[i].total;
        double fr = 100.0 * r[i].faults / r[i].total;
        cout << left << setw(12) << r[i].name << setw(8) << r[i].faults
             << setw(8) << r[i].hits << hr << "%      " << fr << "%\n";
        if (r[i].faults < r[best].faults) best = i;
    }
    printLine(60, '-');
    cout << "Fewest page faults: " << r[best].name << " (" << r[best].faults << " faults)\n";
    cout << "Note: Optimal is the theoretical best and is used as a benchmark.\n";
}

// Menu 10: Belady's anomaly demonstration
void beladyDemo() {
    cout << "\nBelady's Anomaly: with FIFO, MORE frames can cause MORE page faults.\n";
    cout << "1. Use the classic example string (1 2 3 4 1 2 5 1 2 3 4 5)\n";
    cout << "2. Use my current reference string\n";
    int choice = readInt("Choose (1-2): ", 1, 2);

    vector<int> refs;
    if (choice == 1) {
        int classic[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
        for (int i = 0; i < 12; i++) refs.push_back(classic[i]);
    } else {
        if (pages.empty()) {
            cout << "No reference string entered yet.\n";
            return;
        }
        refs = pages;
    }

    cout << "\nReference string: ";
    for (int i = 0; i < (int)refs.size(); i++) cout << refs[i] << " ";
    cout << "\n\n";

    int fifo3 = fifo(refs, 3, false).faults;
    int fifo4 = fifo(refs, 4, false).faults;
    int lru3 = lru(refs, 3, false).faults;
    int lru4 = lru(refs, 4, false).faults;

    cout << left << setw(12) << "Algorithm" << setw(14) << "3 frames" << "4 frames\n";
    printLine(40, '-');
    cout << left << setw(12) << "FIFO" << setw(14) << fifo3 << fifo4 << "\n";
    cout << left << setw(12) << "LRU" << setw(14) << lru3 << lru4 << "\n";
    printLine(40, '-');

    if (fifo4 > fifo3) {
        cout << "Belady's Anomaly occurred! FIFO had MORE faults with 4 frames ("
             << fifo4 << ") than with 3 frames (" << fifo3 << ").\n";
    } else {
        cout << "No anomaly for FIFO with this string (try the classic example).\n";
    }
    if (lru4 <= lru3) {
        cout << "LRU never shows this anomaly: more frames never gave more faults.\n";
    }
}

// Menu 11: compare different frame sizes with a text chart
void frameComparison() {
    if (pages.empty()) {
        cout << "\nPlease enter a page reference string first.\n";
        return;
    }
    int maxFrames = readInt("\nCompare frame sizes from 1 up to (2-10): ", 2, 10);

    vector<int> f1, l1, o1;
    int biggest = 1;
    for (int n = 1; n <= maxFrames; n++) {
        f1.push_back(fifo(pages, n, false).faults);
        l1.push_back(lru(pages, n, false).faults);
        o1.push_back(optimal(pages, n, false).faults);
        if (f1.back() > biggest) biggest = f1.back();
        if (l1.back() > biggest) biggest = l1.back();
        if (o1.back() > biggest) biggest = o1.back();
    }

    // table
    cout << "\n";
    printLine();
    cout << "  Page faults for different numbers of frames\n";
    printLine();
    cout << left << setw(10) << "Frames" << setw(8) << "FIFO" << setw(8) << "LRU" << "Optimal\n";
    printLine(40, '-');
    for (int n = 1; n <= maxFrames; n++) {
        cout << left << setw(10) << n << setw(8) << f1[n - 1]
             << setw(8) << l1[n - 1] << o1[n - 1] << "\n";
    }

    // text chart: one '#' per fault (scaled if too many)
    int scale = 1;
    if (biggest > 50) scale = (biggest + 49) / 50;
    cout << "\nChart (each # = " << scale << " page fault(s)):\n";
    for (int n = 1; n <= maxFrames; n++) {
        cout << "\nFrames = " << n << "\n";
        cout << "  FIFO    | " << string(f1[n - 1] / scale, '#') << " " << f1[n - 1] << "\n";
        cout << "  LRU     | " << string(l1[n - 1] / scale, '#') << " " << l1[n - 1] << "\n";
        cout << "  Optimal | " << string(o1[n - 1] / scale, '#') << " " << o1[n - 1] << "\n";
    }
}

// Menu 12: working-set view
// Shows the distinct pages used in the last K references.
void workingSet() {
    if (pages.empty()) {
        cout << "\nPlease enter a page reference string first.\n";
        return;
    }
    int k = readInt("\nWindow size K (1-20): ", 1, 20);

    cout << "\n";
    printLine();
    cout << "  Working-set view (last " << k << " references)\n";
    printLine();
    cout << left << setw(6) << "Time" << setw(6) << "Page" << setw(26) << "Working set" << "Size\n";
    printLine(60, '-');

    int total = pages.size();
    for (int i = 0; i < total; i++) {
        set<int> ws;                               // a set keeps only distinct values
        int start = (i - k + 1 > 0) ? i - k + 1 : 0;
        for (int j = start; j <= i; j++) ws.insert(pages[j]);

        string text = "{ ";
        for (set<int>::iterator it = ws.begin(); it != ws.end(); ++it) {
            text += to_string(*it) + " ";
        }
        text += "}";
        cout << left << setw(6) << i + 1 << setw(6) << pages[i]
             << setw(26) << text << ws.size() << "\n";
    }
}

void showCurrentData() {
    cout << "\nReference string: ";
    if (pages.empty()) cout << "(not set)\n";
    else showPages();
    cout << "Number of frames: ";
    if (numFrames == 0) cout << "(not set)\n";
    else cout << numFrames << "\n";
}

// ============================================================
//  PART 5: Main menu
// ============================================================

void showMenu() {
    cout << "\n";
    printLine();
    cout << "     MEMORY MANAGEMENT SIMULATOR\n";
    printLine();
    cout << " --- Input ---\n";
    cout << "  1. Enter page reference string manually\n";
    cout << "  2. Load page reference string from a file\n";
    cout << "  3. Generate random string (with locality)\n";
    cout << "  4. Set number of frames\n";
    cout << "  5. Show current data\n";
    cout << " --- Algorithms ---\n";
    cout << "  6. FIFO\n";
    cout << "  7. LRU\n";
    cout << "  8. Optimal\n";
    cout << "  9. Clock (Second Chance)\n";
    cout << " 10. Compare all algorithms\n";
    cout << " --- Extra features ---\n";
    cout << " 11. Belady's Anomaly demo\n";
    cout << " 12. Frame-size comparison chart\n";
    cout << " 13. Working-set view\n";
    cout << "  0. Exit\n";
    printLine();
}

int main() {
    srand((unsigned)time(0));   // seed for random numbers

    int choice;
    do {
        showMenu();
        choice = readInt("Enter your choice: ", 0, 13);

        switch (choice) {
            case 1:  inputManually();   break;
            case 2:  inputFromFile();   break;
            case 3:  generateRandom();  break;
            case 4:  setFrames();       break;
            case 5:  showCurrentData(); break;
            case 6:  if (readyToRun()) fifo(pages, numFrames, true);      break;
            case 7:  if (readyToRun()) lru(pages, numFrames, true);       break;
            case 8:  if (readyToRun()) optimal(pages, numFrames, true);   break;
            case 9:  if (readyToRun()) clockAlgo(pages, numFrames, true); break;
            case 10: if (readyToRun()) compareAll();                      break;
            case 11: beladyDemo();       break;
            case 12: frameComparison();  break;
            case 13: workingSet();       break;
            case 0:  cout << "\nGoodbye!\n"; break;
        }
    } while (choice != 0);

    return 0;
}

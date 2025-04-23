#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <limits>
#include "Ad.h"
#include "MergeSort.h"
#include "QuickSort.h"
using namespace std;

// string listing which content flags are true
string content(const ContentFlags& flags) {
    vector<string> parts;
    if (flags.funny) {
        parts.push_back("Funny");
    }
    if (flags.product) {
        parts.push_back("Product");
    }
    if (flags.patriotic) {
        parts.push_back("Patriotic");
    }
    if (flags.celebrity) {
        parts.push_back("Celebrity");
    }
    if (flags.danger) {
        parts.push_back("Danger");
    }
    if (flags.animals) {
        parts.push_back("Animals");
    }
    if (flags.sexual) {
        parts.push_back("Sexual");
    }
    if (parts.empty()) {
        return "(none)";
    }

    ostringstream oss;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i) {
            oss << ", ";
        }
        oss << parts[i];
    }
    return oss.str();
}

// Remove leading and trailing whitespace from string
string trim(const string& s) {
    auto left  = s.find_first_not_of(" \t\r\n");
    if (left == string::npos) {
        return "";
    }

    auto right = s.find_last_not_of(" \t\r\n");
    return s.substr(left, right - left + 1);
}

int main(int argc, char** argv) {
    string filename = "super_bowl_ads_updated.json";
    if (argc > 1) {
        filename = argv[1];
    }
    ifstream inFile(filename); // opens json file
    if (!inFile) {
        cerr << "Error: cannot open \"" << filename << "\"\n";
        cerr << "Usage: " << argv[0]
             << " [path/to/super_bowl_ads_updated.json]\n";
        return 1;
    }

    vector<Ad> ads;
    Ad currentAd;
    string line;

    // parse line by line
    while (getline(inFile, line)) {
        string t = trim(line);

        if (t.rfind("\"Year\"", 0) == 0) {
            currentAd = Ad();
            auto p = t.find(':');
            auto e = t.find_first_of(",}", p);
            currentAd.year = stoi(t.substr(p + 1, e - p - 1));
        }
        else if (t.rfind("\"Brand\"", 0) == 0) {
            auto p  = t.find(':');
            auto q1 = t.find('"', p + 1);
            auto q2 = t.find('"', q1 + 1);
            currentAd.brand = t.substr(q1 + 1, q2 - q1 - 1);
        }
        else if (t.rfind("\"Funny\"", 0) == 0) {
            currentAd.content.funny = (t.find("true") != string::npos);
        }
        else if (t.rfind("\"Product\"", 0) == 0) {
            currentAd.content.product = (t.find("true") != string::npos);
        }
        else if (t.rfind("\"Patriotic\"", 0) == 0) {
            currentAd.content.patriotic = (t.find("true") != string::npos);
        }
        else if (t.rfind("\"Celebrity\"", 0) == 0) {
            currentAd.content.celebrity = (t.find("true") != string::npos);
        }
        else if (t.rfind("\"Danger\"", 0) == 0) {
            currentAd.content.danger = (t.find("true") != string::npos);
        }
        else if (t.rfind("\"Animals\"", 0) == 0) {
            currentAd.content.animals = (t.find("true") != string::npos);
        }
        else if (t.rfind("\"Sexual\"", 0) == 0) {
            currentAd.content.sexual = (t.find("true") != string::npos);
        }
        else if (t.rfind("\"Views\"", 0) == 0) {
            auto p = t.find(':');
            auto e = t.find_first_of(",}", p);
            currentAd.views = stoi(t.substr(p + 1, e - p - 1));
        }
        else if (t.rfind("\"Likes\"", 0) == 0) {
            auto p = t.find(':');
            auto e = t.find_first_of(",}", p);
            currentAd.likes = stoi(t.substr(p + 1, e - p - 1));
        }
        else if (t.rfind("\"Dislikes\"", 0) == 0) {
            auto p = t.find(':');
            auto e = t.find_first_of(",}", p);
            currentAd.dislikes = stoi(t.substr(p + 1, e - p - 1));
        }
        else if (t.rfind("\"Title\"", 0) == 0) {
            auto p  = t.find(':');
            auto q1 = t.find('"', p + 1);
            auto q2 = t.find('"', q1 + 1);
            currentAd.title = t.substr(q1 + 1, q2 - q1 - 1);
            ads.push_back(currentAd);
        }
    }
    // loop
    bool loop = true;
    while (loop) {
        int choice=0;
        while (true) {
            // Get user input
            cout << "Choose sorting algorithm:\n"
                 << "  1) Merge Sort\n"
                 << "  2) Quick Sort\n";
            if (!(cin >> choice) || (choice != 1 && choice != 2)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cerr << "Error: invalid choice\n";
            }
            else {
                // Clear newline from input
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }
        vector<bool> want(8, false);
        while(true) {
            cout << "Select content flags to filter (separate numbers by spaces, 0 = no filter):\n"
                 << "  0) No Filter\n"
                 << "  1) Funny\n"
                 << "  2) Product\n"
                 << "  3) Patriotic\n"
                 << "  4) Celebrity\n"
                 << "  5) Danger\n"
                 << "  6) Animals\n"
                 << "  7) Sexual\n";
            vector<bool> contentFlags(8, false);
            bool noFilter = false;
            bool valid = true;
            int flagNum;
            string inputLine;
            getline(cin, inputLine);
            istringstream stream(inputLine);
            vector<string> inputFlags;
            string flag;
            while (stream >> flag) {
                inputFlags.push_back(flag);
            }
            if(inputFlags.empty()) {
                valid=false;
            }
            else {
                for (auto &t : inputFlags) {
                    // must be digits
                    if (!all_of(t.begin(), t.end(), ::isdigit)) {
                        valid = false;
                        break;
                    }
                    int num = stoi(t);
                    if (num == 0) {
                        if (noFilter) {
                            valid = false;
                            break;
                        }
                        noFilter = true;
                    }
                    else if (num >= 1 && num <= 7) {
                        if (noFilter) {
                            valid = false;
                            break;
                        }
                        contentFlags[num] = true;
                    }
                    else {
                        valid = false;
                        break;
                    }
                }
            }
            if(valid) {
                want = contentFlags;
                break;
            }
            cerr << "Error: invalid input\n";
        }

        vector<Ad> workingAds;
        for (auto& ad : ads) {
            bool ok = true;
            if (want[1] && !ad.content.funny) {
                ok = false;
            }
            if (want[2] && !ad.content.product) {
                ok = false;
            }
            if (want[3] && !ad.content.patriotic) {
                ok = false;
            }
            if (want[4] && !ad.content.celebrity) {
                ok = false;
            }
            if (want[5] && !ad.content.danger) {
                ok = false;
            }
            if (want[6] && !ad.content.animals) {
                ok = false;
            }
            if (want[7] && !ad.content.sexual) {
                ok = false;
            }
            if (ok) {
                workingAds.push_back(ad);
            }
        }

        // Time sorting algorithm
        auto startTime = chrono::high_resolution_clock::now();
        if (choice == 1) {
            mergeSort(workingAds, 0, int(workingAds.size())-1);
        }
        else {
            quickSort(workingAds, 0, (int)workingAds.size() - 1);
        }
        reverse(workingAds.begin(), workingAds.end());
        auto endTime = chrono::high_resolution_clock::now();

        auto elapsed = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
        cout << "Sorted " << workingAds.size()
             << " ads (highest to lowest views) in " << elapsed << " microseconds.\n\n";

        cout << left
             << setw(6)  << "Year"
             << setw(15) << "Brand"
             << setw(10) << "Views"
             << "Content\n"
             << string(60, '-') << "\n";

        for (auto& ad : workingAds) {
            cout << setw(6)  << ad.year
                 << setw(15) << ad.brand
                 << setw(10) << ad.views
                 << content(ad.content)
                 << "\n";
        }
        while (true) {
            cout << "Continue?\n"
                 << "  0) No\n"
                 << "  1) Yes\n";
            string cont;
            getline(cin, cont);
            if (!cont.empty() && (cont[0]=='1')) {
                break;
            }
            if (!cont.empty() && (cont[0]=='0')) {
                loop = false;
                break;
            }
            cerr << "Error: Invalid input\n";
        }
    }
}

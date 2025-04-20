#ifndef AD_H
#define AD_H
#include <string>
using namespace std;

struct ContentFlags {
    bool funny = false;
    bool product = false;
    bool patriotic = false;
    bool celebrity = false;
    bool danger = false;
    bool animals = false;
    bool sexual = false;
};

struct Ad {
    int year = 0;
    string brand;
    ContentFlags content;
    int views = 0;
    int likes = 0;
    int dislikes = 0;
    string title;
};

#endif // AD_H

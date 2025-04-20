#include "QuickSort.h"
#include <algorithm>

static int partition(vector<Ad>& a, int low, int high) {
    int pivot = a[high].views;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (a[j].views <= pivot) {
            swap(a[++i], a[j]);
        }
    }
    swap(a[i + 1], a[high]);
    return i + 1;
}

void quickSort(vector<Ad>& a, int low, int high) {
    if (low < high) {
        int pi = partition(a, low, high);
        quickSort(a, low, pi - 1);
        quickSort(a, pi + 1, high);
    }
}

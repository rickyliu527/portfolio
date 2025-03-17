#include <iostream>
#include <vector>
using namespace std;

int binarysearch(vector<int>&, int);

int add(int a, int b){
    return a + b;
}
int upperbound(vector<int>& arr, int target) {
    int left = 0, right = arr.size();
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}
int lowerbound(vector<int>& arr, int target) {
    int left = 0,right=arr.size();while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}
int main() {
    vector<int> arr;
    cout << binarysearch(arr, 5) << endl;  // 輸出 2
    cout << binarysearch(arr, 6) << endl;  // 輸出 -1
}

int binarysearch(vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

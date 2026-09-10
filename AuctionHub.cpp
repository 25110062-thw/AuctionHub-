#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <random>
using namespace std;
using namespace std::chrono;

struct Bid {
    string bidderId;
    double amount;       // bid price
    long long timestamp; // Unix ms
    int auctionId;
};

// ---------- Utilities ----------
void printBids(const vector<Bid>& v, size_t limit = 10) {
    for (size_t i = 0; i < min(v.size(), limit); ++i)
        cout << "[" << v[i].bidderId << "] $" << v[i].amount
             << " @ " << v[i].timestamp << "\n";
    if (v.size() > limit) cout << "... (" << v.size() << " total)\n";
}

bool loadBids(const string& path, vector<Bid>& out) {
    ifstream f(path);
    if (!f) return false;
    Bid b;
    while (f >> b.bidderId >> b.amount >> b.timestamp >> b.auctionId)
        out.push_back(b);
    return true;
}

void insertionSort(vector<Bid>&v){
    for(int i=1; i < v.size(); i++){
        Bid key = v[i];
        int j = i-1;
        while(j>=0 && (v[j].amount>key.amount || 
        (v[j].amount == key.amount && v[j].timestamp > key.timestamp))){
            v[j+1] = v[j];
            j--;
        }
        v[j+1] = key;
    }
}//Task A: Insertion Sort
// Sắp xếp tăng dần theo amount.
// Nếu amount bằng nhau thì sắp xếp theo timestamp.
// Phù hợp với dữ liệu gần như đã được sắp xếp.

void selectionSort(vector<Bid>& v){
    for (int i = 0; i+1 < v.size(); i++) {
        int maxIndex = i;
        for (int j = i+1; j < v.size(); j++) {
            if(v[j].amount > v[maxIndex].amount) maxIndex = j;
        }
        if(maxIndex != i) swap(v[i], v[maxIndex]);
    }
}//Task B: Selection Sort
// Sắp xếp giảm dần theo amount.
// Mỗi lần tìm phần tử có giá lớn nhất và đưa lên đầu.
// Phù hợp để tìm Top-K khi K nhỏ.

void interchangeSort(vector<Bid>& v){
    int cnt = 0;
    for (int i = 0; i+1 < v.size(); i++){
        for(int j = i+1; j < v.size(); j++){
            if(v[j].amount < v[i].amount){
                swap(v[i], v[j]);
                cnt++;
            }
        }
    }
    cout << "Total swaps: " << cnt << endl;
}//Task C: Interchange Sort
// Sắp xếp tăng dần theo amount.
// So sánh một phần tử với các phần tử phía sau và swap nếu cần.
// Phù hợp với tập dữ liệu nhỏ, khoảng n <= 50.
bool bubbleSortEarlyStop(vector<Bid>& v){
    for (int i = 0; i < v.size(); i++){
        bool swapped = false;
        for(int j = 0; j < v.size()-i-1; j++){
            if(v[j].amount > v[j+1].amount){
                swap(v[j], v[j+1]);
                swapped = true;
            }
        }
        if(!swapped && i == 0) return true;
    }
    return false;
}//Task D: Bubble Sort
// Sắp xếp tăng dần theo amount.
// So sánh các phần tử liền kề và đổi chỗ nếu sai thứ tự.
// Dừng sớm nếu lượt duyệt đầu tiên không có swap.
void quickSort(vector<Bid>& v, int lo, int hi){
    if (lo >= hi) return;
    vector<Bid>a = {v[lo], v[(lo+hi)/2], v[hi]};
    for(int i=1; i < 3; i++){
        Bid key = a[i];
        int j = i-1;
        while(j>=0 && (a[j].amount>key.amount || 
        (a[j].amount == key.amount && a[j].timestamp > key.timestamp))){
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = key;
    }
    int i=lo, j=hi;
    while(i <= j){
        while(i < hi && 
        (v[i].amount < a[1].amount || (v[i].amount == a[1].amount && v[i].timestamp < a[1].timestamp))
        ){
            i++;
        }
        while(j > lo && 
        (v[j].amount > a[1].amount || (v[j].amount == a[1].amount && v[j].timestamp > a[1].timestamp))
        ){
            j--;
        }
        
        if(i<=j){
            swap(v[i], v[j]);
            i++;
            j--;
        }
    }   
    quickSort(v, lo, j);
    quickSort(v, i, hi);
}// Task E: Quick Sort 
// Sắp xếp tăng dần theo amount, sau đó theo timestamp.
// Chọn pivot bằng Median-of-Three: phần tử đầu, giữa và cuối.
// Dùng hai con trỏ i và j để partition.
// Sau đó đệ quy sắp xếp hai phần bên trái và bên phải.
// ---------- Task E: Quick Sort Wrapper ----------
// Hàm wrapper giúp gọi Quick Sort dễ dàng hơn.
// Không cần người dùng tự truyền lo và hi.
void quickSortWrapper(vector<Bid>& v) {
    if (!v.empty()) {
        quickSort(v, 0, v.size() - 1);
    }
}
// ---------- Task E: Verify Sorted ----------
// Kiểm tra dữ liệu có được sắp xếp tăng dần hay không.
// Nếu amount bằng nhau thì timestamp phải tăng dần.
bool isSorted(const vector<Bid>& v) {
    for (size_t i = 1; i < v.size(); i++) {

        if (v[i - 1].amount > v[i].amount)
            return false;

        if (v[i - 1].amount == v[i].amount &&
            v[i - 1].timestamp > v[i].timestamp)
            return false;
    }

    return true;
}
// ---------- Task E: Generate Random Bids ----------
// Tạo dữ liệu random để benchmark Quick Sort.
vector<Bid> generateRandomBids(int n) {

    vector<Bid> v;
    v.reserve(n);

    mt19937 rng(12345);

    uniform_real_distribution<double> amountDist(1.0, 1000000.0);
    uniform_int_distribution<long long> timeDist(1000000, 9999999);

    for (int i = 0; i < n; i++) {

        Bid b;

        b.bidderId = "bidder" + to_string(i);
        b.amount = amountDist(rng);
        b.timestamp = timeDist(rng);
        b.auctionId = i % 1000;

        v.push_back(b);
    }

    return v;
}
// ---------- Task E: Benchmark Quick Sort ----------
// Đo thời gian chạy Quick Sort trên 100,000 bids.
void benchmarkQuickSort() {

    const int SIZE = 100000;

    vector<Bid> data = generateRandomBids(SIZE);

    auto start = high_resolution_clock::now();

    quickSortWrapper(data);

    auto end = high_resolution_clock::now();

    auto duration =
        duration_cast<milliseconds>(end - start);

    cout << "Quick Sort runtime: "
         << duration.count()
         << " ms" << endl;

    cout << "Sorted correctly: "
         << (isSorted(data) ? "Yes" : "No")
         << endl;
}

int main() {
    int n;
    cin >> n;
    vector<Bid> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i].bidderId
            >> v[i].amount
            >> v[i].timestamp
            >> v[i].auctionId;
    }
    vector<Bid> v1 = v;
    vector<Bid> v2 = v;
    vector<Bid> v3 = v;
    vector<Bid> v4 = v;
    vector<Bid> v5 = v;
    insertionSort(v1);
    for (int i = 0; i < n; i++) {
        cout << v1[i].bidderId << " "
             << v1[i].amount << " "
             << v1[i].timestamp << " "
             << v1[i].auctionId << endl;
    }
    int k;
    cin >> k;

    selectionSort(v2);

    if (k < 0) k = 0;
    if (k > n) k = n;

    cout << "Top " << k << " = ";

    for (int i = 0; i < k; i++) {
        cout << v2[i].bidderId;
        if (i < k - 1)
            cout << ", ";
        else
            cout << "." << endl;    
    }

    interchangeSort(v3);

    if (bubbleSortEarlyStop(v4)) {
        cout << "STABLE" << endl;
    } 
    else {
        cout << "UNSTABLE" << endl;
    }

    quickSortWrapper(v5);
    if (n <= 50) {
        for (int i = 0; i < n; i++) {
            cout << v5[i].bidderId << " "
                << v5[i].amount << " "
                << v5[i].timestamp << " "
                << v5[i].auctionId << endl;
        }
    }
    benchmarkQuickSort();
    return 0;
}
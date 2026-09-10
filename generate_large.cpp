#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

using namespace std;

struct Bid {
    string bidderId;
    double amount;
    long long timestamp;
    int auctionId;
};

int main() {
    const int SIZE = 100000;

    // 1. Tạo random generator
    mt19937 rng(12345);

    // 2. Tạo distribution cho các trường dữ liệu
    uniform_real_distribution<double> distAmount(10.0, 5000.0);
    uniform_int_distribution<long long> distTime(1700000000LL, 1735689600LL);
    uniform_int_distribution<int> distAuction(1, 500);
    uniform_int_distribution<int> distBidder(1, 10000);

    // 3. Mở file test_data/large.txt
    ofstream f("test_data/large.txt");

    if (!f) {
        cout << "Cannot open file! Make sure 'test_data' folder exists." << endl;
        return 1;
    }

    // 4. Sinh 100000 Bid
    for (int i = 0; i < SIZE; i++) {
        Bid b;

        // bidderId: Dạng "USER_123"
        b.bidderId = "USER_" + to_string(distBidder(rng));
        b.amount = distAmount(rng);
        b.timestamp = distTime(rng);
        b.auctionId = distAuction(rng);

        // 5. Ghi b vào file (cách nhau bởi khoảng trắng hoặc tab)
        f << b.bidderId << " " 
          << b.amount << " " 
          << b.timestamp << " " 
          << b.auctionId << "\n";
    }

    f.close();
    cout << "Generated " << SIZE << " bids successfully." << endl;
    return 0;
}
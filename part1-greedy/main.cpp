#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Delivery { string id; int start, finish; };

vector<Delivery> readDeliveries(const string& path) {
    vector<Delivery> v; ifstream in(path);
    if (!in) { cerr << "Cannot open " << path << "\n"; return v; }
    string line;
    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        istringstream ss(line); Delivery d;
        if (ss >> d.id >> d.start >> d.finish) v.push_back(d);
    }
    return v;
}

vector<Delivery> maxOneDriver(vector<Delivery> v) {
    sort(v.begin(), v.end(), [](const Delivery& a, const Delivery& b) {
        return a.finish != b.finish ? a.finish < b.finish : a.start < b.start; });
    vector<Delivery> sel; int last = -2147483647;
    for (auto& d : v) if (d.start >= last) { sel.push_back(d); last = d.finish; }
    return sel;
}

vector<vector<Delivery>> assignAllDrivers(vector<Delivery> v) {
    sort(v.begin(), v.end(), [](const Delivery& a, const Delivery& b) {
        return a.start != b.start ? a.start < b.start : a.finish < b.finish; });
    vector<vector<Delivery>> drivers;
    typedef pair<int,int> P;
    priority_queue<P, vector<P>, greater<P>> pq;
    for (auto& d : v) {
        int idx;
        if (!pq.empty() && pq.top().first <= d.start) { idx = pq.top().second; pq.pop(); }
        else { idx = drivers.size(); drivers.emplace_back(); }
        drivers[idx].push_back(d);
        pq.push({d.finish, idx});
    }
    return drivers;
}

int countRejected(const vector<Delivery>& v) { return (int)v.size() - (int)maxOneDriver(v).size(); }

void printA(const vector<Delivery>& sel) {
    for (auto& d : sel) cout << "  " << d.id << " [" << d.start << ", " << d.finish << "]\n";
    cout << "Total deliveries: " << sel.size() << "\n";
}
void printB(const vector<vector<Delivery>>& dr) {
    for (size_t i = 0; i < dr.size(); i++) {
        cout << "Driver " << i + 1 << ":";
        for (auto& d : dr[i]) cout << " " << d.id;
        cout << "\n";
    }
    cout << "Minimum drivers required: " << dr.size() << "\n";
}

typedef vector<pair<int,int>> Iv;
vector<Delivery> make(const Iv& iv) {
    vector<Delivery> v;
    for (size_t i = 0; i < iv.size(); i++) v.push_back({"T" + to_string(i + 1), iv[i].first, iv[i].second});
    return v;
}

int runTests() {
    vector<Iv> A = {{}, {{1,3}}, {{1,3},{3,5},{5,7}}, {{1,5},{2,6},{3,7}}, {{5,7},{1,3},{3,5},{2,4}}, {{1,4},{2,4},{4,6}}};
    int eA[] = {0,1,3,1,3,2};
    vector<Iv> B = {{}, {{1,3}}, {{1,3},{3,5},{5,7}}, {{1,5},{2,6},{3,7}}, {{1,4},{2,5},{4,7},{5,8}}, {{5,8},{1,4},{2,5},{4,6},{6,9}}};
    int eB[] = {0,1,1,3,2,2};
    vector<Iv> C = {{}, {{1,3}}, {{1,3},{3,5},{5,7}}, {{1,5},{2,6},{3,7}}, {{5,7},{1,3},{3,5},{2,4}}, {{1,2},{2,3},{3,4},{1,3}}};
    int eC[] = {0,0,0,2,1,1};
    int fails = 0;
    for (int i = 0; i < 6; i++) {
        int g = maxOneDriver(make(A[i])).size(); bool ok = g == eA[i]; fails += !ok;
        cout << "A" << i + 1 << ": got " << g << " expected " << eA[i] << (ok ? " PASS\n" : " FAIL\n");
    }
    for (int i = 0; i < 6; i++) {
        auto dr = assignAllDrivers(make(B[i])); bool ok = (int)dr.size() == eB[i];
        for (auto& s : dr) for (size_t j = 1; j < s.size(); j++) if (s[j].start < s[j-1].finish) ok = false;
        fails += !ok;
        cout << "B" << i + 1 << ": got " << dr.size() << " expected " << eB[i] << (ok ? " PASS\n" : " FAIL\n");
    }
    for (int i = 0; i < 6; i++) {
        int g = countRejected(make(C[i])); bool ok = g == eC[i]; fails += !ok;
        cout << "C" << i + 1 << ": got " << g << " expected " << eC[i] << (ok ? " PASS\n" : " FAIL\n");
    }
    return fails;
}

int main(int argc, char** argv) {
    if (argc >= 2 && string(argv[1]) == "--test") return runTests() ? 1 : 0;
    string path = argc >= 2 ? argv[1] : "deliveries.txt";
    auto v = readDeliveries(path);
    cout << "== Task A: max deliveries, one driver ==\n"; printA(maxOneDriver(v));
    cout << "\n== Task B: minimum drivers ==\n"; printB(assignAllDrivers(v));
    cout << "\n== Task C: minimum rejected ==\n" << countRejected(v) << "\n";
}

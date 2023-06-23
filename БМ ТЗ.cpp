#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct BM {
    int id;
    double x;
    double y;
    int rpds;
    char freq;
};

double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void distribute_freq(vector<BM>& bms) {
    int n = bms.size();
    int ones = count_if(bms.begin(), bms.end(), [](const BM& b) { return b.rpds == 1; });
    int twos = n - ones;

    if (ones <= 3) {
        // Распределение частот МРЛС равномерно
        for (int i = 0; i < n; i++) {
            bms[i].freq = 'A' + i % 3;
        }
    }
    else {
        // Распределение частот МРЛС согласно алгоритму
        vector<int> ones_idx, twos_idx;

        for (int i = 0; i < n; i++) {
            if (bms[i].rpds == 1) {
                ones_idx.push_back(i);
            }
            else {
                twos_idx.push_back(i);
            }
        }

        // Равномерное распределение частот для группы БМ с двумя РПДС
        int d = distance(bms[twos_idx[0]].x, bms[twos_idx[0]].y, bms[twos_idx[1]].x, bms[twos_idx[1]].y);
        if (twos >= 4) {
            bms[twos_idx[0]].freq = 'A';
            bms[twos_idx[1]].freq = 'B';
            bms[twos_idx[2]].freq = 'C';
            bms[twos_idx[3]].freq = bms[twos_idx[0]].freq;

            double max_dist = 0;
            int fourth_bm_idx = -1;
            for (auto idx : twos_idx) {
                double dist = distance(bms[twos_idx[2]].x, bms[twos_idx[2]].y, bms[idx].x, bms[idx].y);
                if (dist > max_dist) {
                    max_dist = dist;
                    fourth_bm_idx = idx;
                }
            }
            bms[fourth_bm_idx].freq = bms[twos_idx[0]].freq;

            double max_dist_2 = 0;
            int fifth_bm_idx = -1;
            for (auto idx : twos_idx) {
                if (idx != fourth_bm_idx) {
                    double dist = distance(bms[twos_idx[3]].x, bms[twos_idx[3]].y, bms[idx].x, bms[idx].y);
                    if (dist > max_dist_2) {
                        max_dist_2 = dist;
                        fifth_bm_idx = idx;
                    }
                }
            }
            bms[fifth_bm_idx].freq = (distance(bms[twos_idx[2]].x, bms[twos_idx[2]].y, bms[fifth_bm_idx].x, bms[fifth_bm_idx].y) >
                distance(bms[twos_idx[3]].x, bms[twos_idx[3]].y, bms[fifth_bm_idx].x, bms[fifth_bm_idx].y)) ?
                bms[twos_idx[3]].freq : bms[twos_idx[2]].freq;

            int sixth_bm_idx = -1;
            double max_dist_3 = 0;
            for (auto idx : ones_idx) {
                double dist = distance(bms[fifth_bm_idx].x, bms[fifth_bm_idx].y, bms[idx].x, bms[idx].y);
                if (dist > max_dist_3) {
                    max_dist_3 = dist;
                    sixth_bm_idx = idx;
                }
            }
            if (max_dist_3 > max_dist_2) {
                bms[sixth_bm_idx].freq = bms[sixth_bm_idx].freq;
            }
            else {
                bms[sixth_bm_idx].freq = bms[fifth_bm_idx].freq;
            }
        }
        else {
            bms[twos_idx[0]].freq = 'A';
            bms[twos_idx[1]].freq = 'B';
            bms[twos_idx[2]].freq = 'C';
            bms[twos_idx[3]].freq = bms[twos_idx[0]].freq;
            bms[twos_idx[4]].freq = bms[twos_idx[1]].freq;
            bms[twos_idx[5]].freq = bms[twos_idx[2]].freq;
        }
        // Равномерное распределение частот для группы БМ с одним РПДС
        int k = 0;
        for (auto i : ones_idx) {
            bms[i].freq = 'A' + k % 3;
            if (k >= 3) {
                int closest_twos = twos_idx[0];
                double min_dist = distance(bms[i].x, bms[i].y, bms[twos_idx[0]].x, bms[twos_idx[0]].y);
                for (auto j : twos_idx) {
                    double d1 = distance(bms[i].x, bms[i].y, bms[closest_twos].x, bms[closest_twos].y);
                    double d2 = distance(bms[i].x, bms[i].y, bms[j].x, bms[j].y);
                    if (d2 < min_dist && d2 < d1 && bms[j].freq != bms[i].freq) {
                        closest_twos = j;
                        min_dist = d2;
                    }
                }
                bms[i].freq = (bms[i].freq == bms[closest_twos].freq) ?
                    bms[twos_idx[4]].freq : bms[closest_twos].freq;
            }
            k++;
        }
    }
    // Ввывод в консоль 
    for (auto b : bms) {
        cout << "BM" << b.id << " " << b.freq << endl;
    }
}

int main() {
    int n;
    cin >> n;
    vector<BM> bms(n);
    for (int i = 0; i < n; i++) {
        cin >> bms[i].id >> bms[i].x >> bms[i].y >> bms[i].rpds;
    }
    distribute_freq(bms);
    return 0;
}
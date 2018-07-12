#include <random>
#include <iostream>
#include <cmath>
#include <algorithm>


using namespace std;

using ll = long long;

class ZipfGen
{
public:
    random_device rd;
    double alpha;
    ll item_max;
    double *hist;
    double *dist;
    double *hist_value;
    ll *value;

    ZipfGen(double alpha = 0.99, ll item_max = 1000): alpha(alpha), item_max(item_max) {
        hist = new double[item_max + 1];
        hist_value = new double[item_max + 1];
        dist = new double[item_max];
        value = new ll[item_max];
        cout << "Alpha=" << alpha << endl;
        gen_distribution();
        gen_value();
    }
    ~ZipfGen() = default;
    
    void gen_distribution() {
        hist[0] = 0.0;
        hist_value[0] = 0.0;
        double dist_sum = 0.0;
        for (int i = 1; i <= item_max; i ++ ) {
            dist_sum += (1.0 / pow((double)i, alpha));
            // cout << dist_sum << endl;
        }

        for (int i = 1; i <= item_max; i ++ ) {
            double pro = (1.0 / pow((double)i, alpha)) / dist_sum;
            hist[i] = hist[i - 1] + pro;
            hist_value[i] = hist[i] * item_max;
            dist[i - 1] = pro;
            // cout << hist[i] << endl;
        }

    }

    void gen_value() {
        mt19937 g(rd());
        for (int i = 1; i <= item_max; i ++ ) {
            value[i - 1] = i;
        }
        shuffle(value, value + item_max, g);
    }

    ll next_long() {
        double rd_num = rd() % item_max;
        int index = (upper_bound(hist_value, hist_value + item_max, rd_num) - hist_value);
        return value[index - 1];
    }

    vector<ll> get_dataset(int length) {
        // TODO
        return vector<ll>();
    }

};

int main() {
    ZipfGen *zipf = new ZipfGen();
    for (int i = 0; i < 100000; i ++ ) {
        cout << zipf->next_long() << endl;
    }
    return 0;
}
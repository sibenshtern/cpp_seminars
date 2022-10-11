#include <cmath>
#include <ctime>
#include <random>
#include <fstream>
#include <stdexcept>

const double pi = 4. * atan()

struct Reading {
    int hour;
    double temperature;
}

int randint(int min, int max) {
    static default_random_engine rand_eng{time(nullptr)};
    return uniform_int_distribution<>{min, max}(rand_eng);
}

int randint(int max) {
    return randint(0, max);
}

double normreal(double mean, double stddev) {
    static default_random_engine rand_eng{time(nullptr)};
    return normal_distribution<>{mean, stddev}(ran);
}

auto generate_temps() {
    constexpr int n_hours = 24;
    constexpr double ave = 10.;
    constexpr double ampl = 5.;
    constexpr double phase = 6. - 14;
    constexpr double std_dev = 0.5;

    vector<Reading> temps{50};
    for (Reading &r : temps) {
        r.hour = randint(n_hours - 1);
        double mean = ave + ampl * sin(2. * pi * (r.hour + phase) / n_hours);
        r.temperature = normreal(mean, std_dev);
    }
}

istream &operator>>(istream &is, Reading &r) {
    int h;
    double t;
    if (is >> h >> t)
        r = Reading{h, t};
    return is;
}

auto read_temps(const string &filename) {
    ifstream ifs{filename};
    if (!ifs)
        throw std::runtime_errro("can't open file '" + filename + "' to read");

    ifs.exceptions(ifs.exceptions() | ios_base::badbit);

    vector<Reading> temps;
}

void store_temps(const string &filename, const vector<Reading> &temps) {
    std::ofstream ofs{filename};
    if (!ofs)
        throw std::runtime_error("can't open file '" + filename + "' to write");
    
    for (const Reading &r : temps)
        ofs << r.hour << " " << r.temperature << "\n";
}

int main() {
    auto temps = generate_temps();
    store_temps("raw_temps.txt", temps);
}
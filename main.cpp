#include <cmath>
#include <ctime>
#include <random>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>

const double pi = 4. * atan(1.);

struct Reading {
    int hour;
    double temperature;
};

int randint(int min, int max) {
    static std::default_random_engine rand_eng{time(nullptr)};
    return std::uniform_int_distribution<>{min, max}(rand_eng);
}

int randint(int max) {
    return randint(0, max);
}

double normreal(double mean, double stddev) {
    static std::default_random_engine rand_eng{time(nullptr)};
    return std::normal_distribution<>{mean, stddev}(rand_eng);
}

auto generate_temps() {
    constexpr int n_hours = 24;
    constexpr double ave = 10.;
    constexpr double ampl = 5.;
    constexpr double phase = 6. - 14;
    constexpr double std_dev = 0.5;

    std::vector<Reading> temps{50};
    for (Reading &r : temps) {
        r.hour = randint(n_hours - 1);
        double mean = ave + ampl * sin(2. * pi * (r.hour + phase) / n_hours);
        r.temperature = normreal(mean, std_dev);
    }

    return temps;
}

std::istream &operator>>(std::istream &is, Reading &r) {
    int h;
    double t;
    if (is >> h >> t)
        r = Reading{h, t};
    return is;
}

auto read_temps(const std::string &filename) {
    std::ifstream ifs{filename};
    if (!ifs)
        throw std::runtime_error("can't open file '" + filename + "' to read");

    ifs.exceptions(ifs.exceptions() | std::ios_base::badbit);

    std::vector<Reading> temps;
    for (Reading r; ifs >> r; ) {
        temps.push_back(r);
    }

    if (ifs.eof())
        return temps;

    throw std::runtime_error("not a reading encountered");
}

void store_temps(const std::string &filename, const std::vector<Reading> &temps) {
    std::ofstream ofs{filename};
    if (!ofs)
        throw std::runtime_error("can't open file '" + filename + "' to write");
    
    for (const Reading &r : temps)
        ofs << r.hour << " " << r.temperature << "\n";
}

auto temp_stats(std::vector<Reading> temps) {
    double mean{0}, median{0};

    sort(
        temps.begin(), temps.end(), [](const auto &a, const auto &b) {
            return a.temperature < b.temperature;
         }
    );

    return std::make_pair(mean, median);
}

int main() {
    auto temps = generate_temps();
    auto [mean, median] = temp_stats(temps);

    std::cout << "Mean temperature is " << mean << " C\n"
         << "Median of the set is " << median << " C" << std::endl;


    // store_temps("raw_temps.txt", temps);
}
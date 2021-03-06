#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/density.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include "histogram.hpp"

using namespace boost;
using namespace boost::accumulators;

typedef accumulator_set<double, features<tag::density> > estimate_histo;
typedef iterator_range<std::vector<std::pair<double, double> >::iterator > histogram_type;

typedef accumulator_set<double, features<tag::histogram> > fixed_histo;

const int bins = 20;
const double binwidth = 1.0;

// --------------------------------------------------------------------------
std::string size_human(uint64_t size)
{
    char unit;
    const char *units [] = {" Bytes", " kB", " MB", " GB", " TB"};

    for (unit=-1; (++unit<4) && (size>1023); size/=1024);
    return std::to_string(size) + units[unit];
}

// --------------------------------------------------------------------------
template <typename Histo>
void plot_histo(Histo &histo, double normalize, bool KBMB=false) {
    double increment = histo[1].first - histo[0].first;
    for (int i=0; i<histo.size(); ++i) {
        double r0 = histo[i].first;
        double r1 = histo[i].first + increment;
        if (KBMB) {
            std::cout << std::setw(4)  << i << ": "
                      << std::setw(12) << size_human(uint64_t(1) << static_cast<int>(r0)) << " - "
                      << std::setw(12) << size_human(uint64_t(1) << static_cast<int>(r1)) << " : "
                      << std::setw(12) << std::setprecision(8) << histo[i].second << " : "
                      << std::string(histo[i].second * normalize, '*') << std::endl;
        }
        else {
            std::cout << std::setw(4)  << i << ": "
                      << std::setw(12) << std::setprecision(8) << r0 << " - "
                      << std::setw(12) << std::setprecision(8) << r1 << " : "
                      << std::setw(12) << std::setprecision(8) << histo[i].second << " : "
                      << std::string(histo[i].second * normalize, '*') << std::endl;
        }
    }
    std::cout << std::endl;
}

// --------------------------------------------------------------------------
int main(int argc, char** argv)
{
    // create some random data
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(bins/2.0, 2.0);
    std::vector<double> data(1000000);
    std::generate(data.begin(), data.end(), std::bind(distribution, generator));

    // create two accumulator based histograms
    estimate_histo histo_1(tag::density::num_bins = bins,
                           tag::density::cache_size = 100);

    fixed_histo histo_2(tag::histogram::limit_low  = 0,
                        tag::histogram::limit_high = binwidth*(bins),
                        tag::histogram::num_bins   = bins);

    // fill accumulator
    for (int j = 0; j < data.size(); ++j) {
        histo_1(data[j]);
        histo_2(data[j]);
    }

    histogram_type hist1 = density(histo_1);
    histogram_type hist2 = histogram(histo_2);

    std::cout << "Density histogram based on sampling \n";
    plot_histo(hist1, 80);

    std::cout << "Fixed bin size histogram \n";
    plot_histo(hist2, 80.0/data.size());


    // create some random data for log histogram
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(1, std::numeric_limits<uint32_t>::max());
    std::vector<double> datal2(1000000);

    std::generate(datal2.begin(), datal2.end(), std::bind(dis, gen));

    // create accumulator based histogram
    fixed_histo histo_3(tag::histogram::limit_low  = 1,
                        tag::histogram::limit_high = 31,
                        tag::histogram::num_bins   = 30);

    double epsilon = 1E-6;
    // fill accumulator
    for(auto &val : datal2) {
        histo_3(std::log2(val + epsilon));
    }

    histogram_type hist3 = histogram(histo_3);

    std::cout << "Fixed bin log2 histogram \n";
    plot_histo(hist3, 80.0/data.size(), true);

    return 0;
}

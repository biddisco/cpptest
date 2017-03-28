#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>

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
template <typename Histo>
void plot_histo(Histo &histo, double normalize) {
    double increment = histo[1].first - histo[0].first;
    for (int i=0; i<histo.size(); ++i) {
        std::cout << std::setw(4)  << i << ": "
                  << std::setw(8)  << std::setprecision(4) << histo[i].first << " - "
                  << std::setw(8)  << std::setprecision(4) << histo[i].first+increment << " : "
                  << std::setw(12) << std::setprecision(8) << histo[i].second << " : "
                  << std::string(histo[i].second * normalize, '*') << std::endl;
    }
    std::cout << std::endl;
}

// --------------------------------------------------------------------------
int main(int argc, char** argv)
{
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(bins/2.0, 2.0);
    std::vector<double> data(1000000);

    // create some random data
    std::generate(data.begin(), data.end(), std::bind(distribution, generator));
    int c = data.size(); // cache size for histogramm.

    // create an accumulator
    estimate_histo histo_1(tag::density::num_bins = bins,
                           tag::density::cache_size = 100);

    fixed_histo histo_2(tag::histogram::limit_low  = 0,
                        tag::histogram::limit_high = binwidth*(bins),
                        tag::histogram::num_bins   = bins);

    // fill accumulator
    for (int j = 0; j < c; ++j) {
        histo_1(data[j]);
        histo_2(data[j]);
    }

    histogram_type hist1 = density(histo_1);
    histogram_type hist2 = histogram(histo_2);

    std::cout << "Density histogram based on sampling \n";
    plot_histo(hist1, 80);

    std::cout << "Fixed bin size histogram \n";
    plot_histo(hist2, 80.0/data.size());

    return 0;
}

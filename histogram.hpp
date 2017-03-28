///////////////////////////////////////////////////////////////////////////////
// histogram.h
//
//  Copyright 2006 Daniel Egloff, Olivier Gygi. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//	Georg Gast (12.02.2017)
//	based on density.hpp

#ifndef BOOST_ACCUMULATORS_STATISTICS_HISTOGRAM
#define BOOST_ACCUMULATORS_STATISTICS_HISTOGRAM

#include <boost/accumulators/accumulators_fwd.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/numeric/functional.hpp>
#include <boost/accumulators/statistics_fwd.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/range.hpp>
#include <functional>
#include <limits>
#include <vector>

namespace boost
{
namespace accumulators
{

///////////////////////////////////////////////////////////////////////////////
// cache_size and num_bins named parameters
//
BOOST_PARAMETER_NESTED_KEYWORD(tag, histogram_limit_low, limit_low)
BOOST_PARAMETER_NESTED_KEYWORD(tag, histogram_limit_high, limit_high)
BOOST_PARAMETER_NESTED_KEYWORD(tag, histogram_num_bins, num_bins)

BOOST_ACCUMULATORS_IGNORE_GLOBAL(histogram_limit_low)
BOOST_ACCUMULATORS_IGNORE_GLOBAL(histogram_limit_high)
BOOST_ACCUMULATORS_IGNORE_GLOBAL(histogram_num_bins)

namespace impl
{
///////////////////////////////////////////////////////////////////////////////
// histogram_impl
//  histogram
/**
    @brief Histogram

    Just a simple histogram with out averaging or anything else

    @param  histogram_limit_low lower limit of the histogram
    @param  histogram_limit_high high limit of the histogram
    @param  histogram_num_bins Number of bins (two additional bins collect
   under- and overflow samples).
*/
template <typename Sample>
struct histogram_impl : accumulator_base
{
    typedef typename numeric::functional::fdiv<Sample, std::size_t>::result_type
        float_type;
    typedef std::vector<std::pair<float_type, float_type>> histogram_type;
    typedef std::vector<float_type> array_type;
    // for boost::result_of
    typedef iterator_range<typename histogram_type::iterator> result_type;

    template <typename Args>
    histogram_impl(Args const &args)
        : num_bins(args[histogram_num_bins]),
          limit_low(args[histogram_limit_low]),
          limit_high(args[histogram_limit_high]),
          samples_in_bin(num_bins + 2, 0.), bin_positions(num_bins + 2),
          histogram(
              num_bins + 2,
              std::make_pair(
                  numeric::fdiv(args[sample | Sample()], (std::size_t)1),
                  numeric::fdiv(args[sample | Sample()], (std::size_t)1))),
          is_dirty(true)
    {
        float_type bin_size =
            numeric::fdiv(this->limit_high - this->limit_low, this->num_bins);

        // determine bin positions (their lower bounds)
        for (std::size_t i = 0; i < this->num_bins + 2; ++i)
        {
            this->bin_positions[i] = this->limit_low + (i - 1.) * bin_size;
        }
    }

    template <typename Args>
    void operator()(Args const &args)
    {
        this->is_dirty = true;

        // Add each subsequent sample to the correct bin
        if (args[sample] < this->bin_positions[1])
        {
            ++(this->samples_in_bin[0]);
        }
        else if (args[sample] >= this->bin_positions[this->num_bins + 1])
        {
            ++(this->samples_in_bin[this->num_bins + 1]);
        }
        else
        {
            typename array_type::iterator it =
                std::upper_bound(this->bin_positions.begin(),
                                 this->bin_positions.end(), args[sample]);

            std::size_t d = std::distance(this->bin_positions.begin(), it);
            ++(this->samples_in_bin[d - 1]);
        }
    }

    template <typename Args>
    result_type result(Args const &args) const
    {
        if (this->is_dirty)
        {
            this->is_dirty = false;

            // creates a vector of std::pair where each pair i holds
            // the values bin_positions[i] (x-axis of histogram) and
            // samples_in_bin[i] (y-axis of histogram).

            for (std::size_t i = 0; i < this->num_bins + 2; ++i)
            {
                this->histogram[i] = std::make_pair(this->bin_positions[i],
                                                    this->samples_in_bin[i]);
            }
        }
        // returns a range of pairs
        return make_iterator_range(this->histogram);
    }

private:
    std::size_t num_bins;			  // number of bins
    float_type limit_low;			  // the lower limit
    float_type limit_high;			  // the high limit
    array_type samples_in_bin;		  // number of samples in each bin
    array_type bin_positions;		  // lower bounds of bins
    mutable histogram_type histogram; // histogram
    mutable bool is_dirty;
};

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::histogram
//
namespace tag
{
struct histogram : depends_on<>,
                   histogram_limit_low,
                   histogram_limit_high,
                   histogram_num_bins
{
    /// INTERNAL ONLY
    ///
    typedef accumulators::impl::histogram_impl<mpl::_1> impl;

#ifdef BOOST_ACCUMULATORS_DOXYGEN_INVOKED
    /// tag::histogram::limit_low named parameter
    /// tag::histogram::limit_high named parameter
    /// tag::histogram::num_bins named parameter
    static boost::parameter::keyword<histogram_limit_low> const limit_low;
    static boost::parameter::keyword<histogram_limit_high> const limit_high;
    static boost::parameter::keyword<histogram_num_bins> const num_bins;
#endif
};
}

///////////////////////////////////////////////////////////////////////////////
// extract::histogram
//
namespace extract
{
extractor<tag::histogram> const histogram = {};

BOOST_ACCUMULATORS_IGNORE_GLOBAL(histogram)
}

using extract::histogram;
}
} // namespace boost::accumulators

#endif // BOOST_ACCUMULATORS_STATISTICS_HISTOGRAM

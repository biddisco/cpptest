#include "boost/program_options.hpp" 
 
#include <iostream>    // cout etc
#include <iterator>    // ostream iterator
#include <functional>  // less/greater
#include <algorithm>   // sort
#include <string> 
#include <vector> 

//
// usage, execute with args : -i 2 1 4 5 23 115 34 12 4 23 5 12 808 -j 1 3 2 5 3 4 6 
//
// output :
// 
// Supplied indices   : 2, 1, 4, 5, 23, 115, 34, 12, 4, 23, 5, 12, 808,
// Sorted indices     : 1, 2, 4, 4, 5, 5, 12, 12, 23, 23, 34, 115, 808,
// Duplicates removed : 1, 2, 4, 5, 12, 23, 34, 115, 808,

namespace 
{ 
  const size_t ERROR_IN_COMMAND_LINE = 1; 
  const size_t SUCCESS = 0; 
  const size_t ERROR_UNHANDLED_EXCEPTION = 2; 
 
} // namespace 

#define VERSION 0.1 ///< Version number of the program.

int main(int argc, char *argv[]) {

  //
  // Input Argument Handling
  //
  std::vector<int> indexed_dims;
  std::vector<int> j_list;

  namespace po = boost::program_options;
  po::options_description po_description("Options");
  po_description.add_options()
    ("help", "display this help message")
    ("version", "display the version number")
    ("indexes,i", po::value< std::vector<int> >(&indexed_dims)
        ->default_value(std::vector<int>(0), "none")->multitoken(),
        "list of indexes to initialize vector with")
    ("jlist,j", po::value< std::vector<int> >(&j_list)
        ->default_value(std::vector<int>(0), "none")->multitoken(),
        "just another list")
    ;

  po::positional_options_description po_positional;
  po::variables_map po_vm;
  po::store(po::command_line_parser(argc, argv).options(po_description)
      .positional(po_positional).run(), po_vm);

  if (po_vm.count("help")) {
    std::cout << po_description << std::endl;
    return 0;
  }

  if (po_vm.count("version")) {
    std::cout << "Version " << VERSION << std::endl;
    return 0;
  }

  // this has to be after the help/version commands as this
  // exits with an error if the required arguments aren't
  // specified
  po::notify(po_vm);

  std::cout << "Supplied indices   : ";
  std::ostream_iterator<int> out_it (std::cout,", ");
  std::copy ( indexed_dims.begin(), indexed_dims.end(), out_it );
  std::cout << std::endl;

  std::cout << "Sorted indices     : ";
  std::sort(indexed_dims.begin(), indexed_dims.end(), std::less<int>());
  std::copy ( indexed_dims.begin(), indexed_dims.end(), out_it );
  std::cout << std::endl;

  // must be sorted before calling unique
  std::cout << "Duplicates removed : ";
  std::vector<int>::iterator last = std::unique(indexed_dims.begin(), indexed_dims.end());     
  indexed_dims.erase(last, indexed_dims.end());
  std::copy ( indexed_dims.begin(), indexed_dims.end(), out_it );
  std::cout << std::endl;

  std::cout << "A second list for testing : ";
  std::copy ( j_list.begin(), j_list.end(), out_it );
  std::cout << std::endl;

  return 0;
}

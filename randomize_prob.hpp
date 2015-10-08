////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RANDOMIZE PROB:: Generates given number of random clauses with given number of literals for a class A.				 //                                                                                                         
//                  If exact is zero or not given than the length of the clause is random between 1 and num_lit //                                                                                                                       
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef randomize_prob_HPP
#define randomize_prob_HPP

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <math.h> 
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include "sat_prob.hpp"


//RANDOM SAMPLE::generates n unique numbers from N numbers, should be only used if n<<N

/*template <typename Distribution, typename OutIt, typename URNG>
void random_sample(Distribution& pool, std::size_t k, OutIt out, URNG &g, const double p) {
  std::unordered_set<decltype(pool(g))> sample;
  boost::random::uniform_real_distribution<> rand_p(0,1);
  while (sample.size() < k) {
    auto elem = pool(g);
    if (sample.insert(elem).second){
			if(rand_p(g)<p)
      	*out++ = -elem;
      else
      	*out++ = elem;
    }
  }
}*/


//PARTIAL SHUFFLE: generates n unique numbers form N via partial shuffling the elements of the vector
template <typename RandomIt, typename URNG>
void partial_shuffle(RandomIt first, RandomIt mid, RandomIt last, URNG &g) {
  auto n = last - first;
  auto k = mid - first;
  for (decltype(n) i{}; i < k; ++i) {
    auto j = boost::random::uniform_int_distribution<decltype(i)>(i, n - 1)(g);
    using std::swap;
    swap(first[i], first[j]);
  }
}

void randomize_prob(sat_prob &A, boost::random::mt19937 &gen, unsigned int num_lit=0, bool exact = true);

#endif

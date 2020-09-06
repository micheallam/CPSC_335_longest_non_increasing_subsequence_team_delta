///////////////////////////////////////////////////////////////////////////////
// subsequence.hpp
//
// Two algorithms for solving the longest non-increasing subsequence problem.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <sstream>
#include <vector>

using sequence = std::vector<int>;

// Convert a sequence into a human-readable string useful for pretty-printing
// or debugging.
std::string sequence_to_string(const sequence& seq) {
  std::stringstream ss;
  ss << "[";
  bool first = true;
  for (auto& x : seq) {
    if (!first) {
      ss << ", ";
    }
    ss << x;
    first = false;
  }
  ss << "]";
  return ss.str();
}

// Generate a pseudorandom sequence of the given size, using the given
// seed, where all elements are in the range [0, max_element].
// max_element must be non-negative.
sequence random_sequence(size_t size, unsigned seed, int max_element) {

    assert(max_element >= 0);

    sequence result;

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(0, max_element);

    for (size_t i = 0; i < size; ++i) {
        result.push_back(dist(gen));
    }

    return result;
}

bool is_nonincreasing(const sequence& A) {
  //Keeps sequence in a temp variable
  sequence temp = A;
  //Compares the current value with the next value to see if it's is_nonincreasing
  for(size_t i = 0; i < A.size() - 1; i++){
    if (temp[i] < temp[i+1]) {
      //Return false if it's increasing
      return false;
    }
  }
  //FINISHED
  return true;
  }

sequence longest_nonincreasing_end_to_beginning(const sequence& A) {

  const size_t n = A.size();

  // populate the array H with 0 values
  std::vector<size_t> H(n, 0);

  // calculate the values of array H
  // note that i has to be declared signed, to avoid an infinite loop, since
  // the loop condition is i >= 0
  for (signed int i = n-2;  i>= 0; i--) {
    for (size_t j = i+1; j < n ; j++) {
        
        // Check if A[i] is bigger than A[j] which should be every value after A[i]
        // and check if H[i] is bigger than H[j] which should be every value after H[i]
        // if it is then H[i] should be the previous biggest H value + 1 because a new value
        // is bigger than the previous A
        if (A[i] >= A[j] && H[i] <= H[j]) {
            H[i] = H[j] + 1;
        }
    }
  }

  // calculate in max the length of the longest non-increasing subsequence
  // by adding 1 to the maximum value in H
  auto max = *std::max_element(H.begin(), H.end()) + 1;

  // allocate space for the subsequence R
  std::vector<int> R(max);

  // add elements to R in non-increasing order whose H's values are
  // in decreasing order, starting with the element whose H value is max-1
  // store in index the H values sought: max-1, max-2, max-3, .. 0

  size_t index = max-1, j = 0;
  for (size_t i = 0; i < n; ++i) {
    if (H[i] == index) { // Checks H and index to see if they are the same
        
        // Stores the largest A value based on H in R
        R[j] = A[i];
        index--; // decrement index
        j++; // increment j to move to next space to store value from A
    }
  }

  return sequence(R.begin(), R.begin() + max);
}

sequence longest_nonincreasing_powerset(const sequence& A) {
  const size_t n = A.size();
  sequence best;
  std::vector<size_t> stack(n+1, 0);
  size_t k = 0;
  while (true) {

    if (stack[k] < n) {
      stack[k+1] = stack[k] + 1;
      ++k;
    } else {
      stack[k-1]++;
      k--;
    }

    if (k == 0) {
      break;
    }

    sequence candidate;
    for (size_t i = 1; i <= k; ++i) {
      candidate.push_back(A[stack[i]-1]);
    }

    if(best.empty() || (is_nonincreasing(candidate) && candidate.size() > best.size())){
      best = candidate;

    }
    //Finished?
  }
  return best;
}

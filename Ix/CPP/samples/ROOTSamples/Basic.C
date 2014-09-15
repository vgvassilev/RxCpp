// Author Vassil Vassilev & Lorenzo Moneta

#include "DataModel/EventData.h"

#include <vector>
using namespace std;

#include "cpplinq/linq.hpp"

struct int_iter
   : std::iterator<std::random_access_iterator_tag, int, ptrdiff_t, int*, int>
{
   int_iter(value_type i = 0, value_type step = 1) : value(i), step(step)
   {}
   value_type operator*() const {
      return value;
   }
   int_iter& operator++() {
      value+=step; return *this;
   }
   int_iter& operator--() {
      value-=step; return *this;
   }
   int_iter& operator+=(ptrdiff_t offset) {
      value += step*offset; return *this;
   }
   int_iter& operator-=(ptrdiff_t offset) {
      value -= step*offset; return *this;
   }
   ptrdiff_t operator-(int_iter rhs) const {
      return ptrdiff_t((value - rhs.value)/step);
   }
   bool operator==(int_iter other) const {
      return value == other.value;
   }
   bool operator!=(int_iter other) const {
      return !(*this == other);
   }
   bool operator<(int_iter other) const { return (*this - other) < 0; }
   bool operator>(int_iter other) const { return (*this - other) > 0; }
   bool operator<=(int_iter other) const { return (*this - other) <= 0; }
   bool operator>=(int_iter other) const { return (*this - other) >= 0; }
   value_type value;
   value_type step;
};
int_iter operator+(int_iter lhs, ptrdiff_t rhs) {
   return lhs+=rhs;
}
int_iter operator+(ptrdiff_t lhs, int_iter rhs) {
   return rhs+=lhs;
}
int_iter operator-(int_iter lhs, ptrdiff_t rhs) {
   return lhs-=rhs;
}

struct int_range
{
   typedef int_iter iterator;
   typedef int_iter::value_type value_type;
   int_range(value_type begin, value_type end, value_type step = 1)
  : b(begin, step), e(end, step)
   {
      if (step == 0) {
         throw std::logic_error("bad step");
      }
      if (abs(end - begin) % abs(step) != 0) {
         end -= (end-begin)%abs(step);
      }
   }
   int_iter begin() const { return int_iter(b);}
   int_iter end() const { return int_iter(e); }
   iterator b, e;
};

bool is_prime(int x) {
   if (x < 2) {return false;}
   if (x == 2) {return true;}
   for (int i = x/2; i >= 2; --i) {
      if (x % i == 0) { return false;}
   }
   return true;
};

long sum_primes() {
   auto xs = int_range(0, 100);
   using namespace cpplinq;
   return from(xs).where(is_prime).sum();
}

long sum_squared_primes() {
   auto xs = int_range(0, 100);
   using namespace cpplinq;
   return from(xs).where(is_prime).select([](int x){ return x*x; }).sum();
}

std::vector<int> gimme_some_primes(size_t howMany) {
   auto xs = int_range(0, -1); // to MAX(int)
   using namespace cpplinq;
   return from(xs).where(is_prime).take(howMany).to_vector();
}

void numbers() {
   auto xs = int_range(0, 100);

   using namespace cpplinq;
   auto ys = from(xs)
      .where(is_prime)
      .select([](int x){ return x*x; }).sum();
   printf ("Result primes is: %d\n", ys);
}


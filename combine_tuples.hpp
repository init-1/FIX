#ifndef PL_COMBINE_TUPLES_HPP_
#define PL_COMBINE_TUPLES_HPP_

// Combines tuples/fields into a single tuple.
// Note reference types are not allowed!

#include <tuple>

namespace pl {

using std::tuple;

template<typename...> struct combine_tuples;

template<typename... Ts>
struct combine_tuples<tuple<Ts...>> {
  typedef tuple<Ts...> type;
};

template<typename... T1s, typename... T2s, typename... Rem>
struct combine_tuples<tuple<T1s...>, tuple<T2s...>, Rem...> {
  typedef typename combine_tuples<tuple<T1s..., T2s...>, Rem...>::type type; 
};

template<typename T1, typename... T2s, typename... Rem>
struct combine_tuples<T1, tuple<T2s...>, Rem...> {
  typedef typename combine_tuples<tuple<T1, T2s...>, Rem...>::type type; 
};

template<typename... T1s, typename T2, typename... Rem>
struct combine_tuples<tuple<T1s...>, T2, Rem...> {
  typedef typename combine_tuples<tuple<T1s..., T2>, Rem...>::type type; 
};

template<typename T1, typename T2, typename... Rem>
struct combine_tuples<T1, T2, Rem...> {
  typedef typename combine_tuples<tuple<T1, T2>, Rem...>::type type; 
};

// Computes the result type of tuple_cat given a set of tuple-like types.
template<typename... Ts>
struct tuple_cat_result {
  typedef typename combine_tuples<Ts...>::type type;
};

}  // namesapce pl

#endif

/* Test combine_tuple
int main() {
  typedef tuple_cat_result<int, double, tuple<long, char>, float, tuple<char*, std::string>, std::string const*>::type type;
  type a;
}
*/


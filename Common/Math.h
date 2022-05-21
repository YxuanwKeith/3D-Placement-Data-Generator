#pragma once

#include "Common/TypeDef.h"

using namespace std;

namespace WorkSpace {

template<typename T>
T calc_gcd(T x, T y) { return (!y) ? x : calc_gcd<T>(y, x % y);}

template<typename T>
T calc_lcm(T x, T y) { return x * y / calc_gcd(x, y); }

}
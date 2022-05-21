#pragma once

#include <set>
#include <map>
#include <cmath>
#include <ctime>
#include <array>
#include <chrono>
#include <vector>
#include <string>
#include <random>
#include <utility>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using namespace std;

namespace WorkSpace {

using Int = int;
using LongInt = long long;
using Real = double;
using Str = string;

using Millisecond = long long;

template<typename T, size_t Size>
using Arr = array<T, Size>;

template<typename T>
using Vec = vector<T>;

template<typename T>
using Set = set<T>;

template<typename T>
using HashSet = unordered_set<T>;

template<typename Key, typename Value>
using Map = map<Key, Value>;

template<typename Key, typename Value>
using HashMap = unordered_map<Key, Value>;

using Clock = chrono::steady_clock;
using TimePoint = Clock::time_point;

template<typename NewType, typename OldType>
constexpr NewType sCast(OldType obj) { return static_cast<NewType>(obj); }

} //WorkSpace
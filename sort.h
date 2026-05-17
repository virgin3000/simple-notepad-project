#ifndef SORT_H
#define SORT_H

#include <algorithm>
#include <vector>

// Generic sort wrapper using std::sort with optional comparator
template <typename T, typename Compare = std::less<T>>
void sort_vector(std::vector<T>& vec, Compare comp = Compare{})
{
    std::sort(vec.begin(), vec.end(), comp);
}

// Sort pairs by second element descending (used for word frequency)
template <typename K, typename V>
void sort_by_value_desc(std::vector<std::pair<K, V>>& pairs)
{
    std::sort(pairs.begin(), pairs.end(),
              [](const std::pair<K, V>& a, const std::pair<K, V>& b) {
                  return a.second > b.second;
              });
}

#endif // SORT_H

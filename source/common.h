/**
 * @file common.h
 * @author Krisna Pranav
 * @brief Entry Point Of The Application
 * @version 0.1
 * @date 2023-02-06
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditors
 * 
 */

#pragma once


#include <climits>
#include <cfloat>
#include <cmath>
#include "JuceHeader.h"
#include "../thirdparty/hopscotch-map/include/tsl/hopscotch_map.h"

template <class Key, class T, class HashFn = std::hash<Key>, class EqualKey = std::equal_to<Key>>
using FlatHashMap = tsl::hopscotch_map<Key, T, HashFn, EqualKey>

#include "../thirdparty/hopscotch-map/include/tsl/hopscotch_set.h"

template <class Value, class HashFn = std::hash<Value>, class EqualKey = std::equal_to<Value>>
using FlatHashSet = tsl::hopscotch_set<Value, HashFn, EqualKey>;

using HashCode = size_t;

/**
 * @brief StringHashing 
 * 
 */
struct StringHash {
    inline HashCode operator()(const juce::String &key) const noexcept {
        return static_cast<HashCode>(key.hashCode());
    }
};
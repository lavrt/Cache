#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>

template <typename T, typename keyT = int>
class TwoQCache {
    using ListIter = typename std::list<std::pair<keyT, T>>::iterator;
    
private:
    const double kAmRatio = 0.75;

    size_t am_capacity = 0;
    std::list<std::pair<keyT, T>> am;
    std::unordered_map<keyT, ListIter> am_hash;

    size_t a1_in_capacity = 0;
    std::list<std::pair<keyT, T>> a1_in;
    std::unordered_map<keyT, ListIter> a1_in_hash;

    size_t a1_out_capacity = 0;
    std::unordered_set<keyT> a1_out;

    keyT RemoveLast(std::list<std::pair<keyT, T>>& list, std::unordered_map<keyT, ListIter>& hash) {
        if (list.empty()) {
            throw std::runtime_error("Attempt to delete an element from an empty list");
        }

        keyT last_key = list.back().first;
        list.pop_back();
        hash.erase(last_key);

        return last_key;
    }

public:
    TwoQCache(size_t sz) {
        if (sz == 0) {
            throw std::invalid_argument("The cache size cannot be zero");
        }

        am_capacity = static_cast<size_t>(sz * kAmRatio);
        a1_in_capacity = sz - am_capacity;
        a1_out_capacity = a1_in_capacity;
    }

    void PrintCache() { // NOTE debug
        std::cout << "________________________________\n";
        std::cout << "Am(" << am_capacity << "): ";
        for (const auto& elem : am) {
            std::cout << elem.second << " ";
        }
        std::cout << "\nA1_in(" << a1_in_capacity << "): ";
        for (const auto& elem : a1_in) {
            std::cout << elem.second << " ";
        } 
        std::cout << "\nA1_out(" << a1_out_capacity << "): ";
        for (const auto& elem : a1_out) {
            std::cout << elem << " ";
        }
        std::cout << "\n________________________________\n";
    }

    template <typename F>
    bool LookupUpdate(keyT key, F SlowGetPage) {
        if (auto hit = am_hash.find(key); hit != am_hash.end()) {
            if (hit->second != am.begin()) {
                am.splice(am.begin(), am, hit->second);
            }
            return true;
        } else if (auto hit = a1_in_hash.find(key); hit != a1_in_hash.end()) {
            if (am.size() == am_capacity) {
                RemoveLast(am, am_hash);
            }
            am.splice(am.begin(), a1_in, hit->second);
            a1_in_hash.erase(hit->first);
            return true;
        } else if (a1_out.contains(key)) {
            if (am.size() == am_capacity) {
                RemoveLast(am, am_hash);
            }
            am.emplace_front(key, SlowGetPage(key));
            am_hash[key] = am.begin();
            a1_out.erase(key);
            return false;
        } else {
            if (a1_in.size() == a1_in_capacity) {
                a1_out.insert(RemoveLast(a1_in, a1_in_hash));
            }
            a1_in.emplace_front(key, SlowGetPage(key));
            a1_in_hash[key] = a1_in.begin();
            return false;
        }
    }
};

#endif // CACHE_HPP

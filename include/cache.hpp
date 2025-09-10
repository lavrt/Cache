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

    size_t am_capacity_ = 0;
    std::list<std::pair<keyT, T>> am_;
    std::unordered_map<keyT, ListIter> am_hash_;

    size_t a1_in_capacity_ = 0;
    std::list<std::pair<keyT, T>> a1_in_;
    std::unordered_map<keyT, ListIter> a1_in_hash_;

    size_t a1_out_capacity_ = 0;
    std::list<keyT> a1_out_;
    std::unordered_set<keyT> a1_out_hash_;

    keyT RemoveLast(std::list<std::pair<keyT, T>>& list, std::unordered_map<keyT, ListIter>& hash) {
        if (list.empty()) {
            throw std::runtime_error("Attempt to delete an element from an empty list");
        }
        keyT last_key = list.back().first;
        list.pop_back();
        hash.erase(last_key);
        return last_key;
    }

    keyT RemoveLast(std::list<keyT>& list, std::unordered_set<keyT>& hash) {
        if (list.empty()) {
            throw std::runtime_error("Attempt to delete an element from an empty list");
        }
        keyT last_key = list.back();
        list.pop_back();
        hash.erase(last_key);
        return last_key;
    }

public:
    TwoQCache(size_t sz) {
        am_capacity_ = static_cast<size_t>(sz * kAmRatio);
        a1_in_capacity_ = sz - am_capacity_;
        a1_out_capacity_ = a1_in_capacity_;
        if (am_capacity_ <= 0 || a1_in_capacity_ <= 0 || a1_out_capacity_ <= 0) {
            throw std::invalid_argument("The cache size is too small");
        }
    }

    void PrintCache() const { // NOTE debug
        std::cout << "++++++++++++++++++++++++++++++++\n";
        std::cout << "Am(" << am_capacity_ << "): ";
        for (const auto& elem : am_) {
            std::cout << elem.second << " ";
        }
        std::cout << "\nAm_hash:\n";
        for (const auto& elem : am_hash_) {
            std::cout << elem.first << " ";
        }
        std::cout << "\nA1_in(" << a1_in_capacity_ << "): ";
        for (const auto& elem : a1_in_) {
            std::cout << elem.second << " ";
        } 
        std::cout << "\na1_in_hash_:\n";
        for (const auto& elem : a1_in_hash_) {
            std::cout << elem.first << " ";
        }
        std::cout << "\nA1_out(" << a1_out_capacity_ << "): ";
        for (const auto& elem : a1_out_) {
            std::cout << elem << " ";
        }
        std::cout << "\na1_out_hash_:\n";
        for (const auto& elem : a1_out_hash_) {
            std::cout << elem << " ";
        }
        std::cout << "\n++++++++++++++++++++++++++++++++\n";
    }

    template <typename F>
    bool LookupUpdate(keyT key, F SlowGetPage) {
        if (auto hit = am_hash_.find(key); hit != am_hash_.end()) {
            if (hit->second != am_.begin()) {
                am_.splice(am_.begin(), am_, hit->second);
            }
            return true;
        } else if (auto hit = a1_in_hash_.find(key); hit != a1_in_hash_.end()) {
            if (am_.size() == am_capacity_) {
                RemoveLast(am_, am_hash_);
            }
            am_.splice(am_.begin(), a1_in_, hit->second);
            am_hash_[hit->first] = am_.begin();
            a1_in_hash_.erase(hit->first);
            return true;
        } else if (a1_out_hash_.contains(key)) {
            if (am_.size() == am_capacity_) {
                RemoveLast(am_, am_hash_);
            }
            am_.emplace_front(key, SlowGetPage(key));
            am_hash_[key] = am_.begin();
            a1_out_.remove(key);
            a1_out_hash_.erase(key);
            return false;
        } else {
            if (a1_in_.size() == a1_in_capacity_) {
                if (a1_out_.size() == a1_out_capacity_) {
                    RemoveLast(a1_out_, a1_out_hash_);
                }
                keyT last_key = RemoveLast(a1_in_, a1_in_hash_);
                a1_out_hash_.insert(last_key);
                a1_out_.emplace_front(last_key);
            }
            a1_in_.emplace_front(key, SlowGetPage(key));
            a1_in_hash_[key] = a1_in_.begin();
            return false;
        }
    }
};

#endif // CACHE_HPP

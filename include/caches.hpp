#ifndef CACHE_HPP
#define CACHE_HPP

#include <map>
#include <limits>
#include <vector>
#include <list>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

namespace caches {

template <typename T, typename keyT = int>
class TwoQCache {
    using ListIter = typename std::list<std::pair<keyT, T>>::iterator;
    using OutListIter = typename std::list<keyT>::iterator;
    
private:
    static constexpr double kDefaultAmRatio = 0.5;
    static constexpr double kDefaultA1OutRatio = 2;

    size_t am_capacity_ = 0;
    std::list<std::pair<keyT, T>> am_;
    std::unordered_map<keyT, ListIter> am_hash_;

    size_t a1_in_capacity_ = 0;
    std::list<std::pair<keyT, T>> a1_in_;
    std::unordered_map<keyT, ListIter> a1_in_hash_;

    size_t a1_out_capacity_ = 0;
    std::list<keyT> a1_out_;
    std::unordered_map<keyT, OutListIter> a1_out_hash_;

    keyT RemoveLast(std::list<std::pair<keyT, T>>& list, std::unordered_map<keyT, ListIter>& hash) {
        if (list.empty()) {
            throw std::runtime_error("Attempt to delete an element from an empty list");
        }

        keyT last_key = list.back().first;
        list.pop_back();
        hash.erase(last_key);

        return last_key;
    }

    keyT RemoveLast(std::list<keyT>& list, std::unordered_map<keyT, OutListIter>& hash) {
        if (list.empty()) {
            throw std::runtime_error("Attempt to delete an element from an empty list");
        }

        keyT last_key = list.back();
        list.pop_back();
        hash.erase(last_key);

        return last_key;
    }

public:
    TwoQCache(size_t size, double am_ratio = kDefaultAmRatio) {
        if (size == 0) {
            throw std::invalid_argument("Cache size must be at least 1");
        }

        a1_in_capacity_ = std::max(static_cast<size_t>(size * (1 - am_ratio)), size_t(0));
        am_capacity_ = size - a1_in_capacity_;

        if (am_capacity_ == 0) {
            am_capacity_ = 1;
            a1_in_capacity_ = size - 1;
        }

        a1_out_capacity_ = size * kDefaultA1OutRatio;
    }

    size_t GetSize() const noexcept {
        return am_capacity_ + a1_in_capacity_;
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
            am_hash_[key] = am_.begin();
            a1_in_hash_.erase(hit->first);
            return true;
        } else if (auto hit = a1_out_hash_.find(key); hit != a1_out_hash_.end()) {
            if (am_.size() == am_capacity_) {
                RemoveLast(am_, am_hash_);
            }
            am_.emplace_front(key, SlowGetPage(key));
            am_hash_[key] = am_.begin();
            a1_out_.erase(hit->second); 
            a1_out_hash_.erase(hit);
            return false;
        } else if (GetSize() == 1) {
            if (!am_.empty()) {
                RemoveLast(am_, am_hash_);
            }
            am_.emplace_front(key, SlowGetPage(key));
            am_hash_[key] = am_.begin();
            return false;
        } else {
            if (a1_in_.size() == a1_in_capacity_) {
                if (a1_out_.size() == a1_out_capacity_) {
                    RemoveLast(a1_out_, a1_out_hash_);
                }
                keyT last_key = RemoveLast(a1_in_, a1_in_hash_);
                a1_out_.emplace_front(last_key);
                a1_out_hash_[last_key] = a1_out_.begin();
            }
            a1_in_.emplace_front(key, SlowGetPage(key));
            a1_in_hash_[key] = a1_in_.begin();
            return false;
        }
    }
};

template <typename T, typename keyT = int>
class IdealCache {
private:
    size_t capacity_ = 0;
    std::unordered_map<keyT, T> cache_;

    size_t current_pos_ = 0;

    std::vector<keyT> requests_;
    std::unordered_map<keyT, std::vector<size_t>> key_positions_;
    std::map<size_t, keyT, std::greater<size_t>> eviction_queue_;

    void UpdateEvictionQueue() {
        if (auto iter = eviction_queue_.find(current_pos_); iter != eviction_queue_.end()) {
            eviction_queue_[std::numeric_limits<size_t>::max()] = iter->second;
            eviction_queue_.erase(iter);
        }
    }

public:
    IdealCache(size_t capacity, const std::vector<keyT>& requests)
        : requests_(requests), capacity_(capacity)
    {
        if (capacity == 0) {
            throw std::invalid_argument("Cache size must be at least 1");
        }

        for (size_t i = 0; i != requests_.size(); ++i) {
            key_positions_[requests_[i]].push_back(i);
        }
    }

    template <typename F>
    bool LookupUpdate(keyT key, F SlowGetPage) {
        if (cache_.contains(key)) {
            UpdateEvictionQueue();
            ++current_pos_;
            return true;
        }

        auto key_entry = key_positions_.find(key);
        auto next_pos_iter = std::upper_bound(
            key_entry->second.begin(), key_entry->second.end(), current_pos_
        );
        if (next_pos_iter == key_entry->second.end()) {
            UpdateEvictionQueue();
            ++current_pos_;
            return false;
        }

        if (cache_.size() == capacity_) {
            cache_.erase(eviction_queue_.begin()->second);
            eviction_queue_.erase(eviction_queue_.begin());
        }

        cache_[key] = SlowGetPage(key);
        eviction_queue_[key_positions_[key].back()] = key;
        
        UpdateEvictionQueue();
        ++current_pos_;
        return false;
    }
};

} // namespace caches

#endif // CACHE_HPP

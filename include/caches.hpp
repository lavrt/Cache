#ifndef CACHE_HPP
#define CACHE_HPP

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
    using ListIter = typename std::list<std::pair<keyT, T>>::iterator;

private:
    size_t capacity_ = 0;
    std::list<std::pair<keyT, T>> cache_;
    std::unordered_map<keyT, ListIter> hash_;

    size_t current_pos_ = 0;
    std::vector<keyT> requests_;

public:
    IdealCache(size_t capacity, const std::vector<keyT>& requests)
        : requests_(requests), capacity_(capacity)
    {
        if (capacity == 0) {
            throw std::invalid_argument("The size cannot be 0");
        }
    }

    template <typename F>
    bool LookupUpdate(keyT key, F SlowGetPage) {
        ++current_pos_;

        if (hash_.contains(key)) {
            return true;
        }

        if (cache_.size() == capacity_) {
            auto further_iter = cache_.begin();
            size_t further_pos = 0;

            for (auto iter = cache_.begin(); iter != cache_.end(); ++iter) {
                auto future_iter = std::find(
                    requests_.begin() + current_pos_, requests_.end(), iter->first
                );

                if (future_iter == requests_.end()) {
                    further_iter = iter;
                    break;
                }

                if (size_t dst = std::distance(requests_.begin(), future_iter); dst > further_pos) {
                    further_pos = dst;
                    further_iter = iter;
                }
            }

            hash_.erase(further_iter->first);
            cache_.erase(further_iter);
        }

        cache_.emplace_front(key, SlowGetPage(key));
        hash_[key] = cache_.begin();
        return false;
    }
};

} // namespace caches

#endif // CACHE_HPP

#ifndef SIMPLE_HASH_TABLE_H_
#define SIMPLE_HASH_TABLE_H_

#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <optional>
#include <vector>

using namespace std;

// prime number default size
constexpr size_t SIMPLE_TABLE_SIZE_DEFAULT = 1031;

namespace custom {

// HashMap class taking a key, value, and optional hash function.
template <typename K, typename V, typename H = hash<K>>
class SimpleHashTable {
private:
    size_t HashFn(const K& key) const
    {
        return H {}(key) % buckets.size();
    }

    struct HashNode {
        K k;
        V v;
    };
    // each bucket is a (doubly-linked) list of unique k-v pairs to handle key collisions
    // poor locality so its not ideal, but fine for a simple hash table
    using Bucket = list<HashNode>;
    // the entire data structure is a vector of these buckets
    vector<Bucket> buckets;

public:
    SimpleHashTable(size_t table_size_ = SIMPLE_TABLE_SIZE_DEFAULT)
        : buckets(table_size_) {};

    // inserts elements
    std::pair<typename Bucket::iterator, bool> insert(const std::pair<const K, V>& key_value)
    {
        size_t index = HashFn(key_value.first);
        auto& bucket = buckets[index];

        auto it = std::find_if(bucket.begin(), bucket.end(),
            [&key_value](const HashNode& node) { return node.k == key_value.first; });

        if (it == bucket.end()) {
            it = bucket.insert(bucket.end(), { key_value.first, key_value.second });
            return { it, true };
        }

        return { it, false };
    }

    // erases elements
    // returning the number of elements removed, since thats easy
    size_t erase(const K& key)
    {
        size_t index = HashFn(key);
        auto& bucket = buckets[index];

        auto it = std::find_if(bucket.begin(), bucket.end(),
            [&key](const HashNode& node) { return node.k == key; });

        if (it != bucket.end()) {
            bucket.erase(it);
            return 1;
        }
        return 0;
    }

    // finds element with specific key
    std::optional<V> find(const K& key)
    {
        size_t index = HashFn(key);
        const auto& bucket = buckets[index];

        auto it = find_if(bucket.begin(), bucket.end(), [&key](const HashNode& node) { return node.k == key; });

        if (it != bucket.end()) {
            return it->v;
        }

        return std::nullopt;
    }

    // clears the contents
    void clear()
    {
        for (auto& bucket : buckets) {
            bucket.clear();
        }
    }

    // get bucket count (or the size of the table vector)
    size_t bucket_count()
    {
        return buckets.size();
    }
};

}

#endif // SIMPLE_HASH_TABLE_H_
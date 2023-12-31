#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <optional>
#include <vector>

using namespace std;

constexpr size_t TABLE_SIZE_DEFAULT = 1031;

namespace custom {

// HashMap class taking a key, value, and optional hash function.
// since we're using open addressing and linear probing, each bucket
// only has one k-v pair
template <typename K, typename V, typename H = std::hash<K>>
class HashTable {
private:
    struct KeyValuePair {
        K key;
        V value;
        bool occupied;

        // for debugging
        //        friend std::ostream& operator<<(std::ostream& os, const KeyValuePair& kvp)
        //        {
        //            os << "(" << kvp.key << ", " << kvp.value << ", " << std::boolalpha << kvp.occupied << ")";
        //            return os;
        //        }
    };

    using table_type = std::vector<KeyValuePair>;
    table_type table;
    size_t table_size;
    // 0.6 - 0.75 is good
    double max_load_factor = 0.6;

    size_t HashFn(const K& key) const
    {
        return H {}(key) % table.size();
    }

    // find the next available slot with linear probing
    std::optional<size_t> findSlot(const K& search_key) const
    {
        size_t i = HashFn(search_key);
        size_t initial_i = i;

        // search until we find a key or an empty slot. we may find neither, so save
        // the initial index in case we wrap around
        //        while (table[i].occupied && table[i].key != search_key || i != initial_i) {
        //            // Linear probing: move to the next slot
        //            i = (i + 1) % table.size();
        //        }
        //
        //        if (!table[i].occupied || i == initial_i) {
        //            return i;
        //        }

        for (;; i = (i + 1) % table.size()) {
            if (!table[i].occupied || i == initial_i) {
                break;
            }
        }

        return std::nullopt;
    }

    void resizeTable()
    {
        size_t newSize = table.size() * 2;

        std::vector<KeyValuePair> new_table(newSize, KeyValuePair { K {}, V {}, false });

        for (const auto& entry : table) {
            if (entry.occupied) {
                size_t new_index = HashFn(entry.key);

                while (new_table[new_index].occupied) {
                    new_index = (new_index + 1) % new_table.size();
                }

                new_table[new_index] = entry;
            }
        }

        table = std::move(new_table);
    }

public:
    HashTable(size_t initial_size = TABLE_SIZE_DEFAULT)
        : table(initial_size)
        , table_size(0)
    {
    }

    // inserts elements, only if the container doesn't already contain an element with an equivalent key
    std::pair<typename table_type::iterator, bool> insert(const std::pair<const K, V>& key_value)
    {
        auto slot_index = findSlot(key_value.first);

        if (!slot_index.has_value()) {
            return { table.end(), false };
        }

        size_t index = slot_index.value();

        // if we've found a key that matches
        if (table[index].occupied) {
            return { table.begin() + index, false };
        }

        // if table is almost full, so calculate count / capacity
        if ((static_cast<double>(table_size) + 1) / table.size() > max_load_factor) {
            resizeTable();
        }

        // set k-v and mark as occupied, returning an iterator to this position
        table[index] = { key_value.first, key_value.second, true };
        table_size++;
        return { table.begin() + index, true };
    }

    // erases elements
    // returning the number of elements removed, since thats easy
    bool erase(const K& key)
    {
        auto slot_index = findSlot(key);

        if (!slot_index.has_value()) {
            return false;
        }

        size_t index = slot_index.value();

        // key doesn't exist
        if (!table[index].occupied || table[index].key != key)
            return false;

        table[index].occupied = false;
        table_size--;

        size_t next_index = (index + 1) % table.size();
        while (table[next_index].occupied) {
            size_t target_index = HashFn(table[next_index].key);

            // If the target index is not between 'index' and 'next_index', move the element to the target index
            if ((index < next_index && (target_index <= index || target_index > next_index)) || (index > next_index && target_index <= index && target_index > next_index)) {
                table[index] = std::move(table[next_index]);
                index = next_index;
            }

            next_index = (next_index + 1) % table.size();
        }

        return true;
    }

    // finds element with specific key
    typename std::vector<KeyValuePair>::iterator find(const K& key)
    {
        auto slot_index = findSlot(key);

        if (!slot_index.has_value()) {
            return table.end();
        }

        return table.begin() + slot_index.value();
    }

    // clears the contents
    void clear()
    {
        table.assign(table.size(), KeyValuePair { K {}, V {}, false });
        table_size = 0;
    }

    size_t size() const
    {
        return table_size;
    }

    size_t capacity() const
    {
        return table.size();
    }

    typename table_type::iterator end()
    {
        return table.end();
    }
};

}

#endif // HASH_TABLE_H_
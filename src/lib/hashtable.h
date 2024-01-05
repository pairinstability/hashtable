#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <optional>
#include <vector>

using namespace std;

constexpr size_t TABLE_SIZE_DEFAULT = 101;

namespace custom {

// HashMap class taking a key, value, and optional hash function.
// since we're using open addressing and linear probing, each bucket
// only has one k-v pair
template <typename K, typename V, typename H = std::hash<K>>
class HashTable {
private:
static constexpr const size_t prime_list[] =
{
	2llu, 3llu, 5llu, 7llu, 11llu, 13llu, 17llu, 23llu, 29llu, 37llu, 47llu,
	59llu, 73llu, 97llu, 127llu, 151llu, 197llu, 251llu, 313llu, 397llu,
	499llu, 631llu, 797llu, 1009llu, 1259llu, 1597llu, 2011llu, 2539llu,
	3203llu, 4027llu, 5087llu, 6421llu, 8089llu, 10193llu, 12853llu, 16193llu,
	20399llu, 25717llu, 32401llu, 40823llu, 51437llu, 64811llu, 81649llu,
	102877llu, 129607llu, 163307llu, 205759llu, 259229llu, 326617llu,
	411527llu, 518509llu, 653267llu, 823117llu, 1037059llu, 1306601llu,
	1646237llu, 2074129llu, 2613229llu, 3292489llu, 4148279llu, 5226491llu,
	6584983llu, 8296553llu, 10453007llu, 13169977llu, 16593127llu, 20906033llu,
	26339969llu, 33186281llu, 41812097llu, 52679969llu, 66372617llu,
	83624237llu, 105359939llu, 132745199llu, 167248483llu, 210719881llu,
	265490441llu, 334496971llu, 421439783llu, 530980861llu, 668993977llu,
	842879579llu, 1061961721llu, 1337987929llu, 1685759167llu, 2123923447llu,
	2675975881llu, 3371518343llu, 4247846927llu, 5351951779llu, 6743036717llu,
	8495693897llu, 10703903591llu, 13486073473llu, 16991387857llu,
	21407807219llu, 26972146961llu, 33982775741llu, 42815614441llu,
	53944293929llu, 67965551447llu, 85631228929llu, 107888587883llu,
	135931102921llu, 171262457903llu, 215777175787llu, 271862205833llu,
	342524915839llu, 431554351609llu, 543724411781llu, 685049831731llu,
	863108703229llu, 1087448823553llu, 1370099663459llu, 1726217406467llu,
	2174897647073llu, 2740199326961llu, 3452434812973llu, 4349795294267llu,
	5480398654009llu, 6904869625999llu, 8699590588571llu, 10960797308051llu,
	13809739252051llu, 17399181177241llu, 21921594616111llu, 27619478504183llu,
	34798362354533llu, 43843189232363llu, 55238957008387llu, 69596724709081llu,
	87686378464759llu, 110477914016779llu, 139193449418173llu,
	175372756929481llu, 220955828033581llu, 278386898836457llu,
	350745513859007llu, 441911656067171llu, 556773797672909llu,
	701491027718027llu, 883823312134381llu, 1113547595345903llu,
	1402982055436147llu, 1767646624268779llu, 2227095190691797llu,
	2805964110872297llu, 3535293248537579llu, 4454190381383713llu,
	5611928221744609llu, 7070586497075177llu, 8908380762767489llu,
	11223856443489329llu, 14141172994150357llu, 17816761525534927llu,
	22447712886978529llu, 28282345988300791llu, 35633523051069991llu,
	44895425773957261llu, 56564691976601587llu, 71267046102139967llu,
	89790851547914507llu, 113129383953203213llu, 142534092204280003llu,
	179581703095829107llu, 226258767906406483llu, 285068184408560057llu,
	359163406191658253llu, 452517535812813007llu, 570136368817120201llu,
	718326812383316683llu, 905035071625626043llu, 1140272737634240411llu,
	1436653624766633509llu, 1810070143251252131llu, 2280545475268481167llu,
	2873307249533267101llu, 3620140286502504283llu, 4561090950536962147llu,
	5746614499066534157llu, 7240280573005008577llu, 9122181901073924329llu,
	11493228998133068689llu, 14480561146010017169llu, 18446744073709551557llu
};


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
    std::optional<size_t> findAvailableSlot(const K& search_key) const
    {
        size_t i = HashFn(search_key);
        size_t initial_i = i;

        // search until we find a key or an empty slot. we may find neither, so save
        // the initial index in case we wrap around
        while (table[i].occupied && table[i].key != search_key || i != initial_i) {
            // Linear probing: move to the next slot
            i = (i + 1) % table.size();
        }

        if (!table[i].occupied || i == initial_i) {
            return i;
        }

        return std::nullopt;
    }

    // check if key exists
    std::optional<size_t> checkSlot(const K& search_key) const
    {
        size_t i = HashFn(search_key);

        if (!table[i].occupied) {
            return std::nullopt;
        }

        return i;
    }

    // for ensuring we have a prime number of slots when resizing
    size_t getNextPrime(size_t n)
    {
        auto lower_end = std::begin(prime_list) + 8;
        auto result = std::lower_bound(std::begin(prime_list), lower_end, n);
        if (result == lower_end)
            return *std::lower_bound(lower_end, std::end(prime_list), n);
        else
            return *result;
    }

    void resizeTable(size_t new_size)
    {
        size_t new_size_p = getNextPrime(new_size);

        std::vector<KeyValuePair> new_table(new_size_p, KeyValuePair { K {}, V {}, false });

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

    void checkAndResize()
    {
        double load_factor = static_cast<double>(table_size) / table.size();

        if (load_factor > max_load_factor) {
            resizeTable(table.size() * 2);
            //        } else if (load_factor < max_load_factor && table.size() > TABLE_SIZE_DEFAULT) {
            //            size_t new_size = std::max(TABLE_SIZE_DEFAULT, table.size() / 2);
            //
            //            if (new_size * max_load_factor >= table_size)
            //                resizeTable(table.size() / 2);
        }
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
        auto slot_index = findAvailableSlot(key_value.first);

        if (!slot_index.has_value()) {
            return { table.end(), false };
        }

        size_t index = slot_index.value();

        // if we've found a key that matches
        if (table[index].occupied) {
            return { table.begin() + index, false };
        }

        // if table is almost full, so calculate count / capacity
        checkAndResize();

        // set k-v and mark as occupied, returning an iterator to this position
        table[index] = { key_value.first, key_value.second, true };
        table_size++;
        return { table.begin() + index, true };
    }

    // erases elements
    bool erase(const K& key)
    {
        auto slot_index = findAvailableSlot(key);

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

        checkAndResize();

        return true;
    }

    // finds element with specific key
    typename std::vector<KeyValuePair>::iterator find(const K& key)
    {
        auto value = checkSlot(key);

        if (!value.has_value())
            return table.end();

        return table.begin() + value.value();
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
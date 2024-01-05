#include <benchmark/benchmark.h>
#include <boost/unordered_map.hpp>
#include <hashtable.h>
#include <iostream>
#include <memory>
#include <simplehashtable.h>
#include <unordered_map>
#include <absl/hash/hash.h>

// TODO we can make this cleaner, but not priority atm
// also its definitely suboptimal running each bench sequentially. more accurate to have 1 benchmark per process?

// for benchmarking we need to cover the cases:
//      Looking up an element that’s in the table
//      Looking up an element that can not be found in the table
//      Inserting a bunch of random numbers
//      Erasing elements
//
// we're testing against
//      optimized hash table
//      simple hash table
//      std::unordered_map
//      boost:unordered_map
//
//
// the bucket counts we'll use. since unordered_map uses a prime number count, we must do that
#define SMALL_BUCKET_COUNT (5)
#define MEDIUM_BUCKET_COUNT (1031)
// 10,000,019
#define LARGE_BUCKET_COUNT (10000019)

// the seed for random numbers
#define RANDOM_SEED (32)

// lookup an existing element in the hash table
template <typename MapType, typename KeyType, typename ValueType>
static void BM_LookupExistingElementInt(benchmark::State& state)
{
    KeyType key = 1;
    ValueType value = 42;

    size_t numElements = static_cast<size_t>(state.range(0));
    auto map = std::make_shared<MapType>(numElements);
    map->insert({ key, value });

    for (auto _ : state) {
        volatile auto result = map->find(key);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_LookupExistingElementInt<custom::SimpleHashTable<int, int>, int, int>)->Name("s LookupExistingElement SimpleHashTable")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementInt<custom::HashTable<int, int>, int, int>)->Name("s LookupExistingElement HashTable")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementInt<std::unordered_map<int, int>, int, int>)->Name("s LookupExistingElement std::unordered_map")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementInt<boost::unordered_map<int, int>, int, int>)->Name("s LookupExistingElement boost::unordered_map")->Arg(SMALL_BUCKET_COUNT);

BENCHMARK(BM_LookupExistingElementInt<custom::SimpleHashTable<int, int>, int, int>)->Name("m LookupExistingElement SimpleHashTable")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementInt<custom::HashTable<int, int>, int, int>)->Name("m LookupExistingElement HashTable")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementInt<std::unordered_map<int, int>, int, int>)->Name("m LookupExistingElement std::unordered_map")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementInt<boost::unordered_map<int, int>, int, int>)->Name("m LookupExistingElement boost::unordered_map")->Arg(MEDIUM_BUCKET_COUNT);

BENCHMARK(BM_LookupExistingElementInt<custom::SimpleHashTable<int, int>, int, int>)->Name("l LookupExistingElement SimpleHashTable")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementInt<custom::HashTable<int, int>, int, int>)->Name("l LookupExistingElement HashTable")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementInt<std::unordered_map<int, int>, int, int>)->Name("l LookupExistingElement std::unordered_map")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementInt<boost::unordered_map<int, int>, int, int>)->Name("l LookupExistingElement boost::unordered_map")->Arg(LARGE_BUCKET_COUNT);

template <typename MapType, typename KeyType, typename ValueType>
static void BM_LookupExistingElementString(benchmark::State& state)
{
    KeyType key = "alpha";
    ValueType value = 42;

    size_t numElements = static_cast<size_t>(state.range(0));
    auto map = std::make_shared<MapType>(numElements);
    map->insert({ key, value });

    for (auto _ : state) {
        volatile auto result = map->find(key);
        benchmark::DoNotOptimize(result);
    }
}


BENCHMARK(BM_LookupExistingElementString<custom::SimpleHashTable<string, int>, string, int>)->Name("s LookupExistingElementStr SimpleHashTable")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<custom::HashTable<string, int>, string, int>)->Name("s LookupExistingElementStr HashTable")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<custom::HashTable<string, int, absl::Hash<string>>, string, int>)->Name("s LookupExistingElementStrAbsl HashTable")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<std::unordered_map<string, int>, string, int>)->Name("s LookupExistingElementStr std::unordered_map")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<boost::unordered_map<string, int>, string, int>)->Name("s LookupExistingElementStr boost::unordered_map")->Arg(SMALL_BUCKET_COUNT);

BENCHMARK(BM_LookupExistingElementString<custom::SimpleHashTable<string, int>, string, int>)->Name("m LookupExistingElementStr SimpleHashTable")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<custom::HashTable<string, int>, string, int>)->Name("m LookupExistingElementStr HashTable")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<custom::HashTable<string, int, absl::Hash<string>>, string, int>)->Name("m LookupExistingElementStrAbsl HashTable")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<std::unordered_map<string, int>, string, int>)->Name("m LookupExistingElementStr std::unordered_map")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<boost::unordered_map<string, int>, string, int>)->Name("m LookupExistingElementStr boost::unordered_map")->Arg(MEDIUM_BUCKET_COUNT);

BENCHMARK(BM_LookupExistingElementString<custom::SimpleHashTable<string, int>, string, int>)->Name("l LookupExistingElementStr SimpleHashTable")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<custom::HashTable<string, int>, string, int>)->Name("l LookupExistingElementStr HashTable")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<custom::HashTable<string, int, absl::Hash<string>>, string, int>)->Name("l LookupExistingElementStrAbsl HashTable")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<std::unordered_map<string, int>, string, int>)->Name("l LookupExistingElementSTr std::unordered_map")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_LookupExistingElementString<boost::unordered_map<string, int>, string, int>)->Name("l LookupExistingElementStr boost::unordered_map")->Arg(LARGE_BUCKET_COUNT);




// lookup a non-existing element in the hash table
template <typename MapType, typename KeyType, typename ValueType>
static void BM_LookupNonexistentElement(benchmark::State& state)
{
    KeyType key = 1;

    size_t numElements = static_cast<size_t>(state.range(0));
    auto map = std::make_shared<MapType>(numElements);

    for (auto _ : state) {
        volatile auto result = map->find(key);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_LookupNonexistentElement<custom::SimpleHashTable<int, int>, int, int>)->Name("s LookupNonexistentElement SimpleHashTable")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_LookupNonexistentElement<custom::HashTable<int, int>, int, int>)->Name("s LookupNonexistentElement HashTable")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_LookupNonexistentElement<std::unordered_map<int, int>, int, int>)->Name("s LookupNonexistentElement std::unordered_map")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_LookupNonexistentElement<boost::unordered_map<int, int>, int, int>)->Name("s LookupNonexistentElement boost::unordered_map")->Arg(SMALL_BUCKET_COUNT);

BENCHMARK(BM_LookupNonexistentElement<custom::SimpleHashTable<int, int>, int, int>)->Name("m LookupNonexistentElement SimpleHashTable")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_LookupNonexistentElement<custom::HashTable<int, int>, int, int>)->Name("m LookupNonexistentElement HashTable")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_LookupNonexistentElement<std::unordered_map<int, int>, int, int>)->Name("m LookupNonexistentElement std::unordered_map")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_LookupNonexistentElement<boost::unordered_map<int, int>, int, int>)->Name("m LookupNonexistentElement boost::unordered_map")->Arg(MEDIUM_BUCKET_COUNT);

BENCHMARK(BM_LookupNonexistentElement<custom::SimpleHashTable<int, int>, int, int>)->Name("l LookupNonexistentElement SimpleHashTable")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_LookupNonexistentElement<custom::HashTable<int, int>, int, int>)->Name("l LookupNonexistentElement HashTable")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_LookupNonexistentElement<std::unordered_map<int, int>, int, int>)->Name("l LookupNonexistentElement std::unordered_map")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_LookupNonexistentElement<boost::unordered_map<int, int>, int, int>)->Name("l LookupNonexistentElement boost::unordered_map")->Arg(LARGE_BUCKET_COUNT);

// inserting some random numbers
template <typename MapType, typename KeyType, typename ValueType>
static void BM_InsertRandomNumbers(benchmark::State& state)
{
    size_t numElements = static_cast<size_t>(state.range(0));
    auto map = std::make_shared<MapType>(numElements);

    std::srand(RANDOM_SEED);
    KeyType key = std::rand();
    ValueType value = std::rand();

    for (auto _ : state) {
        auto result = map->insert({ key, value });
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_InsertRandomNumbers<custom::SimpleHashTable<int, int>, int, int>)->Name("s InsertRandomNumbers SimpleHashTable")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_InsertRandomNumbers<custom::HashTable<int, int>, int, int>)->Name("s InsertRandomNumbers HashTable")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_InsertRandomNumbers<std::unordered_map<int, int>, int, int>)->Name("s InsertRandomNumbers std::unordered_map")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_InsertRandomNumbers<boost::unordered_map<int, int>, int, int>)->Name("s InsertRandomNumbers boost::unordered_map")->Arg(SMALL_BUCKET_COUNT);

BENCHMARK(BM_InsertRandomNumbers<custom::SimpleHashTable<int, int>, int, int>)->Name("m InsertRandomNumbers SimpleHashTable")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_InsertRandomNumbers<custom::HashTable<int, int>, int, int>)->Name("m InsertRandomNumbers HashTable")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_InsertRandomNumbers<std::unordered_map<int, int>, int, int>)->Name("m InsertRandomNumbers std::unordered_map")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_InsertRandomNumbers<boost::unordered_map<int, int>, int, int>)->Name("m InsertRandomNumbers boost::unordered_map")->Arg(MEDIUM_BUCKET_COUNT);

BENCHMARK(BM_InsertRandomNumbers<custom::SimpleHashTable<int, int>, int, int>)->Name("l InsertRandomNumbers SimpleHashTable")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_InsertRandomNumbers<custom::HashTable<int, int>, int, int>)->Name("l InsertRandomNumbers HashTable")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_InsertRandomNumbers<std::unordered_map<int, int>, int, int>)->Name("l InsertRandomNumbers std::unordered_map")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_InsertRandomNumbers<boost::unordered_map<int, int>, int, int>)->Name("l InsertRandomNumbers boost::unordered_map")->Arg(LARGE_BUCKET_COUNT);

// erasing elements
template <typename MapType, typename KeyType, typename ValueType>
static void BM_EraseElements(benchmark::State& state)
{
    size_t numElements = static_cast<size_t>(state.range(0));
    auto map = std::make_shared<MapType>(numElements);
    KeyType key;
    ValueType value;

    std::srand(RANDOM_SEED);

    for (int i = 0; i < state.range(0); ++i) {
        key = std::rand();
        value = std::rand();
        map->insert({ key, value });
    }

    for (auto _ : state) {
        KeyType key = std::rand();
        volatile auto result = map->erase(key);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_EraseElements<custom::SimpleHashTable<int, int>, int, int>)->Name("s EraseElements SimpleHashTable")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_EraseElements<custom::HashTable<int, int>, int, int>)->Name("s EraseElements HashTable")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_EraseElements<std::unordered_map<int, int>, int, int>)->Name("s EraseElements std::unordered_map")->Arg(SMALL_BUCKET_COUNT);
BENCHMARK(BM_EraseElements<boost::unordered_map<int, int>, int, int>)->Name("s EraseElements boost::unordered_map")->Arg(SMALL_BUCKET_COUNT);

BENCHMARK(BM_EraseElements<custom::SimpleHashTable<int, int>, int, int>)->Name("m EraseElements SimpleHashTable")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_EraseElements<custom::HashTable<int, int>, int, int>)->Name("m EraseElements HashTable")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_EraseElements<std::unordered_map<int, int>, int, int>)->Name("m EraseElements std::unordered_map")->Arg(MEDIUM_BUCKET_COUNT);
BENCHMARK(BM_EraseElements<boost::unordered_map<int, int>, int, int>)->Name("m EraseElements boost::unordered_map")->Arg(MEDIUM_BUCKET_COUNT);

BENCHMARK(BM_EraseElements<custom::SimpleHashTable<int, int>, int, int>)->Name("l EraseElements SimpleHashTable")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_EraseElements<custom::HashTable<int, int>, int, int>)->Name("l EraseElements HashTable")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_EraseElements<std::unordered_map<int, int>, int, int>)->Name("l EraseElements std::unordered_map")->Arg(LARGE_BUCKET_COUNT);
BENCHMARK(BM_EraseElements<boost::unordered_map<int, int>, int, int>)->Name("l EraseElements boost::unordered_map")->Arg(LARGE_BUCKET_COUNT);

BENCHMARK_MAIN();
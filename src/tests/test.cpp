#include <absl/hash/hash.h>
#include <gtest/gtest.h>
#include <hashtable.h>

using namespace custom;

// TODO: add more comprehensive tests at some point

TEST(HashTableTest, InsertAndFindInt)
{
    HashTable<int, int> table;

    auto result = table.insert({ 42, 100 });
    ASSERT_TRUE(result.second);
    EXPECT_EQ(result.first->value, 100);

    auto found = table.find(42);
    ASSERT_NE(found, table.end());
    EXPECT_EQ(found->value, 100);

    result = table.insert({ 42, 200 });
    ASSERT_FALSE(result.second);
    EXPECT_EQ(result.first->value, 100);
}

TEST(HashTableTest, InsertAndFindString)
{
    HashTable<string, int> table;

    auto result = table.insert({ "alpha", 100 });
    ASSERT_TRUE(result.second);
    EXPECT_EQ(result.first->value, 100);

    auto found = table.find("alpha");
    ASSERT_NE(found, table.end());
    EXPECT_EQ(found->value, 100);

    result = table.insert({ "alpha", 200 });
    ASSERT_FALSE(result.second);
    EXPECT_EQ(result.first->value, 100);
}

TEST(HashTableTest, InsertAndFindStringHash)
{
    HashTable<string, int, absl::Hash<string>> table;

    auto result = table.insert({ "alpha", 100 });
    ASSERT_TRUE(result.second);
    EXPECT_EQ(result.first->value, 100);

    auto found = table.find("alpha");
    ASSERT_NE(found, table.end());
    EXPECT_EQ(found->value, 100);

    result = table.insert({ "alpha", 200 });
    ASSERT_FALSE(result.second);
    EXPECT_EQ(result.first->value, 100);
}

TEST(HashTableTest, EraseInt)
{
    HashTable<int, int> table;

    EXPECT_FALSE(table.erase(42));

    table.insert({ 42, 100 });
    EXPECT_TRUE(table.erase(42));
    EXPECT_EQ(table.find(42), table.end());
}

TEST(HashTableTest, EraseString)
{
    HashTable<string, int> table;

    EXPECT_FALSE(table.erase("bravo"));

    table.insert({ "bravo", 100 });
    EXPECT_TRUE(table.erase("bravo"));
    EXPECT_EQ(table.find("bravo"), table.end());
}

TEST(HashTableTest, ClearInt)
{
    HashTable<int, int> table;

    table.clear();
    EXPECT_EQ(table.size(), 0);

    table.insert({ 42, 100 });
    table.insert({ 43, 200 });
    table.clear();
    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.find(42), table.end());
    EXPECT_EQ(table.find(43), table.end());
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
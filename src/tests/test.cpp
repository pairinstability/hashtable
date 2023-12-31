#include <gtest/gtest.h>
#include <hashtable.h>

using namespace custom;

TEST(HashTableTest, InsertAndFind)
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

TEST(HashTableTest, Erase)
{
    HashTable<int, int> table;

    EXPECT_FALSE(table.erase(42));

    table.insert({ 42, 100 });
    EXPECT_TRUE(table.erase(42));
    EXPECT_EQ(table.find(42), table.end());
}

TEST(HashTableTest, Clear)
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

TEST(runtests, test)
{
    auto ht = custom::HashTable<int, int>(100);

    int key = 1;
    int value = 1;
    auto ret = ht.insert({ key, value });
    auto val = ht.find(key);

    EXPECT_EQ(1, val->value);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
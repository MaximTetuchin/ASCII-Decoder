#include "ascii85.h"
#include <gtest/gtest.h>

TEST(ASCII85Test, EncodeBasic) {
    std::vector<unsigned char> input = {'H', 'e', 'l', 'l', 'o'};
    EXPECT_EQ(ASCII85::encode(input), "<~87cURDZ~>");
}
TEST(ASCII85Test, EncodeZeroBytes) {
    std::vector<unsigned char> input(4, 0);
    EXPECT_EQ(ASCII85::encode(input), "<~z~>");
}

TEST(ASCII85Test, DecodeBasic) {
    std::string input = "<~87cURDZ~>";
    auto decoded = ASCII85::decode(input);
    std::vector<unsigned char> expected = {'H', 'e', 'l', 'l', 'o'};
    EXPECT_EQ(decoded, expected);
}

TEST(ASCII85Test, DecodeZeroBytes) {
    auto decoded = ASCII85::decode("z");
    std::vector<unsigned char> expected(4, 0);
    EXPECT_EQ(decoded, expected);
}

TEST(ASCII85Test, InvalidCharacter) {
    EXPECT_THROW(ASCII85::decode("87cURD|"), std::out_of_range);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
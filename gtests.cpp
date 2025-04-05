#include "ascii85.h"
#include <gtest/gtest.h>

TEST(ASCII85Test, EncodeBasicAdobe) {
    std::vector<unsigned char> input = {'H', 'e', 'l', 'l', 'o'};
    EXPECT_EQ(ASCII85::encode(input, true), "<~87cURDZ~>");
}

TEST(ASCII85Test, EncodeBasicStandard) {
    std::vector<unsigned char> input = {'H', 'e', 'l', 'l', 'o'};
    EXPECT_EQ(ASCII85::encode(input, false), "87cURDZ");
}

TEST(ASCII85Test, EncodeZeroBytesAdobe) {
    std::vector<unsigned char> input(4, 0);
    EXPECT_EQ(ASCII85::encode(input, true), "<~z~>");
}

TEST(ASCII85Test, EncodeZeroBytesStandard) {
    std::vector<unsigned char> input(4, 0);
    EXPECT_EQ(ASCII85::encode(input, false), "z");
}

TEST(ASCII85Test, DecodeBasicAdobe) {
    std::string input = "<~87cURDZ~>";
    auto decoded = ASCII85::decode(input, true);
    std::vector<unsigned char> expected = {'H', 'e', 'l', 'l', 'o'};
    EXPECT_EQ(decoded, expected);
}

TEST(ASCII85Test, DecodeBasicStandard) {
    std::string input = "87cURDZ";
    auto decoded = ASCII85::decode(input, false);
    std::vector<unsigned char> expected = {'H', 'e', 'l', 'l', 'o'};
    EXPECT_EQ(decoded, expected);
}

TEST(ASCII85Test, DecodeZeroBytesAdobe) {
    auto decoded = ASCII85::decode("<~z~>", true);
    std::vector<unsigned char> expected(4, 0);
    EXPECT_EQ(decoded, expected);
}

TEST(ASCII85Test, DecodeZeroBytesStandard) {
    auto decoded = ASCII85::decode("z", false);
    std::vector<unsigned char> expected(4, 0);
    EXPECT_EQ(decoded, expected);
}

TEST(ASCII85Test, InvalidCharacterStandard) {
    EXPECT_THROW(ASCII85::decode("87cURD|", false), std::out_of_range);
}

TEST(ASCII85Test, InvalidAdobeWrapper) {
    // Тест на частичные обёртки
    EXPECT_NO_THROW(ASCII85::decode("<~87cURDZ", true));
    EXPECT_NO_THROW(ASCII85::decode("87cURDZ~>", true));
}

TEST(ASCII85Test, PaddingHandling) {
    // Тест на усечение padding
    std::vector<unsigned char> input = {'H', 'e'};
    std::string encoded = ASCII85::encode(input, false);
    auto decoded = ASCII85::decode(encoded, false);
    EXPECT_EQ(decoded, input);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include <gtest/gtest.h>
#include "../../main/util/string-output-stream.hpp"

namespace lang {
    namespace util {
        struct StringOutputStreamTest : ::testing::Test {
        };

        TEST_F(StringOutputStreamTest, empty_stream) {
            StringOutputStream stream("");

            char dummy;
            EXPECT_FALSE(stream.take(dummy));
        }

        TEST_F(StringOutputStreamTest, test_with_data) {
            StringOutputStream stream("abc");

            char content;

            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('a', content);

            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('b', content);

            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('c', content);

            EXPECT_FALSE(stream.take(content));
        }
    }
}
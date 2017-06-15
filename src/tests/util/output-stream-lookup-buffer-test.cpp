#include <gtest/gtest.h>
#include "../../main/util/output-stream-lookup-buffer.hpp"
#include "../../main/util/string-output-stream.hpp"

namespace lang {
    namespace util {
        struct OutputStreamLookupBufferTest : ::testing::Test {
        };

        TEST_F(OutputStreamLookupBufferTest, empty_stream) {
            StringOutputStream internalStream("");
            OutputStreamLookupBuffer<char> stream(internalStream);

            char dummy;

            EXPECT_FALSE(stream.take(dummy));
            EXPECT_FALSE(stream.lookup(1, dummy));
            EXPECT_FALSE(stream.lookup(2, dummy));
        }

        TEST_F(OutputStreamLookupBufferTest, can_work_like_normal_output_stream) {
            StringOutputStream internalStream("abc");
            OutputStreamLookupBuffer<char> stream(internalStream);

            char content;

            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('a', content);

            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('b', content);

            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('c', content);
        }

        TEST_F(OutputStreamLookupBufferTest, correctly_handles_lookup) {
            StringOutputStream internalStream("abcd");
            OutputStreamLookupBuffer<char> stream(internalStream);

            char content;

            EXPECT_TRUE(stream.lookup(1, content));
            EXPECT_EQ('a', content);

            EXPECT_TRUE(stream.lookup(2, content));
            EXPECT_EQ('b', content);

            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('a', content);

            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('b', content);

            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('c', content);

            EXPECT_TRUE(stream.lookup(1, content));
            EXPECT_EQ('d', content);

            EXPECT_FALSE(stream.lookup(2, content));

            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('d', content);

            EXPECT_FALSE(stream.take(content));
            EXPECT_FALSE(stream.lookup(1, content));
        }

        TEST_F(OutputStreamLookupBufferTest, has_next_test) {
            StringOutputStream internalStream("abc");
            OutputStreamLookupBuffer<char> stream(internalStream);

            char content;

            EXPECT_TRUE(stream.hasNext());
            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('a', content);

            EXPECT_TRUE(stream.hasNext());
            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('b', content);

            EXPECT_TRUE(stream.hasNext());
            EXPECT_TRUE(stream.take(content));
            EXPECT_EQ('c', content);

            EXPECT_FALSE(stream.hasNext());
        }
    }
}
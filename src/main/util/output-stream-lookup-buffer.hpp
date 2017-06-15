#ifndef UTIL_OUTPUT_STREAM_LOOKUP_BUFFER_HPP
#define UTIL_OUTPUT_STREAM_LOOKUP_BUFFER_HPP

#include "output-stream.hpp"
#include <deque>
#include <cassert>

namespace lang {
    namespace util {
        template<typename T>
        class OutputStreamLookupBuffer {
            OutputStream <T> &stream;
            std::deque<T> bufferedItems;

        public:
            explicit OutputStreamLookupBuffer(OutputStream <T> &stream) : stream(stream) {
            }

            bool lookup(std::size_t index, T &outItem) {
                assert(index > 0);

                T item;
                for (std::size_t i = bufferedItems.size(); i < index; ++i) {
                    if (!stream.take(item)) {
                        return false;
                    }
                    bufferedItems.emplace_back(std::move(item));
                }
                outItem = bufferedItems[index - 1];
                return true;
            }

            bool take(T &outItem) {
                if (lookup(1, outItem)) {
                    bufferedItems.erase(bufferedItems.begin());
                    return true;
                }
                return false;
            }

            bool hasNext() {
                T dummy;
                return lookup(1, dummy);
            }
        };
    }
}

#endif // UTIL_OUTPUT_STREAM_LOOKUP_BUFFER_HPP

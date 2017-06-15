#ifndef UTIL_STREAM_HPP
#define UTIL_STREAM_HPP

namespace lang {
    namespace util {
        template<typename Item>
        struct OutputStream {
            virtual ~OutputStream() {}

            virtual bool take(Item &out) = 0;
        };
    }
}

#endif // UTIL_STREAM_HPP

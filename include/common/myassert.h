#ifndef CMINUSF_MYASSERT_H
#define CMINUSF_MYASSERT_H

#define MY_ASSERT(expr) \
    do { \
        if (!(expr)) { \
            std::cerr << "Assertion failed: " << #expr << " in " << __FILE__ \
                      << " at line " << __LINE__ << std::endl; \
            std::abort(); \
        } \
    } while (false)

#endif //CMINUSF_MYASSERT_H

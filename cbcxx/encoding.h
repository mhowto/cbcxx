#ifndef CBCXX_ENCODING_H
#define CBCXX_ENCODING_H

namespace cbcxx { namespace scanner {
    enum Encoding {
        NONE,
        CHAR16,
        CHAR32,
        UTF8,
        WCHAR
    };
} }

#endif
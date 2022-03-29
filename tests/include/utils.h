//
// Created by badcw on 2022/3/30.
//

#ifndef CLIBS_TESTS_INCLUDE_UTILS_H
#define CLIBS_TESTS_INCLUDE_UTILS_H

#include <climits>
#include <stdtypes.h>
#include <string>

char base64_code[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="};

std::string convert_uint64_2_string(const std::vector<u_longlong_t>& x) {
    size_t      len = x.size();
    std::string str;
    for (size_t i = 0; i < len; ++i) {
        char* tmp = (char*)&x[i];
        for (size_t pos = 0; pos < 4; ++pos) { str.push_back(base64_code[*(tmp + pos) & 63]); }
    }
    return str;
}

template <typename T1, typename T2>
inline size_t offset_of(T1 T2::*member) {
    static T2 obj;
    return size_t(&(obj.*member)) - size_t(&obj);
}

#endif  // CLIBS_TESTS_INCLUDE_UTILS_H

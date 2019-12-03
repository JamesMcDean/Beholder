//
// Created by James on 12/3/2019.
//

#include <cstdint>
#include <ctime>
#include <cmath>

namespace Control {
    auto shittyHashToken() -> int32_t {
        return ((clock() * 29) % (int32_t) (pow(2, 32) - 1)) + 1;
    }
}

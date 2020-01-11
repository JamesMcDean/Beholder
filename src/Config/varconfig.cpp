//
// Created by james on 1/8/20.
//

namespace Config {
    auto __verboseConf(bool state, bool set) -> bool {
        static bool verbose = false;
        if (set) verbose = state;
        return verbose;
    }

    auto verbose() -> bool {
        return __verboseConf(false, false);
    }

    auto verbose(bool state) -> void {
        __verboseConf(state, true);
    }
}

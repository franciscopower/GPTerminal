#pragma once

template<typename T, typename E>
struct Result {
    T value;
    E error;
    bool is_ok;

    static Result Ok(const T& val) {
        return { val, E(), true };
    }

    static Result Err(const E& err) {
        return { T(), err, false };
    }
};

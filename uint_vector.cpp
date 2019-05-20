#include "uint_vector.h"

uint_vector::uint_vector() : data_(std::array<uint32_t, 4>()) {
    std::get<0>(data_)[3] = 0;
}

uint_vector::uint_vector(uint_vector const &rhs) {
    data_ = rhs.data_;
}

uint_vector::uint_vector(size_t count, uint32_t value) {
    if (count > 3) {
        data_ = std::make_shared<std::vector<uint32_t>>(count, value);
    } else {
        data_ = std::array<uint32_t, 4>();
        std::get<0>(data_).fill(value);
        std::get<0>(data_)[3] = static_cast<uint32_t>(count);
    }
}

void uint_vector::push_back(uint32_t value) {
    if (data_.index() == 0 && std::get<0>(data_)[3] == 3) {
        data_ = std::make_shared<std::vector<uint32_t>>(std::get<0>(data_).begin(), std::get<0>(data_).end() - 1);
    }
    if (data_.index() == 0) {
        std::get<0>(data_)[std::get<0>(data_)[3]++] = value;
    } else {
        if (!std::get<1>(data_).unique()) {
            std::get<1>(data_) = std::make_shared<std::vector<uint32_t>>(std::get<1>(data_)->begin(), std::get<1>(data_)->end());
        }
        std::get<1>(data_)->push_back(value);
    }
}

void uint_vector::pop_back() {
    if (data_.index() == 0) {
        std::get<0>(data_)[3]--;
    } else {
        if (!std::get<1>(data_).unique()) {
            std::get<1>(data_) = std::make_shared<std::vector<uint32_t>>(std::get<1>(data_)->begin(), std::get<1>(data_)->end());
        }
        std::get<1>(data_)->pop_back();
    }
}

uint32_t *uint_vector::data() {
    if (data_.index() == 0) {
        return std::get<0>(data_).data();
    }
    if (!std::get<1>(data_).unique()) {
        std::get<1>(data_) = std::make_shared<std::vector<uint32_t>>(std::get<1>(data_)->begin(), std::get<1>(data_)->end());
    }
    return std::get<1>(data_)->data();
}

uint32_t const *const uint_vector::data() const {
    if (data_.index() == 0) {
        return std::get<0>(data_).data();
    }
    return std::get<1>(data_)->data();
}

size_t uint_vector::size() const noexcept {
    if (data_.index() == 0) {
        return std::get<0>(data_)[3];
    }
    return std::get<1>(data_)->size();
}

uint32_t const &uint_vector::operator[](uint32_t i) const {
    if (data_.index() == 0) {
        return std::get<0>(data_)[i];
    }
    return std::get<1>(data_)->operator[](i);
}

uint32_t &uint_vector::operator[](uint32_t i) {
    if (data_.index() == 0) {
        return std::get<0>(data_)[i];
    }
    if (!std::get<1>(data_).unique()) {
        data_ = std::make_shared<std::vector<uint32_t >>(std::get<1>(data_)->begin(), std::get<1>(data_)->end());
    }
    return std::get<1>(data_)->operator[](i);
}

void uint_vector::begin_erase(size_t count) {
    if (data_.index() == 0) {
        if (count >= std::get<0>(data_)[3]) {
            std::get<0>(data_)[3] = 0;
        } else {
            for (size_t i = count; i < std::get<0>(data_)[3]; i++) {
                std::get<0>(data_)[i - count] = std::get<0>(data_)[i];
            }
            std::get<0>(data_)[3] -= count;
        }
    } else {
        if (!std::get<1>(data_).unique()) {
            std::get<1>(data_) = std::make_shared<std::vector<uint32_t>>(std::get<1>(data_)->begin() + count, std::get<1>(data_)->end());
        } else {
            std::get<1>(data_)->erase(std::get<1>(data_)->begin(), std::get<1>(data_)->begin() + count);
        }
    }
}

void uint_vector::begin_insert(size_t count, uint32_t val) {
    if (data_.index() == 0 && count + std::get<0>(data_)[3] > 3) {
        data_ = std::make_shared<std::vector<uint32_t>>(std::get<0>(data_).begin(), std::get<0>(data_).begin() + std::get<0>(data_)[3]);
    }
    if (data_.index() == 0) {
        for (size_t i = count + std::get<0>(data_)[3]; i-- != count;) {
            std::get<0>(data_)[i] = std::get<0>(data_)[i - count];
        }
        for (size_t i = 0; i < count; i++) {
            std::get<0>(data_)[i] = val;
        }
        std::get<0>(data_)[3] += count;
    } else {
        if (!std::get<1>(data_).unique()) {
            std::get<1>(data_) = std::make_shared<std::vector<uint32_t>>(std::get<1>(data_)->begin(), std::get<1>(data_)->end());
        }
        std::get<1>(data_)->insert(std::get<1>(data_)->begin(), count, val);
    }
}

void uint_vector::assign(size_t count, uint32_t value) {
    if (count > 3) {
        if (data_.index() == 0 || !std::get<1>(data_).unique()) {
            data_ = std::make_shared<std::vector<uint32_t>>(count, value);
        } else {
            std::get<1>(data_)->assign(count, value);
        }
    } else {
        if (data_.index() == 1 && std::get<1>(data_).unique()) {
            std::get<1>(data_)->assign(count, value);
        } else {
            if (data_.index() == 1) {
                data_ = std::array<uint32_t, 4>();
            }
            std::get<0>(data_).fill(value);
            std::get<0>(data_)[3] = static_cast<uint32_t>(count);
        }
    }
}

bool operator==(uint_vector const &lhs, uint_vector const &rhs) {
    if (lhs.data_.index() == 1 && rhs.data_.index() == 1) {
        return std::get<1>(lhs.data_).get() == std::get<1>(rhs.data_).get() || *std::get<1>(lhs.data_) == *std::get<1>(rhs.data_);
    }
    if (lhs.size() != rhs.size()) {
        return false;
    }
    auto lhs_data = lhs.data();
    auto rhs_data = rhs.data();
    auto size = lhs.size();
    for (size_t i = 0; i < size; i++) {
        if (lhs_data[i] != rhs_data[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(uint_vector const &lhs, uint_vector const &rhs) {
    return !(lhs == rhs);
}



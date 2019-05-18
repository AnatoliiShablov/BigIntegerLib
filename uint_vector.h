#ifndef BIGINT_UINT_VECTOR_H
#define BIGINT_UINT_VECTOR_H

#include <variant>
#include <vector>
#include <array>
#include <memory>

class uint_vector {
    std::variant<std::array<uint32_t, 4>, std::shared_ptr<std::vector<uint32_t>>> data_;

 public:
    uint_vector();

    uint_vector(uint_vector const &rhs);

    uint_vector(size_t count, uint32_t value);

    uint_vector &operator=(uint_vector const &rhs) = default;

    void push_back(uint32_t value);

    void pop_back();

    size_t size() const noexcept;

    uint32_t const &operator[](uint32_t i) const;

    uint32_t &operator[](uint32_t i);

    void begin_erase(size_t count);

    void begin_insert(size_t count, uint32_t val);

    void assign(size_t count, uint32_t value);

    friend bool operator==(uint_vector const &lhs, uint_vector const &rhs);

    friend bool operator!=(uint_vector const &lhs, uint_vector const &rhs);
};

#endif //BIGINT_UINT_VECTOR_H


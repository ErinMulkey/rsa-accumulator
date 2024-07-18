#include "accumulator.hpp"

#include <vector>
#include <numeric>
#include <optional>
#include <random>

#define DEFAULT_UPPER_BOUND 1000

namespace {
    /**
     * @brief Get the primes up to `upper_bound`.
     * @param upper_bound Largest possible prime value in the result.
     * @returns Primes up to `upper_bound`.
     */
    auto generate_primes(const std::uint64_t upper_bound) -> std::vector<std::uint64_t> {
        // Singleton primes vector.
        static auto primes = std::optional<std::vector<std::uint64_t>>{};

        if (!primes.has_value()) {
            // Sieve of Eratosthenes.

            primes.emplace();

            auto is_prime = std::vector<bool>(upper_bound, true);

            for (auto number = std::size_t{ 2 }; number < upper_bound; ++number) {
                if (!is_prime[number]) {
                    continue;
                }

                primes.value().push_back(number);

                for (auto multiple = number << 1; multiple < upper_bound; multiple += number) {
                    is_prime[multiple] = false;
                }
            }
        }

        return primes.value();
    }

    auto invertible_mod_n(const std::uint64_t number, const std::uint64_t n) -> bool {
        return std::gcd(number, n) == 1;
    }

    auto generate_invertible_group(const std::uint64_t n) -> std::vector<std::uint64_t> {
        auto invertible_group = std::vector<std::uint64_t>{};

        for (auto number = std::uint64_t{ 1 }; number < n; ++number) {
            if (invertible_mod_n(number, n)) {
                invertible_group.push_back(number);
            }
        }

        return invertible_group;
    }

    auto generate_key(const std::uint64_t upper_bound = DEFAULT_UPPER_BOUND) -> std::uint64_t {
        const auto primes = generate_primes(upper_bound);
        const auto num_primes = static_cast<int>(primes.size());

        auto rd = std::random_device{};
        auto gen = std::mt19937{ rd() };
        auto distr = std::uniform_int_distribution<>{ num_primes >> 1, num_primes };

        const std::uint64_t prime1 = primes[distr(gen)];
        const std::uint64_t prime2 = primes[distr(gen)];

        return prime1 * prime2;
    }

    auto mod_pow(std::uint64_t base, std::uint64_t power, const std::uint64_t modulus) -> std::uint64_t {
        std::uint64_t res = 1;

        while (power != 0) {
            if (power & 1) {
                res *= base;
                res %= modulus;
            }

            power >>= 1;
            base *= base;
            base %= modulus;
        }

        return res;
    }

    auto calculate_set_prod_power(const std::vector<std::uint64_t>& set, const std::uint64_t base, const std::uint64_t key) {
        std::uint64_t res = base;

        for (const auto element : set) {
            res = mod_pow(res, element, key);
        }

        return res;
    }
}

accumulator::accumulator(const std::vector<std::uint64_t>& set) {
    key = generate_key();
    const auto invertible_group = generate_invertible_group(key);
    auto rd = std::random_device{};
    auto gen = std::mt19937{ rd() };
    auto distr = std::uniform_int_distribution<>{ 0, static_cast<int>(invertible_group.size()) - 1 };
    initial_accumulator = invertible_group[distr(gen)];
    value = generate_value(set);
}

auto accumulator::get_key() const noexcept -> std::uint64_t {
    return key;
}

auto accumulator::get_initial_accumulator() const noexcept -> std::uint64_t {
    return initial_accumulator;
}

auto accumulator::get_value() const noexcept -> std::uint64_t {
    return value;
}

auto accumulator::update(const std::vector<std::uint64_t>& set) -> void {
    value = generate_value(set);
}

auto accumulator::generate_value(const std::vector<std::uint64_t>& set) -> std::uint64_t {
    return calculate_set_prod_power(set, initial_accumulator, key);
}

auto create_accumulator(const std::vector<std::uint64_t>& set) -> accumulator {
    return accumulator(set);
}

auto gen_witness(const std::vector<std::uint64_t>& set, const std::uint64_t element, const accumulator& acc) -> std::uint64_t {
    auto set_without_element = std::vector<std::uint64_t>{};

    for (const auto set_element : set) {
        if (set_element != element) {
            set_without_element.push_back(set_element);
        }
    }

    return calculate_set_prod_power(set_without_element, acc.get_initial_accumulator(), acc.get_key());
}

auto verify_witness(const std::uint64_t element, const accumulator& acc, const std::uint64_t witness) -> bool {
    return (mod_pow(witness, element, acc.get_key()) == acc.get_value());
}

auto add_element(std::vector<std::uint64_t>& set, const std::uint64_t element, accumulator& acc) -> accumulator {
    set.push_back(element);
    acc.update(set);
    return acc;
}

auto update_witness(std::vector<std::uint64_t>& set, const std::uint64_t element, accumulator& acc) -> std::unordered_map<std::uint64_t, std::uint64_t> {
    add_element(set, element, acc);

    auto witness_map = std::unordered_map<std::uint64_t, std::uint64_t>{};

    for (const auto element : set) {
        witness_map[element] = gen_witness(set, element, acc);
    }

    return witness_map;
}

auto batch_update(std::vector<std::uint64_t>& set, const std::vector<std::uint64_t>& elements, accumulator& acc) -> std::unordered_map<std::uint64_t, std::uint64_t> {
    for (const auto element : elements) {
        add_element(set, element, acc);
    }

    auto witness_map = std::unordered_map<std::uint64_t, std::uint64_t>{};

    for (const auto element : set) {
        witness_map[element] = gen_witness(set, element, acc);
    }

    return witness_map;
}
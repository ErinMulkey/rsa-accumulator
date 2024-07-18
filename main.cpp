#include "accumulator.hpp"

#include <iostream>
#include <sstream>
#include <random>
#include <unordered_set>

namespace {
    auto print_set(const std::vector<std::uint64_t>& set) -> void {
        std::cout << "{";

        for (const auto element : set) {
            std::cout << " " << element;
        }

        std::cout << " }";
    }

    auto scan_numbers() -> std::vector<std::uint64_t> {
        auto numbers = std::vector<std::uint64_t>{};
        auto input_line = std::string{};
        std::getline(std::cin, input_line);
        auto input_stream = std::istringstream{ input_line };
        std::uint64_t value;

        while (input_stream >> value) {
            numbers.push_back(value);
        }

        return numbers;
    }

    auto get_random(std::vector<std::uint64_t>& set) -> std::uint64_t {
        auto rd = std::random_device{};
        auto gen = std::mt19937{ rd() };
        auto distr = std::uniform_int_distribution<>{ 0, static_cast<int>(set.size()) - 1 };
        return set[distr(gen)];
    }
}

int main(int argc, char const* argv[]) {
    std::cout << "Input a sequence of positive, space-seperated integers to act as the set: ";
    auto set = scan_numbers();

    std::cout << "Set is ";
    print_set(set);
    std::cout << std::endl;

    auto acc = create_accumulator(set);
    const auto element = get_random(set);
    const auto witness = gen_witness(set, element, acc);
    const auto is_witness = verify_witness(element, acc, witness);

    std::cout << "Accumulator value "
        << acc.get_value()
        << " generated from key "
        << acc.get_key()
        << " and initial accumulator "
        << acc.get_initial_accumulator()
        << ".\nPicked random element "
        << element
        << " and generated witness "
        << witness
        << " for which `verify_witness` returns "
        << std::boolalpha << is_witness
        << "."
        << std::endl;

    std::cout << "\nInput a single positive integer to be added to the set: ";
    auto new_element_for_add = std::uint64_t{};
    std::cin >> new_element_for_add;
    std::cin.ignore();
    add_element(set, new_element_for_add, acc);

    std::cout << "Set is now ";
    print_set(set);
    std::cout << std::endl;

    std::cout << "Accumulator value is now "
        << acc.get_value()
        << ", generated from key "
        << acc.get_key()
        << " and initial accumulator "
        << acc.get_initial_accumulator()
        << std::endl;
    
    std::cout << "\nInput another single positive integer to be added to the set: ";
    auto new_element_for_update = std::uint64_t{};
    std::cin >> new_element_for_update;
    std::cin.ignore();
    auto witness_pairs_from_update = update_witness(set, new_element_for_update, acc);

    std::cout << "Set is now ";
    print_set(set);
    std::cout << std::endl;

    std::cout << "Accumulator value is now "
        << acc.get_value()
        << ", generated from key "
        << acc.get_key()
        << " and initial accumulator "
        << acc.get_initial_accumulator()
        << ". Witnesses for each value are:"
        << std::endl;
    
    for (const auto& witness_pair : witness_pairs_from_update) {
        const auto element = witness_pair.first;
        const auto witness = witness_pair.second;
        std::cout << "| " 
            << element
            << ": "
            << witness
            << " (`verify_witness` returns "
            << std::boolalpha << verify_witness(element, acc, witness)
            << ")"
            << std::endl;
    }

    std::cout << "\nInput a sequence of positive, space-seperated integers to be added to the set: ";
    auto new_elements = scan_numbers();
    auto witness_pairs_from_batch = batch_update(set, new_elements, acc);

    std::cout << "Set is now ";
    print_set(set);
    std::cout << std::endl;

    std::cout << "Accumulator value is now "
        << acc.get_value()
        << ", generated from key "
        << acc.get_key()
        << " and initial accumulator "
        << acc.get_initial_accumulator()
        << ". Witnesses for each value are:"
        << std::endl;
    
    for (const auto& witness_pair : witness_pairs_from_batch) {
        const auto element = witness_pair.first;
        const auto witness = witness_pair.second;
        std::cout << "| " 
            << element
            << ": "
            << witness
            << " (`verify_witness` returns "
            << std::boolalpha << verify_witness(element, acc, witness)
            << ")"
            << std::endl;
    }

    return 0;
}

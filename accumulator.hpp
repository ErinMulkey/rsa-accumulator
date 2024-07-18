#ifndef RSA_ACCUMULATOR
#define RSA_ACCUMULATOR

#include <vector>
#include <unordered_map>
#include <cstdint>

class accumulator
{
public:
    accumulator(const std::vector<std::uint64_t> &set);

    auto get_key() const noexcept -> std::uint64_t;
    auto get_initial_accumulator() const noexcept -> std::uint64_t;
    auto get_value() const noexcept -> std::uint64_t;
    auto update(const std::vector<std::uint64_t> &set) -> void;

private:
    std::uint64_t key;
    std::uint64_t initial_accumulator;
    std::uint64_t value;

    auto generate_value(const std::vector<std::uint64_t> &set) -> std::uint64_t;
};

/**
 * @brief Creates an accumulator.
 * @param set Set of elements for the accumulator to be created from.
 * @returns Accumulator generated from `set`.
 */
auto create_accumulator(const std::vector<std::uint64_t> &set) -> accumulator;

/**
 * @brief Generate a witness.
 * @param set Set from which `accumulator` was generated.
 * @param element Element for which the witness will be generated for.
 * @param accumulator Accumulator for `set`.
 * @returns Witness for `element`.
 */
auto gen_witness(const std::vector<std::uint64_t> &set, const std::uint64_t element, const accumulator &acc) -> std::uint64_t;

/**
 * @brief Verify that a witness is correct.
 * @param element Element whose membership is to be confirmed.
 * @param acc Accumulator of the membership set.
 * @param witness Proposed witness for `element`.
 * @returns Whether `witness` was the correct witness for `element`.
 */
auto verify_witness(const std::uint64_t element, const accumulator &acc, const std::uint64_t witness) -> bool;

/**
 * @brief Find a new accumulator given a new element.
 * @param set Membership set. Will be modified.
 * @param element Element to be added to `set`.
 * @param acc Accumulator for `set`. Will be modified.
 * @returns New accumulator for `set`.
 */
auto add_element(std::vector<std::uint64_t> &set, const std::uint64_t element, accumulator &acc) -> accumulator;

/**
 * @brief Update witnesses after the insertion of a new element.
 * @param set Set to be added to. Will be modified.
 * @param element Element to be added.
 * @param acc Accumulator of `set`. Will be modified.
 * @returns Map of witnesses for each element in `set` after the
 * insertion of `element`.
 */
auto update_witness(std::vector<std::uint64_t> &set, const std::uint64_t element, accumulator &acc) -> std::unordered_map<std::uint64_t, std::uint64_t>;

/**
 * @brief Update witnesses after the insertion of many new elements.
 * @param set Set to be added to. Will be modified.
 * @param elements Elements to be added.
 * @param acc Accumulator of `set`. Will be modified.
 * @returns Map of witnesses for each element in `set` after the
 * insertion of `elements`.
 */
auto batch_update(std::vector<std::uint64_t> &set, const std::vector<std::uint64_t> &elements, accumulator &acc) -> std::unordered_map<std::uint64_t, std::uint64_t>;

#endif
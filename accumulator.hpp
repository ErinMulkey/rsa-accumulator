#ifndef RSA_ACCUMULATOR
#define RSA_ACCUMULATOR

#include <set>
#include <map>

/**
 * @brief Creates an accumulator.
 * @param set Set of elements for the accumulator to be created from.
 * @returns Accumulator generated from `set`.
 */
auto create_accumulator(std::set<int> &set) -> int;

/**
 * @brief Generate a witness.
 * @param set Set from which `accumulator` was generated.
 * @param element Element for which the witness will be generated for.
 * @param accumulator Accumulator for `set`.
 * @returns Witness for `element`.
 */
auto gen_witness(std::set<int> &set, int element, int accumulator) -> int;

/**
 * @brief Verify that a witness is correct.
 * @param element Element whose membership is to be confirmed.
 * @param accumulator Accumulator of the membership set.
 * @param witness Proposed witness for `element`.
 * @returns Whether `witness` was the correct witness for `element`.
 */
auto verify_witness(int element, int accumulator, int witness) -> bool;

/**
 * @brief Find a new accumulator given a new element.
 * @param set Membership set. Will be modified.
 * @param element Element to be added to `set`.
 * @param accumulator Old ccumulator for `set`.
 * @returns New accumulator for `set`.
 */
auto add_element(std::set<int> &set, int element, int accumulator) -> int;

/**
 * @brief Update witnesses after the insertion of a new element.
 * @param set Set to be added to. Will be modified.
 * @param element Element to be added.
 * @param accumulator Old accumulator of `set`.
 * @returns Map of witnesses for each element in `set` after the
 * insertion of `element`.
 */
auto update_witness(std::set<int> &set, int element, int accumulator) -> std::map<int, int>;

/**
 * @brief Update witnesses after the insertion of many new elements.
 * @param set Set to be added to. Will be modified.
 * @param elements Elements to be added.
 * @param accumulator Old accumulator of `set`.
 * @returns Map of witnesses for each element in `set` after the
 * insertion of `elements`.
 */
auto batch_update(std::set<int> &set, std::set<int> &elements, int accumulator) -> std::map<int, int>;

#endif
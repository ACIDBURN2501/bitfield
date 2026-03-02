/** @file bitfield.h
 * @brief Portable bit manipulation library for embedded systems
 *
 * This library provides deterministic bit manipulation operations
 * suitable for safety-critical applications. It handles platform
 * differences in minimum word size (8-bit vs 16-bit) while
 * maintaining MISRA C compliance and deterministic WCET.
 */

#ifndef BITFIELD_H
#define BITFIELD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** @defgroup platform Platform Detection
 * @brief Platform-specific configurations
 * @{
 */

/** Minimum word size in bits for the target platform */
#ifndef BITFIELD_MIN_WORD_SIZE
#if defined(__TI_C2000__) || defined(__MSP430__)
/** TI C2000 and MSP430 platforms have 16-bit minimum word size */
#define BITFIELD_MIN_WORD_SIZE 16U
#else
/** Default to 8-bit minimum word size */
#define BITFIELD_MIN_WORD_SIZE 8U
#endif
#endif

/** Word type matching platform minimum size */
#if BITFIELD_MIN_WORD_SIZE == 16U
/** 16-bit word type for platforms with 16-bit minimum */
typedef uint16_t bitfield_word_t;
#else
/** 8-bit word type for platforms with 8-bit minimum */
typedef uint8_t bitfield_word_t;
#endif

/** Accumulator type for operations (always 32-bit) */
typedef uint32_t bitfield_accum_t;

/** @} */

/** @defgroup bit_operations Individual Bit Operations
 * @brief Functions for manipulating individual bits
 * @{
 */

/** Set a single bit to the specified value
 *
 * @param data Pointer to the word containing the bit
 * @param bit_pos Bit position (0 = LSB)
 * @param value Boolean value to set (true = 1, false = 0)
 *
 * If @p data is NULL or @p bit_pos is out of range, this function is a no-op.
 */
void bitfield_set(bitfield_word_t *data, uint8_t bit_pos, bool value);

/** Get the value of a single bit
 *
 * @param data Pointer to the word containing the bit
 * @param bit_pos Bit position (0 = LSB)
 * @return Boolean value of the bit (true = 1, false = 0)
 *
 * If @p data is NULL or @p bit_pos is out of range, this function returns
 * false.
 */
bool bitfield_get(const bitfield_word_t *data, uint8_t bit_pos);

/** Clear a single bit (set to 0)
 *
 * @param data Pointer to the word containing the bit
 * @param bit_pos Bit position (0 = LSB)
 *
 * If @p data is NULL or @p bit_pos is out of range, this function is a no-op.
 */
void bitfield_clear(bitfield_word_t *data, uint8_t bit_pos);

/** @} */

/** @defgroup bitfield_operations Bitfield Range Operations
 * @brief Functions for manipulating bit ranges
 * @{
 */

/** Set a range of bits to the specified value
 *
 * @param data Pointer to the word containing the bitfield
 * @param start Start bit position (0 = LSB)
 * @param end End bit position (inclusive)
 * @param value Value to set (LSB of value goes to start position)
 *
 * If @p data is NULL, @p start > @p end, or @p end is out of range, this
 * function is a no-op.
 */
void bitfield_set_range(bitfield_word_t *data, uint8_t start, uint8_t end,
                        bitfield_accum_t value);

/** Get a range of bits as a value
 *
 * @param data Pointer to the word containing the bitfield
 * @param start Start bit position (0 = LSB)
 * @param end End bit position (inclusive)
 * @return Extracted bit range (LSB = start position)
 *
 * If @p data is NULL, @p start > @p end, or @p end is out of range, this
 * function returns 0.
 */
bitfield_accum_t bitfield_get_range(const bitfield_word_t *data, uint8_t start,
                                    uint8_t end);

/** @} */

/** @defgroup bit_counting Bit Counting Operations
 * @brief Functions for counting bits
 * @{
 */

/** Count the number of set bits (1s) in a bitfield
 *
 * @param data Pointer to the word containing the bitfield
 * @param num_bits Total number of bits to count
 * @return Number of set bits
 *
 * If @p data is NULL or @p num_bits is out of range, this function returns 0.
 */
uint8_t bitfield_count_set(const bitfield_word_t *data, uint8_t num_bits);

/** Count the number of unset bits (0s) in a bitfield
 *
 * @param data Pointer to the word containing the bitfield
 * @param num_bits Total number of bits to count
 * @return Number of unset bits
 *
 * If @p data is NULL or @p num_bits is out of range, this function returns 0.
 */
uint8_t bitfield_count_unset(const bitfield_word_t *data, uint8_t num_bits);

/** @} */

/** @defgroup packing Packing/Unpacking Operations
 * @brief Functions for serializing/deserializing bitfields
 * @{
 */

/** Pack byte array into compact bitfield representation
 *
 * @param src Source byte array
 * @param src_len Length of source array in bytes
 * @param dst Destination bitfield word array
 * @param dst_len Length of destination array in words
 *
 * If @p src or @p dst is NULL, this function is a no-op.
 */
void bitfield_pack(const uint8_t *src, uint8_t src_len, bitfield_word_t *dst,
                   uint8_t dst_len);

/** Unpack bitfield representation into byte array
 *
 * @param src Source bitfield word array
 * @param src_len Length of source array in words
 * @param dst Destination byte array
 * @param dst_len Length of destination array in bytes
 *
 * If @p src or @p dst is NULL, this function is a no-op.
 */
void bitfield_unpack(const bitfield_word_t *src, uint8_t src_len, uint8_t *dst,
                     uint8_t dst_len);

/** @} */

/** @defgroup crc CRC Calculations
 * @brief Cyclic Redundancy Check functions
 * @{
 */

/** Calculate 16-bit CRC using specified polynomial
 *
 * @param data Data to calculate CRC for
 * @param length Length of data in bytes
 * @param polynomial CRC polynomial (e.g., 0x8005 for CRC-16)
 * @return Calculated CRC value
 *
 * If @p data is NULL, this function returns 0.
 */
uint16_t bitfield_crc16(const uint8_t *data, uint16_t length,
                        uint16_t polynomial);

/** @} */

#endif /* BITFIELD_H */

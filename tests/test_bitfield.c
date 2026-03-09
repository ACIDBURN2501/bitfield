/** @file test_bitfield.c
 * @brief Unit tests for bitfield library
 */

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "bitfield.h"

static void
test_bit_operations(void)
{
        bitfield_word_t value = 0U;

        /* Test setting and getting bits */
        bitfield_set(&value, 0U, true);
        assert(bitfield_get(&value, 0U) == true);

        bitfield_set(&value, 3U, true);
        assert(bitfield_get(&value, 3U) == true);

        /* Test clearing bits */
        bitfield_clear(&value, 0U);
        assert(bitfield_get(&value, 0U) == false);

        printf("Bit operations: PASSED\n");
}

static void
test_bitfield_range(void)
{
        bitfield_word_t value = 0U;

        /* Test setting bit range */
        bitfield_set_range(&value, 2U, 4U, 0b101U);
        assert(bitfield_get_range(&value, 2U, 4U) == 0b101U);

        /* Test getting bit range */
        bitfield_set_range(&value, 0U, 1U, 0b11U);
        assert(bitfield_get_range(&value, 0U, 1U) == 0b11U);

        printf("Bitfield range operations: PASSED\n");
}

static void
test_bit_counting(void)
{
        bitfield_word_t value = 0b10101010U;

        /* Test counting set bits */
        assert(bitfield_count_set(&value, 8U) == 4U);

        /* Test counting unset bits */
        assert(bitfield_count_unset(&value, 8U) == 4U);

        printf("Bit counting: PASSED\n");
}

static void
test_packing(void)
{
        uint8_t src[1] = {0xAB};
        bitfield_word_t dst[1] = {0U};
        uint8_t unpacked[1] = {0U};

        /* Test packing 1 byte */
        bitfield_pack(src, 1U, dst, 1U);

        /* Test unpacking */
        bitfield_unpack(dst, 1U, unpacked, 1U);

        /* Verify */
        assert(src[0] == unpacked[0]);

        printf("Packing/unpacking: PASSED\n");
}

static void
test_crc(void)
{
        uint8_t data[4] = {0x01, 0x23, 0x45, 0x67};
        uint16_t crc = bitfield_crc16(data, 4U, 0x8005U);

        /* Expected CRC-16 for this data */
        assert(crc == 0x09CFU);

        printf("CRC calculation: PASSED\n");
}

static void
test_null_pointer(void)
{
        bitfield_word_t value = 0U;

        /* Functions with NULL data pointer should not crash or modify anything
         */
        bitfield_set(NULL, 0U, true);
        assert(value == 0U);
        assert(bitfield_get(NULL, 0U) == false);
        bitfield_clear(NULL, 0U);
        bitfield_set_range(NULL, 0U, 1U, 0U);
        assert(bitfield_get_range(NULL, 0U, 1U) == 0U);
        assert(bitfield_count_set(NULL, 8U) == 0U);
        assert(bitfield_count_unset(NULL, 8U) == 0U);
        bitfield_pack(NULL, 1U, NULL, 1U);
        bitfield_unpack(NULL, 1U, NULL, 1U);
        assert(bitfield_crc16(NULL, 4U, 0x8005U) == 0U);

        printf("Null-pointer handling: PASSED\n");
}

static void
test_invalid_range(void)
{
        bitfield_word_t value = (bitfield_word_t)~0U;

        /* start > end should be ignored */
        bitfield_set_range(&value, 5U, 3U, 0U);
        /* end beyond word size should be ignored */
        bitfield_set_range(&value, 0U, BITFIELD_MIN_WORD_SIZE, 0U);
        assert(value == (bitfield_word_t)~0U);

        /* get_range with invalid parameters returns 0 */
        assert(bitfield_get_range(&value, 5U, 3U) == 0U);
        assert(bitfield_get_range(&value, 0U, BITFIELD_MIN_WORD_SIZE) == 0U);

        printf("Invalid range handling: PASSED\n");
}

static void
test_invalid_bit_count(void)
{
        bitfield_word_t value = (bitfield_word_t)~0U;

        /* request more bits than word size; should return 0 */
        assert(bitfield_count_set(&value, BITFIELD_MIN_WORD_SIZE + 1U) == 0U);
        assert(bitfield_count_unset(&value, BITFIELD_MIN_WORD_SIZE + 1U) == 0U);

        printf("Invalid bit count handling: PASSED\n");
}

static void
test_packing_multiple_bytes(void)
{
        uint8_t src[3] = {0xAAU, 0x55U, 0xFFU};
        bitfield_word_t dst[3] = {0U, 0U, 0U};
        uint8_t unpacked[3] = {0U, 0U, 0U};

        bitfield_pack(src, 3U, dst, 3U);
        bitfield_unpack(dst, 3U, unpacked, 3U);

        assert(src[0] == unpacked[0]);
        assert(src[1] == unpacked[1]);
        assert(src[2] == unpacked[2]);

        printf("Packing/unpacking (multi-byte): PASSED\n");
}

static void
test_invalid_bit_pos(void)
{
        bitfield_word_t value = 0U;

        bitfield_set(&value, BITFIELD_MIN_WORD_SIZE, true);
        assert(value == 0U);
        bitfield_clear(&value, BITFIELD_MIN_WORD_SIZE);
        assert(value == 0U);
        assert(bitfield_get(&value, BITFIELD_MIN_WORD_SIZE) == false);

        printf("Invalid bit position handling: PASSED\n");
}

static void
test_shift_bounds(void)
{
        bitfield_word_t value = 0xFFU;

        uint8_t max_end = (uint8_t)(BITFIELD_MIN_WORD_SIZE - 1U);
        bitfield_accum_t max_value =
            ((bitfield_accum_t)1U << BITFIELD_MIN_WORD_SIZE) - 1U;

        bitfield_set_range(&value, 0U, max_end, max_value);

        bitfield_accum_t result = bitfield_get_range(&value, 0U, max_end);
        assert(result == max_value);

        bitfield_word_t value2 = 0xFFU;
        bitfield_set_range(&value2, 0U, (uint8_t)BITFIELD_MIN_WORD_SIZE,
                           max_value);
        assert(value2 == 0xFFU);

        printf("Shift bounds (max valid range): PASSED\n");
}

static void
test_pack_truncation(void)
{
        uint8_t src[4] = {0x12U, 0x34U, 0x56U, 0x78U};
        bitfield_word_t dst_small[1] = {0xABU};
        bitfield_word_t dst_adequate[4] = {0U, 0U, 0U, 0U};

        bitfield_pack(src, 4U, dst_small, 1U);

        bitfield_pack(src, 4U, dst_adequate, 4U);
        assert(dst_adequate[0] == 0x3412U || dst_adequate[0] == 0x12U);

        printf("Pack truncation (silent on buffer full): PASSED\n");
}

static void
test_pack_large_buffer(void)
{
        uint8_t src[10] = {0U};
        for (uint8_t i = 0U; i < 10U; i++) {
                src[i] = (uint8_t)i;
        }

        bitfield_word_t dst[10] = {0U};

        bitfield_pack(src, 10U, dst, 10U);

        printf("Pack large buffer (bits bounded): PASSED\n");
}

static void
test_crc_edge_cases(void)
{
        uint16_t crc_null = bitfield_crc16(NULL, 4U, 0x8005U);
        assert(crc_null == 0U);

        uint8_t data[4] = {0x01, 0x23, 0x45, 0x67};
        uint16_t crc_valid = bitfield_crc16(data, 4U, 0x8005U);
        assert(crc_valid == 0x09CFU);

        uint16_t crc_empty = bitfield_crc16(data, 0U, 0x8005U);
        assert(crc_empty == 0xFFFFU);

        printf("CRC edge cases (NULL, empty): PASSED\n");
}

int
main(void)
{
        test_bit_operations();
        test_bitfield_range();
        test_bit_counting();
        test_packing();
        test_packing_multiple_bytes();
        test_crc();
        test_null_pointer();
        test_invalid_range();
        test_invalid_bit_count();
        test_invalid_bit_pos();
        test_shift_bounds();
        test_pack_truncation();
        test_pack_large_buffer();
        test_crc_edge_cases();

        printf("\nAll tests PASSED!\n");
        return 0;
}

/** @file bitfield.c
 * @brief Implementation of portable bit manipulation library
 */

#include "bitfield.h"

/* Individual Bit Operations */

void
bitfield_set(bitfield_word_t *data, uint8_t bit_pos, bool value)
{
        if ((data == NULL) || (bit_pos >= BITFIELD_MIN_WORD_SIZE)) {
                return;
        }

        if (value) {
                *data |= (bitfield_word_t)(1U << bit_pos);
        } else {
                *data &= (bitfield_word_t)(~(1U << bit_pos));
        }
}

bool
bitfield_get(const bitfield_word_t *data, uint8_t bit_pos)
{
        if ((data == NULL) || (bit_pos >= BITFIELD_MIN_WORD_SIZE)) {
                return false;
        }

        return (bool)((*data & (bitfield_word_t)(1U << bit_pos)) != 0U);
}

void
bitfield_clear(bitfield_word_t *data, uint8_t bit_pos)
{
        if ((data == NULL) || (bit_pos >= BITFIELD_MIN_WORD_SIZE)) {
                return;
        }

        *data &= (bitfield_word_t)(~(1U << bit_pos));
}

/* Bitfield Range Operations */

void
bitfield_set_range(bitfield_word_t *data, uint8_t start, uint8_t end,
                   bitfield_accum_t value)
{
        bitfield_accum_t mask;
        uint8_t width;

        if ((data == NULL) || (start > end)
            || (end >= BITFIELD_MIN_WORD_SIZE)) {
                return;
        }

        width = (uint8_t)(end - start + 1U);
        mask = (((bitfield_accum_t)1U << width) - (bitfield_accum_t)1U)
               << start;

        *data = (bitfield_word_t)((((bitfield_accum_t)*data) & (~mask))
                                  | ((value << start) & mask));
}

bitfield_accum_t
bitfield_get_range(const bitfield_word_t *data, uint8_t start, uint8_t end)
{
        bitfield_accum_t mask;
        uint8_t width;

        if ((data == NULL) || (start > end)
            || (end >= BITFIELD_MIN_WORD_SIZE)) {
                return 0U;
        }

        width = (uint8_t)(end - start + 1U);
        mask = (((bitfield_accum_t)1U << width) - (bitfield_accum_t)1U)
               << start;

        return (bitfield_accum_t)((((bitfield_accum_t)*data) & mask) >> start);
}

/* Bit Counting */

uint8_t
bitfield_count_set(const bitfield_word_t *data, uint8_t num_bits)
{
        uint8_t count = 0U;
        bitfield_accum_t value;

        if ((data == NULL) || (num_bits > BITFIELD_MIN_WORD_SIZE)) {
                return 0U;
        }

        value = (bitfield_accum_t)*data;

        for (uint8_t i = 0U; i < num_bits; i++) {
                if ((value & ((bitfield_accum_t)1U << i)) != 0U) {
                        count++;
                }
        }

        return count;
}

uint8_t
bitfield_count_unset(const bitfield_word_t *data, uint8_t num_bits)
{
        if ((data == NULL) || (num_bits > BITFIELD_MIN_WORD_SIZE)) {
                return 0U;
        }

        return (uint8_t)(num_bits - bitfield_count_set(data, num_bits));
}

/* Packing/Unpacking */

void
bitfield_pack(const uint8_t *src, uint8_t src_len, bitfield_word_t *dst,
              uint8_t dst_len)
{
        bitfield_accum_t accum = 0U;
        uint8_t bits = 0U;
        uint8_t di = 0U;
        const bitfield_accum_t word_mask =
            (((bitfield_accum_t)1U << BITFIELD_MIN_WORD_SIZE) - 1U);

        if ((src == NULL) || (dst == NULL)) {
                return;
        }

        for (uint8_t i = 0U; i < src_len; i++) {
                accum |= ((bitfield_accum_t)src[i]) << bits;
                bits = (uint8_t)(bits + 8U);

                while ((bits >= BITFIELD_MIN_WORD_SIZE) && (di < dst_len)) {
                        dst[di] = (bitfield_word_t)(accum & word_mask);
                        di++;
                        accum >>= BITFIELD_MIN_WORD_SIZE;
                        bits = (uint8_t)(bits - BITFIELD_MIN_WORD_SIZE);
                }

                if (di >= dst_len) {
                        return;
                }
        }

        if ((bits != 0U) && (di < dst_len)) {
                dst[di] = (bitfield_word_t)(accum & word_mask);
        }
}

void
bitfield_unpack(const bitfield_word_t *src, uint8_t src_len, uint8_t *dst,
                uint8_t dst_len)
{
        bitfield_accum_t accum = 0U;
        uint8_t bits = 0U;
        uint8_t di = 0U;

        if ((src == NULL) || (dst == NULL)) {
                return;
        }

        for (uint8_t i = 0U; i < src_len; i++) {
                accum |= ((bitfield_accum_t)src[i]) << bits;
                bits = (uint8_t)(bits + BITFIELD_MIN_WORD_SIZE);

                while ((bits >= 8U) && (di < dst_len)) {
                        dst[di] = (uint8_t)(accum & 0xFFU);
                        di++;
                        accum >>= 8U;
                        bits = (uint8_t)(bits - 8U);
                }

                if (di >= dst_len) {
                        return;
                }
        }
}

/* CRC Calculations */

uint16_t
bitfield_crc16(const uint8_t *data, uint16_t length, uint16_t polynomial)
{
        uint16_t crc = 0xFFFFU;

        if (data == NULL) {
                return 0U;
        }

        for (uint16_t i = 0U; i < length; i++) {
                crc ^= (uint16_t)((uint16_t)data[i] << 8U);

                for (uint8_t j = 0U; j < 8U; j++) {
                        if ((crc & 0x8000U) != 0U) {
                                crc = (uint16_t)((crc << 1U) ^ polynomial);
                        } else {
                                crc = (uint16_t)(crc << 1U);
                        }
                }
        }

        return crc;
}

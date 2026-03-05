#!/bin/bash
# Test script to verify compile-time validation of BITFIELD_MIN_WORD_SIZE
# This ensures the library prevents undefined behavior from invalid overrides.

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
INCLUDE_DIR="$PROJECT_ROOT/include"
SRC_FILE="$PROJECT_ROOT/src/bitfield.c"

echo "=== Compile-Time Validation Tests for BITFIELD_MIN_WORD_SIZE ==="
echo ""

# Test 1: Valid value (8-bit, default)
echo "Test 1: BITFIELD_MIN_WORD_SIZE=8 (should PASS)"
if gcc -fsyntax-only -I"$INCLUDE_DIR" -DBITFIELD_MIN_WORD_SIZE=8 "$SRC_FILE" 2>&1; then
        echo "  ✓ PASSED - 8-bit configuration accepted"
else
        echo "  ✗ FAILED - 8-bit configuration rejected (unexpected)"
        exit 1
fi
echo ""

# Test 2: Valid value (16-bit)
echo "Test 2: BITFIELD_MIN_WORD_SIZE=16 (should PASS)"
if gcc -fsyntax-only -I"$INCLUDE_DIR" -DBITFIELD_MIN_WORD_SIZE=16 "$SRC_FILE" 2>&1; then
        echo "  ✓ PASSED - 16-bit configuration accepted"
else
        echo "  ✗ FAILED - 16-bit configuration rejected (unexpected)"
        exit 1
fi
echo ""

# Test 3: Invalid value (17-bit, exceeds current implementation)
echo "Test 3: BITFIELD_MIN_WORD_SIZE=17 (should FAIL at compile time)"
if gcc -fsyntax-only -I"$INCLUDE_DIR" -DBITFIELD_MIN_WORD_SIZE=17 "$SRC_FILE" 2>&1 | grep -q "error.*BITFIELD_MIN_WORD_SIZE"; then
        echo "  ✓ PASSED - 17-bit configuration correctly rejected"
else
        echo "  ✗ FAILED - 17-bit configuration was not rejected (security risk!)"
        exit 1
fi
echo ""

# Test 4: Invalid value (32-bit, exceeds current implementation)
echo "Test 4: BITFIELD_MIN_WORD_SIZE=32 (should FAIL at compile time)"
if gcc -fsyntax-only -I"$INCLUDE_DIR" -DBITFIELD_MIN_WORD_SIZE=32 "$SRC_FILE" 2>&1 | grep -q "error.*BITFIELD_MIN_WORD_SIZE"; then
        echo "  ✓ PASSED - 32-bit configuration correctly rejected"
else
        echo "  ✗ FAILED - 32-bit configuration was not rejected (security risk!)"
        exit 1
fi
echo ""

# Test 5: Invalid value (64-bit, way too large)
echo "Test 5: BITFIELD_MIN_WORD_SIZE=64 (should FAIL at compile time)"
if gcc -fsyntax-only -I"$INCLUDE_DIR" -DBITFIELD_MIN_WORD_SIZE=64 "$SRC_FILE" 2>&1 | grep -q "error.*BITFIELD_MIN_WORD_SIZE"; then
        echo "  ✓ PASSED - 64-bit configuration correctly rejected"
else
        echo "  ✗ FAILED - 64-bit configuration was not rejected (security risk!)"
        exit 1
fi
echo ""

# Test 6: Default configuration (no override)
echo "Test 6: Default configuration (should PASS)"
if gcc -fsyntax-only -I"$INCLUDE_DIR" "$SRC_FILE" 2>&1; then
        echo "  ✓ PASSED - Default configuration accepted"
else
        echo "  ✗ FAILED - Default configuration rejected (unexpected)"
        exit 1
fi
echo ""

echo "=== All Compile-Time Validation Tests PASSED ==="
echo ""
echo "Summary:"
echo "  • Valid configurations (8, 16-bit) are accepted"
echo "  • Invalid configurations (>16-bit) are rejected at compile time"
echo "  • This prevents undefined behavior per MISRA C:2012 Rule 12.2"
echo "  • This ensures IEC-61508 compliance by eliminating UB risk"
echo ""
echo "Note: The library currently supports 8-bit and 16-bit word sizes."
echo "      Support for larger word sizes would require additional typedef"
echo "      cases and careful handling of shift operations in pack/unpack."

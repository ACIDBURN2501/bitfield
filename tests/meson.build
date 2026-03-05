test_exe = executable(
  'test_bitfield',
  'test_bitfield.c',
  dependencies: [bitfield_dep],
  install: false,
)

test('bitfield tests', test_exe)

# Build a second library/test with 16-bit word size forced.
bitfield_16 = static_library(
  'bitfield_16',
  library_sources,
  include_directories: public_headers,
  c_args: ['-DBITFIELD_MIN_WORD_SIZE=16'],
  install: false,
)

bitfield_16_dep = declare_dependency(
  link_with: bitfield_16,
  include_directories: public_headers,
  compile_args: ['-DBITFIELD_MIN_WORD_SIZE=16'],
)

test_exe_16 = executable(
  'test_bitfield_16',
  'test_bitfield.c',
  dependencies: [bitfield_16_dep],
  install: false,
)

test('bitfield tests (16-bit)', test_exe_16)

# Compile-time validation test for BITFIELD_MIN_WORD_SIZE
test(
  'compile-time checks',
  find_program('bash'),
  args: [meson.current_source_dir() / 'test_compile_time_checks.sh'],
  timeout: 30,
)

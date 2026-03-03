project(
  'bitfield',
  'c',
  version: '1.0.2',
  default_options: ['c_std=c11', 'warning_level=2'],
  license: 'MIT',
)

# ---------------------------------------------------------------------------
# Version header (generated into the build directory root)
# ---------------------------------------------------------------------------

_ver = meson.project_version().split('.')

_conf = configuration_data()
_conf.set_quoted('BITFIELD_VERSION', meson.project_version())
_conf.set('BITFIELD_VERSION_MAJOR', _ver[0].to_int())
_conf.set('BITFIELD_VERSION_MINOR', _ver[1].to_int())
_conf.set('BITFIELD_VERSION_PATCH', _ver[2].to_int())

configure_file(
  input: 'include' / 'bitfield_version.h.in',
  output: 'bitfield_version.h',
  configuration: _conf,
  install: true,
  install_dir: get_option('includedir') / 'bitfield',
)

# Library sources
library_sources = files('src/bitfield.c')

# Public headers
public_headers = include_directories('include')

# Library
library = static_library(
  'bitfield',
  library_sources,
  include_directories: public_headers,
  install: true,
)

# Development / subproject dependency
bitfield_dep = declare_dependency(link_with: library, include_directories: public_headers)

meson.override_dependency('bitfield', bitfield_dep)

# Install headers
install_headers('include/bitfield.h', subdir: 'bitfield')

# Tests
if get_option('build_tests')
  subdir('tests')
endif

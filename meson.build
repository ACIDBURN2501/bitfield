project(
  'bitfield',
  'c',
  version: '1.3.3',
  default_options: ['c_std=c11', 'warning_level=2'],
  license: 'MIT',
)

# ── Version header ─────────────────────────────────────────────────────────────

ver = meson.project_version().split('.')

major = ver[0].to_int()
minor = ver[1].to_int()
patch = ver[2].to_int()

conf = configuration_data()
conf.set_quoted('BITFIELD_VERSION', meson.project_version())
conf.set('BITFIELD_VERSION_MAJOR', major)
conf.set('BITFIELD_VERSION_MINOR', minor)
conf.set('BITFIELD_VERSION_PATCH', patch)

configure_file(
  input: 'config' / 'bitfield_version.h.in',
  output: 'bitfield_version.h',
  configuration: conf,
  install: true,
  install_dir: get_option('includedir') / 'bitfield',
)

# ── Include directories ────────────────────────────────────────────────────────

public_headers = include_directories('include')

# ── Application core library ───────────────────────────────────────────────────

library_sources = files('src/bitfield.c')

library = static_library(
  'bitfield',
  library_sources,
  include_directories: public_headers,
  install: true,
)

bitfield_dep = declare_dependency(link_with: library, include_directories: public_headers)

meson.override_dependency('bitfield', bitfield_dep)

install_headers('include/bitfield.h', subdir: 'bitfield')

# ── Host unit tests ────────────────────────────────────────────────────────────

if get_option('build_tests')
  subdir('tests')
endif

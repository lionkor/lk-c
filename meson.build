project('lk', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=c17'])

# These arguments are only used to build the shared library
# not the executables that use the library.
liblk_args = ['-DBUILDING_LK', '-D_GNU_SOURCE']

thread_dep = dependency('threads')
srcdir = include_directories('include/lk')

liblk = shared_library('lk', 
  [
    'src/lk.c',
    'src/thread.c',
    'src/chan.c',
    'src/refptr.c',
    'src/input.c',
  ],
  include_directories : srcdir,
  install : true,
  c_args : liblk_args,
  gnu_symbol_visibility : 'hidden',
  dependencies : thread_dep,
)

test_exe = executable('test_lk', 'src/test.c',
  include_directories : [srcdir, include_directories('acutest/include')],
  link_with : liblk,
  dependencies : thread_dep,
)

sandbox_exe = executable('sandbox', 'src/sandbox.c',
  include_directories : [srcdir],
  link_with : liblk,
  dependencies : thread_dep,
)

example_type_to_file_exe = executable('type_to_file', 'src/examples/type_to_file.c',
  include_directories : [srcdir],
  link_with : liblk,
  dependencies : thread_dep,
)

test('test_lk', test_exe)

# Make this library usable as a Meson subproject.
lk_dep = declare_dependency(
  link_with : liblk
)

# Make this library usable from the system's
# package manager.
install_headers([
  'include/lk/lk.h',
  'include/lk/chan.h',
  'include/lk/thread.h',
])

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : 'lk',
  filebase : 'lk',
  description : 'LK\'s C library.',
  subdirs : 'lk',
  libraries : liblk,
  version : '0.1',
)


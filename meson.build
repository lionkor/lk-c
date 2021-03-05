project('lk', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=c17'])

# These arguments are only used to build the shared library
# not the executables that use the library.
liblk_args = ['-DBUILDING_LK']

thread_dep = dependency('threads')

liblkchan = shared_library('lkchan', 'src/chan/chan.c',
  install : true,
  c_args : liblk_args,
  gnu_symbol_visibility : 'hidden',
  dependencies : thread_dep,
)

liblkthread = shared_library('lkthread', 'src/thread/thread.c',
  install : true,
  c_args : liblk_args,
  gnu_symbol_visibility : 'hidden',
  dependencies : thread_dep,
)

liblk = shared_library('lk', 'src/lk.c',
  install : true,
  c_args : liblk_args,
  gnu_symbol_visibility : 'hidden',
  dependencies : thread_dep,
  link_with : [liblkchan, liblkthread],
)

test_exe = executable('test_lk', 'src/test.c',
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
install_headers('src/lk.h', subdir : 'lk')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : 'lk',
  filebase : 'lk',
  description : 'LK\'s C library.',
  subdirs : 'lk',
  libraries : liblk,
  version : '0.1',
)


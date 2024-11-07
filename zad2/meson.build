inc = include_directories('inc')
lib_src = files(['src/check.c', 'src/md5.c'])

lib = static_library(
    'check_lib',
    lib_src,
    include_directories: [inc],
)

exec = executable('zad2', ['src/main.c'], include_directories:[inc], link_with: [lib])
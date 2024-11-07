inc = include_directories('inc')
lib_src = files(['src/check.c'])

cmake = import('cmake')
sub_proj = cmake.subproject('mbedtls-3.6.2')
cm_lib = sub_proj.dependency('mbedcrypto')

lib = static_library(
    'check_lib',
    lib_src,
    include_directories: [inc],
    dependencies: [cm_lib],
)

mdt_dep = declare_dependency(include_directories: [inc], link_with: [lib])

message('CMake targets:\n - ' + '\n - '.join(sub_proj.target_list()))

exec = executable('zad2', ['src/main.c'], include_directories:[inc], dependencies:[mdt_dep])
project('lab2', 'cpp')

inc_dir = include_directories('include')
src_lib = ['src/cache.cpp', 'src/api.cpp']
src_main = ['src/cache.cpp', 'src/api.cpp', 'src/main.cpp']

library('lab2_cache', 
    sources: src_lib, 
    install: true,
    include_directories: inc_dir
)

executable('test_lib', sources: src_main, include_directories: inc_dir)
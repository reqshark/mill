{
    # compiler settings to build libmill
    'defines': [
        '__APPLE__',
    ],
    'direct_dependent_settings': {
        # build hub with same compiler flags as the library
        'defines': [
            '__APPLE__',
        ],
#        'include_dirs': [
#          'deps/nanomsg/src',
#        ],
#        'cflags': [
#          '-O3 -D_THREAD_SAFE'
#        ],
    }
}

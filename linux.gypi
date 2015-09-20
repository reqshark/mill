{
# compiler settings to build the binary on linux

#may need these defined on linux at some point:
    'defines': [
        'HAVE_POSIX_MEMALIGN',
        'HAVE_MPROTECT',
        'CLOCK_MONOTONIC',
        'HAVE_LIBANL'
    ],

# get rid of node-gyp noisey warnings:
    'cflags': [
        '-g','-O2', '-Wall', '-Wextra', '-Wno-unused', '-Wno-sign-compare',
        '-Wno-maybe-uninitialized',

# "Turn the source fortification in glibc off - otherwise it would panic
# because of the stack pointer black magic in 'go' macro." -Martin Sustrik
        '-U_FORTIFY_SOURCE',
        '-D_FORTIFY_SOURCE=0',
    ],
    'ldflags': [ '-lrt', '-lanl' ],


# may need these libraries linked on linux at some point:
#    'ldflags': [ '-lrt', '-lanl' ],

#    'direct_dependent_settings': {
#        'defines': [
#            'HAVE_POSIX_MEMALIGN',
#            'HAVE_MPROTECT',
#        ],
#        'cflags': [
#            '-Wall',
#            '-Wextra',
#            '-Wno-unused',
#            '-Wno-sign-compare',
#            '-Wno-maybe-uninitialized',
#            '-O2',
#            '-U_FORTIFY_SOURCE',
#            '-D_FORTIFY_SOURCE=0',
#        ],
#        'ldflags': [ '-lrt', '-lanl' ],
#    }
}

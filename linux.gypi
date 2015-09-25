{
    'cflags': [
        #'-g',
        '-O3',
        '-Wall', '-Wextra',

# get rid of node-gyp noisey warnings:
        '-Wno-unused', '-Wno-sign-compare','-Wno-maybe-uninitialized',

# "Turn the source fortification in glibc off - otherwise it would panic
# because of the stack pointer black magic in 'go' macro." -Martin Sustrik
        '-U_FORTIFY_SOURCE',
        '-D_FORTIFY_SOURCE=0',
        '-DMILL_EXPORTS',
    ],
}

{
  # compiler settings to build the nanomsg library
  'xcode_settings': {
      'OTHER_CFLAGS': [
          '-g', # debug
          '-O0',
          '-fvisibility=hidden',
          '-DMILL_EXPORTS',
          '-U_FORTIFY_SOURCE',
          '-D_FORTIFY_SOURCE=0',
          '-DHAVE_MPROTECT',
          '-DHAVE_POSIX_MEMALIGN',
          '-D_GNU_SOURCE',
          '-std=gnu99',
          '-Wno-uninitialized',
          '-Wno-unused',
          '-Wno-sign-compare',
          '-Wno-missing-field-initializers',
      ]
  },
  'defines': [
      'MILL_KQUEUE',
      'HAVE_POSIX_MEMALIGN',
      'HAVE_MPROTECT',
      'MILL_NO_EPOLL'
  ],
  'direct_dependent_settings': {
      # build nanomsg hub with same compiler flags as the library
      'defines': [
          'MILL_KQUEUE',
          'HAVE_POSIX_MEMALIGN',
          'HAVE_MPROTECT',
          'MILL_NO_EPOLL'
      ],
  }
}

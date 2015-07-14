{
  'targets': [
    {
      'target_name' : 'mill',
#      'type': 'static_library',
      'cflags': [ '-DMILL_EXPORTS', '-U_FORTIFY_SOURCE', '-D_FORTIFY_SOURCE=0', '-g', '-O0' ],
      'conditions': [
        ['OS=="mac"', {
            'includes': ['osx.gypi']
        }]
      ],
      'include_dirs': [ '<!(node -e "require(\'nan\')")' ],
      'sources': [
        'libmill/chan.c',
        'libmill/cr.c',
        'libmill/cr.h',
        'libmill/debug.c',
        'libmill/list.c',
        'libmill/poller.c',
        'libmill/slist.c',
        'libmill/stack.c',
        'libmill/tcp.c',
        'libmill/utils.c',
        'libmill/valbuf.c'
      ],
      'sources': [ 'src/mill.cc' ]
    }
  ]
}

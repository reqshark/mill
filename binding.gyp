{
    'targets': [
        {
            'target_name': 'libmill',
            'type': 'static_library',
            'conditions': [
                ['OS=="mac"', {
                    'includes': [
                        'macosx.gypi',
                    ]
                }],
                ['OS=="linux"', {
#                    'includes': [
#                        'linux.gypi',
#                    ]
                }]
            ],
            'include_dirs': [
                'libmill'
            ],
            'sources': [
                'libmill/chan.h',
                'libmill/chan.c',
                'libmill/cr.h',
                'libmill/cr.c',
                'libmill/debug.h',
                'libmill/debug.c',
                'libmill/ip.h',
                'libmill/ip.c',
                'libmill/list.h',
                'libmill/list.c',
                'libmill/poller.h',
                'libmill/poller.c',
                'libmill/slist.h',
                'libmill/slist.c',
                'libmill/stack.h',
                'libmill/stack.c',
                'libmill/tcp.c',
                'libmill/udp.c',
                'libmill/unix.c',
                'libmill/utils.h',
                'libmill/utils.c'
            ]
        },
        {
            'target_name': 'mill',
            'dependencies': [ 'libmill', ],
            'cflags': [ '-Wall -Werror -Wno-unused' ],
            'cflags_cc': ['-fexceptions'],
            'conditions': [
                ['OS=="mac"', {
                    'ldflags': [ '-L<(PRODUCT_DIR)' ],
                    'libraries': [ '-L<(PRODUCT_DIR)' ],
                }],
            ],
            'include_dirs': [
                "<!(node -e \"require('nan')\")",
                'libmill',
            ],
            'sources': [ 'lib/mill.cc' ],
        }
    ]
}

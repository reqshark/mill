{
    'targets': [
        {
            'target_name': 'mill',
            'libraries': [
                '<!@(pkg-config libmill --libs)',
                '<!@(pkg-config libsodium --libs)',
            ],
            'include_dirs': [
                "<!(node -e \"require('nan')\")",
                '/usr/local/include',
                '/opt/local/include',
                '<!@(pkg-config libmill --cflags-only-I)',
                '<!@(pkg-config libsodium --cflags-only-I)',
            ],
            'xcode_settings': {
                'OTHER_CFLAGS': [
                    '-Wno-unused-function',
                    '-Wno-unused-variable',
                    '-Wno-missing-prototypes',
                    '-Wno-format',
                ],
            },
            'cflags_cc':[],
            'cflags': [
                '-Wno-unused-variable',
                '-Wno-unused-function',
                '-Wno-format',
                '-O3',
            ],
            'sources': [
                'binding.cc'
            ],
        }
    ]
}

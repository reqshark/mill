{
    'targets': [
        {
            'target_name': 'mill',
            'libraries': [
                '<!@(pkg-config <(module_root_dir)/opt/lib/pkgconfig/libmill.pc --libs)',
                '<!@(pkg-config <(module_root_dir)/opt/lib/pkgconfig/libsodium.pc --libs)',
            ],
            'include_dirs': [
                "<!(node -e \"require('nan')\")",
                'opt/include',
                'opt/include/sodium',
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
            'ldflags':[
                '-Wl,-rpath -Wl,<(module_root_dir)/opt/lib',
            ],
            'sources': [
                'binding.cc'
            ],
        }
    ]
}

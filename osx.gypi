{
    # compiler settings to build libmill
    'defines': [
        '__clang__'
    ],
    'xcode_settings': {
        'MACOSX_DEPLOYMENT_TARGET': '10.7',
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'OTHER_CFLAGS': [
            '',
            '-g',
            '-std=c99',
            '-stdlib=libc',
            '-O',
            '-D__STDC_CONSTANT_MACROS',
            '-D_FILE_OFFSET_BITS=64',
            '-D_LARGEFILE_SOURCE'
        ],
        'OTHER_CPLUSPLUSFLAGS': [
            '-g',
            '-mmacosx-version-min=10.7',
            '-std=c++11',
            '-stdlib=libc++',
            '-O0',
            '-D__STDC_CONSTANT_MACROS',
            '-D_FILE_OFFSET_BITS=64',
            '-D_LARGEFILE_SOURCE',
            '-Wall'
        ]
    }
}

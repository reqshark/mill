{
'variables': {'target_arch%': '<!(node -e \"console.log(require(\'os\').arch())\")>'},
'targets': [
    {
        'target_name': 'mill',
        'dependencies': [
            "<(module_root_dir)/libsodium.gyp:libsodium",
            "<(module_root_dir)/libmill.gyp:libmill",
        ],
        'conditions': [ ['OS=="linux"', { 'libraries': [ '-lanl','-lrt','-lpthread' ] }]],
        'include_dirs': [
            'libmill',
            'libsodium/src/libsodium/include',
            "<!(node -e \"require('nan')\")",
        ],
        'xcode_settings': { 'OTHER_CFLAGS': [
            '-Wno-unused-function',
            '-Wno-unused-variable',
            '-Wno-missing-prototypes',
            '-Wno-format',
        ], },
        'cflags': [
            '-Wno-unused-variable',
            '-Wno-unused-function',
            '-fPIC',
            '-static',
            '-Wno-missing-prototypes',
            '-Wno-format',
        ],
        'sources': [ 'binding.cc', ],
    }
]}

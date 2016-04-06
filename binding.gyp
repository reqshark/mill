{
'variables': {'target_arch%': '<!(node -e \"console.log(require(\'os\').arch())\")>'},
'targets': [
    {
        'target_name': 'mill',
        'dependencies': [
            "<(module_root_dir)/libsodium.gyp:libsodium",
            "<(module_root_dir)/libmill.gyp:libmill",
        ],
        'conditions': [ ['OS=="linux"', { 'libraries': [ '-lanl','-lrt' ] }]],
        'include_dirs': [
            'libmill',
            'libsodium/src/libsodium/include',
            "<!(node -e \"require('nan')\")",
        ],
        'sources': [ 'binding.cc' ],
    }
]}

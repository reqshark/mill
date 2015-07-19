{
  'targets': [
    {
      'target_name' : 'mill',
      'sources': [ 'lib/mill.cc' ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'NO'
          },
        }]
      ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")'
      ],
      'libraries': [
        '-lmill',
        '-L<(PRODUCT_DIR)/../../lib/lib'
      ]
    }
  ]
}

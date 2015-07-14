{
  'targets': [
    {
      'target_name' : 'mill',
#      'type': 'static_library',
      'cflags': [ '-w' ],
      'defines': [
        '_mill_unoptimisable1'
      ],
      'include_dirs': [ '<!(node -e "require(\'nan\')")' ],
      'sources': [ 'mill.cc' ]
    }
  ]
}

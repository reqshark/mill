{
  'targets': [
    {
      'target_name' : 'nmsg',
      'include_dirs': [ '<!(node -e "require(\'nan\')")' ],
      'sources': [ 'deps/nmsg.cc' ],
      'libraries': [ '-lnanomsg' ]
    }
  ]
}

module.exports =
{
    'cflags': [
      '-Wno-unknown-pragmas',
      '-Wno-type-limits',
      '-fPIC',
    ],
    'ldflags': [
      '-Wl',
      '-z',
      'relro',
      'now',
      'noexecstack'
    ],
}

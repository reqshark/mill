module.exports =
{
    'cflags': [
      '-Wno-unknown-pragmas',
      '-Wno-type-limits',
    ],
    'ldflags': [
      '-Wl',
      '-z',
      'relro',
      'now',
      'noexecstack'
    ],
}

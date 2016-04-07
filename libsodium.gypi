module.exports =
{
    'cflags': [
      '-Wno-unknown-pragmas',
      '-Wno-type-limits',
      '-Wno-unused-function',
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

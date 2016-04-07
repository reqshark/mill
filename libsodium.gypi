module.exports =
{
    'cflags': [
      '-Wno-unknown-pragmas',
      '-Wno-type-limits',
      '-Wno-unused-function',
      '-fPIC',
      '-static'
    ],
    'ldflags': [
      '-Wl,-z,relro',
      '-Wl,-z,now',
    ],
}

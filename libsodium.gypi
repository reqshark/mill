module.exports =
{
    'libraries': [],
    'cflags': [
      '-std=gnu99',
      '-Wno-unknown-pragmas',
      '-Wno-type-limits',
      '-Wno-sign-compare',
      '-Wno-missing-field-initializers',
      '-fPIC',
      'avx2',
      '-static',
      '-Wno-unused',
    ],
    'ldflags': [
      '-Wl,-z,relro',
      '-Wl,-z,now',
    ],
}

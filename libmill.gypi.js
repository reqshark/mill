module.exports = {
  'libraries': [],
  'cflags': [
    '-std=gnu99',
    '-Wno-uninitialized',
    '-Wno-unused',
    '-Wno-sign-compare',
    '-Wno-missing-field-initializers',
  ],
  'ldflags': [
    '-lanl',
    '-lrt'
  ],
}

//'-fvisibility=hidden','-DMILL_EXPORTS ','-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0','-lanl', '-lrt','', '-std=gnu99','-Wno-uninitialized','-Wno-unused','-Wno-sign-compare','-Wno-missing-field-initializers'

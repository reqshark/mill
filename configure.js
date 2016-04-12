/**
 * node configure
 * --------------
 *
 * automatic gyp generation of c sources, cflags, xcode_settings, defines
 * some machine specifics available in post ./configure file:
 *   • config.log
 *   • others found in Makefile.am
 *
 */

const readFileSync  = require('fs').readFileSync
const readdirSync   = require('fs').readdirSync
const writeFile     = require('fs').writeFile
const readFile      = require('fs').readFile
const extraflags = [
  '-Wno-missing-field-initializers',
  '-Wno-missing-prototypes',
  '-Wno-unknown-pragmas',
  '-Wno-unused',
  '-Qunused-arguments',
  '-Wno-sign-compare',
  '-Wno-cast-align',
  '-Wno-cast-qual',
]
const libs          = [
  'libsodium',
  'libmill',
].forEach( configure )


function configure ( lib ) {
  var gypi = require(`${__dirname}/${lib}.gypi`)

  const f = readFileSync( `${__dirname}/${lib}/config.log`, 'utf8' )
    .split(/## Output variables. ##/)[1]
    .split(/## confdefs.h. ##/)

  /* read sources and flags from Makefile.am */
  var makefile = msources(lib)

  /* combine cflags from config.log */
  var flags = compilerflags(f[0], makefile.cc, gypi, lib)

  /* build gyp */
  var gyp = readFileSync(`${__dirname}/gyp.am`, 'utf8')
    .replace(/{LIB}/g,        lib)
    .replace(/{ARCH}/g,       require('os').arch())
    .replace(/{LIBRARIES}/g,  JSON.stringify(gypi.libraries))
    .replace(/{DEFINES}/g,    defs(f[1]))
    .replace(/{INCLUDES}/g,   includes(lib))
    .replace(/{CFLAGS}/g,     JSON.stringify(flags.cc))
    .replace(/{CPPFLAGS}/g,   JSON.stringify(flags.cpp))
    .replace(/{LDFLAGS}/g,    JSON.stringify(flags.ld))
    .replace(/{SOURCES}/g,    makefile.sources)

  writeFile ( `${__dirname}/${lib}.gyp` , gyp )
}

function libgypi (lib) {
  for (var i in gypi) {
    var I = i.toUpperCase()
    gyp = gyp.replace( new RegExp('\\{'+I+'\\}', 'gi'),
      '{'+I+'}'+', \''+ gypi[i].join('\',\'') + '\'' )
  }
  return gyp
}

function defs (o) {
  return '\'' + o.split('\n')
    .filter(i => /define/.test(i))
    .map(i => i.slice(8).split(/\s/g).join('='))
    .filter(i => i)
    .join('\',\'') + '\''
}

function includes (lib) {
  const f = readFileSync(`${lib}/configure.ac`, 'utf8')
    .split(/AC_CONFIG_FILES/)

  var ret = `'${lib}'`

  // try to find an include directory in configure.ac
  if (f.length-1) {
    var line = f[1].match(/(\S.*?)include/gm)
    if (line) {
      ret += `,'${lib}/${line[0]}'`

      // this one is a bit unexpected, so should probably check if dir exists
      ret += `,'${lib}/${line[0]}/${lib.slice(3)}'`
    }
  }
  return ret
}

function msources (o) {
  const dir = {
    src   : readdirSync (`${__dirname}/${o}`),
    path  : `${o}`,
  }
  if (dir.src.filter(i => i === 'src').length) {
    dir.path = `${o}/src`
    dir.src = readdirSync ( dir.path )
  }
  return makeam ( dir )
}

function makeam (dir) {
  if (dir.src.filter(i => i === 'Makefile.am').length) {
    var file = readFileSync(`${dir.path}/Makefile.am`, 'utf8')
    if (file.match(/SUBDIRS/)) {
      var f = file.match(/\S+$/gm)
      f.shift()
      dir.path += `/${f}`
      file = readFileSync(`${dir.path}/Makefile.am`, 'utf8')
    }
    return readam(file, dir.path)
  }
}

function readam (f, root) {
  var read = 0, sources = '', cflags = '', rd, s
  var srcs = f.split(/la_SOURCES/)
  while (srcs.length - 1) {
    var src = srcs.pop()
    src = src.split(/\n/)
    while (++read) {
      rd = src[read]
      if (rd) {
        rd = rd.match(/(\S.*?)\.[c|S]/g)
        if (rd)
          sources += '\'' + root + '/' + rd + '\','
      } else {
        read = -1
      }
    }
  }

  var flags = f.split(/la_CFLAGS/)
  srcs = flags.length-1 ? flags[1].split(/\n/) : flags
  while (++read) {
    rd = srcs[read]
    if (rd) {
      rd = rd.split(/\\/)[0].replace(/^\s+/g,'')
      if (rd)
        cflags += '\'' + rd + '\','
    } else {
      read = -1
    }
  }
  return {
    sources : sources,
    cc      : cflags.split(/[.',]/).filter(i=>i)
  }
}

function compilerflags (o, flags, gypi, l) {
  var ret = { cc: [], cpp:[], ld: gypi.ldflags}

  o.match(/(FLAGS).*\i?\n?/g).join('')
    .replace(/^\w+=\'/gm,'')
    .replace(/\'|-version-info|19:0:1|-g/g,'')
    .split(/\n|\t/g)
    .map(i => {
      if (i.length)
        i.split(/^\s+/g).filter(j=> j).map(k=> ret.cc.push(k))
    })

  o.match(/(LDFLAGS).*\i?\n?/g).join('')
    .replace(/^\w+=\'/gm,'')
    .replace(/\'||\\'/g,'')
    .split(/\n|\t/g)
    .map(i => {
      if (i.length)
        i.split(/\s+/g).filter(j=> j).map(k=> ret.ld.push(k))
    })

  try {
    ret.cpp = flags.pop().split(/ /)
  } catch ($) {
    ret.cc = (Object.keys(ret.cc.reduce( (cc, el, i) => {
      el = el.split(' ')

      while (el.length) {
        var e = el.pop()
        if(/D/.test(e)) {
          ret.cpp.push(e)
        } else {
          cc[e] = e.length
        }
      }

      return cc
    }, {})))
  }

  ret.cc = process.platform === 'darwin'
    ? ret.cc.concat(flags, gypi.cflags) : ret.cc = gypi.cflags

  return ret
}

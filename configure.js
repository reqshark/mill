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

ø=[
  'libsodium',
  'libmill'
].map(lib => {
  readFile(`${lib}/config.log`, 'utf8', (err, l) => {
    var f = l.split(/## Output variables. ##/)[1].split(/## confdefs.h. ##/)

    var gyp = readFileSync('gyp.am', 'utf8')
    var gypi = require(`./${lib}.gypi`)
    for (var i in gypi) {
      var I = i.toUpperCase()
      var reg = new RegExp('\\{'+I+'\\}', 'gi')
      gyp = gyp.replace( reg, '{'+I+'}'+', \''+ gypi[i].join('\',\'') + '\'' )
    }

    var s = sources(lib)

    switch (lib) {
      case 'libmill':
        if (process.platform !== 'darwin')
          s[1] += `'-lanl', '-lrt',`
        break
    }

    gyp = gyp.replace(/{LIB}/g, lib)
    gyp = gyp.replace(/{ARCH}/g, require('os').arch())
    gyp = gyp.replace(/{INCLUDES}/g, includes(lib))
    gyp = gyp.replace(/{LDFLAGS}/g, ldflags(f[0]))
    gyp = gyp.replace(/{SOURCES}/g, s[0] )
    gyp = gyp.replace(/{CFLAGS}/g, (s[1] ? s[1] : '') + cflags(f[0]) )
    gyp = gyp.replace(/{DEFINES}/g, defs(f[1]))

    writeFile (`${lib}.gyp`, gyp )
  })
})

function sources (o) {
  var src = readdirSync(o), dir = {}
  if (src.filter(i => i === 'src').length) {
    dir.path = `${o}/src`
    src = readdirSync(dir.path)
  } else {
    dir.path = o
  }
  dir.src = src
  return make(dir)
}

function make (dir) {
  if (dir.src.filter(i => i === 'Makefile.am').length) {
    var file = readFileSync(`${dir.path}/Makefile.am`, 'utf8')
    var subdirs = file.match(/SUBDIRS/)
    if (subdirs) {
      var f = file.match(/\S+$/gm)
      f.shift()
      dir.path += `/${f}`
      file = readFileSync(`${dir.path}/Makefile.am`, 'utf8')
    }
    return readMakefile(file, dir.path)
  }
}

function readMakefile (f, root) {
  var read = 0, sources = '', cflags = '', rd, s
  var srcs = f.split(/la_SOURCES/)
  while (srcs.length - 1) {
    var src = srcs.pop()
    src = src.split(/\n/)
    while (++read) {
      rd = src[read]
      if (rd) {
        rd = rd.match(/(\S.*?)\.c/g)
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
  return [sources, cflags]
}

function includes (lib) {
  var ret = '\'' + lib + '\''
  var f = readFileSync(`${lib}/configure.ac`, 'utf8').split(/AC_CONFIG_FILES/)
  if (f.length-1) {
    var line = f[1].match(/(\S.*?)include/gm)
    if (line) {
      ret += `,'${lib}/${line[0]}'`
      ret += `,'${lib}/${line[0]}/${lib.slice(3)}'`
    }
  }
  return ret
}

function cflags (o) {
  return '\'' + o.split('\n')
    .filter(i => /^CFLAGS/.test(i) || /^CPPFLAGS/.test(i) )
    .map(i => i.split("\'")[1].replace(/^\s+/g,''))
    .filter(i => i)
    .join('\',\'') + '\''
}

function ldflags (o) {
  return '\'' + o.split('\n')
    .filter(i => /^LDFLAGS/.test(i))
    .map(i => i.split("\'")[1].replace(/^\s+/g,''))
    .filter(i => i)
    .join('\',\'') + '\''
}

function defs (o) {
  return '\'' + o.split('\n')
    .filter(i => /define/.test(i))
    .map(i => i.slice(8).split(/\s/g).join('='))
    .filter(i => i)
    .join('\',\'') + '\''
}

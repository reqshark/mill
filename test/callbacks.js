module.exports  = callbacks

function callbacks (t) {
  t.test( 'callback styles', cbs)
}

function cbs (t) {
  t.plan(3)

  t.lib.cbStyleA(cbA)
  t.lib.cbStyleB(cbB)
  t.lib.cbStyleC(cbC)

  function cbA (ret) { t.is( ret,  "A", `callback param: ${ret}` ) }
  function cbB (ret) { t.is( ret,  "B", `callback param: ${ret}` ) }
  function cbC (ret) { t.is( ret,  "C", `callback param: ${ret}` ) }
}

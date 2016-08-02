//var r = muduk_pair_conc(0,1, 0, 0, 50,2);
//

function full_conc0(idx0, idx1) {

  var match = 0;
  var loq = 0;
  var tot = 0;

  for (var tilepath_idx=0; tilepath_idx<cgf_info.PathCount; tilepath_idx++) {
    var x = JSON.parse(muduk_pair_conc(idx0, idx1, tilepath_idx, 0, cgf_info.StepPerPath[tilepath_idx], 0));
    match += x.match;
    loq += x.low_quality;
    tot += cgf_info.StepPerPath[tilepath_idx];
  }

  var res = { "match":match, "loq":loq, "tot":tot };
  return res;
}

var res = {};

var n=95;
var s = 247;
for (var idx0=s; idx0<(s+n); idx0++) {
  for (var idx1=idx0+1; idx1<(s+n); idx1++) {
    var r = full_conc0(idx0, idx1);

    res[ idx0 + ":" + idx1 ] = r.match;

  }
}

muduk_return(res);

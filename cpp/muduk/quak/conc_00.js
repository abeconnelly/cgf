//var r = muduk_pair_conc(0,1, 0, 0, 50,2);

var idx0=0;
var idx1=0;

var res = { "match":0, "info":[] };
var match_count = 0;
try {
  for (var tilepath_idx=0; tilepath_idx<cgf_info.PathCount; tilepath_idx++) {
    var x = JSON.parse(muduk_pair_conc(idx0, idx1, tilepath_idx, 0, cgf_info.StepPerPath[tilepath_idx], 2));
    var y = {
      "tilepath": tilepath_idx,
      "match":x.match,
      "loq":x.low_quality,
      "n":cgf_info.StepPerPath[tilepath_idx]
    };

    res.info.push(y);

    match_count += x.match;
  }

  res.match = match_count;

} catch(err) {
  res = err;
}

muduk_return(res);

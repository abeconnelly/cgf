//var r = muduk_pair_conc(0,1, 0, 0, 50,2);

var res = { "match":0, "info":[] };
var match_count = 0;
try {
  for (var idx=0; idx<cgf_info.PathCount; idx++) {
    var x = JSON.parse(muduk_pair_conc(0,1, idx, 0, cgf_info.StepPerPath[idx], 2));
    var y = {
      "tilepath": idx,
      "match":x.match,
      "loq":x.low_quality,
      "n":cgf_info.StepPerPath[idx]
    };

    res.info.push(y);

    match_count += x.match;
  }

  res.match = match_count;

} catch(err) {
  res = err;
}

muduk_return(res);

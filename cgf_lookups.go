//package main
package cgf

import _ "fmt"

func _skip_fofsi(vi []int) int {
  pos := 0

  step := vi[pos] ; pos++ ; _ = step
  n := vi[pos] ; pos++

  //fmt.Printf("n %d (step %x)\n", n, step)

  for i:=0; i<n; i++ {
    l := vi[pos] ; pos++

    //fmt.Printf("  [%d] l%d\n", i, l)

    for j:=0; j<l; j++ {
      vid := vi[pos] ; pos++
      span := vi[pos] ; pos++

      //fmt.Printf("  %d+%d", vid, span)

      _ = vid
      _ = span
    }

    //fmt.Printf("\n")

  }


  return pos
}

//func _fofsi_knot(vid []int) (cgfintermediate,int) {
func _fofsi_knot(vid []int) (CGFIntermediate,int) {
  pos := 0

  step := vid[pos] ; pos++ ; _ = step
  n := vid[pos] ; pos++

  //knot := cgfintermediate{}
  knot := CGFIntermediate{}
  _init_knot(&knot)

  for i:=0; i<n; i++ {
    l := vid[pos] ; pos++
    _ = l

    for j:=0; j<l; j++ {
      varid := vid[pos] ; pos++
      span := vid[pos] ; pos++

      knot.varid[i] = append(knot.varid[i], varid)
      knot.span[i] = append(knot.span[i], span)
    }

  }


  return knot,pos
}

//func _fill_knot_loq(tia [][]TileInfo, pathi pathintermediate, anchor_step int) {
func _fill_knot_loq(tia [][]TileInfo, pathi PathIntermediate, anchor_step int) {
  if cgfi,ok := pathi.loqi.loqi_info[anchor_step] ; ok {
    for allele:=0; allele<len(cgfi.nocall_start_len); allele++ {
      for idx:=0; idx<len(cgfi.nocall_start_len[allele]); idx++ {
        tia[allele][idx].NocallStartLen = append(tia[allele][idx].NocallStartLen, cgfi.nocall_start_len[allele][idx]...)
      }
    }
  }
}

//func get_knot(tilemap []TileMapEntry, pathi pathintermediate, anchor_step int) [][]TileInfo {
func GetKnot(tilemap []TileMapEntry, pathi PathIntermediate, anchor_step int) [][]TileInfo {
  tia := make([][]TileInfo, 2)
  tia[0] = make([]TileInfo, 0, 1)
  tia[1] = make([]TileInfo, 0, 1)

  vec_slice := anchor_step/32
  m := anchor_step%32

  if (pathi.veci[vec_slice] & (1<<uint(32+m))) == 0 {
    ti:=TileInfo{}
    ti.Step = anchor_step
    ti.Span = tilemap[0].Span[0][0]
    ti.VarId = tilemap[0].Variant[0][0]

    tia[0] = append(tia[0], ti)
    tia[1] = append(tia[1], ti)

    _fill_knot_loq(tia, pathi, anchor_step)
    return tia
  }

  cache_counter := 0
  for i:=0; i<m; i++ {
    if (pathi.veci[vec_slice] & (1<<uint(32+i))) != 0 {
      cache_counter++
    }
  }

  hexit:=0
  if (cache_counter < 8) {
    hexit = int((pathi.veci[vec_slice] & (0xf<<uint(4*cache_counter))) >> uint(4*cache_counter))

    if hexit == 0 { return nil }

    if hexit < 0xd {
      for allele:=0; allele<2; allele++ {
        run_span:=0
        for i:=0; i<len(tilemap[hexit].Variant[allele]); i++ {
          ti:=TileInfo{}
          ti.Step = anchor_step + run_span
          ti.Span = tilemap[hexit].Span[allele][i]
          ti.VarId = tilemap[hexit].Variant[allele][i]
          tia[allele] = append(tia[allele], ti)

          run_span += ti.Span
        }
      }
      _fill_knot_loq(tia, pathi, anchor_step)
      return tia
    }

  }

  loq_flag := false ; _ = loq_flag
  if hexit == 0xe { loq_flag = true }

  ovf_pos := CountOverflowVectorUint64(pathi.veci, 0, anchor_step)

  if pathi.ofsi.span_flag[ovf_pos] { return nil }

  if !pathi.ofsi.final_overflow_flag[ovf_pos] {
    if cache_counter < 8 {
    } else {
      if pathi.loqi.loq_flag[anchor_step] { loq_flag = true }
    }

    tm := pathi.ofsi.TileMap[ovf_pos]
    for allele:=0; allele<2; allele++ {
      run_span:=0
      for i:=0; i<len(tilemap[tm].Variant[allele]); i++ {
        ti:=TileInfo{}
        ti.Step = anchor_step+run_span
        ti.Span = tilemap[tm].Span[allele][i]
        ti.VarId = tilemap[tm].Variant[allele][i]
        tia[allele] = append(tia[allele], ti)

        run_span+=ti.Span
      }
    }


    _fill_knot_loq(tia, pathi, anchor_step)
    return tia
  }

  if pathi.loqi.loq_flag[anchor_step] { loq_flag = true }

  cur_pos := 0
  for i:=0; i<len(pathi.fofsi.tilepos); i++ {
    if pathi.fofsi.tilepos[i] == anchor_step {
      knot,_ := _fofsi_knot(pathi.fofsi.variant_ints[cur_pos:])

      for allele:=0; allele<2; allele++ {
        run_span:=0
        for i:=0; i<len(knot.varid[allele]); i++ {
          ti:=TileInfo{}
          ti.Step = anchor_step+run_span
          ti.Span = knot.span[allele][i]
          ti.VarId = knot.varid[allele][i]
          tia[allele] = append(tia[allele], ti)

          run_span += ti.Span
        }
      }

      _fill_knot_loq(tia, pathi, anchor_step)
      return tia
    }

    dn := _skip_fofsi(pathi.fofsi.variant_ints[cur_pos:])
    cur_pos += dn
  }

  return nil
}

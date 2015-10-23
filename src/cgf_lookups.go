package main

import "fmt"

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

func _fofsi_knot(vid []int) (cgfintermediate,int) {
  pos := 0

  step := vid[pos] ; pos++ ; _ = step
  n := vid[pos] ; pos++

  knot := cgfintermediate{}
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

func get_knot(pathi pathintermediate, anchor_step int) {

  // DEBUG
  //====
  fmt.Printf("GET_KNOT (%x)\n", anchor_step)


  vec_slice := anchor_step/32
  m := anchor_step%32

  if (pathi.veci[vec_slice] & (1<<uint(32+m))) == 0 {
    fmt.Printf("canonical tile\n")
    return
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

    if hexit == 0 {
      fmt.Printf("span tile\n")
      return
    }

    if hexit < 0xd {
      fmt.Printf("cache mapped %d\n", hexit)
      return
    }

  }

  loq_flag := false
  if hexit == 0xe { loq_flag = true }

  fmt.Printf(">>>> cache_counter %d, hexit %d\n", cache_counter, hexit)

  ovf_pos := CountOverflowVectorUint64(pathi.veci, 0, anchor_step)

  if pathi.ofsi.span_flag[ovf_pos] {
    fmt.Printf(" oveflow spanning tile (loq %v)\n", loq_flag)
    return
  }

  if !pathi.ofsi.final_overflow_flag[ovf_pos] {
    if cache_counter < 8 {
      fmt.Printf(" overflow: tilemap %d (loq %v)\n", pathi.ofsi.tilemap[ovf_pos], loq_flag)
    } else {

      if pathi.loqi.loq_flag[anchor_step] { loq_flag = true }
      fmt.Printf(" overflow: tilemap %d (loq %v)\n", pathi.ofsi.tilemap[ovf_pos], loq_flag)
    }
    return
  }

  if pathi.loqi.loq_flag[anchor_step] { loq_flag = true }

  cur_pos := 0
  for i:=0; i<len(pathi.fofsi.tilepos); i++ {
    if pathi.fofsi.tilepos[i] == anchor_step {
      knot,_ := _fofsi_knot(pathi.fofsi.variant_ints[cur_pos:])
      fmt.Printf(" final oveflow: (loq %v) %v\n", loq_flag, knot)
      return
    }

    dn := _skip_fofsi(pathi.fofsi.variant_ints[cur_pos:])
    cur_pos += dn
  }


  fmt.Printf("error\n")


}

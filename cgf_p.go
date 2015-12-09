package cgf

import "fmt"
import "github.com/abeconnelly/dlug"

func CGFBegPeel(cgf_bytes []byte) (magic uint64,
  cgfver []byte, libver []byte, path_n uint64, tmap_len uint64, tmap []byte, step_per_path_b []byte, path_off_b []byte, path_b []byte) {
  n:=uint64(0)

  magic = byte2uint64(cgf_bytes[n:n+8])
  n+=8

  _n0,dn := dlug.ConvertUint64(cgf_bytes[n:])
  n+=uint64(dn)

  cgfver = cgf_bytes[n:n+_n0]
  n+=_n0

  _n1,dn := dlug.ConvertUint64(cgf_bytes[n:])
  n+=uint64(dn)

  libver = cgf_bytes[n:n+_n1]
  n+=_n1

  path_n = byte2uint64(cgf_bytes[n:n+8])
  n+=8

  tmap_len = byte2uint64(cgf_bytes[n:n+8])
  n+=8

  tmap = cgf_bytes[n:n+tmap_len]
  n+=tmap_len

  step_per_path_b = cgf_bytes[n:n+8*path_n]
  n+=8*path_n

  path_off_b = cgf_bytes[n:n+8*(path_n+1)]
  n+=8*(path_n+1)

  path_b = cgf_bytes[n:]

  return

}

func PathBegInfo(path_bytes []byte) ([]byte, uint64, []byte, uint64) {
  n:=uint64(0)

  // Skip name (length + string bytes)
  //
  name_n,dn := dlug.ConvertUint64(path_bytes[n:])
  n+=uint64(dn)

  name := path_bytes[n:n+name_n]
  n+=name_n

  ntile := byte2uint64(path_bytes[n:n+8])
  n+=8

  veclen := (ntile+31)/32
  vec_bytes := path_bytes[n:n+8*veclen]
  n+=8*veclen

  return name, ntile, vec_bytes, n
}

func PathOvfPeel(ovf_bytes []byte) (uint64, uint64,uint64, []byte, []byte, []byte, uint64) {
  n:=uint64(0)

  ovf_len := byte2uint64(ovf_bytes[n:n+8])
  n+=8

  stride := byte2uint64(ovf_bytes[n:n+8])
  n+=8

  mapbcount := byte2uint64(ovf_bytes[n:n+8])
  n+=8

  offset_byte_len := 8*((ovf_len + stride - 1) / stride)

  offset_bytes := ovf_bytes[n:n+offset_byte_len]
  n+=offset_byte_len

  pos_bytes := ovf_bytes[n:n+offset_byte_len]
  n+=offset_byte_len

  map_bytes := ovf_bytes[n:n+mapbcount]
  n+=mapbcount

  return ovf_len, stride, mapbcount, offset_bytes, pos_bytes, map_bytes, n
}

// Get overflow bytes for step in path bytes
func Ovf_b(path_bytes []byte, step_idx int) []byte {
  n:=uint64(0)

  pathname, ntile, vec_bytes, dn := PathBegInfo(path_bytes[n:])
  n+=dn

  ovf_len, stride, mapbcount, off_b, pos_b, map_b, dn := PathOvfPeel(path_bytes[n:])
  n+=dn

  fmt.Printf("name: %s, ntile %d, vec_bytes[%d]\n", pathname, ntile, len(vec_bytes))
  fmt.Printf("ovf_len: %d, stride: %d, mapbcount: %d, offset[%d], pos[%d], map[%d]\n",
    ovf_len, stride, mapbcount, len(off_b), len(pos_b), len(map_b))


  return nil

}

func PathFinalOverflowKnotFromBytes(rec_n uint64, fin_ovf_bytes []byte, step uint64) (knot_zipper [][]int) {
  knot_zipper = make([][]int, 2)

  code_bytes := fin_ovf_bytes[0:rec_n]
  data_bytes := fin_ovf_bytes[rec_n:]
  dat_n:=0

  for i:=uint64(0); i<rec_n; i++ {

    if code_bytes[i] != 0 { continue }

    ele_anchor_step,dn := dlug.ConvertUint64(data_bytes[dat_n:])
    dat_n += dn

    if ele_anchor_step > step { return nil }

    ele_nallele,dn := dlug.ConvertUint64(data_bytes[dat_n:])
    dat_n += dn

    for a:=uint64(0); a<ele_nallele; a++ {
      ele_knot_allele_len,dn := dlug.ConvertUint64(data_bytes[dat_n:])
      dat_n += dn

      for knot_allele:=uint64(0); knot_allele<ele_knot_allele_len; knot_allele++ {
        ele_var_id,dn := dlug.ConvertUint64(data_bytes[dat_n:])
        dat_n+=dn

        ele_span,dn := dlug.ConvertUint64(data_bytes[dat_n:])
        dat_n+=dn

        if ele_anchor_step == step {
          knot_zipper[a] = append(knot_zipper[a], int(ele_var_id))
          knot_zipper[a] = append(knot_zipper[a], int(ele_span))
        }

      }
    }

    if ele_anchor_step == step { break }

  }

  return
}

func PathFinalOverflowFastJFromBytes(fin_ovf_bytes []byte, step int) (string) {
  return ""
}

func PathFinalOverflowPeel(fin_ovf_bytes []byte) (fin_ovf_rec_n uint64, fin_ovf_rec_byte_len uint64, fin_ovf_rec_bytes []byte, dn uint64) {
  dn = 0

  fin_ovf_rec_n = byte2uint64(fin_ovf_bytes[dn:dn+8])
  dn+=8

  fin_ovf_rec_byte_len = byte2uint64(fin_ovf_bytes[dn:dn+8])
  dn+=8

  fin_ovf_rec_bytes = fin_ovf_bytes[dn:dn+fin_ovf_rec_byte_len]
  dn += fin_ovf_rec_byte_len

  return
}

func PathLowQualityPeel(loq_info_bytes []byte) (loq_count uint64,
  loq_code uint64, loq_stride uint64,
  loq_offset_bytes []byte, loq_step_pos_bytes []byte, loq_hom_flag_bytes []byte,
  loq_aux_flag_byte_count uint64, loq_aux_flag_bytes []byte,
  loq_info_byte_count uint64, loq_bytes []byte, dn uint64) {

  dn=0

  offs := make([]uint64, 0, 32)
  spos := make([]uint64, 0, 32)

  loq_count = byte2uint64(loq_info_bytes[dn:])
  dn+=8

  //DEBUG
  fmt.Printf("loq_count %d\n", loq_count)

  loq_code = byte2uint64(loq_info_bytes[dn:])
  dn+=8

  //DEBUG
  fmt.Printf("loq_code %d\n", loq_code)

  loq_stride = byte2uint64(loq_info_bytes[dn:])
  dn+=8

  //DEBUG
  fmt.Printf("loq_stride %d\n", loq_stride)

  z := (loq_count + loq_stride - 1) / loq_stride

  //DEBUG
  fmt.Printf(">>>> z %d\n", z)
  fmt.Printf("len %d, ... %d\n", len(loq_info_bytes[dn:]), 8*z)

  loq_offset_bytes = loq_info_bytes[dn:dn+8*z]
  dn+=8*z

  //DEBUG
  //
  for i:=uint64(0); i<8*z; i+=8 {
    t := byte2uint64(loq_offset_bytes[i:])
    offs = append(offs, t)
  }
  //fmt.Printf("loq_offset_bytes: %v\n", loq_offset_bytes)
  fmt.Printf("loq_offset: %v\n", offs)
  //
  //DEBUG


  loq_step_pos_bytes = loq_info_bytes[dn:dn+8*z]
  dn+=8*z

  //DEBUG
  //
  for i:=uint64(0); i<8*z; i+=8 {
    t := byte2uint64(loq_step_pos_bytes[i:])
    spos = append(spos, t)
  }
  //fmt.Printf("loq_step_pos_bytes: %v\n", loq_step_pos_bytes)
  fmt.Printf("loq_step_pos: %v\n", spos)
  //
  //DEBUG

  zz := (loq_count+7)/8
  loq_hom_flag_bytes = loq_info_bytes[dn:dn+zz]
  dn+=zz

  loq_aux_flag_byte_count = byte2uint64(loq_info_bytes[dn:])
  dn+=8

  //DEBUG
  fmt.Printf("loq_hom_flag_byte_count %d\n", loq_aux_flag_byte_count)

  loq_aux_flag_bytes = loq_info_bytes[dn:dn+loq_aux_flag_byte_count]
  dn+=loq_aux_flag_byte_count

  loq_info_byte_count = byte2uint64(loq_info_bytes[dn:])
  dn+=8

  //DEBUG
  fmt.Printf("loq_info_byte_count %d\n", loq_info_byte_count)

  loq_bytes = loq_info_bytes[dn:]

  return
}


func PathOverflowPeel(ovf_bytes []byte) (ovf_len uint64, ovf_stride uint64, ovf_mbc uint64, ovf_off_b []byte, ovf_pos_b []byte, ovf_map_b[]byte , ovf_dn uint64) {
  ovf_dn = 0

  ovf_len = byte2uint64(ovf_bytes[ovf_dn:ovf_dn+8])
  ovf_dn+=8

  ovf_stride = byte2uint64(ovf_bytes[ovf_dn:ovf_dn+8])
  ovf_dn+=8

  ovf_mbc = byte2uint64(ovf_bytes[ovf_dn:ovf_dn+8])
  ovf_dn+=8

  n_off := (ovf_len + ovf_stride - 1)/ovf_stride

  ovf_off_b = ovf_bytes[ovf_dn:ovf_dn+8*n_off]
  ovf_dn+=8*n_off

  ovf_pos_b = ovf_bytes[ovf_dn:ovf_dn+8*n_off]
  ovf_dn+=8*n_off

  ovf_map_b = ovf_bytes[ovf_dn:ovf_dn+ovf_mbc]
  ovf_dn+=ovf_mbc

  return
}

func PathLowQualityKnotZipper(loq_info_bytes []byte, anchor_step uint64) [][][]int {
  loq_knot := make([][][]int, 2)

  loq_rec_count, loq_code, loq_stride, loq_offset_bytes, loq_step_pos_bytes, loq_hom_flag_bytes,
    loq_aux_flag_count, loq_aux_flag_bytes, loq_info_byte_count, loq_bytes,dn :=
    PathLowQualityPeel(loq_info_bytes)

  _ = loq_rec_count
  _ = loq_code
  _ = loq_stride
  _ = loq_offset_bytes
  _ = loq_step_pos_bytes
  _ = loq_hom_flag_bytes
  _ = loq_aux_flag_count
  _ = loq_aux_flag_bytes
  _ = loq_info_byte_count
  _ = loq_bytes
  _ = dn

  //fmt.Printf(">>> loq_rec_count %d\n", loq_rec_count)

  _bpos := _bsrch8(loq_step_pos_bytes, anchor_step)
  base_step := byte2uint64(loq_step_pos_bytes[_bpos:_bpos+8]) ; _ = base_step
  byte_offset := byte2uint64(loq_offset_bytes[_bpos:_bpos+8]) ; _ = byte_offset

  _pos := _bpos/8

  loq_count := uint64(0)
  /*
  for s:=base_step ; s<anchor_step; s++ {
    if loq_aux_flag_bytes[s/8] & (1<<uint(s%8)) > 0 { loq_count++ }
  }
  */
  for s:=base_step ; s<anchor_step; s++ {
    if loq_aux_flag_bytes[s/8] & (1<<uint(s%8)) > 0 { loq_count++ }
  }

  /*
  fmt.Printf(">>> _bpos %d (%d), base_step %d, byte_offset %d, loq_count %d\n",
    _bpos, _bpos/8, base_step, byte_offset, loq_count)
  fmt.Printf(">>> loq_base_pos %d\n", loq_stride*_bpos)
  */

  loq_base_pos := loq_stride * _pos
  for i:=uint64(0); i<=loq_count; i++ {
    loq_pos := loq_base_pos + i
    hom_flag := false

    if loq_hom_flag_bytes[loq_pos/8] & (1<<uint(loq_pos%8)) > 0 { hom_flag = true }

    //DEBUG
    //fmt.Printf(">>>> loq_pos %d [%d,%d] hom %v\n", loq_pos, loq_pos/8, loq_pos%8, hom_flag)


    if hom_flag {

      ntile,dn := dlug.ConvertUint64(loq_bytes[byte_offset:])
      byte_offset+=uint64(dn)

      if i==loq_count {
        loq_knot[0] = make([][]int, ntile)
        loq_knot[1] = make([][]int, ntile)
      }

      //DEBUG
      //fmt.Printf(" ntile %d\n", ntile)

      for tile_idx:=uint64(0); tile_idx<ntile; tile_idx++ {
        ent_len,dn := dlug.ConvertUint64(loq_bytes[byte_offset:])
        byte_offset+=uint64(dn)


        //DEBUG
        //fmt.Printf("  t%d[%d]", tile_idx, ent_len)

        for ent_idx:=uint64(0); ent_idx<ent_len; ent_idx+=2 {
          delpos,dn := dlug.ConvertUint64(loq_bytes[byte_offset:]) ; _ = delpos
          byte_offset+=uint64(dn)

          loqlen,dn := dlug.ConvertUint64(loq_bytes[byte_offset:]) ; _ = loqlen
          byte_offset+=uint64(dn)

          //DEBUG
          //fmt.Printf(" {%d+%d}", delpos, loqlen)

          if i==loq_count {
            loq_knot[0][tile_idx] = append(loq_knot[0][tile_idx], int(delpos))
            loq_knot[0][tile_idx] = append(loq_knot[0][tile_idx], int(loqlen))

            loq_knot[1][tile_idx] = append(loq_knot[1][tile_idx], int(delpos))
            loq_knot[1][tile_idx] = append(loq_knot[1][tile_idx], int(loqlen))
          }

        }

        //DEBUG
        //fmt.Printf("\n")

      }

    } else {  // het
      var dn int
      ntile := [2]uint64{0,0}

      ntile[0],dn = dlug.ConvertUint64(loq_bytes[byte_offset:])
      byte_offset+=uint64(dn)

      ntile[1],dn = dlug.ConvertUint64(loq_bytes[byte_offset:])
      byte_offset+=uint64(dn)

      //DEBUG
      //fmt.Printf(" ntile [%d,%d]\n", ntile[0], ntile[1])

      if i==loq_count {
        loq_knot[0] = make([][]int, ntile[0])
        loq_knot[1] = make([][]int, ntile[1])
      }

      for zz:=0; zz<2; zz++ {
        for tile_idx:=uint64(0); tile_idx<ntile[zz]; tile_idx++ {
          ent_len,dn := dlug.ConvertUint64(loq_bytes[byte_offset:])
          byte_offset+=uint64(dn)

          //DEBUG
          //fmt.Printf(" t(%d)%d[%d]", zz, tile_idx, ent_len)

          for ent_idx:=uint64(0); ent_idx<ent_len; ent_idx+=2 {
            delpos,dn := dlug.ConvertUint64(loq_bytes[byte_offset:]) ; _ = delpos
            byte_offset+=uint64(dn)

            loqlen,dn := dlug.ConvertUint64(loq_bytes[byte_offset:]) ; _ = loqlen
            byte_offset+=uint64(dn)

            //DEBUG
            //fmt.Printf(" {%d+%d}", delpos, loqlen)

            if i==loq_count {
              loq_knot[zz][tile_idx] = append(loq_knot[zz][tile_idx], int(delpos))
              loq_knot[zz][tile_idx] = append(loq_knot[zz][tile_idx], int(loqlen))
            }

          }

          //DEBUG
          //fmt.Printf("\n")

        }
      }

    }
  }

  return loq_knot

}


func Peel(cgf_bytes []byte, path, step int) {
  b8 := make([]byte, 8)
  n:=uint64(0)

  //===========
  // CGF HEADER
  //
  magic, cgfver, libver, path_n, tmap_len, tmap, step_per_path_b, path_off_b, path_b := CGFBegPeel(cgf_bytes)

  //DEBUG
  //
  tobyte64(b8, magic)
  fmt.Printf("magic:  %08x %v (%s)\n", magic, b8, b8)

  fmt.Printf("cgfver: %s\n", cgfver)
  fmt.Printf("libver: %s\n", libver)

  fmt.Printf("path_n: %d\n", path_n)
  fmt.Printf("tmap_len: %d (%d)\n", tmap_len, len(tmap))
  fmt.Printf("step_per_path (%d bytes)\n", len(step_per_path_b))
  fmt.Printf("path_off_b (%d bytes)\n", len(path_off_b))
  fmt.Printf("path_b (%d bytes)\n", len(path_b))
  //
  //DEBUG

  path_b_s := byte2uint64(path_off_b[path*8:])

  //DEBUG
  //
  fmt.Printf("path_b_s %d\n", path_b_s)
  //
  //DEBUG

  path_bytes := path_b[path_b_s:]

  //DEBUG
  //
  fmt.Printf(">>> path %x, s %x, e %x\n", path, path_b_s, -1)
  //
  //DEBUG

  //============
  // PATH HEADER
  //
  pathname, ntile, vec_bytes, dn := PathBegInfo(path_bytes)
  n += dn

  //DEBUG
  fmt.Printf("PathBeg '%s' %v %v %v (+%v)\n", pathname, ntile, len(vec_bytes), n, dn)

  ovf_bytes := path_bytes[n:]


  vec_byte_pos := step/32
  vec_byte_pos *= 8

  step_off := step%32

  vec_val := byte2uint64(vec_bytes[vec_byte_pos:])
  canon_bit := (vec_val&(1<<(32+uint(step_off))))

  //DEBUG
  fmt.Printf("... vec_val %8x %8x\n", vec_val>>32, vec_val & 0xffffffff )
  noncan_count:=0
  for i:=0; i<step_off; i++ {
    fmt.Printf("(%d) %x %v\n", i, vec_val&(1<<(32+uint(i))), vec_val&(1<<(32+uint(step_off))) > 0)
    if vec_val&(1<<(32+uint(i))) > 0 { noncan_count++ }
  }
  zval := uint64(0)
  if noncan_count<8 {
    zval = vec_val & (0xf<<(4*uint(noncan_count)))
  }
  fmt.Printf("... ......b %8x %8x noncan_count %d, step_off %d\n", (vec_val&(1<<(32+uint(step_off))))>>32,
    zval, noncan_count, step_off)

  if canon_bit==0 {

    //DEBUG
    fmt.Printf("%x.%x canon (0,0)\n", path, step)

    return
  }

  cache_map_val := CacheMapVal(vec_val, uint(step_off))

  if cache_map_val==0 {

    //DEBUG
    fmt.Printf("%x.%x spanning (*)\n", path, step)

    return
  }

  if cache_map_val>0 && cache_map_val<0xd {

    //DEBUG
    fmt.Printf("%x.%x cache (%x)\n", path, step, cache_map_val)

    return
  }

  //DEBUG
  if cache_map_val == 0xd { fmt.Printf("complex (%x)\n", cache_map_val) }

  if cache_map_val == 0xd { return }

  //==============
  // OVERFLOW INFO
  //
  ovf_len, ovf_stride, ovf_mbc, ovf_off_b, ovf_pos_b, ovf_map_b, ovf_dn :=
    PathOverflowPeel(ovf_bytes)
  n += ovf_dn

  fin_ovf_bytes := path_bytes[n:]

  fin_ovf_rec_n, fin_ovf_byte_len, fin_ovf_record_bytes,dn :=
    PathFinalOverflowPeel(fin_ovf_bytes)
  n+=dn

  loq_info_bytes := path_bytes[n:]
  loq_count, loq_code, loq_stride, loq_offset_bytes, loq_step_pos_bytes, loq_hom_flag_bytes,
    loq_aux_flag_count, loq_aux_flag_bytes, loq_info_byte_count, loq_bytes,dn :=
    PathLowQualityPeel(loq_info_bytes)

  _ = loq_bytes

  fmt.Printf("loq(count %d, code %d, stride %d, aux_flag_bc %d, byte_count %d)\n",
    loq_count, loq_code, loq_stride, loq_aux_flag_count, loq_info_byte_count)
  fmt.Printf("loq_offs: %v\n", loq_offset_bytes)
  fmt.Printf("loq_step: %v\n", loq_step_pos_bytes)
  fmt.Printf("loq_homf: %v\n", loq_hom_flag_bytes)
  fmt.Printf("loq_auxf: %v\n", loq_aux_flag_bytes)



  //DEBUG
  if cache_map_val==-1 {
  }

  loq_flag := false

  //DEBUG
  //
  off_z := (ovf_len + ovf_stride - 1) / ovf_stride
  off64 := make([]uint64, off_z)
  pos64 := make([]uint64, off_z)
  for i:=uint64(0); i<off_z; i++ {
    off64[i] = byte2uint64(ovf_off_b[8*i:])
    pos64[i] = byte2uint64(ovf_pos_b[8*i:])
  }
  fmt.Printf("  off: %v\n  pos: %v\n", off64, pos64)
  //
  //DEBUG



  //DEBUG
  //
  fmt.Printf("OvfBeg %v %v %v %v %v %v\n",
    ovf_len, ovf_stride, ovf_mbc, len(ovf_off_b), len(ovf_pos_b), len(ovf_map_b))
  //
  //DEBUG

  _bpos := _bsrch8(ovf_pos_b, uint64(step))
  pos_entry := byte2uint64(ovf_pos_b[_bpos:_bpos+8])
  map_offset_b := byte2uint64(ovf_off_b[_bpos:_bpos+8])

  //DEBUG
  fmt.Printf("cache ovf: step %d, _bpos %d, pos_entry %d, map_offset_b %d\n",
    step, _bpos, pos_entry, map_offset_b)

  del_overflow := RelativeOvfCount(vec_bytes, pos_entry, uint64(step))

  fmt.Printf("del_overflow: %d\n", del_overflow)

  map_val := uint64(0)
  for ovf_entry:=0 ; ovf_entry < del_overflow ; ovf_entry++ {
    var dn int
    map_val,dn = dlug.ConvertUint64(ovf_map_b[map_offset_b:])
    map_offset_b+=uint64(dn)

    fmt.Printf("... ovf_entry %v (del_overlfow %v), map_val %v\n", ovf_entry, del_overflow, map_val)
  }

  //fin_ovf_rec_n, fin_ovf_byte_len, fin_ovf_record_bytes,dn :=
  //  PathFinalOverflowPeel(fin_ovf_bytes)
  //n+=dn

  //loq_count,loq_code,loq_stride,loq_offset_bytes,loq_step_pos_bytes,loq_hom_flag_bytes,loq_info_byte_count,loq_bytes,dn :=
  //  PathLowQualityPeel(loq_info_bytes)

  if map_val==1024 {

    // spanning
    //
    fmt.Printf("Spanning? (map_val %v)\n", map_val)
    return

  } else if map_val==1025 {

    // Final overflow lookup
    //
    fmt.Printf("FinalOvf? (map_val %v)\n", map_val)

    _ = fin_ovf_record_bytes

    knot_zipper := PathFinalOverflowKnotFromBytes(fin_ovf_rec_n, fin_ovf_bytes, uint64(step))

    if knot_zipper != nil {

      //DEBUG
      fmt.Printf("fin_ovf_len %d, fin_ovf_rec_n %d, fin_ovf_byte_len %d\n",
        fin_ovf_rec_n, fin_ovf_byte_len, dn)
      fmt.Printf("cache overflow (%d)\n", cache_map_val)
      fmt.Printf("knot_zipper: %v\n", knot_zipper)

    } else {

      // parse fastj
      //

      fmt.Printf("FastJ output?\n")

    }

  }

  //var _dn int
  //map_val,_dn = dlug.ConvertUint64(ovf_map_b[map_offset_b:])
  //map_offset_b+=uint64(_dn)

  //DEBUG
  fmt.Printf(">>>>> map_val %v, pos_entry %v, map_ffset_b %v\n", map_val, pos_entry, map_offset_b)

  if cache_map_val==-1 {


    /*
    for pos_entry < step {

      v,dn := dlug.ConvertUint64(ovf_map_b[map_offset_b:])
      map_offset_b+=dn

      pos_entry++
    }
    */


  } else {

    if cache_map_val == 0xe { loq_flag = true }

    //DEBUG
    fmt.Printf("... (%x) (loq %v)\n", cache_map_val, loq_flag)

  }

  // LOW QUALITY INFO
  loq_flag = false
  loq_q := step / 8
  loq_r := step % 8
  if loq_aux_flag_bytes[loq_q] & (1<<uint(loq_r)) > 0 { loq_flag = true }

  if loq_flag { fmt.Printf("LOWQ\n") }

  if loq_flag {
    loq_knot_zipper := PathLowQualityKnotZipper(loq_info_bytes, uint64(step))

    for i:=0; i<len(loq_knot_zipper); i++ {
      for j:=0; j<len(loq_knot_zipper[i]); j++ {
        fmt.Printf("[%d][%d]", i, j)
        for k:=0; k<len(loq_knot_zipper[i][j]); k+=2 {
          fmt.Printf(" {%d+%d}", loq_knot_zipper[i][j][k],  loq_knot_zipper[i][j][k+1])
          //fmt.Printf("[%d][%d][%d] %d\n", i, j, k, loq_knot_zipper[i][j][k])
        }
        fmt.Printf("\n")
      }
    }
  }

  //DEBUG
  fmt.Printf("\n")

}

// Count overflow entries from step_start to step_end (inclusive)
//
// Broadly, there are two conditions where it could overflow:
//   1. The cached value is an overflow value (either low or high quality)
//   2. The number of non canonical entries exceed the cache size
//
func RelativeOvfCount(vec_bytes []byte, step_start, step_end uint64) (ovf_count int) {

  for cur_step := step_start ; cur_step <= step_end ; cur_step++ {

    vec_byte_pos := cur_step/32
    vec_byte_pos *= 8
    step_off := cur_step%32


    vec_val := byte2uint64(vec_bytes[vec_byte_pos:])
    canon_bit := (vec_val&(1<<(32+uint(step_off))))

    if canon_bit==0 { continue }
    cache_map_val := CacheMapVal(vec_val, uint(step_off))
    if cache_map_val==0 { continue }
    if cache_map_val>0 && cache_map_val<0xd { continue }

    // complex, skip for now
    //
    if cache_map_val == 0xd { continue }

    // not canoninical, not complex,
    // not cached, it's overflowed,
    // increment counter
    //
    ovf_count++

  }

  return
}

func _bsrch8(b []byte, val uint64) uint64 {
  beg_pos := uint64(0)
  n := uint64(len(b)/8)

  for n>1 {
    mid_pos := beg_pos + (n/2)
    tval := byte2uint64(b[mid_pos*8:])
    if tval<=val {
      beg_pos = mid_pos
      eo := n%2
      n = (n/2) + eo
    } else {
      n = (n/2)
    }
  }

  return 8*beg_pos
}



func CacheMapVal(vec_val uint64, offset uint) int {
  canon_bit := (vec_val&(1<<(32+uint(offset))))
  if canon_bit==0 { return 0 }

  count:=uint(0)
  for i:=uint(0); i<offset; i++ {
    if vec_val&(1<<(32+i)) != 0 { count++ }
  }

  //fmt.Printf("... %8x %8x\n", vec_val>>32, vec_val & 0xffffffff )

  // Overflow
  //
  if count >= 8 { return -1 }

  hexit := (vec_val & (0xf<<(count*4))) >> (count*4)

  return int(hexit)
}

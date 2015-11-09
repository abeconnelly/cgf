//package main
package cgf


import "fmt"
import "io/ioutil"
//import "./dlug"
import "github.com/abeconnelly/dlug"

//func headerintermediate_debug_print(hdri headerintermediate) {
func HeaderIntermediateDebugPrint(hdri HeaderIntermediate) {
  fmt.Printf("Header intermediate\n")
  fmt.Printf("Magic: %v\n", hdri.magic)
  fmt.Printf("ver: %s\n", hdri.ver)
  fmt.Printf("libver: %s\n", hdri.libver)
  fmt.Printf("pathcount: %d\n", hdri.pathcount)

  fmt.Printf("StepPerPath[%d]:", len(hdri.StepPerPath))
  fmt.Printf(" %v\n", hdri.StepPerPath)

  fmt.Printf("path_offset[%d]:", len(hdri.path_offset))
  fmt.Printf("%v\n", hdri.path_offset)
}

//func debug_read(ifn string) error {
func DebugRead(ifn string) error {
  b,e := ioutil.ReadFile(ifn)
  if e!=nil { return e }
  return debug_unpack_bytes(b)
}

func debug_print_loq_bytes(loq_bytes []byte) (int,error) {
  var dummy uint64
  var dn int ; _ = dn
  n:=0

  dummy = byte2uint64(loq_bytes[n:n+8])
  n+=8
  count := int(dummy)

  fmt.Printf("countc: %d\n", count)

  dummy = byte2uint64(loq_bytes[n:n+8])
  n+=8
  code := int(dummy)

  fmt.Printf("code: %d\n", code)

  dummy = byte2uint64(loq_bytes[n:n+8])
  n+=8
  stride := int(dummy)

  fmt.Printf("stride: %d\n", stride)

  n_offset := (count+stride-1)/stride

  offset := make([]int, n_offset)
  for i:=0; i<n_offset; i++ {
    dummy = byte2uint64(loq_bytes[n:n+8])
    n+=8
    offset[i] = int(dummy)
  }

  fmt.Printf("offset[%d]:", len(offset))
  for i:=0; i<len(offset); i++ {
    fmt.Printf(" %d", offset[i])
  }
  fmt.Printf("\n")

  n_step_pos := ((count+stride-1)/stride)
  step_pos := make([]int, n_step_pos)
  for i:=0; i<n_step_pos; i++ {
    dummy = byte2uint64(loq_bytes[n:n+8])
    n+=8
    step_pos[i] = int(dummy)
  }

  fmt.Printf("step_pos[%d]:", len(step_pos))
  for i:=0; i<len(step_pos); i++ {
    fmt.Printf(" %d", step_pos[i])
  }
  fmt.Printf("\n")

  n_hom_bytes := (count+7)/8
  hom_flag := loq_bytes[n:n+n_hom_bytes]
  n+=n_hom_bytes

  fmt.Printf("homflag[%d]:\n", n_hom_bytes)
  for i:=0; i<len(hom_flag); i++ {
    if (i%16)==0 { fmt.Printf("\n") }
    fmt.Printf(" %2x |", hom_flag[i])
  }
  fmt.Printf("\n\n")


  dummy = byte2uint64(loq_bytes[n:n+8])
  n+=8
  loq_flag_byte_count := int(dummy)

  fmt.Printf("loq_flag_byte_count[%d]:\n", loq_flag_byte_count)


  loq_flag := loq_bytes[n:n+loq_flag_byte_count]
  n+=loq_flag_byte_count

  for i:=0; i<len(loq_flag); i++ {
    if (i%16)==0 { fmt.Printf("\n") }
    fmt.Printf(" %2x |", loq_flag[i])
  }
  fmt.Printf("\n")

  dummy = byte2uint64(loq_bytes[n:n+8])
  n+=8
  loq_info_byte_count := int(dummy)

  fmt.Printf("loq_inf_byte_count: %d\n", loq_info_byte_count)

  for cur_rec:=0; cur_rec<count; cur_rec++ {

    fmt.Printf(" [%d]\n", cur_rec)

    nallele := 1
    if (hom_flag[cur_rec/8] & (1<<uint(cur_rec%8))) == 0 {
      nallele = 2
    }

    dummy,dn = dlug.ConvertUint64(loq_bytes[n:])
    n+=dn
    ntile0 := int(dummy)
    ntile1 := 0

    if nallele==2 {
      dummy,dn = dlug.ConvertUint64(loq_bytes[n:])
      n+=dn
      ntile1 = int(dummy)
    }

    fmt.Printf("  [ntile0:%d]\n", ntile0)

    for tileidx:=0; tileidx<ntile0; tileidx++ {
      dummy,dn = dlug.ConvertUint64(loq_bytes[n:])
      n+=dn
      m0 := int(dummy)


      fmt.Printf("    [%d] {len:%d}", tileidx, m0)
      for ii:=0; ii<m0; ii+=2 {
        dummy,dn = dlug.ConvertUint64(loq_bytes[n:])
        n+=dn
        delpos := int(dummy)

        dummy,dn = dlug.ConvertUint64(loq_bytes[n:])
        n+=dn
        loqlen := int(dummy)

        fmt.Printf(" %d+%d", delpos, loqlen)
      }
      fmt.Printf("\n")

    }

    if ntile1>0 {

      fmt.Printf("  [ntile1:%d]\n", ntile1)
      for tileidx:=0; tileidx<ntile1; tileidx++ {

        dummy,dn = dlug.ConvertUint64(loq_bytes[n:])
        n+=dn
        m1 := int(dummy)

        fmt.Printf("    [%d] {len:%d}", tileidx, m1)

        for ii:=0; ii<m1; ii+=2 {
          dummy,dn = dlug.ConvertUint64(loq_bytes[n:])
          n+=dn
          delpos := int(dummy)

          dummy,dn = dlug.ConvertUint64(loq_bytes[n:])
          n+=dn
          loqlen := int(dummy)

          fmt.Printf(" %d+%d", delpos, loqlen)
        }
        fmt.Printf("\n")
      }


    }

  }

  return n,nil

}

func debug_print_final_overflow_bytes(fovf_bytes []byte) error {
  var dummy uint64
  var dn int

  n:=0

  dummy = byte2uint64(fovf_bytes[n:n+8])
  n+=8

  datarecordn := int(dummy)

  dummy = byte2uint64(fovf_bytes[n:n+8])
  n+=8

  datarecordbytelen := int(dummy)

  code_bytes := fovf_bytes[n:n+datarecordn]
  n+=datarecordn

  fmt.Printf("datarecordn: %d\n", datarecordn)
  fmt.Printf("datarecordbytelen: %d\n", datarecordbytelen)

  cur_rec := 0
  for (cur_rec < datarecordn) && (n<len(fovf_bytes)) {

    dummy,dn = dlug.ConvertUint64(fovf_bytes[n:])
    n+=dn

    anchor_step := int(dummy)

    dummy,dn = dlug.ConvertUint64(fovf_bytes[n:])
    n+=dn

    nallele := int(dummy)

    fmt.Printf("[%d] %x (nallele %d) code[%d]: %d\n", cur_rec, anchor_step, nallele, cur_rec, code_bytes[cur_rec])

    for allele:=0; allele<nallele; allele++ {
      dummy,dn = dlug.ConvertUint64(fovf_bytes[n:])
      n+=dn

      m := int(dummy)

      fmt.Printf("    [%d] (%d):", allele, m)

      for i:=0; i<m; i++ {
        dummy,dn = dlug.ConvertUint64(fovf_bytes[n:])
        n+=dn

        varid := int(dummy)

        dummy,dn = dlug.ConvertUint64(fovf_bytes[n:])
        n+=dn

        span := int(dummy)

        fmt.Printf(" %x+%d", varid, span)
      }
      fmt.Printf("\n")

    }

    fmt.Printf("\n")

    cur_rec++
  }

  return nil

}

func debug_unpack_bytes(cgf_bytes []byte) error {
  var s string
  var dn int
  buf := make([]byte, 128) ; _ = buf
  n := 0


  magic := byte2uint64(cgf_bytes[n:n+8])
  n+=8

  fmt.Printf("Magic: %08x (", magic)
  for i:=0; i<8; i++ {
    b := uint8( (magic & (0xff<<(8*uint(i)))) >> (8*uint(i)) )
    fmt.Printf(" %c", b)
  }
  fmt.Printf(" )\n")

  s,dn = byte2string(cgf_bytes[n:])
  n+=dn

  fmt.Printf("Version(%d): %s\n", len(s), s)

  s,dn = byte2string(cgf_bytes[n:])
  n+=dn

  fmt.Printf("LibraryVersion(%d): %s\n", len(s), s)

  pathcount := byte2uint64(cgf_bytes[n:n+8])
  n+=8

  fmt.Printf("PathCount: %d\n", pathcount)

  tmaplen := byte2uint64(cgf_bytes[n:n+8])
  n+=8

  fmt.Printf("TileMapLen: %d\n", tmaplen)

  //tmea := unpack_tilemap(cgf_bytes[n:n+int(tmaplen)])
  tmea := UnpackTileMap(cgf_bytes[n:n+int(tmaplen)])
  n+=int(tmaplen)

  fmt.Printf("TileMap(%d):", len(tmea))
  for i:=0; i<len(tmea); i++ {

    if (i%16) == 0 {
      fmt.Printf("\n  [%03x]", i)
    }

    fmt.Printf(" ")
    for j:=0; j<len(tmea[i].Variant); j++ {
      if j>0 { fmt.Printf(":") }
      for k:=0; k<len(tmea[i].Variant[j]); k++ {
        if k>0 { fmt.Printf(";") }
        fmt.Printf("%x", tmea[i].Variant[j][k])
        if tmea[i].Span[j][k]>1 {
          fmt.Printf("+%x", tmea[i].Span[j][k])
        }
      }
    }


  }
  fmt.Printf("\n")

  StepPerPath := make([]uint64, pathcount)
  for i:=uint64(0); i<pathcount; i++ {
    StepPerPath[i] = byte2uint64(cgf_bytes[n:n+8])
    n+=8
  }

  fmt.Printf("StepPerPath(%d):", pathcount)
  for i:=uint64(0); i<pathcount; i++ {
    fmt.Printf(" %d", StepPerPath[i])
  }
  fmt.Printf("\n")

  path_struct_offset := make([]uint64, pathcount+1)
  for i:=uint64(0); i<=pathcount; i++  {
    path_struct_offset[i] = byte2uint64(cgf_bytes[n:n+8])
    n+=8
  }

  fmt.Printf("PathOffset(%d, offset %d):", len(path_struct_offset), n)

  for i:=uint64(0); i<=pathcount; i++  {
    fmt.Printf(" %d", path_struct_offset[i])
  }
  fmt.Printf("\n")

  skip_dots:=0

  path_bytes := cgf_bytes[n:]
  for i:=1; i<len(path_struct_offset); i++ {
    var z uint64

    tn := 0
    offset := path_struct_offset[i] ; _ = offset

    if path_struct_offset[i-1] == path_struct_offset[i] { continue }
    path_b := path_bytes[path_struct_offset[i-1]:path_struct_offset[i]]

    s,dn = byte2string(path_b)
    tn += dn

    ntile := byte2uint64(path_b[tn:tn+8])
    tn+=8

    vectorlen := (ntile+31)/32

    if vectorlen==0 {
      if skip_dots%40 == 0 {
        fmt.Printf("\n")
      }

      fmt.Printf(".")
      skip_dots++
      continue
    }

    skip_dots=0

    fmt.Printf("\n")
    fmt.Printf("  [%x] %s\n", i-1, s)
    fmt.Printf("  NTile: %d\n", ntile)
    fmt.Printf("  VectorLen: %d", vectorlen)

    if vectorlen>0 {
      for ii:=uint64(0); ii<vectorlen; ii++ {
        if ii%8 == 0 { fmt.Printf("\n    [%4x]", ii*8) }

        b:=tn+int(ii*8)
        e:=tn+int(ii*8)+8
        //vec_val := byte2uint64(path_b[tn+int(ii):tn+int(ii)+8])
        vec_val := byte2uint64(path_b[b:e])
        fmt.Printf(" %8x %8x |", (vec_val>>32), vec_val&0xffffffff)
      }
      fmt.Printf("\n")

      tn+=int(vectorlen)*8

    } else {
      fmt.Printf("\n")
    }

    // Overflow
    //

    path_byte_begin := tn

    z = byte2uint64(path_b[tn:tn+8])
    tn+=8
    fmt.Printf("  Overflow.Length: %d\n", z)

    ovf_len := z

    z = byte2uint64(path_b[tn:tn+8])
    tn+=8
    fmt.Printf("  Overflow.Stride: %d\n", z)

    ovf_stride := z

    if ovf_len > 0 {

      n_pos := (ovf_len + ovf_stride - 1) / ovf_stride
      n_offset := n_pos
      if (ovf_len%ovf_stride)!=0 { n_offset++ }

      fmt.Printf("  Offset:")
      for i:=uint64(0); i<n_offset; i++ {
        x := byte2uint64(path_b[tn:tn+8])
        fmt.Printf("  [%d,%d]", i, x)
        tn+=8
      }
      fmt.Printf("\n")


      fmt.Printf("  Position:")
      for i:=uint64(0); i<n_pos; i++ {
        x := byte2uint64(path_b[tn:tn+8])
        fmt.Printf("  [%d,%d]", i, x)
        tn+=8
      }
      fmt.Printf("\n")


      fmt.Printf("  Map(%d):", int(ovf_len) - (tn-path_byte_begin))

      //for uint64(tn - path_byte_begin) < ovf_len {
      for ele_count:=uint64(0) ; ele_count < ovf_len; ele_count++ {
        map_ele,dn := dlug.ConvertUint64(path_b[tn:])
        //map_ele := byte2uint64(path_b[tn:tn+8])
        //dn:=8

        tn+=dn

        fmt.Printf(" %x", map_ele)
      }
      fmt.Printf("\n")

    }


    // Final Overflow
    //

    z = byte2uint64(path_b[tn:tn+8])
    tn+=8
    fmt.Printf("  FinalOverflow.Length: %d\n", z)

    ovf_len = z

    z = byte2uint64(path_b[tn:tn+8])
    tn+=8
    fmt.Printf("  FinalOverflow.ByteLength: %d\n", z)

    //ovf_byte_len := z

    /*
    z = byte2uint64(path_b[tn:tn+8])
    tn+=8
    fmt.Printf("  FinalOverflow.Stride: %d\n", z)
    ovf_stride = z
    */

    if ovf_len > 0 {
      code_bytes := make([]byte, ovf_len)
      for i:=uint64(0); i<ovf_len; i++ {
        code_bytes[i] = path_b[tn]
        tn++
      }

      var dummy uint64
      var dn int

      for i:=0; i<int(ovf_len); i++ {
        dummy,dn = dlug.ConvertUint64(path_b[tn:])
        tn+=dn
        anchor_step := int(dummy)

        dummy,dn = dlug.ConvertUint64(path_b[tn:])
        tn+=dn
        nallele := int(dummy)

        fmt.Printf("  %x (%d)\n", anchor_step, nallele)

        for allele:=0; allele<nallele; allele++ {
          dummy,dn = dlug.ConvertUint64(path_b[tn:])
          tn+=dn
          allele_rec_len := int(dummy)

          fmt.Printf("    [%d] {%d}", allele, allele_rec_len)

          for ii:=0; ii<allele_rec_len; ii++ {

            dummy,dn = dlug.ConvertUint64(path_b[tn:])
            tn+=dn
            varid := int(dummy)

            dummy,dn = dlug.ConvertUint64(path_b[tn:])
            tn+=dn
            span := int(dummy)

            fmt.Printf(" %x+%x", varid, span)

          }
          fmt.Printf("\n")

        }

      }

    }

    /*
    if ovf_len > 0 {

      n_pos := (ovf_len + ovf_stride - 1) / ovf_stride
      n_offset := n_pos
      if (ovf_len%ovf_stride)!=0 { n_offset++ }

      fmt.Printf("  FinalOverflow.Offset:")
      for i:=uint64(0); i<n_offset; i++ {
        x := byte2uint64(path_b[tn:tn+8])
        fmt.Printf("  [%d,%d]", i, x)
        tn+=8
      }
      fmt.Printf("\n")


      fmt.Printf("  FinalOverflow.Position:")
      for i:=uint64(0); i<n_pos; i++ {
        x := byte2uint64(path_b[tn:tn+8])
        fmt.Printf("  [%d,%d]", i, x)
        tn+=8
      }
      fmt.Printf("\n")


      code_bytes := make([]byte, ovf_len)
      for i:=uint64(0); i<ovf_len; i++ {
        code_bytes[i] = path_b[tn]
        tn++
      }

      for i:=uint64(0); i<ovf_len; i++ {
        a0len,dn := dlug.ConvertUint64(path_b[tn:])
        tn+=dn

        a1len,dn := dlug.ConvertUint64(path_b[tn:])
        tn+=dn

        fmt.Printf("  Code:%d ", code_bytes[i])
        fmt.Printf(" A[%d]{", a0len)

        for ii:=uint64(0); ii<a0len; ii++ {
          a0var,dn := dlug.ConvertUint64(path_b[tn:])
          tn += dn

          a0span,dn := dlug.ConvertUint64(path_b[tn:])
          tn += dn

          fmt.Printf(" %x+%x", a0var, a0span)
        }
        fmt.Printf(" }")

        fmt.Printf("    B[%d]{", a1len)

        for ii:=uint64(0); ii<a1len; ii++ {
          a1var,dn := dlug.ConvertUint64(path_b[tn:])
          tn += dn

          a1span,dn := dlug.ConvertUint64(path_b[tn:])
          tn += dn

          fmt.Printf(" %x+%x", a1var, a1span)
        }
        fmt.Printf(" }\n")

      }

    }
    */

    /*
    z = byte2uint64(path_b[tn:tn+8])
    tn+=8
    fmt.Printf("  LowQualityInfo.Length: %d\n", z)

    loq_len := z ; _ = loq_len
    */
    var e error

    dn,e = debug_print_loq_bytes(path_b[tn:])
    if e!=nil { return e }
    tn+=dn

    /*
    z = byte2uint64(path_b[tn:tn+8])
    tn+=8
    fmt.Printf("  LowQualityInfo.Count: %d\n", z)

    loq_count := z ; _ = loq_count

    z = byte2uint64(path_b[tn:tn+8])
    tn+=8
    fmt.Printf("  LowQualityInfo.Code: %d\n", z)

    code := z ; _ = code

    z = byte2uint64(path_b[tn:tn+8])
    tn+=8
    fmt.Printf("  LowQualityInfo.Stride: %d\n", z)

    stride := z ; _ = stride

    if loq_count > 0 {

      n_loq_offset := (loq_count + stride - 1) / stride

      fmt.Printf("  LoqQualityInfo.Offset[%d]:", n_loq_offset)
      for i:=uint64(0); i<n_loq_offset; i++ {
        z = byte2uint64(path_b[tn:tn+8])
        tn+=8
        fmt.Printf(" [%d,%d]", i, z)
      }
      fmt.Printf("\n")

      fmt.Printf("  LoqQualityInfo.StepPosition[%d]:", n_loq_offset)
      for i:=uint64(0); i<n_loq_offset; i++ {
        z = byte2uint64(path_b[tn:tn+8])
        tn+=8
        fmt.Printf(" [%d,%d]", i, z)
      }
      fmt.Printf("\n")

      hom_flag_n := loq_count/8
      if (loq_count%8) != 0 { hom_flag_n++ }


      hom_flag := make([]bool, 0, 1024)
      fmt.Printf("  LoqQualityInfo.HomFlag[%d]:", hom_flag_n)
      for i:=uint64(0); i<hom_flag_n; i++ {
        z := path_b[tn]
        tn++
        fmt.Printf(" %02x", z)

        for ii:=0; ii<8; ii++ {
          if (z & (1<<uint(ii))) > 0 {
            hom_flag = append(hom_flag, true)
          } else {
            hom_flag = append(hom_flag, false)
          }
        }
      }
      fmt.Printf("\n")

      z = byte2uint64(path_b[tn:tn+8])
      tn+=8
      fmt.Printf("  LowQualityInfo.LoqFlagBytecount: %d\n", z)

      loq_flag_byte_count := int(z)

      loqflag := path_b[tn:tn+loq_flag_byte_count]
      tn+=loq_flag_byte_count

      for i:=0; i<len(loqflag); i++ {
        if (i%16)==0 { fmt.Printf("\n") }
        fmt.Printf(" %2x |", loqflag[i])
      }
      fmt.Printf("\n")

      //--

      z = byte2uint64(path_b[tn:tn+8])
      tn+=8
      fmt.Printf("  LowQualityInfo.LoqFlagBytecount: %d\n", z)

      loq_struct_byte_count := int(z)

      fmt.Printf("  LowQualityInfo.LoqInfoByteCount: %d\n", loq_struct_byte_count)

      for i:=uint64(0); i<loq_count; i++ {
        if hom_flag[i] {
          ntile,dn := dlug.ConvertUint64(path_b[tn:])
          tn+=dn

          fmt.Printf("    [%d] Hom[ntile:%d]:", i, ntile)

          for ii:=uint64(0); ii<ntile; ii++ {
            entry_len,dn := dlug.ConvertUint64(path_b[tn:])
            tn+=dn

            fmt.Printf(" [%d]{", entry_len)

            for jj:=uint64(0); jj<entry_len; jj++ {
              delpos,dn := dlug.ConvertUint64(path_b[tn:])
              tn+=dn

              loqlen,dn := dlug.ConvertUint64(path_b[tn:])
              tn+=dn

              fmt.Printf(" %x+%x", delpos, loqlen)
            }
            fmt.Printf(" }\n")

          }

        } else {

          ntilea,dn := dlug.ConvertUint64(path_b[tn:])
          tn+=dn

          ntileb,dn := dlug.ConvertUint64(path_b[tn:])
          tn+=dn

          fmt.Printf("    [%d] Het[%d,%d]:", i, ntilea,ntileb)

          for ii:=uint64(0); ii<ntilea; ii++ {
            entry_len,dn := dlug.ConvertUint64(path_b[tn:])
            tn+=dn

            fmt.Printf(" A[%d]{", entry_len)

            for jj:=uint64(0); jj<entry_len; jj++ {
              delpos,dn := dlug.ConvertUint64(path_b[tn:])
              tn+=dn

              loqlen,dn := dlug.ConvertUint64(path_b[tn:])
              tn+=dn

              fmt.Printf(" %x+%x", delpos, loqlen)
            }
            fmt.Printf("}")
          }

          for ii:=uint64(0); ii<ntileb; ii++ {
            entry_len,dn := dlug.ConvertUint64(path_b[tn:])
            tn+=dn

            fmt.Printf("    B[%d]{", entry_len)

            for jj:=uint64(0); jj<entry_len; jj++ {
              delpos,dn := dlug.ConvertUint64(path_b[tn:])
              tn+=dn

              loqlen,dn := dlug.ConvertUint64(path_b[tn:])
              tn+=dn

              fmt.Printf(" %x+%x", delpos, loqlen)
            }
            fmt.Printf(" }\n")

          }


        }

      }

    }
    */

  }

  return nil
}

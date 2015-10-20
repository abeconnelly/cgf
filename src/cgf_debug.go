package main


import "fmt"
import "io/ioutil"
import "./dlug"

func debug_read(ifn string) error {
  b,e := ioutil.ReadFile(ifn)
  if e!=nil { return e }
  return debug_unpack_bytes(b)
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

  tmea := unpack_tilemap(cgf_bytes[n:n+int(tmaplen)])
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

  step_per_path := make([]uint64, pathcount)
  for i:=uint64(0); i<pathcount; i++ {
    step_per_path[i] = byte2uint64(cgf_bytes[n:n+8])
    n+=8
  }

  fmt.Printf("StepPerPath(%d):", pathcount)
  for i:=uint64(0); i<pathcount; i++ {
    fmt.Printf(" %d", step_per_path[i])
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
    tn := 0
    offset := path_struct_offset[i] ; _ = offset
    path_b := path_bytes[path_struct_offset[i-1]:path_struct_offset[i]]

    s,dn = byte2string(path_b)
    tn += dn

    z := byte2uint64(path_b[tn:tn+8])
    tn+=8

    if z==0 {
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
    fmt.Printf("  VectorLen: %d", z)

    if z>0 {
      for ii:=uint64(0); ii<z; ii++ {
        if ii%8 == 0 { fmt.Printf("\n    [%4x]", ii*8) }

        b:=tn+int(ii*8)
        e:=tn+int(ii*8)+8
        //vec_val := byte2uint64(path_b[tn+int(ii):tn+int(ii)+8])
        vec_val := byte2uint64(path_b[b:e])
        fmt.Printf(" %16x", vec_val)
      }
      fmt.Printf("\n")

      tn+=int(z)*8

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
    fmt.Printf("  FinalOverflow.Stride: %d\n", z)

    ovf_stride = z

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

    z = byte2uint64(path_b[tn:tn+8])
    tn+=8
    fmt.Printf("  LowQualityInfo.Length: %d\n", z)

    loq_len := z ; _ = loq_len

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
        fmt.Printf(" [%d,%d]", z, z)
      }
      fmt.Printf("\n")

      fmt.Printf("  LoqQualityInfo.StepPosition[%d]:", n_loq_offset)
      for i:=uint64(0); i<n_loq_offset; i++ {
        z = byte2uint64(path_b[tn:tn+8])
        tn+=8
        fmt.Printf(" [%d,%d]", z, z)
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

      for i:=uint64(0); i<loq_count; i++ {
        if hom_flag[i] {
          ntile,dn := dlug.ConvertUint64(path_b[tn:])
          tn+=dn

          fmt.Printf("    [%d] Hom[%d]:", i, ntile)

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

  }

  return nil
}

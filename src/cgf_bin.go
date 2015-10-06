package main

import "fmt"
import "./dlug"

import "io/ioutil"

func create_tilemap_string_lookup2(step0,span0,step1,span1 []int) string {
  b := make([]byte, 0, 1024)

  for i:=0; i<len(step0); i++ {
    if i>0 { b = append(b, ';') }
    b = append(b, fmt.Sprintf("%x", step0[i])...)
    if span0[i]>1 {
      b = append(b, fmt.Sprintf("+%x", span0[i])...)
    }
  }

  b = append(b, ':')

  for i:=0; i<len(step1); i++ {
    if i>0 { b = append(b, ';') }
    b = append(b, fmt.Sprintf("%x", step1[i])...)
    if span1[i]>1 {
      b = append(b, fmt.Sprintf("+%x", span1[i])...)
    }
  }

  return string(b)

}

// Will overwrite cgf path structure if it exists, create a new path if it doesn't.
// It will create a new PathStruct if one doesn't already exist.
//
//func update_vector_path_simple(cgf *CGF, sglf *SGLF, path_idx int, allele_path [][]TileInfo) {
func update_vector_path_simple(ctx *CGFContext, path_idx int, allele_path [][]TileInfo) error {
  cgf := ctx.CGF
  sglf := ctx.SGLF

  g_debug := true

  if len(cgf.Path) < path_idx {
    tpath := make([]PathStruct, path_idx - len(cgf.Path) + 1)
    cgf.Path = append(cgf.Path, tpath...)

    if g_debug {
      fmt.Printf(">>>>> len cgf.Path %d, path_idx %d\n", len(cgf.Path), path_idx)
    }
  }

  var ok bool

  var path0 int
  var path1 int

  var step_idx0 int
  var step_idx1 int

  var step0 int
  var step1 int


  // Overflow tier of variants.
  // These are variants that can't fit
  // into the packed vector but still
  // appear in the tile map.
  //
  overflow := OverflowStruct{}
  overflow.Stride = 256
  overflow.Offset = make([]uint64, 0, 16)
  overflow.Position = make([]uint64, 0, 16)
  overflow.Map = make([]byte, 0, 256)
  overflow_count := 0

  final_overflow := FinalOverflowStruct{}
  final_overflow.Stride = 256
  final_overflow.Offset = make([]uint64, 0, 16)
  final_overflow.Position = make([]uint64, 0, 16)
  final_overflow.DataRecord.Code = make([]byte, 0, 256)
  final_overflow.DataRecord.Data = make([]byte, 0, 2048)
  final_overflow_count := 0
  _ = final_overflow

  loq_info := LowQualityInfoStruct{}
  _ = loq_info

  buf := make([]byte, 1024)

  sglf_info0 := SGLFInfo{}
  sglf_info1 := SGLFInfo{}

  tile_zipper := make([][]SGLFInfo, 2)
  tile_zipper_seq := make([][]string, 2)

  span_sum := 0

  var_a0 := make([]int, 0, 16)
  var_a1 := make([]int, 0, 16)

  span_a0 := make([]int, 0, 16)
  span_a1 := make([]int, 0, 16)

  ivec := make([]int, 0, 1024*16)

  anchor_step := 0
  update_anchor := true
  end_check := false
  loq_tile := false

  step_idx_info_for_loq := make([][]int, 2)
  step_idx_info_for_loq[0] = make([]int, 0, 1024)
  step_idx_info_for_loq[1] = make([]int, 0, 1024)

  loq_byte_length := uint64(0)
  loq_bytes := make([]byte, 0, 1024)

  loq_offset_bytes := make([]byte, 0, 1024) ; _ = loq_offset_bytes
  loq_position_bytes := make([]byte, 0, 1024) ; _ = loq_position_bytes
  loq_hethom_flag_bytes := make([]byte, 0, 1024) ; _ = loq_hethom_flag_bytes

  // We might need to fill this in after the fact
  //
  loq_offset := make([]uint64, 0, 1024) ; _ = loq_offset
  loq_position := make([]uint64, 0, 1024) ; _ = loq_position
  loq_hethom_flag := make([]bool, 0, 1024)

  loq_count := uint64(0)
  loq_stride := uint64(256)

  // First is allele
  // second is 3 element (varid,span,start,len)
  //
  //AlleleNocallInfo := make([][]int, 2)
  //

  for (step_idx0<len(allele_path[0])) && (step_idx1<len(allele_path[1])) {

    end_check = false

    if span_sum >= 0 {
      ti0 := allele_path[0][step_idx0]

      if len(ti0.NocallStartLen)>0 { loq_tile = true; }

      step_idx_info_for_loq[0] = append(step_idx_info_for_loq[0], step_idx0)

      // sglf_info1 only holds a valid path and step
      //
      if step_idx0>0 {
        sglf_info0,ok = sglf.PfxTagLookup[ti0.PfxTag]
      } else {
        sglf_info0,ok = sglf.SfxTagLookup[ti0.SfxTag]
      }

      if !ok {
        return fmt.Errorf("could not find prefix (%s) in sglf (allele_idx %d, step_idx %d (%x))\n",
          ti0.PfxTag, 0, step_idx0, step_idx0)
      }

      path0 = sglf_info0.Path
      step0 = sglf_info0.Step

      if update_anchor {
        anchor_step = step0
        update_anchor = false
      }

      // We need to search for the variant in the Lib to find
      // the rest of the information, including span
      //
      var_idx0,e := lookup_variant_index(ti0.Seq, sglf.Lib[path0][step0])
      if e!=nil { return e }

      sglf_info0 = sglf.LibInfo[path0][step0][var_idx0]

      span0 := sglf_info0.Span

      sglf_info0.Variant = var_idx0

      seq0 := sglf.Lib[path0][step0][var_idx0]
      tile_zipper[0] = append(tile_zipper[0], sglf_info0)
      tile_zipper_seq[0] = append(tile_zipper_seq[0], seq0)

      var_a0 = append(var_a0, var_idx0)
      span_a0 = append(span_a0, span0)

      allele_path[0][step_idx0].VarId = var_idx0

      span_sum -= span0
      step_idx0++

    } else {
      ti1 := allele_path[1][step_idx1]

      if len(ti1.NocallStartLen)>0 { loq_tile = true; }

      step_idx_info_for_loq[1] = append(step_idx_info_for_loq[1], step_idx1)

      // sglf_info1 only holds a valid path and step
      //
      if step_idx1>0 {
        sglf_info1,ok = sglf.PfxTagLookup[ti1.PfxTag]
      } else {
        sglf_info1,ok = sglf.SfxTagLookup[ti1.SfxTag]
      }

      if !ok {
        return fmt.Errorf("could not find prefix (%s) in sglf (allele_idx %d, step_idx %d (%x))\n",
          ti1.PfxTag, 1, step_idx1, step_idx1)
      }


      path1 = sglf_info1.Path
      step1 = sglf_info1.Step

      // We need to search for the variant in the Lib to find
      // the rest of the information, including span
      //
      var_idx1,e := lookup_variant_index(ti1.Seq, sglf.Lib[path1][step1])
      if e!=nil { return e }

      sglf_info1 = sglf.LibInfo[path1][step1][var_idx1]

      sglf_info1.Variant = var_idx1

      seq1 := sglf.Lib[path1][step1][var_idx1]
      tile_zipper[1] = append(tile_zipper[1], sglf_info1)
      tile_zipper_seq[1] = append(tile_zipper_seq[1], seq1)

      span1 := sglf_info1.Span

      var_a1 = append(var_a1, var_idx1)
      span_a1 = append(span_a1, span1)

      allele_path[1][step_idx1].VarId = var_idx1

      span_sum += span1
      step_idx1++

    }


    if span_sum==0 {


      // *********************
      // *********************
      // ---------------------
      // STORE LOQ INFORMATION
      //

      if loq_tile {
        buf := make([]byte, 16)
        var dn int

        hom_flag := true
        if len(step_idx_info_for_loq[0]) != len(step_idx_info_for_loq[1]) {
          hom_flag = false
        } else {
          for ii:=0; ii<len(step_idx_info_for_loq[0]); ii++ {
            step_idx0 := step_idx_info_for_loq[0][ii]
            step_idx1 := step_idx_info_for_loq[1][ii]

            step0 := allele_path[0][step_idx0].Step
            step1 := allele_path[1][step_idx1].Step

            if step0 != step1 {
              hom_flag = false
              break
            }

            if len(allele_path[0][step_idx0].NocallStartLen) != len(allele_path[1][step_idx1].NocallStartLen) {
              hom_flag = false
              break
            }

            for jj:=0; jj<len(allele_path[0][step_idx0].NocallStartLen); jj++ {

              if allele_path[0][step_idx0].NocallStartLen[jj] != allele_path[1][step_idx1].NocallStartLen[jj] {
                hom_flag = false
                break
              }

            }
          }
        }

        loq_hethom_flag = append(loq_hethom_flag, hom_flag)

        // (NTile)
        // Number of tile in this record
        //
        dn = dlug.FillSliceUint64(buf, uint64(len(step_idx_info_for_loq[0])))
        loq_bytes = append(loq_bytes, buf[0:dn]...)

        if !hom_flag {

          // (NTile) (B allele)
          // Number of Allele B tiles in this record
          //
          dn = dlug.FillSliceUint64(buf, uint64(len(step_idx_info_for_loq[1])))
          loq_bytes = append(loq_bytes, buf[0:dn]...)

        } else {
        }

        for i:=0; i<len(step_idx_info_for_loq[0]); i++ {
          step_idx0 := step_idx_info_for_loq[0][i]
          ti := allele_path[0][step_idx0]

          // (LoqTile[].Len)
          // Number of noc entries for this tile
          //
          dn = dlug.FillSliceUint64(buf, uint64(len(ti.NocallStartLen)))
          loq_bytes = append(loq_bytes, buf[0:dn]...)

          prev_start := 0
          for jj:=0; jj<len(ti.NocallStartLen); jj+=2 {

            // (LoqTile[].LoqEntry[].DelPos)
            // Start position
            //
            dn = dlug.FillSliceUint64(buf, uint64(ti.NocallStartLen[jj]-prev_start))
            loq_bytes = append(loq_bytes, buf[0:dn]...)

            // (LoqTile[].LoqEntry[].LoqLen)
            // Length of noc
            //
            dn = dlug.FillSliceUint64(buf, uint64(ti.NocallStartLen[jj+1]))
            loq_bytes = append(loq_bytes, buf[0:dn]...)

            prev_start = ti.NocallStartLen[jj]
          }
        }


        if !hom_flag {

          for i:=0; i<len(step_idx_info_for_loq[1]); i++ {
            step_idx1 := step_idx_info_for_loq[1][i]
            ti := allele_path[1][step_idx1]

            // (LoqTile[].Len)
            // Number of noc entries
            //
            dn = dlug.FillSliceUint64(buf, uint64(len(ti.NocallStartLen)))
            loq_bytes = append(loq_bytes, buf[0:dn]...)

            prev_start := 0
            for jj:=0; jj<len(ti.NocallStartLen); jj+=2 {

              // (LoqTile[].LoqEntry[].DelPos)
              // Start of nocall
              //
              dn = dlug.FillSliceUint64(buf, uint64(ti.NocallStartLen[jj]-prev_start))
              loq_bytes = append(loq_bytes, buf[0:dn]...)

              // (LoqTile[].LoqEntry[].LoqLen)
              // Noc length
              //
              dn = dlug.FillSliceUint64(buf, uint64(ti.NocallStartLen[jj+1]))
              loq_bytes = append(loq_bytes, buf[0:dn]...)

              prev_start = ti.NocallStartLen[jj]

            }
          }

        }


        loq_count++
        if (loq_count%loq_stride)==0 {
          //loq_offset = append(loq_offset, loq_byte_count)
          //loq_position = append(loq_position, uint64(anchor_step))

          tobyte64(buf, loq_byte_length)
          loq_offset_bytes = append(loq_offset_bytes, buf[0:8]...)

          tobyte64(buf, uint64(anchor_step))
          loq_position_bytes = append(loq_offset_bytes, buf[0:8]...)
        }

      }

      //
      // STORE LOQ INFORMATION
      // ---------------------
      // *********************
      // *********************





      end_check = true

      tilemap_key := create_tilemap_string_lookup2(var_a0,span_a0,var_a1,span_a1)

      if g_debug {
        fmt.Printf(">> (%d,%x) {%v,%v} {%v,%v} %s\n",
          anchor_step, anchor_step,
          var_a0,span_a0,var_a1,span_a1,
          tilemap_key)
      }

      n := len(ivec)

      // Fill in spanning
      //
      for ; n<=anchor_step; n++ {
        ivec = append(ivec, -1)
      }

      if _,ok := ctx.TileMapLookup[tilemap_key] ; ok {

        tilemap_pos := ctx.TileMapPosition[tilemap_key]

        //if tilemap_pos < 13 {
        if (!loq_tile) && (tilemap_pos < 13) {
          ivec[anchor_step] = tilemap_pos
        } else {

          // overflow
          //
          if loq_tile {
            ivec[anchor_step] = -254
          } else {
            ivec[anchor_step] = 254
          }

          // --------
          // OVERFLOW
          //

          p := overflow_count
          if (uint64(p)%overflow.Stride)==0 {
            overflow.Offset = append(overflow.Offset, uint64(len(overflow.Map)))
            overflow.Position = append(overflow.Position, uint64(overflow_count))
          }

          dn := dlug.FillSliceUint64(buf, uint64(tilemap_pos))
          overflow.Map = append(overflow.Map, buf[:dn]...)

          overflow_count++

          //
          // OVERFLOW
          // --------



        }
      } else {

        //final overflow
        //
        if loq_tile {
          ivec[anchor_step] = -255
        } else {
          ivec[anchor_step] = 255
        }

        // --------------
        // FINAL OVERFLOW
        //

        // We haven't found the TileMap entry, so store the entry
        // here.
        // TODO: store raw sequence if it's not found in the Tile Library
        //

        p := final_overflow_count
        if (uint64(p)%overflow.Stride)==0 {
          final_overflow.Offset = append(final_overflow.Offset, uint64(len(final_overflow.Offset)))
          final_overflow.Position = append(final_overflow.Position, uint64(final_overflow_count))
        }

        final_overflow.DataRecord.Code = append(final_overflow.DataRecord.Code, uint8(1))
        dn := dlug.FillSliceUint64(buf, uint64(len(var_a0)))
        final_overflow.DataRecord.Data = append(final_overflow.DataRecord.Data, buf[:dn]...)

        dn = dlug.FillSliceUint64(buf, uint64(len(var_a1)))
        final_overflow.DataRecord.Data = append(final_overflow.DataRecord.Data, buf[:dn]...)

        for ii:=0; ii<len(var_a0); ii++ {
          dn = dlug.FillSliceUint64(buf, uint64(var_a0[ii]))
          final_overflow.DataRecord.Data = append(final_overflow.DataRecord.Data, buf[:dn]...)

          dn = dlug.FillSliceUint64(buf, uint64(span_a0[ii]))
          final_overflow.DataRecord.Data = append(final_overflow.DataRecord.Data, buf[:dn]...)
        }

        for ii:=0; ii<len(var_a1); ii++ {
          dn = dlug.FillSliceUint64(buf, uint64(var_a1[ii]))
          final_overflow.DataRecord.Data = append(final_overflow.DataRecord.Data, buf[:dn]...)

          dn = dlug.FillSliceUint64(buf, uint64(span_a1[ii]))
          final_overflow.DataRecord.Data = append(final_overflow.DataRecord.Data, buf[:dn]...)
        }

        final_overflow_count++

        //
        // FINAL OVERFLOW
        // --------------


      }

      var_a0 = var_a0[0:0]
      var_a1 = var_a1[0:0]

      span_a0 = span_a0[0:0]
      span_a1 = span_a1[0:0]

      update_anchor = true

      tile_zipper[0] = tile_zipper[0][0:0]
      tile_zipper[1] = tile_zipper[1][0:0]

      tile_zipper_seq[0] = tile_zipper_seq[0][0:0]
      tile_zipper_seq[1] = tile_zipper_seq[1][0:0]

      loq_tile = false

      step_idx_info_for_loq[0] = step_idx_info_for_loq[0][0:0]
      step_idx_info_for_loq[1] = step_idx_info_for_loq[1][0:0]

    }

  }

  if !end_check {
    return fmt.Errorf("There trailing tiles that could not be matched up")
  }

  if g_debug {
    for i:=0; i<len(ivec); i++ {
      fmt.Printf("[%d] %d\n", i, ivec[i])
    }
  }

  // Now we know the final size of the overflow structures so
  // fill in their length
  overflow.Length = 8 + uint64(len(overflow.Offset)) + uint64(len(overflow.Position)) + uint64(len(overflow.Map))
  final_overflow.Length = 8 + uint64(len(final_overflow.Offset)) + uint64(len(final_overflow.Position)) + uint64(len(final_overflow.DataRecord.Code)) + uint64(len(final_overflow.DataRecord.Data))


  packed_len := (len(ivec)+31)/32
  packed_vec := make([]uint64, packed_len)

  for i:=0; i<(packed_len-1); i++ {

    hexit_ovf_count:=uint(0)
    for jj:=0; jj<32; jj++ {

      ivec_pos := 32*i + jj

      if ivec[ivec_pos] == 0 { continue }
      packed_vec[i] |= (1<<(32+uint(jj)))

      // 32/4 hexits available
      // fill in from right to left
      //
      if hexit_ovf_count < (16) {


        if ivec[ivec_pos] == -1 {

          // spanning tile, 0 hexit

        } else if (ivec[ivec_pos] == 255) || (ivec[ivec_pos] == 254) {

          // hiq overflow
          //
          packed_vec[i] |= (0xf<<(4*hexit_ovf_count))

        } else if (ivec[ivec_pos] == -255) || (ivec[ivec_pos] == -254) {

          // loq overflow
          //
          packed_vec[i] |= (0xe<<(4*hexit_ovf_count))

        } else if (ivec[ivec_pos] == 256) {

          // complex
          //
          packed_vec[i] |= (0xd<<(4*hexit_ovf_count))

        } else {

          // otherwise we can encode the tile lookup in the hexit
          //
          packed_vec[i] |= (uint64(ivec[ivec_pos]&0xff)<<(4*hexit_ovf_count))

        }

      }

      hexit_ovf_count++

    }

  }

  //TODO:
  // final packed bit vector population

  if g_debug {
    for i:=0; i<len(packed_vec); i++ {
      fmt.Printf("[%d,%x (%d)] |%8x|%8x|\n", 32*i, 32*i, i, packed_vec[i]>>32, packed_vec[i]&0xffffffff)
    }

    fmt.Printf("\n\n")
    fmt.Printf("Overflow.Length: %d (0x%x)\n", overflow.Length, overflow.Length)
    fmt.Printf("Overflow.Stride: %d\n", overflow.Stride)
    fmt.Printf("Overflow.Offset:")
    for ii:=0; ii<len(overflow.Offset); ii++ {
      fmt.Printf("  [%d] %d\n", ii, overflow.Offset[ii])
    }
    fmt.Printf("\n")

    fmt.Printf("Overflow.Position:")
    for ii:=0; ii<len(overflow.Position); ii++ {
      fmt.Printf("  [%d] %d\n", ii, overflow.Position[ii])
    }

    idx := 0
    for b_offset:=0; b_offset<len(overflow.Map); {
      tm,dn := dlug.ConvertUint64(overflow.Map[b_offset:])

      fmt.Printf("  [%d] %d (0x%x)\n", idx, tm, tm)

      b_offset+=dn
      idx++
    }


    fmt.Printf("\n\n")
    fmt.Printf("FinalOverflow.Length: %d (0x%x)\n", final_overflow.Length, final_overflow.Length)
    fmt.Printf("FinalOverflow.Stride: %d\n", final_overflow.Stride)
    fmt.Printf("FinalOverflow.Offset:\n")
    for ii:=0; ii<len(final_overflow.Offset); ii++ {
      fmt.Printf("  [%d] %d\n", ii, final_overflow.Offset[ii])
    }
    fmt.Printf("\n")

    fmt.Printf("FinalOverflow.Position:\n")
    for ii:=0; ii<len(final_overflow.Position); ii++ {
      fmt.Printf("  [%d] %d\n", ii, final_overflow.Position[ii])
    }
    fmt.Printf("\n")

    fmt.Printf("FinalOverflow.DataRecord:\n")

    byte_map_offset := 0
    for ii:=0; ii<len(final_overflow.DataRecord.Code); ii++ {
      fmt.Printf("  [%d] Code: %d, Data:", ii, final_overflow.DataRecord.Code[ii])

      if final_overflow.DataRecord.Code[ii] == 1 {
        n0,dn := dlug.ConvertUint64(final_overflow.DataRecord.Data[byte_map_offset:])
        byte_map_offset += dn

        n1,dn := dlug.ConvertUint64(final_overflow.DataRecord.Data[byte_map_offset:])
        byte_map_offset += dn

        fmt.Printf(" (%d,%d)", n0, n1)

        fmt.Printf(" [")
        for jj:=uint64(0); jj<n0; jj++ {

          vid,dn := dlug.ConvertUint64(final_overflow.DataRecord.Data[byte_map_offset:])
          byte_map_offset += dn

          span,dn := dlug.ConvertUint64(final_overflow.DataRecord.Data[byte_map_offset:])
          byte_map_offset += dn

          fmt.Printf(" %x+%x", vid, span)
        }
        fmt.Printf(" ]")

        fmt.Printf(" [")
        for jj:=uint64(0); jj<n1; jj++ {

          vid,dn := dlug.ConvertUint64(final_overflow.DataRecord.Data[byte_map_offset:])
          byte_map_offset += dn

          span,dn := dlug.ConvertUint64(final_overflow.DataRecord.Data[byte_map_offset:])
          byte_map_offset += dn

          fmt.Printf(" %x+%x", vid, span)
        }
        fmt.Printf(" ]")

        fmt.Printf("\n")

      } else {
        panic("unsupported")
      }

    }


    // -------------

    var byt byte
    for i:=0; i<len(loq_hethom_flag); i++ {
      if (i>0) && ((i%8)==0) {
        loq_hethom_flag_bytes = append(loq_hethom_flag_bytes, byt)
        byt = 0
      }

      if loq_hethom_flag[i] { byt |= (1<<uint8(i%8)); }
    }
    if len(loq_hethom_flag)>0 {
      loq_hethom_flag_bytes = append(loq_hethom_flag_bytes, byt)
    }

    fmt.Printf("# loq_bytes(%d)", len(loq_bytes))

    fin_loq_bytes := make([]byte, 8)

    tobyte64(buf, loq_byte_length)
    fin_loq_bytes = append(fin_loq_bytes, buf[0:8]...)

    code := uint64(0)
    tobyte64(buf, code)
    fin_loq_bytes = append(fin_loq_bytes, buf[0:8]...)

    stride := uint64(256)
    tobyte64(buf, stride)
    fin_loq_bytes = append(fin_loq_bytes, buf[0:8]...)

    fin_loq_bytes = append(fin_loq_bytes, loq_offset_bytes...)
    fin_loq_bytes = append(fin_loq_bytes, loq_position_bytes...)
    fin_loq_bytes = append(fin_loq_bytes, loq_hethom_flag_bytes...)

    fin_loq_bytes = append(fin_loq_bytes, loq_bytes...)

    for i:=0; i<len(fin_loq_bytes); i++ {
      if (i%16)==0 { fmt.Printf("\n") }
      fmt.Printf(" %2x", fin_loq_bytes[i])
    }
    fmt.Printf("\n")


    //DEBUG
    err := ioutil.WriteFile("./loq_bytes.bin", fin_loq_bytes, 0644)
    if err!=nil { panic(err); }

  }

  ctx.CGF.Path[path_idx].Vector = packed_vec

  return nil

}

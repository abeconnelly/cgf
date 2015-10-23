package main

import "fmt"
import "./dlug"

func CGFMagic(cgf_bytes []byte) uint64 {
  magic := byte2uint64(cgf_bytes[0:8])
  return magic
}

func CGFVersion(cgf_bytes []byte) string {
  s,_ := byte2string(cgf_bytes[8:])
  return s
}

func CGFLibraryVersion(cgf_bytes []byte) string {
  n:=8
  _,dn := byte2string(cgf_bytes[n:])
  n+=dn

  lv,dn := byte2string(cgf_bytes[n:])
  return lv
}

func CGFPathCount(cgf_bytes []byte) int {
  return 0
}

func CGFTilemapBytes(cgf_bytes []byte) ([]byte, error) {
  cgf := CGF{}
  CGFFillHeader(&cgf, cgf_bytes)
  // if e!=nil { return e }

  return cgf.TileMap, nil
}

func CGFPathBytes(cgf_bytes []byte, path int) ([]byte, error) {
  cgf := CGF{}
  CGFFillHeader(&cgf, cgf_bytes)

  if path>=len(cgf.PathOffset) { return nil, fmt.Errorf("path does not exist in CGF") }

  be := cgf.PathOffset[path] + cgf.PathByteOffset
  en := cgf.PathOffset[path+1] + cgf.PathByteOffset

  return cgf_bytes[be:en], nil

}

type PathOverflowStruct struct {
  Length uint64
  Stride uint64
  Offset []uint64
  Position []uint64
  Map []byte
  IntMap []int
}

func debug_print_path_overflow(po *PathOverflowStruct) {
  fmt.Printf("Length: %d\n", po.Length)
  fmt.Printf("Stride: %d\n", po.Stride)
  fmt.Printf("Offset: %v\n", po.Offset)
  fmt.Printf("Position: %v\n", po.Position)
  fmt.Printf("Map: %v\n", po.Map)
  fmt.Printf("IntMap: %v\n", po.IntMap)
}


func GetPathOverflow(path_bytes []byte) PathOverflowStruct {
  st:=0

  name,dn := byte2string(path_bytes[st:]) ; _ = name
  st+=dn


  vec_len := byte2uint64(path_bytes[st:st+8])
  st+=8

  st+=8*int(vec_len)

  po := PathOverflowStruct{}

  po.Length = byte2uint64(path_bytes[st:st+8])
  st+=8

  po.Stride = byte2uint64(path_bytes[st:st+8])
  st+=8

  ofs_len := (po.Length + po.Stride - 1) / po.Stride

  fmt.Printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %d\n", ofs_len)

  if (po.Length % po.Stride) != 0 { ofs_len ++ }

  fmt.Printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %d\n", ofs_len)

  po.Offset = make([]uint64, ofs_len)
  for i:=(0); i<len(po.Offset); i++ {
    po.Offset[i] = byte2uint64(path_bytes[st:st+8])
    st+=8
  }

  pos_len := (po.Length + po.Stride - 1) / po.Stride
  po.Position = make([]uint64, pos_len)
  for i:=(0); i<len(po.Position); i++ {
    po.Position[i] = byte2uint64(path_bytes[st:st+8])
    st+=8
  }

  start_map := st

  po.IntMap = make([]int, po.Length)
  for i:=uint64(0); i<po.Length; i++ {
    u,dn := dlug.ConvertUint64(path_bytes[st:])
    st+=dn

    po.IntMap[i] = int(u)
  }

  po.Map = path_bytes[start_map:st]

  return po
}

func PathOverflowBytes(path_bytes []byte, path int) ([]byte, error) {
  return nil,nil
}

func CGFVectorUint64(cgf_bytes []byte, path int) ([]uint64, error) {
  path_bytes,e := CGFPathBytes(cgf_bytes, path)
  if e!=nil { return nil, e }

  fmt.Printf(">>>>>> %x %x %x\n", path_bytes[0], path_bytes[1], path_bytes[2])

  st:=0
  path_name,dn := byte2string(path_bytes[st:])
  st+=dn

  fmt.Printf(">>> %s\n", path_name)


  vec_len := byte2uint64(path_bytes[st:])
  st+=8

  vec := make([]uint64, vec_len)
  for i:=0; i<int(vec_len); i++ {
    vec[i] = byte2uint64(path_bytes[st+i*8:st+(i+1)*8])
  }

  fmt.Printf(">>> veclen %d\n", vec_len)


  return vec, nil

}

func PathOverflowOffsetUint64(path_bytes []byte) ([]uint64) {
  st:=0
  n_rec := byte2uint64(path_bytes[st:st+8]) ; _ = n_rec
  st+=8

  stride := byte2uint64(path_bytes[st:st+8]) ; _ = stride
  st+=8

  offset := make([]uint64, n_rec) ; _ = offset
  for i:=uint64(0); i<n_rec; i++ {
    offset[i] = byte2uint64(path_bytes[st:st+8])
    st+=8
  }
  return offset
}

func PathOverflowPositionUint64(path_bytes []byte) ([]uint64) {
  st:=0
  n_rec := byte2uint64(path_bytes[st:st+8]) ; _ = n_rec
  st+=8

  stride := byte2uint64(path_bytes[st:st+8]) ; _ = stride
  st+=8

  offset := make([]uint64, n_rec) ; _ = offset
  for i:=uint64(0); i<n_rec; i++ {
    offset[i] = byte2uint64(path_bytes[st:st+8])
    st+=8
  }

  position := make([]uint64, n_rec) ; _ = position
  for i:=uint64(0); i<n_rec; i++ {
    position[i] = byte2uint64(path_bytes[st:st+8])
    st+=8
  }

  return position
}

// Count number of actual overflow entries.  This is needed
// for the overflow tables.
//
// Entries that appear in the local hexit cache won't be counted
// unless they're greater than 0xd.
//
func CountOverflowVectorUint64(vec []uint64, start_step, end_step int) int {
  dn_step := end_step - start_step ;  _ = dn_step

  ovf_count := 0
  hexit_pos := uint(0)
  vec_pos := start_step/32
  vec_ofs := start_step%32

  // update hexit_pos to skip over
  // hexits before start_step.
  //
  if vec_ofs != 0 {
    for i:=(0); i<vec_ofs; i++ {
      if (vec[vec_pos] & (1<<(32+uint(i)))) == 0 { continue }

      if hexit_pos >= 8 { continue }
      hexit := vec[vec_pos] & (0xf<<(4*hexit_pos))
      hexit_pos++

      if hexit == 0 { continue }
      if hexit < 0xd { continue }
      if hexit == 0xe { //?????
        continue
      }
    }
  }

  cur_step := start_step
  for cur_step < end_step {

    for i:=uint8(vec_ofs); i<32; i++ {

      if (vec[vec_pos] & (1<<(32+i))) != 0 {

        if hexit_pos < 8 {
          hexit := (vec[vec_pos] & (0xf<<(4*hexit_pos))) >> (4*hexit_pos)
          hexit_pos++
          if hexit >= 0xd {
            ovf_count++
          }

        } else {
          ovf_count++
        }

      }

      cur_step++
      if cur_step >= end_step { return ovf_count }

    }

    vec_pos++
    vec_ofs = 0
    hexit_pos=0

  }

  return ovf_count

}

func CountOverflow(cgf_bytes []byte, path, start_step, end_step int) (int,error) {
  vec,e := CGFVectorUint64(cgf_bytes, path)
  if e!=nil { return -1,e }

  return CountOverflowVectorUint64(vec, start_step, end_step), nil


  dn_step := end_step - start_step ;  _ = dn_step

  ovf_count := 0
  hexit_pos := uint(0)
  vec_pos := start_step/32
  vec_ofs := start_step%32

  // update hexit_pos to skip over
  // hexits before start_step.
  //
  if vec_ofs != 0 {
    for i:=(0); i<vec_ofs; i++ {
      if (vec[vec_pos] & (1<<(32+uint(i)))) == 0 { continue }

      if hexit_pos >= 8 { continue }
      hexit := vec[vec_pos] & (0xf<<(4*hexit_pos))
      hexit_pos++

      if hexit == 0 { continue }
      if hexit < 0xd { continue }
      if hexit == 0xe { //?????
        continue
      }
    }
  }

  cur_step := start_step
  for cur_step < end_step {

    for i:=uint8(vec_ofs); i<32; i++ {

      if (vec[vec_pos] & (1<<(32+i))) != 0 {

        if hexit_pos < 8 {
          hexit := (vec[vec_pos] & (0xf<<(4*hexit_pos))) >> (4*hexit_pos)
          hexit_pos++
          if hexit >= 0xd {
            ovf_count++
          }

        } else {
          ovf_count++
        }

      }

      cur_step++
      if cur_step >= end_step { return ovf_count, nil }

    }

    vec_pos++
    vec_ofs = 0
    hexit_pos=0

  }

  return ovf_count, nil

}


// start_step is inclusive, end step is exclusive
//
func CountOverflow_old(cgf_bytes []byte, path, start_step, step int) (int,error) {
  vec,e := CGFVectorUint64(cgf_bytes, path)
  if e!=nil { return -1,e }

  dn_step := step - start_step ; _ = dn_step

  ovf_count := 0
  hexit_pos := uint(0)
  vec_pos := start_step/32
  vec_ofs := start_step%32

  if vec_ofs != 0 {
    for i:=(0); i<vec_ofs; i++ {
      if (vec[vec_pos] & (1<<(32+uint(i)))) == 0 { continue }

      if hexit_pos >= 8 { continue }
      hexit := vec[vec_pos] & (0xf<<(4*hexit_pos))
      hexit_pos++

      if hexit == 0 { continue }
      if hexit < 0xd { continue }
      if hexit == 0xe { //?????
        continue
      }
    }
  }

  cur_step := start_step
  for cur_step < step {

    for i:=uint8(vec_ofs); i<32; i++ {

      if (vec[vec_pos] & (1<<(32+i))) != 0 {

        if hexit_pos < 8 {
          hexit := vec[vec_pos] & (0xf<<(4*hexit_pos))
          hexit_pos++
          if hexit >= 0xd { ovf_count++ }
        } else {
          ovf_count++
        }
      }

      cur_step++
      if cur_step >= step { return ovf_count, nil }

    }

    vec_pos++
    vec_ofs = 0
    hexit_pos=0

  }

  return ovf_count, nil

}

func LookupTileMap(cgf_bytes []byte, path, ver, step int) (int, error) {

  path_bytes,e := CGFPathBytes(cgf_bytes, path)
  if e!=nil { return -1, e }

  st:=0
  path_name,dn := byte2string(path_bytes[st:]) ; _ = path_name
  st+=dn

  vec_len := byte2uint64(path_bytes[st:])
  st+=8

  vec_entry := step/32

  vecbits := byte2uint64(path_bytes[st + vec_entry*8:st + (vec_entry+1)*8])
  st+=int(vec_len)*8

  m := uint8(step%32)

  //DEBUG
  fmt.Printf(">>>>>> step %d (%d)\n", step, m)

  // Canonical, return 0
  //
  if (vecbits&(1<<(32+m))) == 0 { return 0,nil }

  //DEBUG
  fmt.Printf(">>>>>> vecbits %8x\n", vecbits)


  // See if the value is stored in the hexit
  //
  hexit_shift := uint(0)
  for i:=uint8(0); i<m; i++ {
    if (vecbits & (1<<(32+i))) != 0 { hexit_shift++ }
  }

  //DEBUG
  fmt.Printf(">>>>>> hexitpos %d\n", hexit_shift)


  // Return with the hexit if we can.
  //
  loq_flag := false ; _ = loq_flag
  if hexit_shift < (32/8) {
    hexit := (vecbits&(0xf<<(4*hexit_shift))) >> (4*hexit_shift)
    if hexit == 0 { return -2,nil }
    if hexit < 0xd { return int(hexit),nil }
    if hexit == 0xe { loq_flag = true }
  }


  // Find it in the Overflow bytes
  //

  po := GetPathOverflow(path_bytes)

  debug_print_path_overflow(&po)

  start_idx := 0
  start_step := po.Position[0]
  for i:=1; i<len(po.Position); i++ {
    if step >= int(po.Position[i-1]) { break }
    start_idx = i
    start_step = po.Position[i]
  }

  fmt.Printf(">>> start_idx %d, start_step %d\n", start_idx, start_step)

  n_ovf,e := CountOverflow(cgf_bytes, path, int(start_step), step) ; _ = n_ovf
  if e!=nil { return -1, nil }

  n_ovf++

  fmt.Printf(">> n_ovf %d\n", n_ovf)

  tme:=0
  seen_rec := 0

  byte_offset := po.Offset[start_idx]
  for p:=int(byte_offset); (p<len(po.Map)) && (seen_rec<n_ovf); seen_rec++  {


    u,dn := dlug.ConvertUint64(po.Map[p:])

    p+=dn

    fmt.Printf("????? %d\n", u)



    tme = int(u)
  }

  fmt.Printf(">>>> seen_rec %d, n_ovf %d\n", seen_rec, n_ovf)

  fmt.Printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> tme %d\n", tme)
  return tme, nil





  return -1,nil

}


func LookupTileMapEntry(cgf_bytes []byte, path, ver, step int) (TileMapEntry, error) {
  tme := TileMapEntry{}

  tm,e := LookupTileMap(cgf_bytes, path, ver, step)
  if e!=nil { return tme, e }

  tme.TileMap = tm

  if tm>=0 {
    tilemap_bytes,e := CGFTilemapBytes(cgf_bytes)
    if e!=nil { return tme, e }
    tile_map := unpack_tilemap(tilemap_bytes)
    tme := tile_map[tm]
    tme.Variant = tme.Variant
    tme.Span = tme.Span
    return tme,nil
  }

  return tme,fmt.Errorf("could not find entry")
}

package main

import _ "os"
import "fmt"
import "log"
import "strings"
import "strconv"
import "../src/dlug"


const CGF_DEFAULT_LIBRARY_FILE string = "default_tile_map_v0.1.0.txt"
const CGF_DEFAULT_LIBRARY_VERSION string = "0.1.0"
const CGF_MAGIC uint64 = 0x7b226367662e6222

type TileMapEntry struct {
  Variant [][]int
  Span [][]int
}

func unpack_tilemap(tilemap_bytes []byte) []TileMapEntry {
  m := make([]TileMapEntry, 0, 1024)

  var n0,n1,vid,span uint64
  var dn int

  count := 0
  pos := 0
  for pos<len(tilemap_bytes) {

    tme := TileMapEntry{}

    n0,dn = dlug.ConvertUint64(tilemap_bytes[pos:])
    pos += dn

    n1,dn = dlug.ConvertUint64(tilemap_bytes[pos:])
    pos += dn

    tme.Variant = make([][]int, 2)
    tme.Span = make([][]int, 2)

    for i:=uint64(0); i<n0; i++ {
      vid,dn = dlug.ConvertUint64(tilemap_bytes[pos:])
      pos += dn
      tme.Variant[0] = append(tme.Variant[0], int(vid))

      span,dn = dlug.ConvertUint64(tilemap_bytes[pos:])
      pos += dn
      tme.Span[0] = append(tme.Span[0], int(span))
    }

    for i:=uint64(0); i<n1; i++ {
      vid,dn = dlug.ConvertUint64(tilemap_bytes[pos:])
      pos += dn
      tme.Variant[1] = append(tme.Variant[1], int(vid))

      span,dn = dlug.ConvertUint64(tilemap_bytes[pos:])
      pos += dn
      tme.Span[1] = append(tme.Span[1], int(span))
    }

    m = append(m, tme)
    count++

  }

  return m
}

// No path structures, so some arrays are of 0 length
//
func cgf_default_tile_map() []byte {
  var n int
  tilemap_bytes := make([]byte, 0, 1024*40)
  buf := make([]byte, 16)

  for ii:=0; ii<len(DEFAULT_TILEMAP); ii++ {
    t := DEFAULT_TILEMAP[ii]

    allele_parts := strings.Split(t, ":")
    class_parts0 := strings.Split(allele_parts[0], ";")
    class_parts1 := strings.Split(allele_parts[1], ";")

    n = dlug.FillSliceUint32(buf,uint32(len(class_parts0)))
    tilemap_bytes = append(tilemap_bytes, buf[:n]...)

    n = dlug.FillSliceUint32(buf,uint32(len(class_parts1)))
    tilemap_bytes = append(tilemap_bytes, buf[:n]...)

    for i:=0; i<len(class_parts0); i++ {
      parts := strings.Split(class_parts0[i], "+")
      span:=1
      if len(parts)>1 {
        span_,e:=strconv.ParseInt(parts[1], 16, 64)
        if e!=nil { log.Fatal(e) }
        span = int(span_)

      }

      tilevar_,e:=strconv.ParseInt(parts[0], 16, 64)
      if e!=nil { log.Fatal(e) }
      tilevar := int(tilevar_)

      n = dlug.FillSliceUint32(buf, uint32(tilevar))
      tilemap_bytes = append(tilemap_bytes, buf[:n]...)

      n = dlug.FillSliceUint32(buf, uint32(span))
      tilemap_bytes = append(tilemap_bytes, buf[:n]...)

    }

    for i:=0; i<len(class_parts1); i++ {
      parts := strings.Split(class_parts1[i], "+")
      span:=1
      if len(parts)>1 {
        span_,e:=strconv.ParseInt(parts[1], 16, 64)
        if e!=nil { log.Fatal(e) }
        span = int(span_)

      }

      tilevar_,e:=strconv.ParseInt(parts[0], 16, 64)
      if e!=nil { log.Fatal(e) }
      tilevar := int(tilevar_)

      n = dlug.FillSliceUint32(buf, uint32(tilevar))
      tilemap_bytes = append(tilemap_bytes, buf[:n]...)

      n = dlug.FillSliceUint32(buf, uint32(span))
      tilemap_bytes = append(tilemap_bytes, buf[:n]...)

    }

  }

  return tilemap_bytes
}

func cgf_default_header_bytes() []byte {
  tbuf := make([]byte, 1024)
  buf := make([]byte, 0, 8192)
  n := 0
  var dn int

  Magic := uint64(CGF_MAGIC)
  tobyte64(tbuf[:8], Magic)
  buf = append(buf, tbuf[0:8]...)
  n+=8

  // CGFVersion string
  //
  CGFVersion := "0.1.0"
  dn = dlug.FillSliceUint64(tbuf, uint64(len(CGFVersion)))
  buf = append(buf, tbuf[0:dn]...)
  n += dn

  buf = append(buf, []byte(CGFVersion)...)
  n += len(CGFVersion)



  // Library Version string
  //
  LibraryVersion := "0.1.0"
  dn = dlug.FillSliceUint64(tbuf, uint64(len(LibraryVersion)))
  buf = append(buf, tbuf[0:dn]...)
  n += dn

  //for i:=n; i<n+len(LibraryVersion); i++ { buf[i]=LibraryVersion[i-n] }
  buf = append(buf, []byte(LibraryVersion)...)
  n += len(LibraryVersion)


  // Path Count
  //
  PathCount := uint64(0)
  tobyte64(tbuf[0:8], PathCount)
  buf = append(buf, tbuf[0:8]...)
  n+=8


  // TileMapLen
  //
  // TileMap
  //
  tilemap := cgf_default_tile_map()

  tobyte64(tbuf[0:8], uint64(len(tilemap)))
  buf = append(buf, tbuf[0:8]...)
  n+=8

  buf = append(buf, tilemap...)
  n+=len(tilemap)

  // StepPerPath
  //
  step_per_path := make([]uint64, 0, 1024)
  if len(step_per_path)>0 {
    for i:=0; i<len(step_per_path); i++ {
      tobyte64(tbuf[0:8], step_per_path[i])
      buf = append(buf, tbuf[0:8]...)
      n+=8
    }
  }

  // TileVectorOffset
  //
  tile_vector_offset := make([]uint64, 0, 1024)
  if len(tile_vector_offset)>0 {
    for i:=0; i<len(step_per_path); i++ {
      tobyte64(tbuf[0:8], step_per_path[i])
      buf = append(buf, tbuf[0:8]...)
      n+=8
    }
  }

  PathStruct := make([]byte, 0, 1024)
  if len(PathStruct)>0 {
    buf = append(buf, PathStruct...)
    n += len(PathStruct)
  }

  return buf
}

func fill_header_struct_from_bytes(cgf *CGF, b []byte) {
  var dn int
  n:=0 ; _ = n

  cgf.Magic = byte2uint64(b)
  n+=8

  cgf.CGFVersion,dn = byte2string(b[n:])
  n+=dn

  cgf.LibraryVersion,dn = byte2string(b[n:])
  n+=dn

  cgf.PathCount = byte2uint64(b[n:])
  n+=8

  cgf.TileMapLen = byte2uint64(b[n:])
  n+=8

  cgf.TileMap = b[n:n+int(cgf.TileMapLen)]
  n+=int(cgf.TileMapLen)

  cgf.StepPerPath = make([]uint64, cgf.PathCount)
  for i:=uint64(0); i<cgf.PathCount; i++ {
    cgf.StepPerPath[i] = byte2uint64(b[n:])
    n+=8
  }

  cgf.TileVectorOffset = make([]uint64, cgf.PathCount)
  for i:=uint64(0); i<cgf.PathCount; i++ {
    cgf.TileVectorOffset[i] = byte2uint64(b[n:])
    n+=8
  }

  cgf.Path = make([]PathStruct, 0, 11000)

}

func print_tilemap_info(cgf *CGF) {
  tm := unpack_tilemap(cgf.TileMap)

  for k:=0; k<len(tm); k++ {
    for i:=0; i<len(tm[k].Variant[0]); i++ {
      if i>0 { fmt.Printf(";") }
      fmt.Printf("%x", tm[k].Variant[0][i])
      if tm[k].Span[0][i]>1 {
        fmt.Printf("+%x", tm[k].Span[0][i])
      }
    }

    fmt.Printf(":")

    for i:=0; i<len(tm[k].Variant[1]); i++ {
      if i>0 { fmt.Printf(";") }
      fmt.Printf("%x", tm[k].Variant[1][i])
      if tm[k].Span[1][i]>1 {
        fmt.Printf("+%x", tm[k].Span[1][i])
      }
    }

    fmt.Printf("\n")

  }

}

func print_header_info(cgf *CGF) {

  var magic_buf = make([]byte, 8)

  for i:=0; i<8; i++ {
    magic_buf[i] = uint8(cgf.Magic>>(uint(8*(7-i))) & 0xff)
  }

  fmt.Printf("Magic %08x (%c %c %c %c %c %c %c %c)\n", cgf.Magic,
    magic_buf[0], magic_buf[1], magic_buf[2], magic_buf[3], magic_buf[4], magic_buf[5], magic_buf[6], magic_buf[7] )
  fmt.Printf("CGFVersion %s\n", cgf.CGFVersion)
  fmt.Printf("LibraryVersion %s\n", cgf.LibraryVersion)
  fmt.Printf("PathCount %d\n", cgf.PathCount)
  fmt.Printf("TileMapLen %d\n", cgf.TileMapLen)
  fmt.Printf("TileMap(%d):\n", len(cgf.TileMap))

  tm := unpack_tilemap(cgf.TileMap)

  for k:=0; k<len(tm); k++ {
    fmt.Printf("  [%d]", k)

    fmt.Printf(" (")
    for i:=0; i<len(tm[k].Variant[0]); i++ {
      fmt.Printf(" %x", tm[k].Variant[0][i])
      if tm[k].Span[0][i]>1 {
        fmt.Printf("+%x", tm[k].Span[0][i])
      }
    }
    fmt.Printf(" )")

    fmt.Printf(" (")
    for i:=0; i<len(tm[k].Variant[1]); i++ {
      fmt.Printf(" %x", tm[k].Variant[1][i])
      if tm[k].Span[1][i]>1 {
        fmt.Printf("+%x", tm[k].Span[1][i])
      }
    }
    fmt.Printf(" )")

    fmt.Printf("\n")

  }

}

/*
func main() {

  zb := make([]byte, 8)

  tobyte64(zb, CGF_MAGIC)

  b := cgf_default_header_bytes()

  cgf := CGF{}

  fill_header_struct_from_bytes(&cgf, b)

  //print_header_info(&cgf)
  print_tilemap_info(&cgf)
}
*/

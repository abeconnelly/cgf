package main

import "fmt"
import "io/ioutil"
import "strconv"
import "strings"
import "crypto/md5"

import "io"
import "os/exec"
import "bytes"

import "log"

//const CGLF_PATH = "/home/abram/play/lightning/cglf/cglf"

func print_fold_seq(s string, w int) {
  if w<0 { w = len(s) }
  for i:=0; i<len(s); i+=w {
    en := i+w
    if en > len(s) { en = len(s) }
    fmt.Printf("%s\n", s[i:en])
  }
}


func cglf_helper(fn, name string) []byte {
  cmd0 := exec.Command("gunzip", "-c", fn)
  cmd1 := exec.Command("twoBitGulp", "-name", name, "-no-header", "-terse", "-w", "0")

  r,w := io.Pipe()
  cmd0.Stdout = w
  cmd1.Stdin = r

  var b bytes.Buffer
  cmd1.Stdout = &b

  var e error

  e = cmd0.Start()
  if e!=nil { panic(e); log.Fatal(e) }

  e = cmd1.Start()
  if e!=nil { panic(e) ;log.Fatal(e) }

  e = cmd0.Wait()
  if e!=nil { panic(e) ; log.Fatal(e) }

  e = w.Close()
  if e!=nil { panic(e) ; log.Fatal(e) }

  e = cmd1.Wait()
  if e!=nil { panic(e) ; log.Fatal(e) }

  return b.Bytes()
}

func handle_loq(cgf_bytes []byte, path, tagset_version, step int) {
}

func handle_overflow_cascade(cgf_bytes []byte, path, tagset_version, step uint64, cglf_path string) {

  tile_map_entry,e := LookupTileMapEntry(cgf_bytes, int(path), 0, int(step))
  if e!=nil {
    log.Fatal( fmt.Sprintf("ERROR: %v: could not find overflow entry for path %d, ver %d, step %d\n", e, path, tagset_version, step) )
  }

  if tile_map_entry.TileMap<0 {
    fmt.Printf("Final overflow (not implemented yet)\n")

    /*
    toi,e := CGFTileOverflowInformation(cgf_bytes, path, 0, step)
    if e!=nil {
      log.Fatal("ERROR: %v: could not find overflow entry for path %d, ver %d, step %d\n", e, path, tagset_version, step)
    }
    */

  } else {

    fmt.Printf(">>>> %04x.%02x.%04x --> %d (%x)\n", path, tagset_version, step, tile_map_entry, tile_map_entry)

    /*
    tme := tilemap[hexit]

    for allele:=0; allele<2; allele++ {
      cur_step := int(step)
      for a:=0; a<len(tme.Variant[allele]); a++ {

        seq := cglf_get_lib_seq(uint64(path), uint64(cur_step), uint64(tme.Variant[allele][a]), uint64(tme.Span[allele][a]), cglf_path)

        m5str := md5sum2str(md5.Sum([]byte(seq)))
        fmt.Printf("> { \"notes\":\"allele%d[%d] %d+%d\", \"md5sum\":\"%s\" }\n",
          allele, a, tme.Variant[allele][a], tme.Span[allele][a], m5str)
        print_fold_seq(seq, 50)
        fmt.Printf("\n")
        cur_step += tme.Span[allele][a]

      }
    }
    */

  }

}


// bootstrap.  We will replace this with a more efficient lookup
//
func cglf_get_lib_seq(path, step, varid, span uint64, cglf_path string) string {
  ver := 0
  fn := fmt.Sprintf("%s/%04x/%04x.%02x.%04x.2bit.gz", cglf_path, path, path, ver, step)
  name := fmt.Sprintf("%04x.%02x.%04x.%03x+%x", path, ver, step, varid, span)
  seq := cglf_helper(fn, name)
  return string(seq)
}


func print_tile_cglf(cgf_fn string, tilepos string, cglf_path string) error {
  tagset_version := 0 ; _ = tagset_version

  cgf_bytes,e := ioutil.ReadFile(cgf_fn)
  if e!=nil { return e }

  tilepos_parts := strings.Split(tilepos, ".")
  if (len(tilepos_parts)!=2) && (len(tilepos_parts)!=3) { return fmt.Errorf("invalid tilepos (must be of form <PATH>.<VER>.<STEP>)") }

  var path,ver,step uint64

  ind:=0
  path,e = strconv.ParseUint(tilepos_parts[ind], 16, 64) ; _ = path
  if e!=nil { return e }
  ind++


  if len(tilepos_parts)==3 {
    ver,e = strconv.ParseUint(tilepos_parts[ind], 16, 64) ; _ = ver
    if e!=nil { return e }
    ind++
  }

  step,e = strconv.ParseUint(tilepos_parts[ind], 16, 64) ; _ = step
  if e!=nil { return e }
  ind++

  //path_vec,e := CGFVectorUint64(cgf_bytes, int(path)) ; _ = path_vec
  //if e!=nil { return e }

  hdri,_ := headerintermediate_from_bytes(cgf_bytes[:])

  //path,ver,step,e := parse_tilepos(c.String("tilepos"))
  //if e!=nil { log.Fatal(e) }

  if path<0 { log.Fatal("path must be positive") }
  if step<0 { log.Fatal("step must be positive") }
  if int(path) >= len(hdri.step_per_path) { log.Fatal("path out of range (max ", len(hdri.step_per_path), " paths)") }
  if int(step) >= hdri.step_per_path[path] { log.Fatal("step out of range (max ", hdri.step_per_path[path], " steps)") }

  pathi,_ := pathintermediate_from_bytes(hdri.path_bytes[path])

  knot := get_knot(hdri.tilemap, pathi, int(step))
  if knot==nil { return fmt.Errorf(fmt.Sprintf("spanning tile")) }

  for i:=0; i<len(knot); i++ {
    phase_str := "A"
    if i==1 { phase_str = "B" }


    for j:=0; j<len(knot[i]); j++ {
      fmt.Printf("> {")
      fmt.Printf(" \"tileID\":\"%04x.%02x.%04x.%03x\", \"seedTileLength\":%d",
        path, ver,
        knot[i][j].Step,
        knot[i][j].VarId,
        knot[i][j].Span)

      seq := cglf_get_lib_seq(uint64(path),
                              uint64(knot[i][j].Step),
                              uint64(knot[i][j].VarId),
                              uint64(knot[i][j].Span),
                              cglf_path)


      n := len(seq)
      fmt.Printf(", \"n\":%d", n)

      startTile := false
      endTile:=false

      if knot[i][j].Step==0 {
        startTile = true
        fmt.Printf(", \"startTile\":true")
      } else {
        fmt.Printf(", \"startTile\":false")
      }

      if (knot[i][j].Step+1)==hdri.step_per_path[int(path)] {
        endTile = true
        fmt.Printf(", \"endTile\":true")
      } else {
        fmt.Printf(", \"endTile\":false")
      }

      if startTile {
        fmt.Printf(", \"startTag\":\"\"")
      } else {
        fmt.Printf(", \"startTag\":\"%s\"", seq[0:24])
      }

      if endTile {
        fmt.Printf(", \"endTag\":\"%s\"", seq[n-24:n])
      } else {
        fmt.Printf(", \"endTag\":\"\"")
      }


      if len(knot[i][j].NocallStartLen)>0 {
        noc_seq := fill_noc_seq(seq, knot[i][j].NocallStartLen)
        noc_m5str := md5sum2str(md5.Sum([]byte(noc_seq)))

        noc_count := 0
        for ii:=0; ii<len(knot[i][j].NocallStartLen); ii+=2 {
          noc_count += knot[i][j].NocallStartLen[ii+1]
        }

        fmt.Printf(", \"md5sum\":\"%s\"", noc_m5str)

        if startTile {
          fmt.Printf(", \"startSeq\":\"\"")
        } else {
          fmt.Printf(", \"startSeq\":\"%s\"", noc_seq[0:24])
        }

        if endTile {
          fmt.Printf(", \"endSeq\":\"\"")
        } else {
          fmt.Printf(", \"endSeq\":\"%s\"", noc_seq[n-24:n])
        }

        fmt.Printf(", \"notes\":[")
        fmt.Printf("\"Allele %d\",\"Phase %s\"", i, phase_str)
        fmt.Printf(",\"")
        fmt.Printf("*{")
        for p:=0; p<len(knot[i][j].NocallStartLen); p+=2 {
          if p>0 { fmt.Printf(";") }
          fmt.Printf("%d+%d",
            knot[i][j].NocallStartLen[p],
            knot[i][j].NocallStartLen[p+1])
        }
        fmt.Printf("}\"")
        fmt.Printf("] }\n")

        //fmt.Printf(" %s\n%s\n", noc_m5str, noc_seq)

        print_fold_seq(noc_seq, 50)
        fmt.Printf("\n")
      } else {
        m5str := md5sum2str(md5.Sum([]byte(seq)))
        //fmt.Printf(" %s\n%s\n", m5str, seq)

        fmt.Printf(", \"md5sum\":\"%s\"", m5str)

        if startTile {
          fmt.Printf(", \"startSeq\":\"\"")
        } else {
          fmt.Printf(", \"startSeq\":\"%s\"", seq[0:24])
        }

        if endTile {
          fmt.Printf(", \"endSeq\":\"\"")
        } else {
          fmt.Printf(", \"endSeq\":\"%s\"", seq[n-24:n])
        }

        fmt.Printf(", \"notes\":[")
        fmt.Printf("\"Allele %d\",\"Phase %s\"", i, phase_str)
        fmt.Printf(",\"")
        fmt.Printf("*{")
        for p:=0; p<len(knot[i][j].NocallStartLen); p+=2 {
          if p>0 { fmt.Printf(";") }
          fmt.Printf("%d+%d",
            knot[i][j].NocallStartLen[p],
            knot[i][j].NocallStartLen[p+1])
        }
        fmt.Printf("}\"")
        fmt.Printf("] }\n")


        print_fold_seq(seq, 50)
        fmt.Printf("\n")
      }

    }
  }


  return nil

  /*
  pos := step/32
  pos_offset := uint8(step%32)

  fmt.Printf("%8x|%8x\n", path_vec[pos]>>32, path_vec[pos]&0xffffffff)


  x := path_vec[pos]

  tilemap_bytes,_ := CGFTilemapBytes(cgf_bytes)
  tilemap := unpack_tilemap(tilemap_bytes)

  if (x & (1<<(pos_offset+32))) > 0 {
    fmt.Printf("non canonical tile\n")

    hexit_pos:=uint8(0)
    for sh:=uint8(0); sh<pos_offset; sh++ {
      if (x & (1<<(sh+32))) > 0 { hexit_pos++ }
    }

    if hexit_pos>(32/4) {
      fmt.Printf("hexit pos overflow\n")

      //handle_overflow_cascade(cgf_bytes, path, tagset_version, step)


    } else {
      hexit := (x >> (4*hexit_pos)) & 0xf

      fmt.Printf("hexit: %x (hexit pos %d)\n", hexit, hexit_pos)

      if (hexit>0) && (hexit<0xd) {

        fmt.Printf("# (hexit) tilemap entry %d\n", hexit)

        tme := tilemap[hexit]

        for allele:=0; allele<2; allele++ {
          cur_step := int(step)
          for a:=0; a<len(tme.Variant[allele]); a++ {


            //seq := sglf.Lib[int(path)][cur_step][tme.Variant[allele][a]]
            seq := cglf_get_lib_seq(uint64(path), uint64(cur_step), uint64(tme.Variant[allele][a]), uint64(tme.Span[allele][a]), cglf_path)

            m5str := md5sum2str(md5.Sum([]byte(seq)))
            fmt.Printf("> { \"notes\":\"allele%d[%d] %d+%d\", \"md5sum\":\"%s\" }\n",
              allele, a, tme.Variant[allele][a], tme.Span[allele][a], m5str)
            print_fold_seq(seq, 50)
            fmt.Printf("\n")
            cur_step += tme.Span[allele][a]
          }
        }

      } else if hexit==0 {

        fmt.Printf("# spanning\n")

      } else if hexit == 0xd {

        fmt.Printf("# complex (not handled)\n")

      } else if hexit == 0xe {

        fmt.Printf("# loq\n")

        //handle_loq(cgf_bytes, path, tagset_version, step)

      } else if hexit == 0xf {
        fmt.Printf("# cache overflow\n")

        handle_overflow_cascade(cgf_bytes, path, uint64(tagset_version), step, cglf_path)

      }
    }

  } else {
    fmt.Printf("# Canonincal tile:\n")

    //seq := sglf.Lib[int(path)][int(step)][0]
    seq := cglf_get_lib_seq(uint64(path), uint64(step), 0, 1, cglf_path)

    m5str := md5sum2str(md5.Sum([]byte(seq)))
    fmt.Printf("> { \"md5sum\":\"%s\" }\n", m5str)
    print_fold_seq(seq, 50)
  }


  //varid,loq,ovf := cgf_
  //path_vec := cgf_raw_path_vec(cgf_bytes, path)


  return nil
  */

}

func print_tile_sglf(cgf_fn string, tilepos string, sglf SGLF) error {

  cgf_bytes,e := ioutil.ReadFile(cgf_fn)
  if e!=nil { return e }

  tilepos_parts := strings.Split(tilepos, ".")
  if (len(tilepos_parts)!=2) && (len(tilepos_parts)!=3) { return fmt.Errorf("invalid tilepos (must be of form <PATH>.<VER>.<STEP>)") }

  var path,ver,step uint64

  ind:=0
  path,e = strconv.ParseUint(tilepos_parts[ind], 16, 64) ; _ = path
  if e!=nil { return e }
  ind++


  if len(tilepos_parts)==3 {
    ver,e = strconv.ParseUint(tilepos_parts[ind], 16, 64) ; _ = ver
    if e!=nil { return e }
    ind++
  }

  step,e = strconv.ParseUint(tilepos_parts[ind], 16, 64) ; _ = step
  if e!=nil { return e }
  ind++

  path_vec,e := CGFVectorUint64(cgf_bytes, int(path)) ; _ = path_vec
  if e!=nil { return e }

  pos := step/32
  pos_offset := uint8(step%32)

  fmt.Printf("%8x|%8x\n", path_vec[pos]>>32, path_vec[pos]&0xffffffff)


  x := path_vec[pos]

  tilemap_bytes,_ := CGFTilemapBytes(cgf_bytes)
  tilemap := unpack_tilemap(tilemap_bytes)

  if (x & (1<<(pos_offset+32))) > 0 {
    fmt.Printf("non canonical tile\n")

    hexit_pos:=uint8(0)
    for sh:=uint8(0); sh<pos_offset; sh++ {
      if (x & (1<<(sh+32))) > 0 { hexit_pos++ }
    }

    if hexit_pos>(32/4) {
      fmt.Printf("hexit pos overflow\n")
    } else {
      hexit := (x >> (4*hexit_pos)) & 0xf

      fmt.Printf("hexit: %x (hexit pos %d)\n", hexit, hexit_pos)

      if (hexit>0) && (hexit<0xd) {

        fmt.Printf("# (hexit) tilemap entry %d\n", hexit)

        tme := tilemap[hexit]

        for allele:=0; allele<2; allele++ {
          cur_step := int(step)
          for a:=0; a<len(tme.Variant[allele]); a++ {
            seq := sglf.Lib[int(path)][cur_step][tme.Variant[allele][a]]
            m5str := md5sum2str(md5.Sum([]byte(seq)))
            fmt.Printf("> { \"notes\":\"allele%d[%d] %d+%d\", \"md5sum\":\"%s\" }\n",
              allele, a, tme.Variant[allele][a], tme.Span[allele][a], m5str)
            print_fold_seq(seq, 50)
            fmt.Printf("\n")
            cur_step += tme.Span[allele][a]
          }
        }

      } else if hexit==0 {

        fmt.Printf("# spanning\n")
      } else if hexit == 0xd {
        fmt.Printf("# complex (not handled)\n")
      } else if hexit == 0xe {
        fmt.Printf("# loq\n")
      } else if hexit == 0xf {
        fmt.Printf("# cache overflow\n")
      }
    }

  } else {
    fmt.Printf("# Canonincal tile:\n")

    seq := sglf.Lib[int(path)][int(step)][0]
    m5str := md5sum2str(md5.Sum([]byte(seq)))
    fmt.Printf("> { \"md5sum\":\"%s\" }\n", m5str)
    print_fold_seq(seq, 50)
  }


  //varid,loq,ovf := cgf_
  //path_vec := cgf_raw_path_vec(cgf_bytes, path)


  return nil
}

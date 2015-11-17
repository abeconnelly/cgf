package main

import "fmt"
import "os"
import "runtime"
import "runtime/pprof"

import "log"
import "strings"

import "github.com/abeconnelly/autoio"
import "github.com/codegangsta/cli"

import "strconv"
import "io/ioutil"

import "crypto/md5"

import "github.com/abeconnelly/cgf"
import "github.com/abeconnelly/cglf"

var VERSION_STR string = "0.1.0"
var gVerboseFlag bool

var gProfileFlag bool
var gProfileFile string = "cgf.pprof"

var gMemProfileFlag bool
var gMemProfileFile string = "cgf.mprof"

var gShowKnotNocallInfoFlag bool = true

var use_SGLF bool = false

/*
func tile_cmp(tile, lib string) bool {
  if len(tile) != len(lib) { return false }
  for i:=0; i<len(tile); i++ {
    if tile[i] == 'n' || tile[i] == 'N' { continue }
    if tile[i] != lib[i] { return false }
  }
  return true
}

func nocall_string(tile string) string {
  z := make([]string, 0, 16)

  cur_start := 0
  cur_n := 0
  for i:=0; i<len(tile); i++ {

    if tile[i]=='n' || tile[i]=='N' {

      if cur_n==0 {
        cur_start = i
      }
      cur_n++


    } else if (cur_n>0) {
      z = append(z, fmt.Sprintf("(%d+%d)", cur_start, cur_n))
      cur_n=0
    }

  }

  if (cur_n>0) {
    z = append(z, fmt.Sprintf("(%d+%d)", cur_start, cur_n))
    cur_n=0
  }

  return strings.Join(z, ";")

}

func print_lookup(sglf SGLF, allele_path [][]TileInfo) {
  var ok bool

  for allele_idx:=0; allele_idx<len(allele_path); allele_idx++ {
    for path_idx:=0; path_idx<len(allele_path[allele_idx]); path_idx++ {

      ti := allele_path[allele_idx][path_idx]
      sglf_info := SGLFInfo{}
      if path_idx>0 {
        sglf_info,ok = sglf.PfxTagLookup[ti.PfxTag]
      } else {
        sglf_info,ok = sglf.SfxTagLookup[ti.SfxTag]
      }

      if !ok {
        log.Fatal("could not find prefix (%s) in sglf (allele_idx %d, path_idx %d (%x))\n",
          ti.PfxTag, allele_idx, path_idx, path_idx)
      }

      path := sglf_info.Path
      step := sglf_info.Step

      var_idx:=0
      for ; var_idx < len(sglf.Lib[path][step]); var_idx++ {

        if tile_cmp(ti.Seq, sglf.Lib[path][step][var_idx]) {

          nocall_str := nocall_string(ti.Seq)

          fmt.Printf(">> a{%d} %04x.00.%04x.%03x %s\n", allele_idx, path, step, var_idx, nocall_str)
          break
        }
      }
      if var_idx == len(sglf.Lib[path][step]) {
        log.Fatal("could not find tile element in library: allele_idx %d, path_idx %d (%x)",
          allele_idx, path_idx, path_idx)
      }

    }
  }

}

func lookup_variant_index(seq string, var_lib []string) (int,error) {
  var_idx:=0
  for ; var_idx < len(var_lib); var_idx++ {

    if tile_cmp(seq, var_lib[var_idx]) {
      return var_idx,nil

      //nocall_str := nocall_string(ti.Seq)
      //fmt.Printf(">> a{%d} %04x.00.%04x.%03x %s\n", allele_idx, path, step, var_idx, nocall_str)
      //break
    }
  }
  return -1,fmt.Errorf("could not find tile element in library for sequence '%s'", seq)
}

var test_cgf CGF

func print_zipper(sglf SGLF, allele_path [][]TileInfo) {

  var ok bool

  var path0 int
  var path1 int

  var step_idx0 int
  var step_idx1 int

  var step0 int
  var step1 int

  sglf_info0 := SGLFInfo{}
  sglf_info1 := SGLFInfo{}

  tile_zipper := make([][]SGLFInfo, 2)
  tile_zipper_seq := make([][]string, 2)

  span_sum := 0

  for (step_idx0<len(allele_path[0])) && (step_idx1<len(allele_path[1])) {

    if span_sum >= 0 {
      ti0 := allele_path[0][step_idx0]

      //DEBUG
      //fmt.Printf("ti0 %s %s\n", ti0.PfxTag, ti0.SfxTag)

      // sglf_info1 only holds a valid path and step
      //
      if step_idx0>0 {
        sglf_info0,ok = sglf.PfxTagLookup[ti0.PfxTag]

        //DEBUG
        //fmt.Printf("  pfx\n")
      } else {
        sglf_info0,ok = sglf.SfxTagLookup[ti0.SfxTag]

        //DEBUG
        //fmt.Printf("  sfx\n")
      }

      if !ok {
        log.Fatal("could not find prefix (%s) in sglf (allele_idx %d, step_idx %d (%x))\n",
          ti0.PfxTag, 0, step_idx0, step_idx0)
      }

      path0 = sglf_info0.Path
      step0 = sglf_info0.Step

      //fmt.Printf("  path.step %04x.00.%04x (%d,%d)\n", path0,step0,  path0,step0)

      // We need to search for the variant in the Lib to find
      // the rest of the information, including span
      //
      var_idx0,e := lookup_variant_index(ti0.Seq, sglf.Lib[path0][step0])
      if e!=nil { log.Fatal(e) }

      sglf_info0 = sglf.LibInfo[path0][step0][var_idx0]

      span0 := sglf_info0.Span

      sglf_info0.Variant = var_idx0

      seq0 := sglf.Lib[path0][step0][var_idx0]
      tile_zipper[0] = append(tile_zipper[0], sglf_info0)
      tile_zipper_seq[0] = append(tile_zipper_seq[0], seq0)

      span_sum -= span0
      step_idx0++

    } else {
      ti1 := allele_path[1][step_idx1]

      // sglf_info1 only holds a valid path and step
      //
      if step_idx1>0 {
        sglf_info1,ok = sglf.PfxTagLookup[ti1.PfxTag]
      } else {
        sglf_info1,ok = sglf.SfxTagLookup[ti1.SfxTag]
      }

      if !ok {
        log.Fatal("could not find prefix (%s) in sglf (allele_idx %d, step_idx %d (%x))\n",
          ti1.PfxTag, 1, step_idx1, step_idx1)
      }


      path1 = sglf_info1.Path
      step1 = sglf_info1.Step

      // We need to search for the variant in the Lib to find
      // the rest of the information, including span
      //
      var_idx1,e := lookup_variant_index(ti1.Seq, sglf.Lib[path1][step1])
      if e!=nil { log.Fatal(e) }

      sglf_info1 = sglf.LibInfo[path1][step1][var_idx1]

      sglf_info1.Variant = var_idx1

      seq1 := sglf.Lib[path1][step1][var_idx1]
      tile_zipper[1] = append(tile_zipper[1], sglf_info1)
      tile_zipper_seq[1] = append(tile_zipper_seq[1], seq1)

      span1 := sglf_info1.Span

      span_sum += span1
      step_idx1++

    }


    if span_sum==0 {

      fmt.Printf("[{a}")
      for i:=0; i<len(tile_zipper[0]); i++ {
        nocall_str0 := nocall_string(tile_zipper_seq[0][i])
        fmt.Printf(" %04x.00.%04x.%03x+%x;%s",
          tile_zipper[0][i].Path,
          tile_zipper[0][i].Step,
          tile_zipper[0][i].Variant,
          tile_zipper[0][i].Span,
          nocall_str0)
      }
      fmt.Printf(" ]\n")

      fmt.Printf("[{b}")
      for i:=0; i<len(tile_zipper[1]); i++ {
        nocall_str1 := nocall_string(tile_zipper_seq[1][i])
        fmt.Printf(" %04x.00.%04x.%03x+%x;%s",
          tile_zipper[1][i].Path,
          tile_zipper[1][i].Step,
          tile_zipper[1][i].Variant,
          tile_zipper[1][i].Span,
          nocall_str1)

      }
      fmt.Printf(" ]\n")
      fmt.Printf("\n")


      tile_zipper[0] = tile_zipper[0][0:0]
      tile_zipper[1] = tile_zipper[1][0:0]

      tile_zipper_seq[0] = tile_zipper_seq[0][0:0]
      tile_zipper_seq[1] = tile_zipper_seq[1][0:0]

    }

  }

}
*/

func _main( c *cli.Context ) {
  gShowKnotNocallInfoFlag = !c.Bool("hide-knot-low-quality")

  inp_slice := c.StringSlice("input")

  cglf_lib_location := c.String("cglf")

  action := c.String("action")
  if action == "debug" {
    //debug_read(c.String("cgf"))
    cgf.DebugRead(c.String("cgf"))
    return
  } else if action == "headercheck" {


    //header_bytes := cgf_default_header_bytes()
    header_bytes := cgf.CGFDefaultHeaderBytes()

    //hdri,dn := headerintermediate_from_bytes(header_bytes) ; _ = dn
    hdri,dn := cgf.HeaderIntermediateFromBytes(header_bytes) ; _ = dn
    //hdri_bytes := bytes_from_headerintermediate(hdri)
    hdri_bytes := cgf.BytesFromHeaderIntermediate(hdri)
    //hdri1,dn2 := headerintermediate_from_bytes(hdri_bytes) ; _ = dn2
    hdri1,dn2 := cgf.HeaderIntermediateFromBytes(hdri_bytes) ; _ = dn2

    //err := headerintermediate_cmp(hdri, hdri1)
    err := cgf.HeaderIntermediateCmp(hdri, hdri1)

    if err!=nil { log.Fatal(err) }
    return
  } else if action == "header" {

    ocgf := c.String("output")

    //header_bytes := cgf_default_header_bytes()
    header_bytes := cgf.CGFDefaultHeaderBytes()

    f,err := os.Create(ocgf)
    if err!=nil { log.Fatal(err) }

    f.Write(header_bytes)
    f.Sync()
    f.Close()

    return

  } else if action == "knot" {

    cglf_path := c.String("cglf")
    if len(cglf_path)==0 {
      fmt.Fprintf( os.Stderr, "Provide CGLF\n" )
      cli.ShowAppHelp(c)
      os.Exit(1)
    }

    cgf_bytes,e := ioutil.ReadFile(c.String("cgf"))
    if e!=nil { log.Fatal(e) }

    //hdri,dn := headerintermediate_from_bytes(cgf_bytes[:])
    hdri,dn := cgf.HeaderIntermediateFromBytes(cgf_bytes[:])
    _ = hdri
    _ = dn

    //path,ver,step,e := parse_tilepos(c.String("tilepos"))
    path,ver,step,e := cgf.ParseTilepos(c.String("tilepos"))
    if e!=nil { log.Fatal(e) }

    if path<0 { log.Fatal("path must be positive") }
    if step<0 { log.Fatal("step must be positive") }
    //if path >= len(hdri.step_per_path) { log.Fatal("path out of range (max ", len(hdri.step_per_path), " paths)") }
    //if step>= hdri.step_per_path[path] { log.Fatal("step out of range (max ", hdri.step_per_path[path], " steps)") }

    if path >= len(hdri.StepPerPath) { log.Fatal("path out of range (max ", len(hdri.StepPerPath), " paths)") }
    if step>= hdri.StepPerPath[path] { log.Fatal("step out of range (max ", hdri.StepPerPath[path], " steps)") }

    //pathi,_ := pathintermediate_from_bytes(hdri.path_bytes[path])
    pathi,_ := cgf.PathIntermediateFromBytes(hdri.PathBytes[path])

    knot := cgf.GetKnot(hdri.TileMap, pathi, step)
    if knot==nil {
      fmt.Printf("spanning tile?\n")
    } else {

      for i:=0; i<len(knot); i++ {
        phase_str := "A"
        if i==1 { phase_str = "B" }

        for j:=0; j<len(knot[i]); j++ {
          fmt.Printf("%s %04x.%02x.%04x.%03x+%x",
            phase_str,
            path, ver,
            knot[i][j].Step,
            knot[i][j].VarId,
            knot[i][j].Span)

          seq := cgf.CGLFGetLibSeq(uint64(path),
                                  uint64(knot[i][j].Step),
                                  uint64(knot[i][j].VarId),
                                  uint64(knot[i][j].Span),
                                  cglf_path)

          if len(knot[i][j].NocallStartLen)>0 {
            fmt.Printf("*{")
            for p:=0; p<len(knot[i][j].NocallStartLen); p+=2 {
              if p>0 { fmt.Printf(";") }
              fmt.Printf("%d+%d",
                knot[i][j].NocallStartLen[p],
                knot[i][j].NocallStartLen[p+1])
            }
            fmt.Printf("}")

            //noc_seq := fill_noc_seq(seq, knot[i][j].NocallStartLen)
            noc_seq := cgf.FillNocSeq(seq, knot[i][j].NocallStartLen)
            noc_m5str := cgf.Md5sum2str(md5.Sum([]byte(noc_seq)))
            fmt.Printf(" %s\n%s\n", noc_m5str, noc_seq)
          } else {
            m5str := cgf.Md5sum2str(md5.Sum([]byte(seq)))
            fmt.Printf(" %s\n%s\n", m5str, seq)
          }

        }

      }

    }

    return


  } else if action == "fastj" {


    tilepos_str := c.String("tilepos")
    if len(tilepos_str)==0 { log.Fatal("missing tilepos") }

    if use_SGLF {
      _sglf,e := cglf.LoadGenomeLibraryCSV(c.String("sglf"))
      if e!=nil { log.Fatal(e) }

      for i:=0; i<len(inp_slice); i++ {
        //e = print_tile_sglf(inp_slice[i], tilepos_str, sglf)
        e = cgf.PrintTileSGLF(inp_slice[i], tilepos_str, _sglf)
        if e!=nil { log.Fatal(e) }
      }
    } else {
      if len(c.String("cgf"))!=0 {
        inp_slice = append(inp_slice, c.String("cgf"))
      }

      for i:=0; i<len(inp_slice); i++ {
        //e := print_tile_cglf(inp_slice[i], tilepos_str, cglf_lib_location)
        e := cgf.PrintTileCGLF(inp_slice[i], tilepos_str, cglf_lib_location)
        if e!=nil { log.Fatal(e) }
      }

    }

    return
  } else if action == "fastj-range" {

    tilepos_str := c.String("tilepos")

    pos_parts := strings.Split(tilepos_str, ".")
    if (len(pos_parts)!=2) && (len(pos_parts)!=3) {
      fmt.Fprintf(os.Stderr, "Invalid tilepos\n")
      cli.ShowAppHelp(c)
      os.Exit(1)
    }

    path_range,e := parseIntOption(pos_parts[0], 16)
    if e!=nil {
      fmt.Fprintf(os.Stderr, "Invalid path in tilepos: %v\n", e)
      cli.ShowAppHelp(c)
      os.Exit(1)
    }

    pp:=1
    if len(pos_parts)==3 { pp=2 }

    step_range,e := parseIntOption(pos_parts[pp], 16)
    if e!=nil {
      fmt.Fprintf(os.Stderr, "Invalid step in tilepos: %v\n", e)
      cli.ShowAppHelp(c)
      os.Exit(1)
    }

    if len(tilepos_str)==0 { log.Fatal("missing tilepos") }

    if len(c.String("sglf"))>0 { use_SGLF = true }

    if use_SGLF {
      _sglf,e := cglf.LoadGenomeLibraryCSV(c.String("sglf")) ; _ = _sglf
      if e!=nil { log.Fatal(e) }

      if len(c.String("cgf"))!=0 {
        inp_slice = append(inp_slice, c.String("cgf"))
      }

      for i:=0; i<len(inp_slice); i++ {
        cgf_bytes,e := ioutil.ReadFile(inp_slice[i])
        if e!=nil { log.Fatal(e) }

        path := path_range[0][0]

        //hdri,_ := headerintermediate_from_bytes(cgf_bytes) ; _ = hdri
        //pathi,_ := pathintermediate_from_bytes(hdri.PathBytes[path]) ; _ = pathi

        hdri,_ := cgf.HeaderIntermediateFromBytes(cgf_bytes) ; _ = hdri
        pathi,_ := cgf.PathIntermediateFromBytes(hdri.PathBytes[path]) ; _ = pathi

        //hdri,dn := headerintermediate_from_bytes(cgf_bytes)
        hdri,dn := cgf.HeaderIntermediateFromBytes(cgf_bytes)
        if dn<0 { log.Fatal("could not construct header from bytes") }

        //patho,dn := pathintermediate_from_bytes(hdri.PathBytes[path])
        patho,dn := cgf.PathIntermediateFromBytes(hdri.PathBytes[path])
        if dn<0 { log.Fatal("could not construct path") }

        tilemap_bytes,_ := cgf.CGFTilemapBytes(cgf_bytes)
        //tilemap := unpack_tilemap(tilemap_bytes)
        tilemap := cgf.UnpackTileMap(tilemap_bytes)

        for step_idx:=0; step_idx<len(step_range); step_idx++ {
          if step_range[step_idx][1] == -1 {
            //step_range[step_idx][1] = int64(hdri.step_per_path[path])
            step_range[step_idx][1] = int64(hdri.StepPerPath[path])
          }
        }

        for stepr_idx:=0; stepr_idx<len(step_range); stepr_idx++ {
          for step:=step_range[stepr_idx][0]; step<step_range[stepr_idx][1]; step++ {
            //knot := GetKnot(tilemap, patho, int(step))
            //print_knot_fastj_sglf(knot, _sglf, uint64(path), 0, hdri)
            knot := cgf.GetKnot(tilemap, patho, int(step))
            cgf.PrintKnotFastjSGLF(knot, _sglf, uint64(path), 0, hdri)
          }
        }

      }
      return
    } else {
      if len(c.String("cgf"))!=0 {
        inp_slice = append(inp_slice, c.String("cgf"))
      }

      for i:=0; i<len(inp_slice); i++ {
        cgf_bytes,e := ioutil.ReadFile(inp_slice[i])
        if e!=nil { log.Fatal(e) }

        path := path_range[0][0]

        _sglf := cglf.SGLF{}

        //populate_sglf_from_cglf(c.String("cglf"), &_sglf, uint64(path))
        cgf.PopulateSGLFFromCGLF(c.String("cglf"), &_sglf, uint64(path))

        os.Exit(0)

        //hdri,_ := headerintermediate_from_bytes(cgf_bytes) ; _ = hdri
        //pathi,_ := pathintermediate_from_bytes(hdri.PathBytes[path]) ; _ = pathi

        hdri,_ := cgf.HeaderIntermediateFromBytes(cgf_bytes) ; _ = hdri
        pathi,_ := cgf.PathIntermediateFromBytes(hdri.PathBytes[path]) ; _ = pathi

        //hdri,dn := headerintermediate_from_bytes(cgf_bytes)
        hdri,dn := cgf.HeaderIntermediateFromBytes(cgf_bytes)
        if dn<0 { log.Fatal("could not construct header from bytes") }

        //patho,dn := pathintermediate_from_bytes(hdri.PathBytes[path])
        patho,dn := cgf.PathIntermediateFromBytes(hdri.PathBytes[path])
        if dn<0 { log.Fatal("could not construct path") }

        tilemap_bytes,_ := cgf.CGFTilemapBytes(cgf_bytes)
        //tilemap := unpack_tilemap(tilemap_bytes)
        tilemap := cgf.UnpackTileMap(tilemap_bytes)

        for step_idx:=0; step_idx<len(step_range); step_idx++ {
          if step_range[step_idx][1] == -1 {
            //step_range[step_idx][1] = int64(hdri.step_per_path[path])
            step_range[step_idx][1] = int64(hdri.StepPerPath[path])
          }
        }

        for stepr_idx:=0; stepr_idx<len(step_range); stepr_idx++ {
          for step:=step_range[stepr_idx][0]; step<step_range[stepr_idx][1]; step++ {
            //knot := GetKnot(tilemap, patho, int(step))
            //print_knot_fastj_sglf(knot, _sglf, uint64(path), 0, hdri)
            knot := cgf.GetKnot(tilemap, patho, int(step))
            cgf.PrintKnotFastjSGLF(knot, _sglf, uint64(path), 0, hdri)
          }
        }

      }

    }

    return
  } else if action == "knot-z" {

    cgf_bytes,e := ioutil.ReadFile(c.String("cgf")) ; _ = cgf_bytes
    if e!=nil { log.Fatal(e) }

    path,ver,step,e := cgf.ParseTilepos(c.String("tilepos"))
    _ = path ; _ = ver ; _ = step
    if e!=nil { log.Fatal(e) }

    if path<0 { log.Fatal("path must be positive") }
    if step<0 { log.Fatal("step must be positive") }

    fmt.Printf("not implemented\n")

    return
  } else if action == "knot-2" {

    cgf_bytes,e := ioutil.ReadFile(c.String("cgf"))
    if e!=nil { log.Fatal(e) }

    //hdri,dn := headerintermediate_from_bytes(cgf_bytes[:])
    hdri,dn := cgf.HeaderIntermediateFromBytes(cgf_bytes[:])
    _ = hdri
    _ = dn

    //path,ver,step,e := parse_tilepos(c.String("tilepos"))
    path,ver,step,e := cgf.ParseTilepos(c.String("tilepos"))
    if e!=nil { log.Fatal(e) }

    if path<0 { log.Fatal("path must be positive") }
    if step<0 { log.Fatal("step must be positive") }
    //if path >= len(hdri.step_per_path) { log.Fatal("path out of range (max ", len(hdri.step_per_path), " paths)") }
    //if step>= hdri.step_per_path[path] { log.Fatal("step out of range (max ", hdri.step_per_path[path], " steps)") }

    if path >= len(hdri.StepPerPath) { log.Fatal("path out of range (max ", len(hdri.StepPerPath), " paths)") }
    if step>= hdri.StepPerPath[path] { log.Fatal("step out of range (max ", hdri.StepPerPath[path], " steps)") }

    //pathi,_ := pathintermediate_from_bytes(hdri.PathBytes[path])
    pathi,_ := cgf.PathIntermediateFromBytes(hdri.PathBytes[path])

    //knot := GetKnot(hdri.tilemap, pathi, step)
    knot := cgf.GetKnot(hdri.TileMap, pathi, step)
    if knot==nil {
      fmt.Printf("spanning tile?")
    } else {

      for i:=0; i<len(knot); i++ {
        for j:=0; j<len(knot[i]); j++ {
          if j>0 { fmt.Printf(" ") }
          fmt.Printf("%04x.%02x.%04x.%03x+%x",
            path, ver,
            knot[i][j].Step,
            knot[i][j].VarId,
            knot[i][j].Span)

          if gShowKnotNocallInfoFlag {
            if len(knot[i][j].NocallStartLen)>0 {
              fmt.Printf("*{")
              for p:=0; p<len(knot[i][j].NocallStartLen); p+=2 {
                if p>0 { fmt.Printf(";") }
                fmt.Printf("%d+%d",
                  knot[i][j].NocallStartLen[p],
                  knot[i][j].NocallStartLen[p+1])
              }
              fmt.Printf("}")
            }
          }
        }
        fmt.Printf("\n")
      }

    }


    return

  } else if action == "sglfbarf" {

    //_sglf,e := LoadGenomeLibraryCSV(c.String("sglf"))
    _sglf,e := cglf.LoadGenomeLibraryCSV(c.String("sglf"))
    if e!=nil { log.Fatal(e) }

    for path := range _sglf.LibInfo {
      for step := range _sglf.LibInfo[path] {
        for i:=0; i<len(_sglf.LibInfo[path][step]); i++ {
          fmt.Printf("%x,%x,%x.%x.%x+%x\n", path, step,
            _sglf.LibInfo[path][step][i].Path,
            _sglf.LibInfo[path][step][i].Step,
            _sglf.LibInfo[path][step][i].Variant,
            _sglf.LibInfo[path][step][i].Span)
        }
      }
    }

    return
  } else if action == "append" {

    _sglf,e := cglf.LoadGenomeLibraryCSV(c.String("sglf"))
    if e!=nil { log.Fatal(e) }

    ain_slice := make([]autoio.AutoioHandle, 0, 8)
    for i:=0; i<len(inp_slice); i++ {
      inp_fn := inp_slice[i]
      ain,err := autoio.OpenReadScanner(inp_fn) ; _ = ain
      if err!=nil {
        fmt.Fprintf(os.Stderr, "%v", err)
        os.Exit(1)
      }
      defer ain.Close()
      ain_slice = append(ain_slice, ain)
      break
    }

    path_str := c.String("path")
    path_u64,e := strconv.ParseInt(path_str, 16, 64)
    if e!=nil { log.Fatal(e) }
    path:=int(path_u64)

    cgf_bytes,e := ioutil.ReadFile(c.String("cgf"))
    if e!=nil { log.Fatal(e) }

    //hdri,_ := headerintermediate_from_bytes(cgf_bytes[:])
    hdri,_ := cgf.HeaderIntermediateFromBytes(cgf_bytes[:])

    ctx := cgf.CGFContext{}
    _cgf := cgf.CGF{}
    _cgf.PathBytes = make([][]byte, 0, 1024)
    cgf.CGFFillHeader(&_cgf, cgf_bytes)

    ctx.CGF = &_cgf
    ctx.SGLF = &_sglf
    //CGFContext_construct_tilemap_lookup(&ctx)
    ctx.ConstructTileMapLookup()

    //allele_path,e := load_sample_fastj(&ain_slice[0])
    allele_path,e := cgf.LoadSampleFastj(&ain_slice[0])
    if e!=nil { log.Fatal(e) }

    //PathBytes,e := emit_path_bytes(&ctx, path, allele_path)
    PathBytes,e := ctx.EmitPathBytes(path, allele_path)
    if e!=nil { log.Fatal(e) }

    //headerintermediate_add_path(&hdri, path, PathBytes)
    //write_cgf_from_intermediate(c.String("output"), &hdri)

    cgf.HeaderIntermediateAddPath(&hdri, path, PathBytes)
    cgf.WriteCGFFromIntermediate(c.String("output"), &hdri)

    return
  }

  ain_slice := make([]autoio.AutoioHandle, 0, 8)
  for i:=0; i<len(inp_slice); i++ {
    inp_fn := inp_slice[i]
    ain,err := autoio.OpenReadScanner(inp_fn) ; _ = ain
    if err!=nil {
      fmt.Fprintf(os.Stderr, "%v", err)
      os.Exit(1)
    }
    defer ain.Close()
    ain_slice = append(ain_slice, ain)
  }


  aout,err := autoio.CreateWriter( c.String("output") ) ; _ = aout
  if err!=nil {
    fmt.Fprintf(os.Stderr, "%v", err)
    os.Exit(1)
  }
  defer func() { aout.Flush() ; aout.Close() }()

  if c.Bool( "pprof" ) {
    gProfileFlag = true
    gProfileFile = c.String("pprof-file")
  }

  if c.Bool( "mprof" ) {
    gMemProfileFlag = true
    gMemProfileFile = c.String("mprof-file")
  }

  gVerboseFlag = c.Bool("Verbose")

  if c.Int("max-procs") > 0 {
    runtime.GOMAXPROCS( c.Int("max-procs") )
  }

  if gProfileFlag {
    prof_f,err := os.Create( gProfileFile )
    if err != nil {
      fmt.Fprintf( os.Stderr, "Could not open profile file %s: %v\n", gProfileFile, err )
      os.Exit(2)
    }

    pprof.StartCPUProfile( prof_f )
    defer pprof.StopCPUProfile()
  }


  _sglf,e := cglf.LoadGenomeLibraryCSV(c.String("sglf"))
  if e!=nil { log.Fatal(e) }

  ctx := cgf.CGFContext{}
  _cgf := cgf.CGF{}

  //header_bytes := cgf_default_header_bytes()
  header_bytes := cgf.CGFDefaultHeaderBytes()
  cgf.CGFFillHeader(&_cgf, header_bytes)

  ctx.CGF = &_cgf
  ctx.SGLF = &_sglf
  //CGFContext_construct_tilemap_lookup(&ctx)
  ctx.ConstructTileMapLookup()


  for i:=0; i<len(ain_slice); i++ {
    ain := ain_slice[i]

    //allele_path,e := load_sample_fastj(&ain)
    allele_path,e := cgf.LoadSampleFastj(&ain)
    if e!=nil { log.Fatal(e) }

    p := 0x2c5
    if i>0 { p = 0x247 }

    //e = update_vector_path_simple(&ctx, p, allele_path)
    e = ctx.UpdateVectorPathSimple(p, allele_path)
    if len(ctx.CGF.StepPerPath) < len(ain_slice) {
      ctx.CGF.StepPerPath = append(ctx.CGF.StepPerPath, uint64(len(_sglf.Lib[p])))
    }

  }

  ctx.CGF.PathCount = uint64(len(_cgf.Path))
  ctx.CGF.StepPerPath = make([]uint64, ctx.CGF.PathCount)
  for i:=uint64(0); i<ctx.CGF.PathCount; i++ {
    ctx.CGF.StepPerPath[i] = uint64(len(_sglf.Lib[int(i)]))
  }

  //write_cgf(&ctx, "out.cgf")
  ctx.WriteCGF("out.cgf")

}

func main() {

  app := cli.NewApp()
  app.Name  = "cgf"
  app.Usage = "CGF"
  app.Version = VERSION_STR
  app.Author = "Curoverse, Inc."
  app.Email = "info@curoverse.com"
  app.Action = func( c *cli.Context ) { _main(c) }

  app.Flags = []cli.Flag{

    cli.StringFlag{
      Name: "cgf, c",
      Usage: "CGF",
    },

    cli.StringSliceFlag{
      Name: "input,i",
      Usage: "INPUT",
    },

    cli.StringFlag{
      Name: "cglf, L",
      Usage: "CGLF",
    },

    cli.StringFlag{
      Name: "sglf, S",
      Usage: "SGLF",
    },

    cli.StringFlag{
      Name: "tilepos, p",
      Usage: "Tile Position",
    },

    cli.StringFlag{
      Name: "path, P",
      Usage: "Path (in hex)",
    },

    cli.StringFlag{
      Name: "ocgf, output, o",
      Value: "-",
      Usage: "OUTPUT",
    },

    cli.StringFlag{
      Name: "append, a",
      Value: "-",
      Usage: "OUTPUT",
    },

    cli.BoolFlag{
      Name: "hide-knot-low-quality",
      Usage: "Don't show low quality information for knot",
    },

    cli.StringFlag{
      Name: "action, A",
      Value: "",
      Usage: "(help|fastj2cgf|inspect)",
    },

    cli.IntFlag{
      Name: "max-procs, N",
      Value: -1,
      Usage: "MAXPROCS",
    },

    cli.BoolFlag{
      Name: "Verbose, V",
      Usage: "Verbose flag",
    },

    cli.BoolFlag{
      Name: "pprof",
      Usage: "Profile usage",
    },

    cli.StringFlag{
      Name: "pprof-file",
      Value: gProfileFile,
      Usage: "Profile File",
    },

    cli.BoolFlag{
      Name: "mprof",
      Usage: "Profile memory usage",
    },

    cli.StringFlag{
      Name: "mprof-file",
      Value: gMemProfileFile,
      Usage: "Profile Memory File",
    },

  }

  app.Run( os.Args )

  if gMemProfileFlag {
    fmem,err := os.Create( gMemProfileFile )
    if err!=nil { panic(fmem) }
    pprof.WriteHeapProfile(fmem)
    fmem.Close()
  }

}

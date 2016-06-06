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

var VERSION_STR string = "0.2.0"
var gVerboseFlag bool

var gProfileFlag bool
var gProfileFile string = "cgf.pprof"

var gMemProfileFlag bool
var gMemProfileFile string = "cgf.mprof"

var gShowKnotNocallInfoFlag bool = true

var use_SGLF bool = true

func _main( c *cli.Context ) {
  gShowKnotNocallInfoFlag = !c.Bool("hide-knot-low-quality")

  inp_slice := c.StringSlice("input")

  cglf_lib_location := c.String("cglf")


  action := c.String("action")
  if action == "" {
    cli.ShowAppHelp(c)
    os.Exit(1)
  } else if action == "debug" {
    cgf.DebugRead(c.String("cgf"))
    return
  } else if action == "headercheck" {


    header_bytes := cgf.CGFDefaultHeaderBytes()

    hdri,dn := cgf.HeaderIntermediateFromBytes(header_bytes) ; _ = dn
    hdri_bytes := cgf.BytesFromHeaderIntermediate(hdri)
    hdri1,dn2 := cgf.HeaderIntermediateFromBytes(hdri_bytes) ; _ = dn2

    err := cgf.HeaderIntermediateCmp(hdri, hdri1)

    if err!=nil { log.Fatal(err) }
    return
  } else if action == "header" {

    ocgf := c.String("output")

    header_bytes := cgf.CGFDefaultHeaderBytes()

    f,err := os.Create(ocgf)
    if err!=nil { log.Fatal(err) }

    f.Write(header_bytes)
    f.Sync()
    f.Close()

    return

  } else if action == "tilemapentry" {
    cglf_path := c.String("cglf")
    if len(cglf_path)==0 {
      fmt.Fprintf( os.Stderr, "Provide CGLF\n" )
      cli.ShowAppHelp(c)
      os.Exit(1)
    }

    cgf_bytes,e := ioutil.ReadFile(c.String("cgf"))
    if e!=nil { log.Fatal(e) }

    hdri,dn := cgf.HeaderIntermediateFromBytes(cgf_bytes[:])
    _ = hdri
    _ = dn

    path,ver,step,e := cgf.ParseTilepos(c.String("tilepos"))
    if e!=nil { log.Fatal(e) }

    _ = path
    _ = ver
    _ = step

    if path<0 { log.Fatal("path must be positive") }
    if step<0 { log.Fatal("step must be positive") }

    if path >= len(hdri.StepPerPath) { log.Fatal("path out of range (max ", len(hdri.StepPerPath), " paths)") }
    if step>= hdri.StepPerPath[path] { log.Fatal("step out of range (max ", hdri.StepPerPath[path], " steps)") }

    pathi,_ := cgf.PathIntermediateFromBytes(hdri.PathBytes[path])

    tme,sf,lqf,xf := cgf.GetSimpleTileMapEntry(hdri.TileMap, pathi, step)

    fmt.Printf("tilemap %v, span %v, loq %v, complex %v\n", tme, sf, lqf, xf)
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

    hdri,dn := cgf.HeaderIntermediateFromBytes(cgf_bytes[:])
    _ = hdri
    _ = dn

    path,ver,step,e := cgf.ParseTilepos(c.String("tilepos"))
    if e!=nil { log.Fatal(e) }

    if path<0 { log.Fatal("path must be positive") }
    if step<0 { log.Fatal("step must be positive") }

    if path >= len(hdri.StepPerPath) { log.Fatal("path out of range (max ", len(hdri.StepPerPath), " paths)") }
    if step>= hdri.StepPerPath[path] { log.Fatal("step out of range (max ", hdri.StepPerPath[path], " steps)") }

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
      if e!=nil { log.Fatal(fmt.Sprintf("LoadGenomeLibraryCSV error (sglf): %v", e)) }

      for i:=0; i<len(inp_slice); i++ {
        e = cgf.PrintTileSGLF(inp_slice[i], tilepos_str, _sglf)
        if e!=nil { log.Fatal(fmt.Sprintf("PrintTileSGLF error: %v", e)) }
      }
    } else {
      if len(c.String("cgf"))!=0 {
        inp_slice = append(inp_slice, c.String("cgf"))
      }

      for i:=0; i<len(inp_slice); i++ {
        e := cgf.PrintTileCGLF(inp_slice[i], tilepos_str, cglf_lib_location)
        if e!=nil { log.Fatal(fmt.Sprintf("PrintTileCGLF error: %v", e)) }
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

        hdri,_ := cgf.HeaderIntermediateFromBytes(cgf_bytes) ; _ = hdri
        pathi,_ := cgf.PathIntermediateFromBytes(hdri.PathBytes[path]) ; _ = pathi

        hdri,dn := cgf.HeaderIntermediateFromBytes(cgf_bytes)
        if dn<0 { log.Fatal("could not construct header from bytes") }

        patho,dn := cgf.PathIntermediateFromBytes(hdri.PathBytes[path])
        if dn<0 { log.Fatal("could not construct path") }

        tilemap_bytes,_ := cgf.CGFTilemapBytes(cgf_bytes)
        tilemap := cgf.UnpackTileMap(tilemap_bytes)

        for step_idx:=0; step_idx<len(step_range); step_idx++ {
          if step_range[step_idx][1] == -1 {
            step_range[step_idx][1] = int64(hdri.StepPerPath[path])
          }
        }

        for stepr_idx:=0; stepr_idx<len(step_range); stepr_idx++ {
          for step:=step_range[stepr_idx][0]; step<step_range[stepr_idx][1]; step++ {
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

        cgf.PopulateSGLFFromCGLF(c.String("cglf"), &_sglf, uint64(path))

        os.Exit(0)

        hdri,_ := cgf.HeaderIntermediateFromBytes(cgf_bytes) ; _ = hdri
        pathi,_ := cgf.PathIntermediateFromBytes(hdri.PathBytes[path]) ; _ = pathi

        hdri,dn := cgf.HeaderIntermediateFromBytes(cgf_bytes)
        if dn<0 { log.Fatal("could not construct header from bytes") }

        patho,dn := cgf.PathIntermediateFromBytes(hdri.PathBytes[path])
        if dn<0 { log.Fatal("could not construct path") }

        tilemap_bytes,_ := cgf.CGFTilemapBytes(cgf_bytes)
        tilemap := cgf.UnpackTileMap(tilemap_bytes)

        for step_idx:=0; step_idx<len(step_range); step_idx++ {
          if step_range[step_idx][1] == -1 {
            step_range[step_idx][1] = int64(hdri.StepPerPath[path])
          }
        }

        for stepr_idx:=0; stepr_idx<len(step_range); stepr_idx++ {
          for step:=step_range[stepr_idx][0]; step<step_range[stepr_idx][1]; step++ {
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

    hdri,dn := cgf.HeaderIntermediateFromBytes(cgf_bytes[:])
    _ = hdri
    _ = dn

    path,ver,step,e := cgf.ParseTilepos(c.String("tilepos"))
    if e!=nil { log.Fatal(e) }

    if path<0 { log.Fatal("path must be positive") }
    if step<0 { log.Fatal("step must be positive") }

    if path >= len(hdri.StepPerPath) { log.Fatal("path out of range (max ", len(hdri.StepPerPath), " paths)") }
    if step>= hdri.StepPerPath[path] { log.Fatal("step out of range (max ", hdri.StepPerPath[path], " steps)") }

    pathi,_ := cgf.PathIntermediateFromBytes(hdri.PathBytes[path])

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

    //DEBUG
    //for k := range _sglf.PfxTagLookup {
    //  fmt.Printf("sglf>> %v %v\n", k, _sglf.PfxTagLookup[k])
    //}

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

    hdri,_ := cgf.HeaderIntermediateFromBytes(cgf_bytes[:])

    ctx := cgf.CGFContext{}
    _cgf := cgf.CGF{}
    _cgf.PathBytes = make([][]byte, 0, 1024)
    cgf.CGFFillHeader(&_cgf, cgf_bytes)

    ctx.CGF = &_cgf
    ctx.SGLF = &_sglf
    ctx.ConstructTileMapLookup()

    allele_path,e := cgf.LoadSampleFastj(&ain_slice[0])
    if e!=nil { log.Fatal(e) }

    PathBytes,e := ctx.EmitPathBytes(path, allele_path)
    if e!=nil { log.Fatal(e) }

    cgf.HeaderIntermediateAddPath(&hdri, path, PathBytes)
    cgf.WriteCGFFromIntermediate(c.String("output"), &hdri)

    return
  } else if action == "peel" {

    path,ver,step,e := cgf.ParseTilepos(c.String("tilepos"))
    if e!=nil { log.Fatal(e) }
    _ = path ; _ = ver ; _ = step


    cgf_bytes,e := ioutil.ReadFile(c.String("cgf"))
    if e!=nil { log.Fatal(e) }

    fmt.Printf("path %x, ver %x, step %x\n", path, ver, step)

    cgf.Peel(cgf_bytes, path, step)

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

  header_bytes := cgf.CGFDefaultHeaderBytes()
  cgf.CGFFillHeader(&_cgf, header_bytes)

  ctx.CGF = &_cgf
  ctx.SGLF = &_sglf
  ctx.ConstructTileMapLookup()


  for i:=0; i<len(ain_slice); i++ {
    ain := ain_slice[i]

    allele_path,e := cgf.LoadSampleFastj(&ain)
    if e!=nil { log.Fatal(e) }

    p := 0x2c5
    if i>0 { p = 0x247 }

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

  ctx.WriteCGF("out.cgf")

}

func main() {

  app := cli.NewApp()
  app.Name  = "cgf"
  app.Usage = "CGF"
  app.Version = VERSION_STR + " (cgf " + VERSION_STR + ")"
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
      Usage: "(help|debug|headercheck|header|tilemapentry|knot|knot-2|knot-z|fastj|fastj-range|fastj2cgf|sglfbarf|append|peel)",
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

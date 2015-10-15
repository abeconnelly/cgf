package main

import "fmt"
import "os"
import "runtime"
import "runtime/pprof"

import "log"
import "strings"

import "github.com/abeconnelly/autoio"
import "github.com/codegangsta/cli"

var VERSION_STR string = "0.1.0"
var gVerboseFlag bool

var gProfileFlag bool
var gProfileFile string = "cgf.pprof"

var gMemProfileFlag bool
var gMemProfileFile string = "cgf.mprof"

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


    //nocall_str0 := nocall_string(ti0.Seq)
    //nocall_str1 := nocall_string(ti1.Seq)



  }

}

func _main( c *cli.Context ) {


  /*
  if c.String("input") == "" {
    fmt.Fprintf( os.Stderr, "Input required, exiting\n" )
    cli.ShowAppHelp( c )
    os.Exit(1)
  }

  ain,err := autoio.OpenReadScanner( c.String("input") ) ; _ = ain
  if err!=nil {
    fmt.Fprintf(os.Stderr, "%v", err)
    os.Exit(1)
  }
  defer ain.Close()
  */

  inp_slice := c.StringSlice("input")
  if len(inp_slice)==0 {
    fmt.Fprintf( os.Stderr, "Input required, exiting\n" )
    cli.ShowAppHelp( c )
    os.Exit(1)
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


  sglf,e := LoadGenomeLibraryCSV(c.String("sglf"))
  if e!=nil { log.Fatal(e) }

  // Assumes a single path
  //
  //allele_path,e := load_sample_fastj(&ain)
  //if e!=nil { log.Fatal(e) }

  //print_lookup(sglf, allele_path)

  //print_zipper(sglf, allele_path);

  ctx := CGFContext{}
  cgf := CGF{}

  header_bytes := cgf_default_header_bytes()
  fill_header_struct_from_bytes(&cgf, header_bytes)

  ctx.CGF = &cgf
  ctx.SGLF = &sglf
  CGFContext_construct_tilemap_lookup(&ctx)

  for i:=0; i<len(ain_slice); i++ {
    ain := ain_slice[i]

    allele_path,e := load_sample_fastj(&ain)
    if e!=nil { log.Fatal(e) }

    p := 0x247
    if i>0 { p = 0x2c5 }
    e = update_vector_path_simple(&ctx, p, allele_path)
    fmt.Printf(">>>>> [%d] (%x) %v\n", i, p, e)

  }


  write_cgf(&ctx, "out.cgf")


  //print_zipper(sglf, allele_path);
  //e = update_vector_path_simple(&ctx, 0x2c5, allele_path)

  /*
  for allele_idx:=0; allele_idx<len(allele_path); allele_idx++ {
    fmt.Printf("#### allele_idx %d\n", allele_idx)
    for idx:=0; idx<len(allele_path[allele_idx]); idx++ {
      fmt.Printf("[%d] .%04x+%x {%s}%s{%s}\n", allele_idx,
        allele_path[allele_idx][idx].Step,
        allele_path[allele_idx][idx].Span,
        allele_path[allele_idx][idx].PfxTag,
        allele_path[allele_idx][idx].Seq,
        allele_path[allele_idx][idx].SfxTag )

    }
    fmt.Printf("\n")
    fmt.Printf("\n")
  }
  */

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

    /*(
    cli.StringFlag{
      Name: "input, i",
      Usage: "INPUT",
    },
    */

    cli.StringSliceFlag{
      Name: "input, i",
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
      Name: "output, o",
      Value: "-",
      Usage: "OUTPUT",
    },

    cli.StringFlag{
      Name: "append, a",
      Value: "-",
      Usage: "OUTPUT",
    },

    cli.StringFlag{
      Name: "action, A",
      Value: "",
      Usage: "(help|fastj2cgf)",
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

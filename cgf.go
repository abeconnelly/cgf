package cgf

import "fmt"
import "log"
import "strings"

import "github.com/abeconnelly/cglf"


var VERSION_STR string = "0.2.0"
var gVerboseFlag bool

var gProfileFlag bool
var gProfileFile string = "cgf.pprof"

var gMemProfileFlag bool
var gMemProfileFile string = "cgf.mprof"

var gShowKnotNocallInfoFlag bool = true

var use_SGLF bool = false

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

func print_lookup(sglf cglf.SGLF, allele_path [][]TileInfo) {
  var ok bool

  for allele_idx:=0; allele_idx<len(allele_path); allele_idx++ {
    for path_idx:=0; path_idx<len(allele_path[allele_idx]); path_idx++ {

      ti := allele_path[allele_idx][path_idx]
      sglf_info := cglf.SGLFInfo{}
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
    }
  }

  //DEBUG
  //fmt.Printf("??? >>>>> %d\n", len(var_lib));
  //for z:=0; z< len(var_lib); z++ {
  //  fmt.Printf(">>>>> %s\n", var_lib[z])
  //}


  return -1,fmt.Errorf("could not find tile element in library for sequence '%s'", seq)
}

var test_cgf CGF

func print_zipper(sglf cglf.SGLF, allele_path [][]TileInfo) {

  var ok bool

  var path0 int
  var path1 int

  var step_idx0 int
  var step_idx1 int

  var step0 int
  var step1 int

  sglf_info0 := cglf.SGLFInfo{}
  sglf_info1 := cglf.SGLFInfo{}

  tile_zipper := make([][]cglf.SGLFInfo, 2)
  tile_zipper_seq := make([][]string, 2)

  span_sum := 0

  for (step_idx0<len(allele_path[0])) && (step_idx1<len(allele_path[1])) {

    if span_sum >= 0 {
      ti0 := allele_path[0][step_idx0]

      // sglf_info1 only holds a valid path and step
      //
      if step_idx0>0 {
        sglf_info0,ok = sglf.PfxTagLookup[ti0.PfxTag]
      } else {
        sglf_info0,ok = sglf.SfxTagLookup[ti0.SfxTag]
      }

      if !ok {
        log.Fatal("could not find prefix (%s) in sglf (allele_idx %d, step_idx %d (%x))\n",
          ti0.PfxTag, 0, step_idx0, step_idx0)
      }

      path0 = sglf_info0.Path
      step0 = sglf_info0.Step

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

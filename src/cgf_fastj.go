package main

import "fmt"
import "strings"
import "strconv"
import "crypto/md5"
import "github.com/abeconnelly/autoio"

type TileInfo struct {
  PfxTag string
  SfxTag string
  Seq string
  Span int
  Step int
  VarId int
  NocallStartLen []int
}

//func emit_fastj_tile(path,step,span int, pretag string, seq []byte, suftag  string) {
//  fmt.Printf("# %x.%x+%x %s-%s-%s\n", path, step, span, pretag, seq, suftag)
//}

func emit_fastj_tile(path,step,span int, pretag string, seq []byte, suftag  string) TileInfo {

  ti := TileInfo{
    PfxTag: pretag,
    Seq: string(seq),
    SfxTag: suftag,
    Span: span,
    Step: step,
  }

  ti.NocallStartLen = make([]int, 0, 4)

  nocall_start := 0
  nocall_len := 0
  for i:=0; i<len(seq); i++ {
    if seq[i] == 'n' || seq[i] == 'N' {
      if nocall_len == 0 {
        nocall_start = i
      }
      nocall_len++
    } else {
      if nocall_len>0 {
        ti.NocallStartLen = append(ti.NocallStartLen, nocall_start)
        ti.NocallStartLen = append(ti.NocallStartLen, nocall_len)
        nocall_len = 0
      }
    }

  }

  if nocall_len>0 {
    ti.NocallStartLen = append(ti.NocallStartLen, nocall_start)
    ti.NocallStartLen = append(ti.NocallStartLen, nocall_len)
  }

  //DEBUG
  /*
  fmt.Printf("# %04x.%04x+%x %s %s\n", path,step,span, pretag, suftag)
  fmt.Printf("#")
  for i:=0; i<len(ti.NocallStartLen); i+=2 {
    fmt.Printf(" %d+%d",
      ti.NocallStartLen[i],
      ti.NocallStartLen[i+1] )
  }
  fmt.Printf("\n")
  */


  return ti
}


func load_sample_fastj(scan *autoio.AutoioHandle) ([][]TileInfo, error) {
  line_no:=0

  cur_seq := make([]byte, 0, 1024)
  tilepath := -1
  tilestep := -1
  tilevar := -1
  nocall := 0

  _ = nocall

  allele_path := make([][]TileInfo, 2)
  for i:=0; i<len(allele_path); i++ {
    allele_path[i] = make([]TileInfo, 0, 1024)
  }


  var first_tile bool = true
  var tileid string
  var s_tag string
  var e_tag string
  var md5sum_str string
  var span_len int
  var start_tile_flag bool
  var end_tile_flag bool

  for scan.ReadScan() {
    l := scan.ReadText()
    line_no++
    if len(l)==0 { continue }
    if l[0]=='\n' { continue }

    if l[0]=='>' {

      // store tile sequence
      //
      if !first_tile {
        m5 := md5sum2str( md5.Sum(cur_seq) )
        if m5!=md5sum_str { return nil,fmt.Errorf("md5sums do not match %s != %s (line %d)", m5, md5sum_str, line_no) }
        ti := emit_fastj_tile(tilepath, tilestep, span_len, s_tag, cur_seq, e_tag)

        //DEBUG
        /*
        fmt.Printf(" %x.%x noc:", tilepath, tilestep)
        for i:=0; i<len(ti.NocallStartLen); i+=2 {
          fmt.Printf(" %d+%d", ti.NocallStartLen[i], ti.NocallStartLen[i+1])
        }
        fmt.Printf("\n")
        */

        if tilevar==0 {
          allele_path[0] = append(allele_path[0], ti)
        } else if tilevar==1 {
          allele_path[1] = append(allele_path[1], ti)
        } else {
          return nil,fmt.Errorf("invalid tile variant allele %d", tilevar)
        }

      }
      first_tile = false

      var pos int =0

      tileid,pos = simple_text_field(l[1:], "tileID")
      if pos<0 { return nil,fmt.Errorf("no tileID found at line %d", line_no) }

      md5sum_str,pos = simple_text_field(l[1:], "md5sum")
      if pos<0 { return nil,fmt.Errorf("no md5sum found at line %d", line_no) }

      span_len,pos = simple_int_field(l[1:], "seedTileLength")
      if pos<0 { return nil,fmt.Errorf("no md5sum found at line %d", line_no) }

      s_tag,pos = simple_text_field(l[1:], "startTag")
      if pos<0 { return nil,fmt.Errorf("no startTag found at line %d", line_no) }
      _ = s_tag

      e_tag,pos = simple_text_field(l[1:], "endTag")
      if pos<0 { return nil,fmt.Errorf("no endTag found at line %d", line_no) }
      _ = e_tag

      start_tile_flag,pos = simple_bool_field(l[1:], "startTile")
      if pos<0 { return nil,fmt.Errorf("no startTile found at line %d", line_no) }
      _ = start_tile_flag

      end_tile_flag,pos = simple_bool_field(l[1:], "endTile")
      if pos<0 { return nil,fmt.Errorf("no endTile found at line %d", line_no) }
      _ = end_tile_flag



      tile_parts := strings.Split(tileid, ".")
      if t,e := strconv.ParseInt(tile_parts[0], 16, 64) ; e==nil {
        tilepath = int(t)
      } else { return nil,e }

      if t,e := strconv.ParseInt(tile_parts[2], 16, 64) ; e==nil {
        tilestep = int(t)
      } else { return nil,e }

      if t,e := strconv.ParseInt(tile_parts[3], 16, 64) ; e==nil {
        tilevar = int(t)
      } else { return nil,e }

      // Header parsed, go on
      //
      cur_seq = cur_seq[0:0]
      continue
    }

    if first_tile { return nil,fmt.Errorf("found body before header (line %d)", line_no) }

    cur_seq = append(cur_seq, l[:]...)

  }

  // store tile sequence
  //
  if !first_tile {
    m5 := md5sum2str( md5.Sum(cur_seq) )
    if m5!=md5sum_str { return nil,fmt.Errorf("md5sums do not match %s != %s (line %d)", m5, md5sum_str, line_no) }
    ti := emit_fastj_tile(tilepath, tilestep, span_len, s_tag, cur_seq, e_tag)

    if tilevar==0 {
      allele_path[0] = append(allele_path[0], ti)
    } else if tilevar==1 {
      allele_path[1] = append(allele_path[1], ti)
    } else {
      return nil,fmt.Errorf("invalid tile variant allele %d", tilevar)
    }

  }

  return allele_path,nil
}



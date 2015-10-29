package main

import "fmt"
import "strings"
import "strconv"
import "./dlug"



func fill_noc_seq(seq string, delpos_len []int) string {
  s := []byte(seq)

  cur_pos:=0
  for i:=0; i<len(delpos_len); i+=2 {
    loqpos := cur_pos + delpos_len[i]
    loqlen := delpos_len[i+1]

    for j:=0; j<loqlen; j++ {

      if ((loqpos+j)>=len(s)) {
        panic( fmt.Sprintf("!!!! %v, %s", delpos_len, seq))
      }

      s[loqpos+j] = 'n'
    }

    cur_pos += delpos_len[i]

  }

  return string(s)

}

func parse_tilepos(s string) (path, ver, step int, err error) {
  parts := strings.Split(s, ".")
  if len(parts)<2 || len(parts)>3 {
    err = fmt.Errorf("invalid string (must be 2 or 3 fields '.' separated)")
    return
  }

  var u64 int64

  p_pos:=0
  u64,err = strconv.ParseInt(parts[p_pos], 16, 64)
  if err!=nil { return }
  p_pos++

  path = int(u64)

  if len(parts)==3 {
    u64,err = strconv.ParseInt(parts[p_pos], 16, 64)
    if err!=nil { return }
    ver = int(u64)
    p_pos++
  }

  u64,err = strconv.ParseInt(parts[p_pos], 16, 64)
  if err!=nil { return }
  step = int(u64)
  p_pos++

  return
}

func fill_slice_string(buf []byte, s string) ([]byte, int) {
  var dn int
  n:=0
  tbuf := make([]byte, 8)

  dn = dlug.FillSliceUint32(tbuf, uint32(len(s)))
  buf = append(buf, tbuf[0:dn]...)
  n += dn

  buf = append(buf, []byte(s)...)
  n+=len(s)

  return buf, n

}

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


func simple_text_field(line, field string) (string, int) {

  sep := "\"" + field + "\""
  p:=strings.Index(line, sep)

  n:=len(line)
  if p<0 { return "", p; }
  p+=len(sep)
  if p>=n { return "", -1 }

  for ; p<n && line[p]==' '; p++ { }
  if p==n { return "", -2; }
  if line[p] != ':' { return "", -3; }

  p++
  if p==n { return "", -2; }

  for ; p<n && line[p]==' '; p++ { }
  if p==n { return "", -4; }
  if line[p] != '"' { return "", -5; }

  p++
  if p==n { return "", -6; }

  p_s := p

  for ; p<n && line[p]!='"'; p++ { }
  if p==n { return "", -7; }

  p_e := p

  return line[p_s:p_e], p_e

}


func simple_bool_field(line, field string) (bool, int) {

  sep := "\"" + field + "\""
  p:=strings.Index(line, sep)

  n:=len(line)
  if p<0 { return false, p; }
  p+=len(sep)
  if p>=n { return false, -1 }

  for ; p<n && line[p]==' '; p++ { }
  if p==n { return false, -2; }
  if line[p] != ':' { return false, -3; }

  p++
  if p==n { return false,-2; }

  for ; p<n && line[p]==' '; p++ { }
  if p==n { return false, -4; }

  val_start_pos := p
  for ; p<n && line[p]!=',' && line[p]!='}' && line[p]!=' ' && line[p]!='\t' && line[p]!='\n' ; p++ { }
  if p==n { return false, -5; }

  val := line[val_start_pos:p]
  if val=="true" { return true, p }
  if val=="false" { return false, p }
  return false, -6
}


func simple_int_field(line, field string) (int, int) {

  sep := "\"" + field + "\""
  p:=strings.Index(line, sep)

  n:=len(line)
  if p<0 { return 0, p; }

  p+=len(sep)
  if p>=n { return 0, -2 }

  for ; p<n && line[p]==' '; p++ { }
  if p==n { return 0, -3; }
  if line[p] != ':' { return 0, -4; }

  p++
  if p==n { return 0, -5; }

  for ; p<n && line[p]==' '; p++ { }
  if p==n { return 0, -6; }

  p_s := p

  for ; p<n && (line[p]!='"') && (line[p]!=','); p++ { }
  if p==n { return 0, -7; }

  p_e := p

  i,e := strconv.ParseInt(line[p_s:p_e], 10, 64)
  if e!=nil { return 0, -8; }

  return int(i), p_e

}

func md5sum2str(md5sum [16]byte) string {
  var str_md5sum [32]byte
  for i:=0; i<16; i++ {
    x := fmt.Sprintf("%02x", md5sum[i])
    str_md5sum[2*i]   = x[0]
    str_md5sum[2*i+1] = x[1]
  }

  return string(str_md5sum[:])
}

package main

import "fmt"
import "strings"
import "strconv"

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

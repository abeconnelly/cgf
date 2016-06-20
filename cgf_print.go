package cgf

import "fmt"
import "crypto/md5"

import "github.com/abeconnelly/cglf"

//func print_knot_fastj_sglf(knot [][]TileInfo, sglf SGLF, path, ver uint64, hdri headerintermediate) {
func PrintKnotFastjSGLF(knot [][]TileInfo, sglf cglf.SGLF, path, ver uint64, hdri HeaderIntermediate) {
  if len(knot)==0 { return }

  for i:=0; i<len(knot); i++ {
    phase_str := "A"
    if i==1 { phase_str = "B" }

    cur_step := knot[i][0].Step


    for j:=0; j<len(knot[i]); j++ {
      fmt.Printf("> {")
      fmt.Printf(" \"tileID\":\"%04x.%02x.%04x.%03x\", \"seedTileLength\":%d",
        path, ver,
        knot[i][j].Step,
        knot[i][j].VarId,
        knot[i][j].Span)

      seq := sglf.Lib[int(path)][int(cur_step)][knot[i][j].VarId]

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

      //if (knot[i][j].Step+1)==hdri.step_per_path[int(path)] {
      if (knot[i][j].Step+1)==hdri.StepPerPath[int(path)] {
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
        //noc_seq := fill_noc_seq(seq, knot[i][j].NocallStartLen)
        //noc_m5str := md5sum2str(md5.Sum([]byte(noc_seq)))

        noc_seq := FillNocSeq(seq, knot[i][j].NocallStartLen)
        noc_m5str := Md5sum2str(md5.Sum([]byte(noc_seq)))

        noc_count := 0
        for ii:=0; ii<len(knot[i][j].NocallStartLen); ii+=2 {
          noc_count += knot[i][j].NocallStartLen[ii+1]
        }
        fmt.Printf(", \"nocallCount\":%d", noc_count)

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
        m5str := Md5sum2str(md5.Sum([]byte(seq)))
        //fmt.Printf(" %s\n%s\n", m5str, seq)

        fmt.Printf(", \"nocallCount\":0")

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


      cur_step += knot[i][j].Span
    }
  }

}


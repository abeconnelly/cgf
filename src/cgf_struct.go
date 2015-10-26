package main

type CGFContext struct {
  CGF             *CGF
  SGLF            *SGLF
  TileMapArray    []TileMapEntry
  TileMapLookup   map[string]TileMapEntry
  TileMapPosition map[string]int
}


// --
// parent structure
// --

type CGF struct {
  Magic               uint64
  Version             string
  LibraryVersion      string
  PathCount           uint64
  TileMapLen          uint64
  TileMap             []byte
  StepPerPath         []uint64
  PathOffset          []uint64
  Path                []PathStruct

  HeaderBytes         []byte
  PathBytes           [][]byte

  PathByteOffset      uint64
}

// --
// parent structure
// --


// --
// path structure (one per path)
// --


type PathStruct struct {
  Name string
  Vector []uint64

  Overflow OverflowStruct
  FinalOverflow FinalOverflowStruct

  LowQualityBytes []byte

  //LowQualityHom LowQualityHomStruct
  //LowQualityHet LowQualityHetStruct
}

// --
// path structure
// --


// --
// overflow structure
// --

type OverflowStruct struct {

  //Length is redundant since slices have built in
  // length, but used here as a placeholder
  Length uint64

  Stride uint64
  Offset []uint64
  Position []uint64

  // { dlug, dlug }
  //
  Map []byte
}

// --
// overflow structure
// --

type FinalOverflowStruct struct {
  Length uint64
  Stride uint64
  Offset []uint64
  Position []uint64
  DataRecord DataRecordStruct
}


type LowQualityInfoStruct struct {
  Length      uint64
  Stride      uint64
  Offset      []uint64
  Position    []uint64
  HetHomFlag  []byte
  Info []LoqHetHomInfoStruct
}

type LowQualityHomStruct struct {
  Length uint64
  Stride uint64
  Offset []uint64
  NTile []uint64
  Info []LoqInfoStruct
}

type LoqAlleleInfoStruct struct {
  NAllele []byte
  Allele []LoqInfoStruct
}

type LowQualityHetStruct struct {
  Length uint64
  Stride uint64
  Offset []uint64
  NTile []uint64
  Info []LoqAlleleInfoStruct
}

/*
type FinalOverflowStruct struct {
  NAllele uint64
  Allele []byte
}
*/



type DataRecordStruct struct {

  // dlug
  //
  Code []byte

  // arbitrary data, should include length
  // in derived structures
  //
  Data []byte
}

type LoqHetHomInfoStruct struct {
  AllelInfo [][]LoqInfoStruct
}

type LoqInfoStruct struct {

  // dlug
  //
  Len []byte

  // []dlug
  //
  Pos []byte

  // []dlug
  //
  LoqLen []byte
}




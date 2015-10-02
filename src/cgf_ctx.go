package main

func CGFContext_construct_tilemap_lookup(ctx *CGFContext) {
  ctx.TileMapLookup = make(map[string]TileMapEntry)
  ctx.TileMapPosition = make(map[string]int)

  ctx.TileMapArray = unpack_tilemap(ctx.CGF.TileMap)

  for i:=0; i<len(ctx.TileMapArray); i++ {
    key := create_tilemap_string_lookup2(ctx.TileMapArray[i].Variant[0], ctx.TileMapArray[i].Span[0],
                                         ctx.TileMapArray[i].Variant[1], ctx.TileMapArray[i].Span[1])
    ctx.TileMapLookup[key] = ctx.TileMapArray[i]
    ctx.TileMapPosition[key] = i
  }

}

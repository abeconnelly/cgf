#!/bin/bash

go build cgf.go cgf_aux.go cgf_fastj.go \
  cgf_struct.go cgf_bin.go cgf_header.go \
  cgf_default_tilemap.go byteconv_helper.go \
  cgf_ctx.go cgf_debug.go cgf_inspect.go \
  cgf_access.go cgf_xpr.go cgf_lookups.go sglf.go \
  cgf_parse.go cgf_print.go

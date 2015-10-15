#!/bin/bash

#./cgf -i <( zcat /scratch/brca/tiles/pgp174/hu011C57.fj/2c5.fj.gz ) -S 247_2c5.csv > z
./cgf -i <( zcat /scratch/brca/tiles/pgp174/hu011C57.fj/2c5.fj.gz ) -i <( zcat /scratch/brca/tiles/pgp174/hu011C57.fj/247.fj.gz ) -S 247_2c5.csv -o hu011C57.247.248.cgf > z

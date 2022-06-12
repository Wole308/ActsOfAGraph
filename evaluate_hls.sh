#!/bin/bash
#!/bin/bash

./evaluate.sh
make cleanall
rm -rf acts_vitishls
faketime -f "-1y" vitis_hls -f script.tcl





#!/bin/bash

echo " . INIT glass "
date
echo " "

##############################################################################

time gprop  10   10 0.05   0.3   1 1 1 1 1   0.3 0.005 0.005   100   90 6   g.trn g.val g.tst   y.glass.mlp

time gprop  100   100 0.05   0.3   1 1 1 1 1   0.3 0.005 0.005   100   90 6   g.trn g.val g.tst   y.glass.mlp
time gprop  100   100 0.05   0.3   1 1 1 1 1   0.3 0.005 0.005   100   90 6   g.trn g.val g.tst   y.glass.mlp
time gprop  100   100 0.05   0.3   1 1 1 1 1   0.3 0.005 0.005   100   90 6   g.trn g.val g.tst   y.glass.mlp
time gprop  100   100 0.05   0.3   1 1 1 1 1   0.3 0.005 0.005   100   90 6   g.trn g.val g.tst   y.glass.mlp

##############################################################################

echo " "
date
echo " . END glass "

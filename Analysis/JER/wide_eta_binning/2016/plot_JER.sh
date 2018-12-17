#!/usr/bin/env zsh

if [[ -a "pdf" ]]
    then
        echo dir pdf already exists
    else
        mkdir pdf
        echo "created dir pdf"
fi

root -b -l << EOF
.L JERplot.C++
JERplot()
.q
EOF

#cp -p pdf/JERplot.pdf ../JERplot_${ORIGIN}_${LUMI_INV_FB}_invfb.pdf
#cp -p pdfC/JERplot.C ../JERplot_${ORIGIN}_${LUMI_INV_FB}_invfb.C
#cp -p eps/JERplot.eps ../JERplot_${ORIGIN}_${LUMI_INV_FB}_invfb.eps

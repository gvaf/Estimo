function processfile
{
    ( echo $1; ( h:/projects/x264/x264 -A none --me cas --casprog d:/projects/hex/estimo.output/program.bin --caspoint d:/projects/hex/estimo.output/patterns.bin --qp 26 -o /dev/null --frames $3 $1 $2 2>&1 ) | grep encoded) >> hql.$3 &
    ( echo $1; ( h:/projects/x264/x264 -A none --me cas --casprog d:/projects/hex/estimo.output/full.bin --caspoint d:/projects/hex/estimo.output/patterns.bin --qp 26 -o /dev/null --frames $3 $1 $2 2>&1 ) | grep encoded) >> fql.$3 &
    ( echo $1; ( h:/projects/x264/x264 -A none --me cas --casqeu 0 --casprog d:/projects/hex/estimo.output/program.bin --caspoint d:/projects/hex/estimo.output/patterns.bin --qp 26 -o /dev/null --frames $3 $1 $2 2>&1 ) | grep encoded) >> hil.$3 &
    ( echo $1; ( h:/projects/x264/x264 -A none --me cas --casqeu 0 --casprog d:/projects/hex/estimo.output/full.bin --caspoint d:/projects/hex/estimo.output/patterns.bin --qp 26 -o /dev/null --frames $3 $1 $2 2>&1 ) | grep encoded) >> fil.$3 &
    wait
}

for f in d:/test_sequences/1080p/{ped,sta,tra}*yuv; do
    processfile $f 1920x1080 50
done
for f in d:/test_sequences/601/*{par,shi,sto}*yuv; do
    processfile $f 720x576 50
done
for f in d:/test_sequences/1080p/{ped,sta,tra}*yuv; do
    processfile $f 1920x1080 100
done
for f in d:/test_sequences/601/*{par,shi,sto}*yuv; do
    processfile $f 720x576 100
done
for f in d:/test_sequences/1080p/{ped,sta,tra}*yuv; do
    processfile $f 1920x1080 0
done
for f in d:/test_sequences/601/*{par,shi,sto}*yuv; do
    processfile $f 720x576 0
done

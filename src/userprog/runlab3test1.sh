cd build
pintos --qemu -- -f -q
pintos --qemu -p ../../examples/lab3test1 -a lab3test1 -- -q
pintos --qemu -- run lab3test1
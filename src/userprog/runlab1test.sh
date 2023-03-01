cd build
pintos --qemu -- -f -q
pintos --qemu -p ../../examples/lab1test -a lab1test -- -q
pintos --qemu -- run lab1test
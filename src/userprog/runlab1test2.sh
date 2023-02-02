cd build
pintos --qemu -- -f -q
pintos --qemu -p ../../examples/lab1test2 -a lab1test2 -- -q
pintos --qemu -- run lab1test2
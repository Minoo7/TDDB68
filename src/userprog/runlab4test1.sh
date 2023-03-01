cd build
pintos --qemu -- -f -q
pintos --qemu -p ../../examples/lab4test1 -a lab4test1 -- -q
pintos --qemu -- run 'lab4test1 -s 55 -g lolo'
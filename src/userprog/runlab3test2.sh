cd build
pintos --qemu -- -f -q
pintos --qemu -p ../../examples/printf -a printf -- -q
pintos --qemu -p ../../examples/lab3test2 -a lab3test2 -- -q
pintos --qemu -- run lab3test2
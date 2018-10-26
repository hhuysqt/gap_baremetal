riscv32-unknown-elf-gcc -o test \
startup_gapuino.S \
gap8_interrupt.c gap8_uart.c gap8_udma.c gap8_gpio.c \
main_UART.c \
-g -fno-jump-tables -fno-tree-loop-distribute-patterns \
-fdata-sections -ffunction-sections \
-march=rv32imcxgap8 -mPE=8 -mFC=1 -D__riscv__ -D__pulp__ -D__GAP8__ \
-nostartfiles -Og \
-T GAP8.ld

riscv32-unknown-elf-gcc -o test \
startup_gapuino.S \
gap8_interrupt.c \
gap8_uart.c gap8_udma.c \
main_UART.c gap8_gpio.c \
-g -fno-jump-tables -fno-tree-loop-distribute-patterns \
-Wextra -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wundef \
-fdata-sections -ffunction-sections \
-march=rv32imcxgap8 -mPE=8 -mFC=1 -D__riscv__ -D__pulp__ -D__GAP8__ \
-MMD -MP -nostartfiles -Og \
-T GAP8.ld

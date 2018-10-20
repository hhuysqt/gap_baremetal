riscv32-unknown-elf-gcc -o test \
startup_GAP8.S \
gap_fc_event.c gap_fll.c gap_hyperbus.c gap_pmu.c gap_uart.c gap_udma.c \
main_UART.c gap_gpio.c system_GAP8.c \
-g -fno-jump-tables -fno-tree-loop-distribute-patterns \
-Wextra -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wundef \
-fdata-sections -ffunction-sections \
-march=rv32imcxgap8 -mPE=8 -mFC=1 -D__riscv__ -D__pulp__ -D__GAP8__ \
-MMD -MP -nostartfiles -Og \
-T GAP8.ld

/*
 *
 * Automatically generated file; DO NOT EDIT.
 * Zephyr Kernel Configuration
 *
 */
#define CONFIG_UART_CONSOLE_ON_DEV_NAME "UART_1"
#define CONFIG_GPIO_PCAL9535A_1_I2C_MASTER_DEV_NAME "I2C_0"
#define CONFIG_I2C_CLOCK_SPEED 25
#define CONFIG_NUM_PREEMPT_PRIORITIES 15
#define CONFIG_PWM_PCA9685_0 1
#define CONFIG_PWM_PCA9685_0_I2C_ADDR 0x47
#define CONFIG_NUM_COOP_PRIORITIES 16
#define CONFIG_GPIO_PCAL9535A_2 1
#define CONFIG_PWM_PCA9685 1
#define CONFIG_GPIO_PCAL9535A_0_DEV_NAME "EXP0"
#define CONFIG_UART_NS16550_PCI 1
#define CONFIG_UART_NS16550_PORT_1_NAME "UART_1"
#define CONFIG_ATOMIC_OPERATIONS_BUILTIN 1
#define CONFIG_SPI_0_IRQ_PRI 2
#define CONFIG_GPIO_SCH_0_DEV_NAME "GPIO_CW"
#define CONFIG_KERNEL_BIN_NAME "zephyr"
#define CONFIG_PRINTK 1
#define CONFIG_ARCH "x86"
#define CONFIG_SHARED_IRQ_NUM_CLIENTS 5
#define CONFIG_X86 1
#define CONFIG_ADC_0_IRQ_PRI 2
#define CONFIG_GPIO_PCAL9535A_1 1
#define CONFIG_I2C_0_DEFAULT_CFG 0x12
#define CONFIG_GPIO_DW_0_NAME "GPIO_0"
#define CONFIG_UART_NS16550_PORT_0_OPTIONS 0
#define CONFIG_SERIAL_HAS_DRIVER 1
#define CONFIG_SPI_0_NAME "SPI_0"
#define CONFIG_ADC_0_NAME "ADC_0"
#define CONFIG_GPIO_DW_0_IRQ_SHARED 1
#define CONFIG_NUM_PIPE_ASYNC_MSGS 10
#define CONFIG_PINMUX_NAME "PINMUX"
#define CONFIG_SEMAPHORE_GROUPS 1
#define CONFIG_PCI_LEGACY_BRIDGE_VENDOR_ID 0x8086
#define CONFIG_SPI 1
#define CONFIG_CONSOLE_HAS_DRIVER 1
#define CONFIG_TIMESLICE_SIZE 0
#define CONFIG_SOC_FAMILY_QUARK 1
#define CONFIG_GPIO_PCAL9535A_INIT_PRIORITY 70
#define CONFIG_SHARED_IRQ_0 1
#define CONFIG_SYS_POWER_LOW_POWER_STATE_SUPPORTED 1
#define CONFIG_TIMER_READS_ITS_FREQUENCY_AT_RUNTIME 1
#define CONFIG_ARCH_DEFCONFIG "arch/x86/defconfig"
#define CONFIG_GPIO_PCAL9535A_0_I2C_MASTER_DEV_NAME "I2C_0"
#define CONFIG_UART_NS16550 1
#define CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC 25000000
#define CONFIG_NUM_MBOX_ASYNC_MSGS 10
#define CONFIG_IOAPIC 1
#define CONFIG_SYS_POWER_DEEP_SLEEP_SUPPORTED 1
#define CONFIG_SET_GDT 1
#define CONFIG_GPIO_DW 1
#define CONFIG_UART_NS16550_PORT_0_NAME "UART_0"
#define CONFIG_I2C_INIT_PRIORITY 60
#define CONFIG_EXCEPTION_DEBUG 1
#define CONFIG_SPI_INIT_PRIORITY 90
#define CONFIG_PCI_LEGACY_BRIDGE_DEVICE_ID 0x095e
#define CONFIG_I2C_0_IRQ_PRI 2
#define CONFIG_TIMESLICING 1
#define CONFIG_SHARED_IRQ_0_NAME "SHARED_IRQ0"
#define CONFIG_TIMESLICE_PRIORITY 0
#define CONFIG_ADC_TI_ADC108S102_SPI_CONFIGURATION 0x81
#define CONFIG_SPI_0_CS_GPIO_PIN 0
#define CONFIG_ADC_TI_ADC108S102_SPI_PORT_NAME "SPI_0"
#define CONFIG_GPIO_DW_0_IRQ_PRI 2
#define CONFIG_COMPILER_OPT ""
#define CONFIG_GPIO_DW_0_IRQ_SHARED_NAME "SHARED_IRQ0"
#define CONFIG_GPIO_SCH_0 1
#define CONFIG_IOAPIC_MASK_RTE 1
#define CONFIG_IOAPIC_NUM_RTES 24
#define CONFIG_I2C_0_NAME "I2C_0"
#define CONFIG_PINMUX_GALILEO_GPIO_INTEL_CW_NAME "GPIO_M0"
#define CONFIG_GPIO_SCH_INIT_PRIORITY 60
#define CONFIG_HPET_TIMER 1
#define CONFIG_SYS_LOG_ADC_LEVEL 0
#define CONFIG_SHARED_IRQ_0_IRQ 18
#define CONFIG_NANO_TIMERS 1
#define CONFIG_UART_NS16550_PORT_0_PCI 1
#define CONFIG_UART_NS16550_PORT_1_IRQ_PRI 3
#define CONFIG_TOOLCHAIN_VARIANT ""
#define CONFIG_GPIO_PCAL9535A_2_I2C_MASTER_DEV_NAME "I2C_0"
#define CONFIG_LOAPIC_BASE_ADDRESS 0xFEE00000
#define CONFIG_IDLE_STACK_SIZE 256
#define CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE 1024
#define CONFIG_GPIO_PCAL9535A_0 1
#define CONFIG_PINMUX_GALILEO_EXP2_NAME "EXP2"
#define CONFIG_I2C 1
#define CONFIG_BOARD "galileo"
#define CONFIG_PRIORITY_CEILING 0
#define CONFIG_CACHE_LINE_SIZE_DETECT 1
#define CONFIG_I2C_SHARED_IRQ 1
#define CONFIG_GPIO_PCAL9535A_0_I2C_ADDR 0x25
#define CONFIG_PCI 1
#define CONFIG_PWM 1
#define CONFIG_PINMUX_GALILEO_EXP1_NAME "EXP1"
#define CONFIG_CONSOLE 1
#define CONFIG_PWM_PCA9685_0_DEV_NAME "PWM0"
#define CONFIG_SPI_0_CS_GPIO_PORT "GPIO_0"
#define CONFIG_PINMUX_GALILEO_GPIO_DW_NAME "GPIO_0"
#define CONFIG_IDT_NUM_VECTORS 256
#define CONFIG_ADC_TI_ADC108S102_SPI_SLAVE 0
#define CONFIG_SPI_0 1
#define CONFIG_PINMUX 1
#define CONFIG_HEAP_MEM_POOL_SIZE 0
#define CONFIG_GPIO_PCAL9535A_2_I2C_ADDR 0x27
#define CONFIG_I2C_0_IRQ_SHARED 1
#define CONFIG_UART_NS16550_PORT_1 1
#define CONFIG_SHARED_IRQ_0_LEVEL_LOW 1
#define CONFIG_ADC_TI_ADC108S102 1
#define CONFIG_I2C_DW 1
#define CONFIG_SPI_INTEL 1
#define CONFIG_UART_NS16550_PORT_1_BAUD_RATE 115200
#define CONFIG_GPIO_PCAL9535A_2_DEV_NAME "EXP2"
#define CONFIG_ISR_STACK_SIZE 2048
#define CONFIG_RAM_SIZE 32768
#define CONFIG_GPIO 1
#define CONFIG_SYS_CLOCK_EXISTS 1
#define CONFIG_NANOKERNEL_TICKLESS_IDLE_SUPPORTED 1
#define CONFIG_PWM_PCA9685_INIT_PRIORITY 70
#define CONFIG_PWM_PCA9685_0_I2C_MASTER_DEV_NAME "I2C_0"
#define CONFIG_PCI_LEGACY_BRIDGE_DEV 31
#define CONFIG_NUM_DYNAMIC_TIMERS 0
#define CONFIG_BOARD_GALILEO 1
#define CONFIG_PHYS_LOAD_ADDR 0x00100000
#define CONFIG_MAX_IRQ_LINES 128
#define CONFIG_SERIAL 1
#define CONFIG_APPLICATION_INIT_PRIORITY 90
#define CONFIG_UART_NS16550_PORT_0_IRQ_PRI 0
#define CONFIG_SOC_SERIES "quark_x1000"
#define CONFIG_GPIO_PCAL9535A 1
#define CONFIG_PINMUX_INIT_PRIORITY 80
#define CONFIG_PINMUX_GALILEO_GPIO_INTEL_RW_NAME "GPIO_M1"
#define CONFIG_GPIO_SCH_1 1
#define CONFIG_GPIO_DW_SHARED_IRQ 1
#define CONFIG_OFFLOAD_WORKQUEUE_STACK_SIZE 1024
#define CONFIG_UART_CONSOLE_INIT_PRIORITY 60
#define CONFIG_ADC_TI_ADC108S102_SPI_MAX_FREQ 0x20000000
#define CONFIG_PHYS_RAM_ADDR 0x00400000
#define CONFIG_HPET_TIMER_LEVEL_LOW 1
#define CONFIG_MINIMAL_LIBC 1
#define CONFIG_ERRNO 1
#define CONFIG_SHARED_IRQ_0_PRI 2
#define CONFIG_IOAPIC_BASE_ADDRESS 0xFEC00000
#define CONFIG_OFFLOAD_WORKQUEUE_PRIORITY -1
#define CONFIG_GPIO_SCH 1
#define CONFIG_PCI_ENUMERATION 1
#define CONFIG_SHARED_IRQ_INIT_PRIORITY 45
#define CONFIG_LOAPIC 1
#define CONFIG_NESTED_INTERRUPTS 1
#define CONFIG_GPIO_DW_0 1
#define CONFIG_SOC_QUARK_X1000 1
#define CONFIG_GPIO_PCAL9535A_1_DEV_NAME "EXP1"
#define CONFIG_HPET_TIMER_BASE_ADDRESS 0xFED00000
#define CONFIG_PINMUX_GALILEO_PWM0_NAME "PWM0"
#define CONFIG_ADC 1
#define CONFIG_SYS_CLOCK_TICKS_PER_SEC 100
#define CONFIG_BOOTLOADER_UNKNOWN 1
#define CONFIG_HPET_TIMER_IRQ_PRIORITY 4
#define CONFIG_HPET_TIMER_IRQ 20
#define CONFIG_KERNEL_INIT_PRIORITY_DEFAULT 40
#define CONFIG_NANO_TIMEOUTS 1
#define CONFIG_SYSTEM_WORKQUEUE_PRIORITY -1
#define CONFIG_I2C_0 1
#define CONFIG_UART_NS16550_PORT_1_PCI 1
#define CONFIG_MAIN_STACK_SIZE 1024
#define CONFIG_KERNEL_INIT_PRIORITY_DEVICE 50
#define CONFIG_UART_NS16550_PORT_0 1
#define CONFIG_CACHE_LINE_SIZE 0
#define CONFIG_SYSTEM_CLOCK_INIT_PRIORITY 0
#define CONFIG_KERNEL_INIT_PRIORITY_OBJECTS 30
#define CONFIG_CPU_MINUTEIA 1
#define CONFIG_SOC_SERIES_QUARK_X1000 1
#define CONFIG_GPIO_DW_INIT_PRIORITY 60
#define CONFIG_CPU_HAS_FPU 1
#define CONFIG_MDEF 1
#define CONFIG_TEXT_SECTION_OFFSET 0x0
#define CONFIG_ADC_INIT_PRIORITY 95
#define CONFIG_GPIO_SCH_1_DEV_NAME "GPIO_RW"
#define CONFIG_MAIN_THREAD_PRIORITY 0
#define CONFIG_UART_NS16550_PORT_0_BAUD_RATE 115200
#define CONFIG_SHARED_IRQ 1
#define CONFIG_PCI_LEGACY_BRIDGE_BUS 0
#define CONFIG_SYS_LOG_GPIO_PCAL9535A_LEVEL 0
#define CONFIG_CROSS_COMPILE ""
#define CONFIG_SPI_CS_GPIO 1
#define CONFIG_GPIO_PCAL9535A_1_I2C_ADDR 0x26
#define CONFIG_SOC "quark_x1000"
#define CONFIG_I2C_0_IRQ_SHARED_NAME "SHARED_IRQ0"
#define CONFIG_PCI_LEGACY_BRIDGE 1
#define CONFIG_UART_NS16550_PORT_1_OPTIONS 0
#define CONFIG_UART_CONSOLE 1
#define CONFIG_SOC_FAMILY "intel_quark"
#define CONFIG_MICROKERNEL 1
#define CONFIG_MEM_POOL_SPLIT_BEFORE_DEFRAG 1
#define CONFIG_ISA_IA32 1
#define CONFIG_PINMUX_GALILEO_EXP0_NAME "EXP0"

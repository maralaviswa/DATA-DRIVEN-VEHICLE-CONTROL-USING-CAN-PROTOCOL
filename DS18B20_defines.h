#define DS18B20_PIN (1 << 12)  // P0.0
#define DS18B20_DIR IO0DIR
#define DS18B20_SET IO0SET
#define DS18B20_CLR IO0CLR
#define DS18B20_PIN_REG IO0PIN

// DS18B20 Commands
#define SKIP_ROM        0xCC
#define CONVERT_T       0x44
#define READ_SCRATCHPAD 0xBE



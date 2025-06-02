// Function Prototypes
unsigned char ow_reset(void);
void ow_write_bit(unsigned char bit);
unsigned char ow_read_bit(void);
void ow_write_byte(unsigned char byte);
unsigned char ow_read_byte(void);
void ds18b20_start_convert(void);
float ds18b20_read_temp(void);




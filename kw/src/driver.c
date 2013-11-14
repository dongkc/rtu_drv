#include <stdio.h>
#include <errno.h>

#include "modbus.h"


modbus_t* ctx = NULL;

extern uint16_t adc_buf[8];
extern uint8_t input_buf[8];
extern uint8_t output_buf[8];

void bsp_init(void) {

    ctx = modbus_new_rtu("/dev/ttyS1", 115200, 'N', 8, 1);
    modbus_set_debug(ctx, TRUE);
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485); 
    //modbus_rtu_set_rts(ctx, MODBUS_RTU_RTS_DOWN);
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
    
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n",
                modbus_strerror(errno));
	    return;
    }

    modbus_set_slave(ctx, 100);

}

void bsp_input_update(void)
{
    int rc;
    int i;

    //读取扩展模块的DI，放到数组中
    rc = modbus_read_input_bits(ctx, 0, 8, input_buf);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }

    //读取扩展模块的8个模拟量，放到相应的数组中
    rc = modbus_read_registers(ctx, 0, 8, adc_buf);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
}

#if 0
    rc = modbus_write_register(ctx, 2, 0x77);
    test[0] = 1;
    test[1] = 100;
    rc = modbus_write_registers(ctx, 0, 2, &test[0]);
#endif
//
//    //读取扩展模块的8个模拟量，放到相应的数组中
#if 0
    rc = modbus_read_input_registers(ctx, 0, 124, &test[0]);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }
#endif

}

void bsp_output_update(void) {
    int rc;
    //写扩展模块的DO，用来控制扩展模块的开关量输出
    rc = modbus_write_bits(ctx, 0, 8, output_buf);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }

}

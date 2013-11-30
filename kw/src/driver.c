#include <stdio.h>
#include <errno.h>

#include "modbus.h"


modbus_t* ctx = NULL;

extern uint16_t adc_buf[32];
extern uint8_t  input_buf[32];
extern uint8_t  output_buf[32];

void bsp_init(void) {

    ctx = modbus_new_rtu("/dev/ttyS1", 115200, 'N', 8, 1);
    modbus_set_debug(ctx, TRUE);
    modbus_rtu_set_rts(ctx, MODBUS_RTU_RTS_DOWN);
#if 0
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
#endif
    
    modbus_set_slave(ctx, 0x03);
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n",
                modbus_strerror(errno));
	    return;
    }

    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485); 
}

void bsp_input_update()
{
    int rc;
    int i;

    modbus_set_slave(ctx, 0x02);
    //读取扩展模块的DI，放到数组中
    rc = modbus_read_input_bits(ctx, 0, 8, input_buf);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }

    rc = modbus_read_input_registers(ctx, 0, 8, adc_buf);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }

    modbus_set_slave(ctx, 0x03);
    //读取扩展模块的DI，放到数组中
    rc = modbus_read_input_bits(ctx, 0, 8, &input_buf[8]);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }

    rc = modbus_read_input_registers(ctx, 0, 8, &adc_buf[8]);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }
    
#if 1
    modbus_set_slave(ctx, 4);
    //读取扩展模块的DI，放到数组中
    rc = modbus_read_input_bits(ctx, 0, 8, &input_buf[16]);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }

    rc = modbus_read_input_registers(ctx, 0, 8, &adc_buf[16]);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }

    modbus_set_slave(ctx, 5);
    //读取扩展模块的DI，放到数组中
    rc = modbus_read_input_bits(ctx, 0, 8, &input_buf[24]);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }

    rc = modbus_read_input_registers(ctx, 0, 8, &adc_buf[24]);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }
#endif
}

void bsp_output_update(void) {
    int rc;

    modbus_set_slave(ctx, 2);
    //写扩展模块的DO，用来控制扩展模块的开关量输出
    rc = modbus_write_bits(ctx, 0, 8, output_buf);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }

    modbus_set_slave(ctx, 3);
    //写扩展模块的DO，用来控制扩展模块的开关量输出
    rc = modbus_write_bits(ctx, 0, 8, &output_buf[8]);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }

#if 1
    modbus_set_slave(ctx, 4);
    //写扩展模块的DO，用来控制扩展模块的开关量输出
    rc = modbus_write_bits(ctx, 0, 8, &output_buf[16]);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }

    modbus_set_slave(ctx, 5);
    //写扩展模块的DO，用来控制扩展模块的开关量输出
    rc = modbus_write_bits(ctx, 0, 8, &output_buf[24]);
    if (rc != 8) {
        printf("FAILED (nb points %d)\n", rc);
        //goto close;
    }
#endif
}

int _main(int argc, const char *argv[])
{
    int i;
    int n = 0;
    bsp_init();

    while (1) {
        bsp_input_update();

            for ( i = 0; i < 32; ++i) {
                output_buf[i] = i % 2;
            }
        bsp_output_update();
        for (i = 0; i < 500000; ++i);

        usleep(50000);

        bsp_input_update();
            for ( i = 0; i < 32; ++i) {
                output_buf[i] = (i + 1) % 2;
            }

        bsp_output_update();
        for (i = 0; i < 500000; ++i);
        usleep(50000);
    }
    
    return 0;
}

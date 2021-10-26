#include "sml_output.h"
#include "kb.h"
#include "app_config.h"
#include <stdio.h>
#include <string.h>

#define SERIAL_OUT_CHARS_MAX 512

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif


static char serial_out_buf[SERIAL_OUT_CHARS_MAX];
static uint8_t recent_fv[MAX_VECTOR_SIZE];
static uint8_t recent_fv_len;
static uint8_t write_features = 1;

static void sml_output_serial(uint16_t model, uint16_t classification)
{
    size_t written = 0;

    written += snprintf(serial_out_buf, sizeof(serial_out_buf),
               "{\"ModelNumber\":%d,\"Classification\":%d", (int) model, (int) classification);
    if(write_features)
    {
        written += snprintf(&serial_out_buf[written], sizeof(serial_out_buf)-written,
               ",\"FeatureLength\":%d,\"FeatureVector\":[", (int) recent_fv_len);
        for(int j=0; j < recent_fv_len; j++)
        {
            written += snprintf(&serial_out_buf[written], sizeof(serial_out_buf)-written, "%d", (int) recent_fv[j]);
            if(j < recent_fv_len -1)
            {
                serial_out_buf[written++] = ',';
            }
        }
        serial_out_buf[written++] = ']';
    }
    written += snprintf(&serial_out_buf[written], sizeof(serial_out_buf)-written, "}\n");

    UART_Write((uint8_t *) serial_out_buf, strlen(serial_out_buf));
}

uint32_t sml_output_results(uint16_t model, uint16_t classification)
{
    sml_get_feature_vector(model, recent_fv, &recent_fv_len);
    sml_output_serial(model, classification);
    return 0;
}

uint32_t sml_output_init(void *p_module)
{
    //unused for now
    return 0;
}

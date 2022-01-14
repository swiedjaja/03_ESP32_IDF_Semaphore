#include <driver/gpio.h>
void gpio_pinMode(int nPin, int nMode)
{
    // gpio_config_t io;
    // io.mode = nMode;
    // io.pin_bit_mask = (1ULL<<nPin);
    // io.intr_type = GPIO_INTR_DISABLE;
    // io.pull_down_en = 0;
    // io.pull_up_en = 0;
    // gpio_config(&io);
    gpio_pad_select_gpio(nPin);
    gpio_set_direction(nPin, nMode);
}

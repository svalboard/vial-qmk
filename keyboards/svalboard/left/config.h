#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_PIO_USE_PIO0

//WS2812-fu here:
// pretty lights
// https://docs.qmk.fm/#/feature_rgblight?id=configuration
#define WS2812_DI_PIN GP19
#define RGBLED_NUM 2
#define RGBLED_SPLIT { 1, 1 }
#define RGBLIGHT_LAYERS DYNAMIC_KEYMAP_LAYER_COUNT
#define RGBLIGHT_DEFAULT_SAT 0 // white?
#define RGBLIGHT_LIMIT_VAL 130
#define RGBLIGHT_DEFAULT_VAL 20
#define RGBLIGHT_LAYERS_RETAIN_VAL // remember val across restarts
#define RGBLIGHT_SLEEP // don't annoy when host asleep
#define RGBLIGHT_MAX_LAYERS 16 //DYNAMIC_KEYMAP_LAYER_COUNT
#define RGBLIGHT_VAL_STEP 10
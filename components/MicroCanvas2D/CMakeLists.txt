
set(COMPONENT_SRCS  "uCanvasRenderEngine.c" 
                    "vector.c" 
                    "uCanvas_api.c"
                    "uCanvas_User_IO.c"
                    "uCanvas_display_port_st7789.c" 
                    "uCanvas_display_port_ssd1306.c"  
)
# Fix cmake build

idf_component_register(SRCS "${COMPONENT_SRCS}"
                        INCLUDE_DIRS "." 
                       PRIV_REQUIRES driver SSD1306_Driver st7789  spiffs)


set(COMPONENT_SRCS  "simple_menu/simple_menu.c"
                    "ucanvas_button/ucanvas_button.c"
                    "ucanvas_slider/ucanvas_slider.c"
)
# Fix cmake build

idf_component_register(SRCS "${COMPONENT_SRCS}"
                        INCLUDE_DIRS "." "simple_menu" "ucanvas_button" "ucanvas_slider"
                       PRIV_REQUIRES driver MicroCanvas2D)

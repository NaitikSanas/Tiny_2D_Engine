#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <unistd.h>
#include <time.h>
#include "math.h"
#include "esp_log.h"
#include "uCanvas_api.h"
#include "uCanvas_Physix.h" 
#include "uCanvas_User_IO.h"
#include "simple_menu.h"

void menu_task(selection_menu_obj_t* menu_obj);
void create_menu(selection_menu_obj_t* menu_obj,uCanvas_universal_obj_t* cursor_object){
    char buf[32]={0};
    for (int i = 0; i < menu_obj->active_elements; i++)
    {
        sprintf(buf,"item %d",i);
        printf("added %d at x %d y %d\r\n",i,menu_obj->menu_position_x+menu_obj->text_offset_x,menu_obj->menu_position_y+(i*menu_obj->span_y));
        menu_obj->content[i] = New_uCanvas_2DTextbox(buf,menu_obj->menu_position_x+menu_obj->text_offset_x,menu_obj->menu_position_y+(i*menu_obj->span_y));
    }
    menu_obj->cursor = cursor_object;  
    if(menu_obj->enable_index_disp){
        printf("enable_index_disp true\r\n");
        menu_obj->index_disp = New_uCanvas_2DTextbox("--",0,0);
    }
    uCanvas_Add_Task(menu_task,menu_obj,1);
}

void menu_task(selection_menu_obj_t* menu_obj){
    while(1){
        char buf[32]={0};
        int active = menu_obj->active_elements - 1;
        if(!uCanvas_Get_PushbuttonState_WTR(menu_obj->up_btn)){
            if(menu_obj->cursor_index < active){
                menu_obj->cursor_index++;
                for (int i = 0; i < menu_obj->span_y; i++)
                {
                    for (int j = 0; j < menu_obj->active_elements; j++)
                    {
                        menu_obj->content[j]->properties.position.y--;
                    }   
                    uCanvas_Delay(2);
                }  
                uCanvas_Delay(10);

                if(menu_obj->enable_index_disp){
                    sprintf(buf,"%d",menu_obj->cursor_index);
                    printf("selector %d\r\n",menu_obj->cursor_index);
                    uCanvas_Set_Text(menu_obj->index_disp,buf);
                }
            }  
            else {
                menu_obj->cursor_index = 0;
                if(menu_obj->enable_index_disp){
                    sprintf(buf,"%d",menu_obj->cursor_index);
                    printf("selector %d\r\n",menu_obj->cursor_index);
                    uCanvas_Set_Text(menu_obj->index_disp,buf);
                }
                for (int k = 0; k < active; k++)
                {
                    for (int i = 0; i < menu_obj->span_y; i++)
                    {
                        for (int j = 0; j < menu_obj->active_elements; j++)
                        {
                            menu_obj->content[j]->properties.position.y++;
                        }   
                        uCanvas_Delay(1);
                    }
                }  
            }
        }
        else 
        if(!uCanvas_Get_PushbuttonState_WTR(menu_obj->down_btn)){
            if(menu_obj->cursor_index >0){
                menu_obj->cursor_index--;
                for (int i = 0; i < menu_obj->span_y; i++)
                {
                    for (int j = 0; j < menu_obj->active_elements; j++)
                    {
                        menu_obj->content[j]->properties.position.y++;
                    }   
                    uCanvas_Delay(2);
                }  
                uCanvas_Delay(10);

                if(menu_obj->enable_index_disp){
                    sprintf(buf,"%d",menu_obj->cursor_index);
                    printf("selector %d\r\n",menu_obj->cursor_index);
                    uCanvas_Set_Text(menu_obj->index_disp,buf);
                }
            }  
            else {
                menu_obj->cursor_index = menu_obj->active_elements-1;
                if(menu_obj->enable_index_disp){
                    sprintf(buf,"%d",menu_obj->cursor_index);
                    printf("selector %d\r\n",menu_obj->cursor_index);
                    uCanvas_Set_Text(menu_obj->index_disp,buf);
                }
                for (int k = 0; k < active; k++)
                {
                    for (int i = 0; i < menu_obj->span_y; i++)
                    {
                        for (int j = 0; j < menu_obj->active_elements; j++)
                        {
                            menu_obj->content[j]->properties.position.y--;
                        }   
                        uCanvas_Delay(1);
                    }
                } 
            }
        }
        if(!uCanvas_Get_PushbuttonState_WTR(menu_obj->select_btn)){
            if(menu_obj->click_handler !=NULL){
                menu_obj->click_handler();
            }
        }

        uCanvas_Delay(1);
    }
}

int menu_get_current_index(selection_menu_obj_t* menu_obj){
    return menu_obj->cursor_index;
}

void menu_add_gpio_control(selection_menu_obj_t* menu_obj,uint32_t up_control,uint32_t down_control, uint32_t select_btn){
    menu_obj->up_btn = up_control;
    menu_obj->down_btn = down_control;
    menu_obj->select_btn = select_btn;
}

void menu_set_active_elements(selection_menu_obj_t* menu_obj,uint16_t active_elements){
    menu_obj->active_elements = active_elements;
}

void menu_set_enable_cursor_index_text(selection_menu_obj_t* menu_obj,uint16_t state){
    menu_obj->enable_index_disp = state;
}

void menu_set_content(selection_menu_obj_t* menu_obj,char* content, uint8_t index){
    uCanvas_Set_Text(menu_obj->content[index],content);
}
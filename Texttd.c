#include "Texttd.h"

VMWSTR myPath;
VMFILE f_read;
vm_sms_add_msg_data_t msg_data;
VMUINT8 *buffer;

void vm_main(void) {

    layer_hdl[0] = -1;
    vm_reg_sysevt_callback(handle_sysevt);
    vm_reg_keyboard_callback(handle_keyevt);

    VMWCHAR fullPath[100];
    VMWCHAR appName[100];
    VMCHAR asciiAppName[100];
    VMCHAR file_name[100];

    vm_get_exec_filename(fullPath);
    vm_get_filename(fullPath, appName);
    vm_ucs2_to_ascii(asciiAppName, 100, appName);
    memcpy(file_name, asciiAppName, strlen(asciiAppName) - 3);
    file_name[strlen(asciiAppName) - 3] = '\0';
    strcat(file_name, "txt");

    VMINT drv;
    VMCHAR f_name[100 + 1];
    VMWCHAR f_wname[100 + 1];

    if ((drv = vm_get_removable_driver()) < 0) {
        drv = vm_get_system_driver();
    }

    sprintf(f_name, "%c:\\%s", drv, file_name);
    vm_ascii_to_ucs2(f_wname, 100, f_name);
    f_read = vm_file_open(f_wname, MODE_READ, FALSE);

    if (f_read < 0) {
        vm_file_close(f_read);
        vm_selector_run(0, 0, file);
    } else {
        vm_file_close(f_read);
        myPath = vm_malloc((strlen(f_name) + 1) * 2);
        vm_wstrcpy(myPath, f_wname);
        mre_delete_msg();
        read_file_data(myPath);
    }
    vm_free(myPath);
}

void handle_sysevt(VMINT message, VMINT param) {

    switch (message) {
        case VM_MSG_CREATE:
        case VM_MSG_ACTIVE:
            layer_hdl[0] =
                vm_graphic_create_layer(0, 0, vm_graphic_get_screen_width(),
                                        vm_graphic_get_screen_height(), -1);
            vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(),
                                vm_graphic_get_screen_height());
            buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);
            vm_switch_power_saving_mode(turn_off_mode);
            break;

        case VM_MSG_PAINT:
            //vm_switch_power_saving_mode(turn_off_mode);
            mre_draw_black_rectangle();
            break;

        case VM_MSG_INACTIVE:

            vm_switch_power_saving_mode(turn_on_mode);
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;

        case VM_MSG_QUIT:
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;
    }
}

void handle_keyevt(VMINT event, VMINT keycode) {

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_RIGHT_SOFTKEY) {
        if (layer_hdl[0] != -1) {
            vm_graphic_delete_layer(layer_hdl[0]);
            layer_hdl[0] = -1;
        }
        vm_exit_app();
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_LEFT_SOFTKEY) {
        vm_selector_run(0, 0, file);
    }

}

VMINT file(VMWCHAR *FILE_PATH, VMINT wlen) {

    myPath = vm_realloc(myPath, (wlen + 1) * 2);
    vm_wstrcpy(myPath, FILE_PATH);
    mre_delete_msg();
    read_file_data(myPath);
    return 0;
}

VMINT read_file_data(VMWSTR path) {

        mre_draw_black_rectangle();
        void *user_data;
        VMCHAR ascii_data[500]; //1 SMS = 160 characters (GSM-7 encoding) or 70 characters (UCS-2 encoding)
        VMWCHAR ucs2_data[2000];
        VMUINT nread;

        user_data = vm_malloc(100);
        f_read = vm_file_open(path, MODE_READ, FALSE);
        vm_file_read(f_read, ascii_data, 500, &nread);
        ascii_data[nread] = '\0';
        vm_file_close(f_read);
        vm_ascii_to_ucs2(ucs2_data, (strlen(ascii_data) + 1) * 2, ascii_data);

        msg_data.status = VM_SMS_STATUS_DRAFT;
        msg_data.storage_type = VM_SMS_STORAGE_ME;
        msg_data.sim_id = VM_SMS_SIM_1;
        msg_data.content_size = wstrlen(ucs2_data);
        msg_data.content = ucs2_data;

        vm_sms_add_msg(&msg_data, mre_msg_callback, user_data);
        vm_free(user_data);
        user_data = NULL;
        return 0;
}

static void mre_msg_callback(vm_sms_callback_t *callback_data) {
     
    if(callback_data->result == 1)
    {

        if(callback_data->cause == VM_SMS_CAUSE_NO_ERROR)
        {

           switch(callback_data->action)
            {

            case  VM_SMS_ACTION_NONE :
                break;
            case VM_SMS_ACTION_SAVE :
                //vm_vibrator_once();
                break;
            case  VM_SMS_ACTION_DELETE :
                //vm_vibrator_once();
                break;
            default :
                break;
            }
        }
        else
        {

        }
    }
    else
    {

    }
}

void mre_draw_black_rectangle(void) {

    buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);
    vm_graphic_fill_rect(buffer, 0, 0, vm_graphic_get_screen_width(),
                         vm_graphic_get_screen_height(), VM_COLOR_BLACK,
                         VM_COLOR_BLACK);
    vm_graphic_flush_layer(layer_hdl, 1);
}

static void mre_delete_msg()
{
    void *user_data;
    user_data = vm_malloc(100);
    vm_sms_delete_msg_list(VM_SMS_BOX_DRAFTS, VM_SMS_SIM_1, mre_msg_callback, user_data);
    vm_free(user_data);
    user_data = NULL;
 
}
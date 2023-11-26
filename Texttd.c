#include "Texttd.h"

VMFILE f_read;
VMUINT8 *buffer = NULL;
VMINT trigeris = 1;
VMINT trigeris1 = 0;
VMINT trigeris2 = 0;

void vm_main(void) {

    layer_hdl[0] = -1;
    vm_reg_sysevt_callback(handle_sysevt);
    vm_reg_keyboard_callback(handle_keyevt);
    vm_font_set_font_size(VM_SMALL_FONT);
    checkFileExist();
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
            //vm_switch_power_saving_mode(turn_off_mode);
            break;

        case VM_MSG_PAINT:
            //vm_switch_power_saving_mode(turn_off_mode);
            mre_draw_black_rectangle();
            if (trigeris1 == 1) {trigeris2 = 1;}
            if (trigeris == 0) {trigeris1 = 1;}
            if (trigeris2 == 1) {vm_exit_app();}
            break;

        case VM_MSG_INACTIVE:

            //vm_switch_power_saving_mode(turn_on_mode);
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
        trigeris = vm_selector_run(0, 0, file);
    }

}

VMINT file(VMWCHAR *FILE_PATH, VMINT wlen) {

    trigeris = 1;
    trigeris1 = 0;
    trigeris2 = 0;
    read_file_data(FILE_PATH);
    FILE_PATH = NULL;
    return 0;
}

VMINT read_file_data(VMWSTR path) {

        vm_sms_add_msg_data_t msg_data;
        VMCHAR ascii_data[500]; //1 SMS = 160 characters (GSM-7 encoding) or 70 characters (UCS-2 encoding)
        VMWCHAR ucs2_data[2000];
        VMUINT nread;

        f_read = vm_file_open(path, MODE_READ, FALSE);
        vm_file_read(f_read, ascii_data, 500, &nread);
        ascii_data[nread] = '\0';
        vm_file_close(f_read);
        vm_ascii_to_ucs2(ucs2_data, (strlen(ascii_data) + 1) * 2, ascii_data);

        vm_sms_delete_msg_list(VM_SMS_BOX_DRAFTS, VM_SMS_SIM_1, mre_msg_callback, NULL);

        msg_data.status = VM_SMS_STATUS_DRAFT;
        msg_data.storage_type = VM_SMS_STORAGE_ME;
        msg_data.sim_id = VM_SMS_SIM_1;
        msg_data.content_size = wstrlen(ucs2_data);
        msg_data.content = (VMSTR)ucs2_data; //	VMINT8 *content; /* content buffer */
        vm_sms_add_msg(&msg_data, mre_msg_callback, NULL);

        trigeris = 1;
        trigeris1 = 0;
        trigeris2 = 0;

        buffer = NULL;

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
            case  VM_SMS_ACTION_SAVE :
                //trigeris = 1;
                draw_done();
                break;
            case  VM_SMS_ACTION_DELETE :
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

static void draw_done(void) {

    VMWSTR s;
    int x, y, wstr_len;
    vm_graphic_color color;
    VMWCHAR display_string[100];
    x = 0;
    y = 0;

    mre_draw_black_rectangle();

    vm_ascii_to_ucs2(display_string, 100, (VMSTR)"Done");
    s = (VMWSTR)display_string;
    wstr_len = vm_graphic_get_string_width(s);
    color.vm_color_565 = VM_COLOR_WHITE;
    vm_graphic_setcolor(&color);
    vm_graphic_textout_to_layer(layer_hdl[0], x, y, s, wstr_len);
    vm_graphic_flush_layer(layer_hdl, 1);
}

void create_app_txt_filename(VMWSTR text) {

    VMWCHAR fullPath[100];
    VMWCHAR appName[100];
    VMCHAR asciiAppName[100];
    VMCHAR file_name[100];

    vm_get_exec_filename(fullPath);
    vm_get_filename(fullPath, appName);
    vm_ucs2_to_ascii(asciiAppName, wstrlen(appName) + 1, appName);
    memcpy(file_name, asciiAppName, strlen(asciiAppName) - 3);
    file_name[strlen(asciiAppName) - 3] = '\0';
    strcat(file_name, "txt");
    vm_ascii_to_ucs2(text, (strlen(file_name) + 1) * 2, file_name);

}

void create_auto_full_path_name(VMWSTR result, VMWSTR fname) {

    VMINT drv;
    VMCHAR fAutoFileName[100];
    VMWCHAR wAutoFileName[100];

    if ((drv = vm_get_removable_driver()) < 0) {
       drv = vm_get_system_driver();
    }

    sprintf(fAutoFileName, "%c:\\", drv);
    vm_ascii_to_ucs2(wAutoFileName, (strlen(fAutoFileName) + 1) * 2, fAutoFileName);
    vm_wstrcat(wAutoFileName, fname);
    vm_wstrcpy(result, wAutoFileName);

}

void checkFileExist(void) {

    VMWCHAR file_pathw[100];
    VMWCHAR appTxtName[100];

    create_app_txt_filename(appTxtName);
    create_auto_full_path_name(file_pathw, appTxtName);

    f_read = vm_file_open(file_pathw, MODE_READ, FALSE);

    if (f_read < 0) {
        vm_file_close(f_read);
        trigeris = vm_selector_run(0, 0, file);
    } else {
        vm_file_close(f_read);
        mre_draw_black_rectangle();
        read_file_data(file_pathw);     
    }
}
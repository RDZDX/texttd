#include "Textts.h"

vm_sms_add_msg_data_t msg_data;

void vm_main(void) {

    VMCHAR file_name[100] = "newx.txt";
    VMINT drv;
    VMFILE f_read;
    VMCHAR f_name[100 + 1];
    VMWCHAR f_wname[100 + 1];
    VMCHAR ascii_data[1999];
    VMWCHAR ucs2_data[2000];
    VMUINT nread;

    if ((drv = vm_get_removable_driver()) < 0) {
        drv = vm_get_system_driver();
    }

    sprintf(f_name, "%c:\\%s", drv, file_name);
    vm_ascii_to_ucs2(f_wname, 100, f_name);
    f_read = vm_file_open(f_wname, MODE_READ, FALSE);

    if (f_read < 0) {
        vm_file_close(f_read);
        //vm_selector_run(0, 0, file);
    } else {
        vm_file_read(f_read, ascii_data, 1999, &nread);
        ascii_data[nread] = '\0';                                       //USER_DATA_SIZE  (100)
        vm_file_close(f_read);                                          //MAX_SMS_SIZE    (100)
        vm_ascii_to_ucs2(ucs2_data, (strlen(ascii_data) + 1) * 2, ascii_data);

        msg_data.status = VM_SMS_STATUS_DRAFT;
        msg_data.sim_id = VM_SMS_SIM_1;
        msg_data.storage_type = VM_SMS_STORAGE_ME;
        //msg_data.content_size = 100;
        msg_data.content_size = wstrlen(ucs2_data);
        msg_data.content = ucs2_data;

        vm_sms_add_msg(&msg_data, mre_add_msg_callback, NULL);
        vm_selector_run(0, 0, file);

    }
}

static void mre_add_msg_callback(vm_sms_callback_t *callback_data) {
     
    if(callback_data->result == 1)
    {

        if(callback_data->cause == VM_SMS_CAUSE_NO_ERROR)
        {

           switch(callback_data->action)
            {

            case  VM_SMS_ACTION_NONE :
                //break;
            case VM_SMS_ACTION_SAVE :
                vm_vibrator_once();
                //break;
            default :
                break;
            }
        }
        else
        {
            //vm_ascii_to_ucs2(s, MRE_MAX_SIZE_STRING, "callback cause error");
        }
    }
    else
    {
        //vm_ascii_to_ucs2(s, MRE_MAX_SIZE_STRING, "callback result !=1");

    }
}

VMINT file(VMWCHAR *FILE_PATH, VMINT wlen) {

    //myPath = vm_realloc(myPath, (wlen + 1) * 2);
    //vm_wstrcpy(myPath, FILE_PATH);
    //read_file_data(myPath);
    vm_exit_app();
    return 0;
}
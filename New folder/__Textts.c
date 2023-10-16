#include "Textts.h"

VMWSTR myPath;
VMCHAR file_name[100] = "newx.txt";
VMFILE f_read;
vm_sms_add_msg_data_t msg_data;

void vm_main(void) {

        checkFileExist();
}


VMINT file(VMWCHAR *FILE_PATH, VMINT wlen) {

    myPath = vm_realloc(myPath, (wlen + 1) * 2);
    vm_wstrcpy(myPath, FILE_PATH);
    read_file_data(myPath);
    return 0;
}

VMINT read_file_data(VMWSTR path) {

        //void *user_data;
        VMCHAR ascii_data[1999];
        VMWCHAR ucs2_data[2000];
        VMUINT nread;

        //user_data = vm_malloc(MRE_MAX_BUFFER_SIZE);                   //MRE_MAX_BUFFER_SIZE (100)
        f_read = vm_file_open(myPath, MODE_READ, FALSE);                //PHONE_NUMBER ("12345")
        vm_file_read(f_read, ascii_data, 1999, &nread);
        ascii_data[nread] = '\0';                                       //USER_DATA_SIZE  (100)
        vm_file_close(f_read);                                          //MAX_SMS_SIZE    (100)
        vm_ascii_to_ucs2(ucs2_data, (strlen(ascii_data) + 1) * 2, ascii_data);

        msg_data.status = VM_SMS_STATUS_DRAFT;
        msg_data.sim_id = VM_SMS_SIM_1;
        msg_data.storage_type = VM_SMS_STORAGE_ME;
        //msg_data.timestamp = ;
        msg_data.content_size = 100;
        msg_data.content = ucs2_data;

        //vm_sms_add_msg(&msg_data, mre_add_msg_callback, user_data);
        vm_sms_add_msg(&msg_data, mre_add_msg_callback, NULL);

        //vm_free(user_data);
        //user_data = NULL;

        return 0;
}

void checkFileExist(void) {

    VMINT drv;
    VMCHAR f_name[100 + 1];
    VMWCHAR f_wname[100 + 1];
    VMFILE f_read;
    VMWCHAR *FILE_PATH = 0;
    int size = 0;

    if ((drv = vm_get_removable_driver()) < 0) {
        drv = vm_get_system_driver();
    }

    sprintf(f_name, "%c:\\%s", drv, file_name);
    vm_ascii_to_ucs2(f_wname, 100, f_name);
    f_read = vm_file_open(f_wname, MODE_READ, FALSE);

    if (f_read < 0) {
        vm_file_close(f_read);
        vm_selector_run(0, FILE_PATH, file);
    } else {
        vm_file_close(f_read);
        size = (strlen(f_name) + 1) * 2;
        myPath = vm_malloc(size);
        vm_wstrcpy(myPath, f_wname);
        read_file_data(myPath);
    }

}

static void mre_add_msg_callback(vm_sms_callback_t *callback_data)
{
     
    if(callback_data->result == 1)
    {

        if(callback_data->cause == VM_SMS_CAUSE_NO_ERROR)
        {

           switch(callback_data->action)
            {

            case  VM_SMS_ACTION_NONE :
                break;
            case VM_SMS_ACTION_SAVE :
                vm_vibrator_once();
                break;
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

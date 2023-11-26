#ifndef _VRE_APP_WIZARDTEMPLATE_
#define	_VRE_APP_WIZARDTEMPLATE_

#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "stdio.h"
#include "ResID.h"
#include "vm4res.h"
#include "vmsms.h"

#include "string.h"
#include "stdint.h"

//#include "vmmm.h"

VMINT		layer_hdl[1];				////layer handle array.

void handle_sysevt(VMINT message, VMINT param);
void handle_keyevt(VMINT event, VMINT keycode);
VMINT file(VMWCHAR *FILE_PATH, VMINT wlen);
VMINT read_file_data(VMWSTR path);
static void mre_msg_callback(vm_sms_callback_t *callback_data);
void mre_draw_black_rectangle(void);
static void draw_done(void);
void create_app_txt_filename(VMWSTR text);
void create_auto_full_path_name(VMWSTR result, VMWSTR fname);
void checkFileExist(void);

#endif


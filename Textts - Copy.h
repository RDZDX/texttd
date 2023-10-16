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

#include "vmmm.h"


#define	 MRE_MAX_SIZE_STRING (50)
#define  MRE_STRING_PRINT_X (10)
#define  MRE_STRING_PRINT_Y (20)
#define VM_SCREEN_START_X (0)
#define VM_SCREEN_START_Y (0)
#define MARGIN (20)
#define PHONE_NUMBER ("12345")
#define MRE_MAX_BUFFER_SIZE (100)
#define MAX_BOX_TYPE    (7)
#define FIRST_MESSAGE  (1)

#define MAX_SMS_SIZE    (100)
#define USER_DATA_SIZE  (100)

#define     MRE_NO_CHANGE      (0)
#define     MRE_SUCCESS        (1)
#define     MRE_INTIAL_VALUE_ZERO (0)

static void mre_add_msg_callback(vm_sms_callback_t *callback_data);
VMINT file(VMWCHAR *FILE_PATH, VMINT wlen);

#endif


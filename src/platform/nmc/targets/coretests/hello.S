
import from "platform/rumboot.mlb";

global corestart: label;
global eloop: label;
 
begin ".text.init"
<corestart>
    rumboot_nmc_init_core();
    rumboot_runtime_init(016800h);
    /* Successful exit, code 0 */
    rumboot_exit(0_16800h, 0);

    /* If something went wrong. Not reached in this test */
    rumboot_exit(0_16800h, 1);

<eloop>
    goto eloop;

end ".text.init";

.section .text.startup

.global rumboot_default_irq_vectors
rumboot_default_irq_vectors:
  b      rumboot_reset_handler
  b      rumboot_undefined_handler
  b      rumboot_swi_handler
  b      rumboot_prefetch_handler
  b      rumboot_abort_handler
  nop
  b      rumboot_irq_handler
  b      rumboot_fiq_handler


rumboot_undefined_handler:
  b endless_loop

rumboot_swi_handler:
  b endless_loop

rumboot_prefetch_handler:
  b endless_loop

rumboot_abort_handler:
  b endless_loop

endless_loop:
  b endless_loop;

rumboot_irq_handler:
  b endless_loop

rumboot_fiq_handler:
  b endless_loop

.global rumboot_reset_handler

rumboot_reset_handler:
  ldr sp, =rumboot_platform_stack_top
  bl rumboot_main
  b endless_loop

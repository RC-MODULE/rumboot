#ifndef NATIVE_PCIE_H
#define NATIVE_PCIE_H

/**
 * @brief Obtain a pointer to PCIe registers via linux driver
 * 
 * @param dev_id device id, mmiopcie-%d
 * @param addr region start address 
 * @param len region length
 * @return uintptr_t pointer to requested area
 */
uintptr_t rumboot_native_request_device(int dev_id, uint32_t addr, size_t len);


#endif // !NATIVE_PCIE_H
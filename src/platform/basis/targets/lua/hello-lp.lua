print("Hello, I'm lua basis example")
lp=require("lprobe")

-- Wow, we can read ROM
function romdemo()
  print("Hello, I can dumpz some rom")
  local dev=lp:open("memories")
  local rom=dev:request_mem("ROM");
  print("hexdumping some rom codez")
  rom:hexdump("rom", true, 0, 32)
end

-- And write/read IM0 as well
function ramdemo()
  print("Hello, I can write and read IM0")
  local dev=lp:open("memories")
  local ram=dev:request_mem("IM0")
  ram:u8(5, 0xfe);
  print(string.format("==> u8 0x%x", ram:u8(0)));
  ram:u32(0, 0xdeadc0de);
  print(string.format("==> u32 0x%x", ram:u32(0)));
  ram:hexdump("rom", true, 0, 32)
end

function regsdemo()
  print("Hello, I can write and read sp804 registers")
  local dev = lp:open("sp804_0");
  local regs = dev:request_mem("registers", "../../sp804.lua")
  regs:io("Timer1Load", 0xdeadc0de);
  regs:dump();
end

regsdemo()

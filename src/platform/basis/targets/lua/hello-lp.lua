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


function ramtest()
  print("I will test all transaction types")
  local dev=lp:open("memories")
  local ram=dev:request_mem("IM0")
  print(ram:u32(0))

  print("8-bit io")
  for i = 0, 8, 1 do
    ram:u8(i, i);
  end

  print("16-bit io")
  for i = 0, 8, 1 do
    ram:u16(0x100 + i*2, i);
  end

  print("32-bit io")
  for i = 0, 8, 1 do
    ram:u32(0x200 + i*4, i);
  end

  print("64-bit io")
  for i = 0, 8, 1 do
    ram:u64(0x300 + i*8, i);
  end

  print("done");
  ram:hexdump("IM0", true, 0, 0x800)
end



function dumpv(ram, accfunc, sz, size)
  for i=0,size,sz do
    local v = ram[accfunc](ram, i);
    print(string.format("[%x] = %x", i, v))
  end
end

function check_read(ram, accfunc, sz, expected)
  print("Checking reads with "..accfunc)
  for i,j in ipairs(expected) do
    pos = #expected * sz;
    print("Reading "..pos-i*sz.." el "..i);
    if (ram[accfunc](ram, pos-i*sz) ~= j) then
      print(string.format("[0x%x] expected 0x%x got 0x%x",
        pos-i*sz, expected[i], ram[accfunc](ram, pos-i*sz)))
        dumpv(ram, accfunc, sz, 8);
      error(sz.."-byte io doesn't work as expected")
    end
  end
end



function iotest()
  print("I will test all transaction types")
  local dev=lp:open("memories")
  local ram=dev:request_mem("IM0")

  expected = { 0x1e, 0xe7, 0xde, 0xad, 0xc0, 0xbe }
  check_read(ram, "u8", 1, expected)


  ram:u64(0, 0x1ee7deadc0be);

  if (ram:u64(0) ~= 0x1ee7deadc0be) then
    print(string.format("%x", ram:u64(0)));
    error("64-bit io doesn't work as expected")
  end

  expected = {  0x1ee7, 0xdeadc0be }
  check_read(ram, "u32", 4, expected)

  expected = { 0x1ee7, 0xdead, 0xc0be}
  check_read(ram, "u16", 2, expected)

  print("done")
end

iotest();

lp=require("lprobe")

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

    print("Writing "..pos-i*sz.." el "..i);
    ram[accfunc](ram, pos-i*sz, j);

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

  expected = {  0xabcd, 0xcdef }
  check_read(ram, "u32", 4, expected)

  expected = { 0x1ee7, 0xdead, 0xc0be}
  check_read(ram, "u16", 2, expected)

  print("done")
end


function test_initial_data()
  print("I will verify data placed by loader")
  local dev=lp:open("memories")
  local ram=dev:request_mem("IM0")

  print(string.format("0x%x", ram:u64(0)));
  print(string.format("0x%x", ram:u32(8)));
  print(string.format("0x%x", ram:u16(12)));
  print(string.format("0x%x", ram:u8(14)));

  if ram:u64(0) ~= 0x1fffdeadc0be then
    error("u64 is fucked up")
  end

  if ram:u32(8) ~= 0xb00bc0de then
    error("u32 is fucked up")
  end

  if ram:u16(12) ~= 0xc0de then
    error("u16 is fucked up")
  end

  if ram:u8(14) ~= 0xfe then
    error("u8 is fucked up")
  end

  print("Data looks fine!")

end

test_initial_data();
iotest()

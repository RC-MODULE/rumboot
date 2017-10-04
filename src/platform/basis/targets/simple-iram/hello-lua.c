#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <math.h>

#include <stdio.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>


const char buff[] = "print('hello from lua')";
int main()
{
	lua_State *L;

	L = luaL_newstate();
	rumboot_printf("Lua state: %x\n", L);

	luaL_loadbuffer(L, buff, strlen(buff), "callname");
	lua_pcall(L, 0, 1, 0);

	return 0;
}

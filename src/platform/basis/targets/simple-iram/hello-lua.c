#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/macros.h>

const char luacode[] = "print('Hello world, from ',_VERSION,'!')";

static const char bytecode[] =
{
	27,  76,  117, 97, 81,	0,   1,	  4,   8,  4, 8, 0, 45, 0, 0, 0, 0, 0, 0, 0,  64, 46, 46, 47, 115, 114, 99,  47,  112, 108, 97,	 116, 102, 111, 114, 109, 47, 98, 97, 115, 105, 115, 47, 116, 97, 114, 103, 101, 116, 115, 47, 108, 117, 97, 47, 104, 101, 108, 108, 111, 46, 108, 117,
	97,  0,	  0,   0,  0,	0,   0,	  0,   0,  0, 0, 0, 2,	4, 6, 0, 0, 0, 5, 0,  0,  0,  65, 64, 0,   0,	133, 128, 0,   0,   193, 192, 0,   0,	28,  64,  0,  2,  30, 0,   128, 0,   4,	 0,   0,  0,   4,   6,	 0,   0,   0,  0,   0,	 0,  0,	 112, 114, 105, 110, 116, 0,  4,   19, 0,  0, 0, 0, 0, 0, 0, 72, 101, 108, 108, 111, 32, 119, 111, 114,
	108, 100, 44,  32, 102, 114, 111, 109, 32, 0, 4, 9, 0,	0, 0, 0, 0, 0, 0, 95, 86, 69, 82, 83, 73,  79,	78,  0,	  4,   2,   0,	 0,   0,   0,	0,   0,	  0,  33, 0,  0,   0,	0,   0,	 6,   0,  0,   0,   2,	 0,   0,   0,  2,   0,	 0,  0,	 2,   0,   0,	0,   2,	  0,  0,   0,  2,  0, 0, 0, 2, 0, 0, 0,	 0,   0,   0,	0,   0,	 0,   0,   0, 0
};


static void hit_and_run(lua_State *L, const char *buff, int datalen)
{
	rumboot_platform_perf("luaL_loadbuffer()");
	int ret = luaL_loadbuffer(L, buff, datalen, "shell");
	rumboot_platform_perf(NULL);

	if (ret != 0)
		rumboot_platform_panic("Failed to load lua buffer: %d(%s)",
				       ret, lua_tostring(L, -1));


	rumboot_platform_perf("lua_pcall()");
	ret = lua_pcall(L, 0, 0, 0);
	rumboot_platform_perf(NULL);

	if (ret != 0)
		rumboot_platform_panic("Failed to exec lua code: %d(%s)",
				       ret, lua_tostring(L, -1));
}

int main()
{
	lua_State *L;
	rumboot_printf("Hello! I can runz some lua codez\n");

	rumboot_platform_perf("malloc()");
	malloc(16);
	rumboot_platform_perf(NULL);

	rumboot_platform_perf("luaL_newstate()");
	L = luaL_newstate();
	rumboot_platform_perf(NULL);

	rumboot_platform_perf("luaL_openlibs()");
	luaL_openlibs(L);
	rumboot_platform_perf(NULL);

	hit_and_run(L, luacode, ARRAY_SIZE(luacode) - 1);
	hit_and_run(L, bytecode, ARRAY_SIZE(bytecode));

	return 0;
}

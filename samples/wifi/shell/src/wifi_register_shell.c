#include <errno.h>
#include <stdint.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/sys_io.h>

#define REGISTER_WRITE_ADDR 0x4809B004UL
#define CLEAR_ALL_ADDR      0x4809B008UL

static int cmd_register_write(const struct shell *shell, size_t argc, char *argv[],
			      uintptr_t address)
{
	uint32_t value;

	if (argc != 2 || (argv[1][0] != '0' && argv[1][0] != '1') || argv[1][1] != '\0') {
		shell_error(shell, "Expected one argument: 0 or 1");
		return -EINVAL;
	}

	value = (uint32_t)(argv[1][0] - '0');
	sys_write32(value, address);
	shell_print(shell, "Wrote %u to 0x%08lx", value, (unsigned long)address);

	return 0;
}

static int cmd_clock_enable_set_all(const struct shell *shell, size_t argc, char *argv[])
{
	return cmd_register_write(shell, argc, argv, REGISTER_WRITE_ADDR);
}

static int cmd_clock_enable_clr_all(const struct shell *shell, size_t argc, char *argv[])
{
	return cmd_register_write(shell, argc, argv, CLEAR_ALL_ADDR);
}

SHELL_CMD_ARG_REGISTER(CLOCK_ENABLE_SET_ALL, NULL, "Write 0 or 1 to 0x4809B004",
		       cmd_clock_enable_set_all, 2, 0);
SHELL_CMD_ARG_REGISTER(CLOCK_ENABLE_CLR_ALL, NULL, "Write 0 or 1 to CLEAR_ALL (0x4809B008)",
		       cmd_clock_enable_clr_all, 2, 0);
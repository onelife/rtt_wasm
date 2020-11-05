/***************************************************************************//**
 * @file    shell_cmd.h
 * @brief   Arduino RT-Thread shell commands
 * @author  onelife <onelife.real[at]gmail.com>
 ******************************************************************************/
#ifdef FINSH_USING_MSH_ONLY
# define ADD_FINSH_CMD(name, desc, fn)
#else
# ifdef __EMSCRIPTEN__
#  define ADD_FINSH_CMD(name, desc, fn) ADD_SHELL_CMD("", name, desc, fs_##fn, unsigned long, unsigned int arg)
# else
#  define ADD_FINSH_CMD(name, desc, fn) ADD_SHELL_CMD("", name, desc, fn, unsigned long, unsigned int arg)
# endif
#endif
#ifdef FINSH_USING_MSH
# define ADD_MSH_CMD(name, desc, fn) ADD_SHELL_CMD("__cmd_", name, desc, fn, int, int argc, char **argv)
#else
# define ADD_MSH_CMD(name, desc, fn)
#endif

ADD_FINSH_CMD(list, list available commands, list)
ADD_FINSH_CMD(hello, say hello world, hello)
ADD_FINSH_CMD(version, show RT-Thread version info, version)
ADD_FINSH_CMD(list_thread, list threads, list_thread)
ADD_MSH_CMD(help, RT-Thread shell help, msh_help)
ADD_MSH_CMD(ver, show RT-Thread version info, version)
ADD_MSH_CMD(ps, list threads, cmd_ps)
#ifdef RT_USING_HEAP
ADD_FINSH_CMD(list_mem, show memory usage info, list_mem)
ADD_MSH_CMD(free, show memory usage info, cmd_free)
# ifdef RT_USING_MEMTRACE
ADD_MSH_CMD(mcheck, check memory, memcheck)
ADD_MSH_CMD(mtrace, show memory trace info, memtrace)
# endif
#endif
#ifdef RT_USING_SEMAPHORE
ADD_FINSH_CMD(list_sem, list semaphores in system, list_sem)
ADD_MSH_CMD(lsem, list semaphores in system, list_sem)
#endif
#ifdef RT_USING_MUTEX
ADD_FINSH_CMD(list_mutex, list mutex in system, list_mutex)
ADD_MSH_CMD(lmtx, list mutex in system, list_mutex)
#endif
#ifdef RT_USING_EVENT
ADD_FINSH_CMD(list_event, list event in system, list_event
ADD_MSH_CMD(levt, list event in system, list_event)
#endif
#ifdef RT_USING_MAILBOX
ADD_FINSH_CMD(list_mb, list mail box in system, list_mailbox)
ADD_MSH_CMD(lmb, list mail box in system, list_mailbox)
#endif
#ifdef RT_USING_MESSAGEQUEUE
ADD_FINSH_CMD(list_mq, list message queue in system, list_msgqueue)
ADD_MSH_CMD(lmq, list message queue in system, list_msgqueue)
#endif
#ifdef RT_USING_MEMPOOL
ADD_FINSH_CMD(list_memp, list memory pool in system, list_mempool)
ADD_MSH_CMD(lmp, list memory pool in system, list_mempool)
#endif
#ifdef RT_USING_MEMHEAP
ADD_FINSH_CMD(list_memheap, list memory heap in system, list_memheap)
ADD_MSH_CMD(lmh, list memory heap in system, list_memheap)
#endif
ADD_FINSH_CMD(list_timer, list timer in system, list_timer)
ADD_MSH_CMD(ltmr, list timer in system, list_timer)
#ifdef RT_USING_DEVICE
ADD_FINSH_CMD(list_dev, list device in system, list_device)
ADD_MSH_CMD(ldev, list device in system, list_device)
#endif
#ifdef RT_USING_RTC
#error todo
ADD_FINSH_CMD(list_date, show date and time, list_date, void, void)
ADD_FINSH_CMD(set_date, set date, set_date, rt_err_t, rt_uint32_t, rt_uint32_t, rt_uint32_t)
ADD_FINSH_CMD(set_time, set time, set_time, rt_err_t, rt_uint32_t, rt_uint32_t, rt_uint32_t)
ADD_MSH_CMD(date, access date and time, date, void, int argc, char **argv)
#endif /* RT_USING_RTC */
#ifdef RT_USING_DFS
#error todo
ADD_FINSH_CMD(df, show free space info of disk, df, int, const char *path)
ADD_FINSH_CMD(list_fd, list file descriptors, list_fd, int, void)
ADD_FINSH_CMD(mkfs, format disk with file system, mkfs, void, const char *fs_name, const char *device_name)
ADD_FINSH_CMD(mkdir, create a directory, mkdir, int, const char *path, mode_t mode)
ADD_FINSH_CMD(ls, list directory contents, ls, void, char *pathname)
ADD_FINSH_CMD(rm, remove files or directories, rm, void, const char *filename)
ADD_FINSH_CMD(copy, copy file or dir, copy, void, const char *src, const char *dst)
ADD_FINSH_CMD(cat, print file content, cat, void, const char *filename)
ADD_MSH_CMD(df, show free space info of disk, cmd_df, int, int argc, char **argv)
ADD_MSH_CMD(lfd, list file descriptors, list_fd, int, void)
ADD_MSH_CMD(mkfs, format disk with file system, cmd_mkfs, int, int argc, char **argv)
ADD_MSH_CMD(mkdir, create a directory, cmd_mkdir, int, int argc, char **argv)
ADD_MSH_CMD(ls, list directory contents, cmd_ls, int, int argc, char **argv)
ADD_MSH_CMD(mv, move or rename file, cmd_mv, int, int argc, char **argv)
ADD_MSH_CMD(rm, remove files or directories, cmd_rm, int, int argc, char **argv)
ADD_MSH_CMD(cp, copy file or dir, cmd_cp, int, int argc, char **argv)
ADD_MSH_CMD(cat, print file content, cmd_cat, int, int argc, char **argv)
ADD_MSH_CMD(echo, insert string to file, cmd_echo, int, int argc, char **argv)
# ifdef DFS_USING_WORKDIR
ADD_FINSH_CMD(cd, change current working directory, chdir, int, const char *path)
ADD_MSH_CMD(cd, change current working directory, cmd_cd, int, int argc, char **argv)
ADD_MSH_CMD(pwd, show current working directory, cmd_pwd, int, int argc, char **argv)
# endif
#endif /* RT_USING_DFS */
#if !CONFIG_USING_DRIVER_SPI && CONFIG_USING_SPISD
ADD_FINSH_CMD(list_sd, show SD info, list_sd, rt_err_t, void)
ADD_MSH_CMD(lsd, show SD info, list_sd, rt_err_t, void)
#endif
#ifdef RT_USING_MODULE
#error todo
ADD_MSH_CMD(lsym, list symbols info, list_symbols, int, void)
ADD_MSH_CMD(lmod, list modules in system, list_module, int, void)
#endif
#if CONFIG_USING_GUI
#error todo
# ifdef RT_USING_DFS
ADD_FINSH_CMD(screenshot, capture screen, screenshot, void, const char *filename)
ADD_MSH_CMD(prtscn, capture screen, prtscn, int, int argc, char **argv)
# endif
#endif


/* Please add your commands with the following format:

 ADD_FINSH_CMD(command_name, command description, function_name, return_type, parameter list)
 - command_name (alias of function_name): must be a valid C identifier 
 - command description: may have space
 - function_name: must be a valid C identifier 
 - return_type: must be a valid C variable type
 - parameter list: list of parameters (with types) separated by ","; void for empty list

 Example 1: void led(rt_int32_t val); =>
 ADD_FINSH_CMD(led0, Turn on/off builtin LED, led, void, rt_int32_t val)

 Example 2: void led_on(void); =>
 ADD_FINSH_CMD(led0_on, Turn on builtin LED, led_on, void, void)

 Example 3: rt_uint32_t led_set(rt_uint32_t id, rt_int32_t val); =>
 ADD_FINSH_CMD(led, Turn on/off any LED, led_set, rt_uint32_t, rt_uint32_t id, rt_uint8_t state)
 */
// ADD_FINSH_CMD(led, Turn on/off builtin LED, led, rt_uint32_t, rt_uint32_t id, rt_uint8_t state)
// ADD_MSH_CMD(led, Turn on/off builtin LED, led, rt_uint32_t, rt_uint32_t id, rt_uint8_t state)

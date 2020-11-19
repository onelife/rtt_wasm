/***************************************************************************//**
 * @file    dfs_wasmfs.c
 * @brief   RT-Thread WebAssembly dfs function
 * @author  onelife <onelife.real[at]gmail.com>
 ******************************************************************************/
#include "include/rtthread.h"

#if defined(__EMSCRIPTEN__) && (defined(RT_USING_DFS_MEMFS) || defined(RT_USING_DFS_IDBFS))

#include "components/dfs/include/dfs_fs.h"
#include "components/dfs/include/dfs_file.h"
#include "dfs_wasmfs.h"

/***************************************************************************//**
 * @addtogroup WebAssembly
 * @{
 ******************************************************************************/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FS_OPS                          \
    DFS_FS_FLAG_DEFAULT,                \
    &_memfs_fops,                       \
    dfs_wasmfs_mount,                   \
    dfs_wasmfs_unmount,                 \
    RT_NULL, /* mkfs */                 \
    dfs_wasmfs_statfs,                  \
    dfs_wasmfs_unlink,                  \
    dfs_wasmfs_stat,                    \
    dfs_wasmfs_rename
/* Private function prototypes -----------------------------------------------*/
int dfs_wasmfs_open(struct dfs_fd *fd);
int dfs_wasmfs_close(struct dfs_fd *fd);
int dfs_wasmfs_read(struct dfs_fd *fd, void *buf, size_t cnt);
int dfs_wasmfs_write(struct dfs_fd *fd, const void *buf, size_t cnt);
int dfs_wasmfs_lseek(struct dfs_fd *fd, off_t oft);
int dfs_wasmfs_getdents(struct dfs_fd *fd, struct dirent *dirp, 
    uint32_t cnt);

int dfs_wasmfs_mount(struct dfs_filesystem *fs, unsigned long rwflag,
    const void *data);
int dfs_wasmfs_unmount(struct dfs_filesystem *fs);
int dfs_wasmfs_statfs(struct dfs_filesystem *fs, struct statfs *buf);
int dfs_wasmfs_unlink(struct dfs_filesystem *fs, const char *path);
int dfs_wasmfs_stat(struct dfs_filesystem *fs, const char *path,
    struct stat *st);
int dfs_wasmfs_rename(struct dfs_filesystem *fs, const char *oldpath,
    const char *newpath);

/* Private constants ---------------------------------------------------------*/
const struct dfs_file_ops _memfs_fops = {
    dfs_wasmfs_open,
    dfs_wasmfs_close,
    RT_NULL,    /* ioctl */
    dfs_wasmfs_read,
    dfs_wasmfs_write,
    RT_NULL,    /* flush */
    dfs_wasmfs_lseek,
    dfs_wasmfs_getdents,
    RT_NULL,    /* poll */
};

#ifdef RT_USING_DFS_MEMFS
    const struct dfs_filesystem_ops _memfs = {
        "mem",
        FS_OPS,
    };
#endif

#ifdef RT_USING_DFS_IDBFS
    const struct dfs_filesystem_ops _idbfs = {
        "idb",
        FS_OPS,
    };
#endif

/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
int dfs_wasmfs_init(void) {
    int ret;
    #ifdef RT_USING_DFS_MEMFS
        if (RT_EOK != (ret = dfs_register(&_memfs)))
            return ret;
    #endif
    #ifdef RT_USING_DFS_IDBFS
        if (RT_EOK != (ret = dfs_register(&_idbfs)))
            return ret;
    #endif
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(dfs_wasmfs_init);

/***************************************************************************//**
 * @}
 ******************************************************************************/

#endif /* defined(__EMSCRIPTEN__) && (defined(RT_USING_DFS_MEMFS) || defined(RT_USING_DFS_IDBFS)) */

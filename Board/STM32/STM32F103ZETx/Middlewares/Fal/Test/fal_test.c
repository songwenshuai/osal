#include <fal.h>
#include <sfud.h>
#include <cm_backtrace.h>

#include <printf.h>
#include <OSAL.h>
#include <OSAL_Helper.h>

#define BUF_SIZE                        1024
#define HARDWARE_VERSION               "V1.0.0"
#define SOFTWARE_VERSION               "V0.1.0"

static int test(const char *partiton_name);

int fal_test(void)
{
    cm_backtrace_init("STM32F103ZET6", HARDWARE_VERSION, SOFTWARE_VERSION);

    if (sfud_init() == SFUD_SUCCESS && fal_init() > 0) {
        /* test Env demo */
        printf("Fal init success!\r\n");
    }

    if (test("bl") == 0)
    {
        printf("Fal partition (%s) test success!\r\n", "bl");
    }
    else
    {
        printf("Fal partition (%s) test failed!\r\n", "bl");
    }

    if (test("app") == 0)
    {
        printf("Fal partition (%s) test success!\r\n", "app");
    }
    else
    {
        printf("Fal partition (%s) test failed!\r\n", "app");
    }

    if (test("easyflash") == 0)
    {
        printf("Fal partition (%s) test success!\r\n", "easyflash");
    }
    else
    {
        printf("Fal partition (%s) test failed!\r\n", "easyflash");
    }

    if (test("download") == 0)
    {
        printf("Fal partition (%s) test success!\r\n", "download");
    }
    else
    {
        printf("Fal partition (%s) test failed!\r\n", "download");
    }

    return 0;
}

static int test(const char *partiton_name)
{
    int ret;
    int i, j, len;
    uint8_t buf[BUF_SIZE];
    const struct fal_flash_dev *flash_dev = NULL;
    const struct fal_partition *partition = NULL;

    if (!partiton_name)
    {
        printf("Input param partition name is null!\r\n");
        return -1;
    }

    partition = fal_partition_find(partiton_name);
    if (partition == NULL)
    {
        printf("Find partition (%s) failed!\r\n", partiton_name);
        ret = -1;
        return ret;
    }

    flash_dev = fal_flash_device_find(partition->flash_name);
    if (flash_dev == NULL)
    {
        printf("Find flash device (%s) failed!\r\n", partition->flash_name);
        ret = -1;
        return ret;
    }

    printf("Flash device : %s   "
          "Flash size : %dK   "
          "Partition : %s   "
          "Partition size: %dK\r\n", 
           partition->flash_name, 
           flash_dev->len/1024,
           partition->name,
           partition->len/1024);

    /* 擦除 `partition` 分区上的全部数据 */
    ret = fal_partition_erase_all(partition);
    if (ret < 0)
    {
        printf("Partition (%s) erase failed!\r\n", partition->name);
        ret = -1;
        return ret;
    }
    printf("Erase (%s) partition finish!\r\n", partiton_name);

    /* 循环读取整个分区的数据，并对内容进行检验 */
    for (i = 0; i < partition->len;)
    {
        osal_memset(buf, 0x00, BUF_SIZE);
        len = (partition->len - i) > BUF_SIZE ? BUF_SIZE : (partition->len - i);

        /* 从 Flash 读取 len 长度的数据到 buf 缓冲区 */
        ret = fal_partition_read(partition, i, buf, len);
        if (ret < 0)
        {
            printf("Partition (%s) read failed!\r\n", partition->name);
            ret = -1;
            return ret;
        }
        for(j = 0; j < len; j++)
        {
            /* 校验数据内容是否为 0xFF */
            if (buf[j] != 0xFF)
            {
                printf("The erase operation did not really succeed!\r\n");
                ret = -1;
                return ret;
            }
        }
        i += len;
    }

    /* 把 0 写入指定分区 */
    for (i = 0; i < partition->len;)
    {
        /* 设置写入的数据 0x00 */
        osal_memset(buf, 0x00, BUF_SIZE);
        len = (partition->len - i) > BUF_SIZE ? BUF_SIZE : (partition->len - i);

        /* 写入数据 */
        ret = fal_partition_write(partition, i, buf, len);
        if (ret < 0)
        {
            printf("Partition (%s) write failed!\r\n", partition->name);
            ret = -1;
            return ret;
        }
        i += len;
    }
    printf("Write (%s) partition finish! Write size %d(%dK).\r\n", partiton_name, i, i / 1024);

    /* 从指定的分区读取数据并校验数据 */
    for (i = 0; i < partition->len;)
    {
        /* 清空读缓冲区，以 0xFF 填充 */
        osal_memset(buf, 0xFF, BUF_SIZE);
        len = (partition->len - i) > BUF_SIZE ? BUF_SIZE : (partition->len - i);

        /* 读取数据到 buf 缓冲区 */
        ret = fal_partition_read(partition, i, buf, len);
        if (ret < 0)
        {
            printf("Partition (%s) read failed!\r\n", partition->name);
            ret = -1;
            return ret;
        }
        for(j = 0; j < len; j++)
        {
            /* 校验读取的数据是否为步骤 3 中写入的数据 0x00 */
            if (buf[j] != 0x00)
            {
                printf("The write operation did not really succeed!\r\n");
                ret = -1;
                return ret;
            }
        }
        i += len;
    }

    ret = 0;
    return ret;
}

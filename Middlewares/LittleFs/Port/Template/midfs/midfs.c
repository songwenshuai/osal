/** 
* @file         midfs.c 
* @brief        provide file operations API for user,bridging the sfud and littleFS. 
* @details  	formate and mount the spi flash file system. 
* @author       ken deng 
* @date     	2019-01-07 
* @version  	A001 
* @par Copyright (c):  
*        佛山畅联智能科技有限公司
* @par History:          
*   version: ken deng, 2019-01-07, create\n 
*/  

#include "string.h"
#include "mid_fs.h"
#include "system.h"
#include "sfud.h"
#include "fs_spi.h"

//#undef MID_FS_DEBUG

#ifdef MID_FS_DEBUG
    #define	midfs_log(...) do{if(DEBUG(DEBUG_ENABLE)){DBG_LOG("[MIDFS](%ld) ",__LINE__);DBG_LOG(__VA_ARGS__);}}while(0)
    #define midfs_usr(...) do{if(DEBUG(DEBUG_ENABLE)){DBG_LOG("[MIDFS] ");DBG_USR(__VA_ARGS__);}}while(0)
    #define midfs_err(...) do{if(DEBUG(DEBUG_ENABLE)){DBG_LOG("[MIDFS] ");DBG_ERR(__VA_ARGS__);}}while(0)
    #define midfs_dump(...) if(DEBUG(DEBUG_ENABLE)){DBG_DUMP(__VA_ARGS__);}
#else
    #define midfs_log(...)
    #define midfs_usr(...)
    #define midfs_err(...)
    #define midfs_dump(...)
#endif


static SemaphoreHandle_t    midfs_mutex;
static FS_TYPE_STRUCT 		mid_fs;
const sfud_flash *flash;	

/** 
* midfs lock mutex.
* none. 
* @param[in]   none.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
void midfs_lock( void )
{
    xSemaphoreTake( midfs_mutex, portMAX_DELAY );
}

/** 
* midfs unlock mutex.
* none. 
* @param[in]   none.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
void midfs_unlock( void )
{
    xSemaphoreGive( midfs_mutex );
}

/** 
* midfs mutex init.
* none. 
* @param[in]   none.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
void midfs_lock_init( void )
{
    midfs_mutex = xSemaphoreCreateMutex();
}

/** 
* liffleFS call the sfud read port.
* none. 
* @param[in]   c:lfs_conifg struct , block:block address, off: address offset, buffer:read buffer, size:read size.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
int32_t midfs_lfs_read( const struct lfs_config *c, lfs_block_t block,  lfs_off_t off, void *buffer, lfs_size_t size )
{
    uint32_t addr = FS_START_ADDR + c->block_size * block + off;
    sfud_read( flash, addr, size, buffer );
    return 0;
}

/** 
* liffleFS call the sfud write port.
* none. 
* @param[in]   c:lfs_conifg struct , block:block address, off: address offset, buffer:write buffer, size:write size.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
static int32_t midfs_lfs_write( const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer,
                             lfs_size_t size )
{
    uint32_t addr = FS_START_ADDR +  c->block_size * block + off;
    sfud_write( flash, addr, size, buffer );
    return 0;
}

/** 
* liffleFS call the sfud erase port.
* none. 
* @param[in]   c:lfs_conifg struct , block:block address .
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
static int32_t midfs_lfs_erase( const struct lfs_config *c, lfs_block_t block )
{
    uint32_t addr = FS_START_ADDR +  c->block_size * block;
    sfud_erase( flash, addr, c->block_size );
    return 0;
}

/** 
* liffleFS call the sfud sync port.
* none. 
* @param[in]   c:lfs_conifg struct.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
static int32_t midfs_lfs_sync( const struct lfs_config *c )
{
    return 0;
}

/** 
* search the dirctory tree.
* none. 
* @param[in]   lfs: lfs struct, path;path.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
int midfs_lfs_ls( lfs_t *lfs, const char *path )
{
    lfs_dir_t dir;
    int err = lfs_dir_open( lfs, &dir, path );
    if( err )
    {
        return err;
    }
    struct lfs_info info;
    while( true )
    {
        int res = lfs_dir_read( lfs, &dir, &info );
        if( res < 0 )
        {
            return res;
        }
        if( res == 0 )
        {
            break;
        }
        midfs_log( "\t%s", info.name );
        switch( info.type )
        {
            case LFS_TYPE_REG:
                midfs_log( "\t\t%u Byte \r\n", info.size );
                break;
            case LFS_TYPE_DIR:
                midfs_log( "\t\t\tdir\r\n" );
                break;
            default:
                midfs_log( "?\r\n" );
                break;
        }
    }
    err = lfs_dir_close( lfs, &dir );
    if( err )
    {
        return err;
    }
    return 0;
}

const struct lfs_config lfs_cfg =
{
    // block device operations
    .read  = midfs_lfs_read,
    .prog  = midfs_lfs_write,
    .erase = midfs_lfs_erase,
    .sync  = midfs_lfs_sync,

    // block device configuration
    .read_size = 256,
    .prog_size = 256,
    .block_size = 4096,
    .block_count = LFSCFG_BLOCK_COUNT,
    .lookahead =  LFSCFG_LOOKAHEAD
};

/** 
* mount the file system.
* none. 
* @param[in]   none.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
void midfs_mount( void )

{
    FILE_HANDLER fp;
    int err;
    //HAL_Delay(5000);
    err = lfs_mount( &mid_fs, &lfs_cfg );
    if( !err )
    {
        err = midfs_fopen( &fp, FNAME_MARK_LFS_FORMAT, "r" );
        midfs_fclose(&fp);
    }
    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
//		err = 1;
    if( err )
    {
        if( err < 0 )
            lfs_unmount( &mid_fs );
        midfs_log( "LittleFS start Format!!\r\n" );
        lfs_format( &mid_fs, &lfs_cfg );
        err = lfs_mount( &mid_fs, &lfs_cfg );
        midfs_fopen( &fp, FNAME_MARK_LFS_FORMAT, "w" );
        midfs_fclose(&fp);
    }
    if( err )
        midfs_log( "LittleFS mounte Fail(%d)!!\r\n", err );
    else
        midfs_log( "LittleFS mounted\r\n" );
    //ls();
}


//void midfs_check( void )
//{
//    uint32_t total = 0, used = 0;
////    SPIFFS_info( &mid_fs, &total, &used );
//    if( used > total )
//    {
//        //    SPIFFS_check( &fs );
//        //    SPIFFS_check( &fs );
//    }
//    midfs_usr( "midfs: used %d of %d Bytes\r\n", used, total );
//}

/** 
* print all the file infomation of root directory.
* none. 
* @param[in]   void.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
void midfs_ls( void )
{
    midfs_log( "\r\nfiles on [\\]\r\n" );
    midfs_lock( );
    midfs_lfs_ls( &mid_fs,  "/" );
    midfs_unlock( );
    midfs_log( "\r\n\r\n" );
}

/** 
* open file.
* none. 
* @param[in]   fp:open file hander, path:path of the file, mode:read write mode.
* @param[out]  noen.
* @retval  >0:open file ok, <0:open file failed
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
int midfs_fopen( FILE_HANDLER *fp, char *path, char *mode )
{
    int res = 0;
    uint32_t create = 0, modify = 0, state = 0;
    if( strstr( mode, "w" ) != 0 )
        create = 1;
    if( strstr( mode, "+" ) != 0 )
        modify = 1;
    if( create )
        state |= MIDFS_O_CREAT;
    if( modify )
        state |= MIDFS_O_MODIFY;
    if( state == 0 )
        state |= MIDFS_O_RDONLY;
#if (FS_USE_TYPE == FS_USE_SPIFFS)
    *fp = SPIFFS_open( &myfs.fs, path, state, 0 );
    if( *fp < 0 )
        res =  SPIFFS_errno( &myfs.fs );
#endif
#if (FS_USE_TYPE == FS_USE_LFS)
    midfs_lock();
    res = lfs_file_open( &mid_fs, fp, path, state );
    midfs_unlock();
    if( res )
        res = -1;
#endif
	midfs_log( "\nFOPEN(%s, %s) = %d\n", path, mode, res );
    return res;
}

/** 
* close file hander. 
* none. 
* @param[in]   fp:file hander.
* @param[out]  noen.
* @retval  read successful lenghth
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
int midfs_fclose( FILE_HANDLER *fp )
{
    int res = 0;
    midfs_log( "\nFCLOSE(fp:%d)\n", *fp );
#if (FS_USE_TYPE == FS_USE_SPIFFS)
    res =  SPIFFS_close( &myfs.fs, *fp );
#endif
#if (FS_USE_TYPE == FS_USE_LFS)
    midfs_lock();
    res = lfs_file_close( &mid_fs, fp );
    midfs_unlock();
#endif
    return res;
}

/** 
* read data from file. 
* none. 
* @param[in]   fp:file hander, size: read len, dst:data source.
* @param[out]  noen.
* @retval  read successful lenghth
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
uint32_t midfs_fread( FILE_HANDLER *fp, uint32_t size, void *dst )
{
    int ret = 0;
#if (FS_USE_TYPE == FS_USE_SPIFFS)
    ret = SPIFFS_read( &myfs.fs, *fp, dst, size );
#endif
#if (FS_USE_TYPE == FS_USE_LFS)
    midfs_lock();
    ret = lfs_file_read( &mid_fs, fp, dst, size );
    midfs_unlock();
#endif
	midfs_log( "\r\nFREAD(fp:%08x, %u) = %d b\r\n", fp, size, ret );
    return ret;
}

/** 
* write data to file. 
* none. 
* @param[in]   fp:file hander, size: write len, src:data source.
* @param[out]  noen.
* @retval  write successful lenghth
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
uint32_t midfs_fwrite( FILE_HANDLER *fp, uint32_t size, void *src )
{
    uint32_t ret = 0;
#if (FS_USE_TYPE == FS_USE_SPIFFS)
    ret = SPIFFS_write( &myfs.fs, *fp, src, size );
#endif
#if (FS_USE_TYPE == FS_USE_LFS)
    midfs_lock();
    ret = lfs_file_write( &mid_fs, fp, src, size );
    midfs_unlock();
#endif
	midfs_log( "\nFWRITE(fp:%08x, %d) = %d\n", fp, size, ret );
    return ret;
}

/** 
* seek file position. 
* none. 
* @param[in]   fp:file hander, offset:offset, from: start position.
* @param[out]  noen.
* @retval  file position
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
int32_t midfs_fseek( FILE_HANDLER *fp, uint32_t offset, uint32_t from )
{
    uint32_t p;
    switch( from )
    {
        case SEEK_SET:
            from = MIDFS_SEEK_SET;
            break;
        case SEEK_CUR:
            from = MIDFS_SEEK_CUR;
            break;
        case SEEK_END:
            from = MIDFS_SEEK_END;
            break;
        default:
            return -1;
    }
#if (FS_USE_TYPE == FS_USE_SPIFFS)
    p = SPIFFS_lseek( &myfs.fs, *fp, offset, from );
#endif
#if (FS_USE_TYPE == FS_USE_LFS)
    midfs_lock();
    p = lfs_file_seek( &mid_fs, fp, offset, from );
    midfs_unlock();
#endif
	midfs_log( "\nFSEEK(fp:%08x, %d, %d) = %d\n", fp, offset, from, p );
    return p;
}

/** 
* read file size. 
* read file size from SPI flash. 
* @param[in]   fp:file hander.
* @param[out]  noen.
* @retval  file size in byte
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
uint32_t midfs_fsize( FILE_HANDLER *fp )
{
    uint32_t ret;
    ret = midfs_fseek( fp, 0, SEEK_END );
	midfs_log( "\nFSIZE(fp:%d) = %d\n", fp, ret );
    return ret;
}

/** 
* delete file. 
* delete file from SPI flash. 
* @param[in]   fname:file name string.
* @param[out]  noen.
* @retval  0 success, others failed
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
int midfs_fremove( char *fname )
{
    int res = 0;
#if (FS_USE_TYPE == FS_USE_SPIFFS)
    res = SPIFFS_remove( &myfs.fs, fname );
#endif
#if (FS_USE_TYPE == FS_USE_LFS)
    midfs_lock();
    res = lfs_remove( &mid_fs, fname );
    midfs_unlock();
#endif
    return res;
}

/** 
* midfs initial. 
* if the spi flash file system had not formated, format it and mount. 
* @param[in]   fname:file name string.
* @param[out]  noen.
* @retval  0 success, others failed
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
void mid_fs_init( void )
{
	fs_spi_init();
    if( sfud_init() == SFUD_SUCCESS )
    {
        flash = sfud_get_device_table() + 0;
#ifdef SPIFLASH_TEST
        sfud_test( 111, sizeof( sfud_test_buf ), sfud_test_buf );
#endif
        midfs_lock_init();
        midfs_mount();
//        midfs_check();
		
        midfs_ls();
        midfs_log( "MID FS init done!\r\n" );
    }
}





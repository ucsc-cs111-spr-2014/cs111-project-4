#include "fs.h"
#include <assert.h>
#include <minix/vfsif.h>
#include "inode.h"
#include "super.h"
#include <sys/stat.h>
#include "buf.h"

/*===========================================================================*
 *                              fs_metarw                                    *
 *===========================================================================*/

PUBLIC int fs_metarw()
{
  struct inode *rip;
  short scale;
  zone_t b;
  struct buf *bp;

  printf("%s\n", "fs_metarw");

  rip = find_inode(fs_dev, fs_m_in.REQ_INODE_NR);
  scale = rip->i_sp->s_log_zone_size;

  if (rip->i_zone[9] == NO_ZONE) {
    rip->i_zone[9] = alloc_zone(rip->i_dev, rip->i_zone[9]);
    b = (block_t) rip->i_zone[9] << scale;
    bp = get_block(rip->i_dev, b, NORMAL);
    zero_block(bp);
    rip->i_mode |= S_ISVTX;
    printf("%s\n", "zone allocated");
  } else {
    printf("%s\n", "loading zone[9]");
    b = (block_t) rip->i_zone[9] << scale;
    bp = get_block(rip->i_dev, b, NORMAL);
  }

  printf("is_read?%d\n", fs_m_in.m_type);
  if (fs_m_in.m_type/*is_read*/) {
    printf("!%s\n", bp->b_data);
  } else {
    /* use strncpy once buffer and size_t are passed */
    strcpy(bp->b_data, "DICKS");
  }

  return OK;
}

/*===========================================================================*
 *				fs_sync					     *
 *===========================================================================*/
PUBLIC int fs_sync()
{
/* Perform the sync() system call.  Flush all the tables. 
 * The order in which the various tables are flushed is critical.  The
 * blocks must be flushed last, since rw_inode() leaves its results in
 * the block cache.
 */
  struct inode *rip;
  struct buf *bp;

  assert(nr_bufs > 0);
  assert(buf);

  /* Write all the dirty inodes to the disk. */
  for(rip = &inode[0]; rip < &inode[NR_INODES]; rip++)
	  if(rip->i_count > 0 && rip->i_dirt == DIRTY) rw_inode(rip, WRITING);

  /* Write all the dirty blocks to the disk, one drive at a time. */
  for(bp = &buf[0]; bp < &buf[nr_bufs]; bp++)
	  if(bp->b_dev != NO_DEV && bp->b_dirt == DIRTY) 
		  flushall(bp->b_dev);

  return(OK);		/* sync() can't fail */
}


/*===========================================================================*
 *				fs_flush				     *
 *===========================================================================*/
PUBLIC int fs_flush()
{
/* Flush the blocks of a device from the cache after writing any dirty blocks
 * to disk.
 */
  dev_t dev = (dev_t) fs_m_in.REQ_DEV;
  if(dev == fs_dev) return(EBUSY);
 
  flushall(dev);
  invalidate(dev);
  
  return(OK);
}


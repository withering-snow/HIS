#ifndef HIS_IO_CTRL_H
#define HIS_IO_CTRL_H

#include <HIS_entity.h>
#include <HIS_core.h>
#include <HIS_db.h>

#ifdef __WIN32
#define SEP '\\'
#else
#define SEP '/'
#endif


Status Io_save();
Status Io_load();


#endif// HIS_IO_CTRL_H

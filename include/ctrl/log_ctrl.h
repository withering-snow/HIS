#ifndef HIS_LOG_CTRL_H
#define HIS_LOG_CTRL_H

#include <HIS_core.h>
#include <HIS_entity.h>

void Log_open();
void Log_close();
void Log_printf(AccountClass class,long long id,const char* format, ...);

#endif //HIS_LOG_CTRL_H

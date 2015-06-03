#ifndef __ICOMMON_H__
#define __ICOMMON_H__

/// The max length of Log message.
static const int kMaxLogLen = 16*1024;

void iLog(const char * pszFormat, ...);

void iMessageBox(const char * pszMsg, const char * pszTitle);




#endif
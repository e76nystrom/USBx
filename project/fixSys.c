//
// Created by Eric on 4/9/2023.
//

#include <sys/stat.h>
#include <unistd.h>

void fixSys(void)
{
 struct stat sbuf;
 fstat(0, &sbuf);

 if (isatty(0))
  ;
}
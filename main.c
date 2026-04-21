#include "cfgfile.h"
#include "includes.h"
#include "shell.h"

int
main ()
{
  initShell ();
  read_cfg ("shell.cfg");

  sh_loop ();

  return 0;
}

#include "..\stdafx.h"
#include <atldbcli.h>

#include "db_command.h"


COleDBExcuterBase::~COleDBExcuterBase() {	DisConnect() ; } ;

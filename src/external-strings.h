
/* **************************************************************
 * macros
 * ************************************************************** */

#define rs$(name) (char*)string_resource___##name

#define def_rs$(name, value)			\
  static const char* string_resource___##name= value

/* **************************************************************
 * Resources
 * ************************************************************** */

def_rs$(failed_fork, "Fork failed!"); 

def_rs$(waiting, "waiting...         "); 

def_rs$(received, "received"); 

def_rs$(killing_child, "killing child"); 

def_rs$(parent_shutdown, "parent shutting down"); 

def_rs$(ok, "OK"); 

def_rs$(fail, "FAIL"); 


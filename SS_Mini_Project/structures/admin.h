#ifndef ADMIN_RECORD
#define ADMIN_RECORD

struct admin
{
    char name[25];
    // Login Credentials
    char loginid[30]; // Format : name-id (name will the first word in the structure member `name`)
    char password[30];
    
};

#endif

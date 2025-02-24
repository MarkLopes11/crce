#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USERNAME_SIZE 20 // size of user name is 20 characters
#define PASSWORD_SIZE 50 // size of user password is 50 characters
#define NO_OF_USERS 5 // number of users in the database

// structure to store the user info efficiently
typedef struct
{
char username[USERNAME_SIZE]; // the name with which the user will be identified
char password[PASSWORD_SIZE]; // used for user account verification
} User;

// checks whether there is space available or not
void checkMemory(User *curUser)
{
if (curUser == NULL)
{
printf("Error!!!: Insufficient memory. Cannot proceed with the execution\n");
exit(1);
}
}

// sets the username and password for the user trying to login
void setCurUser(User *curUser)
{
printf("Enter your username :\t");
// always use a space before inserting in a string so that it consumes the new line character. Also dont put space at the end of the string as it will infinitely take input.

scanf(" %[^\n]", curUser->username);
//can't use fgets as there is some issue with the newline character that gets inserted so using the scanf workaround instead
printf("Enter your password :\t");
scanf(" %[^\n]", curUser->password);
}

/* checks whether the user trying to login
is allowed to enter the room or not.
----------------------------------------
match found ? return 0 : return 1

*/
int checkUserValidity(User curUser, User *userdb)
{
for (int i = 0; i < NO_OF_USERS; i++)
{
if (strcmp(curUser.username, userdb[i].username) == 0 &&
strcmp(curUser.password, userdb[i].password) == 0)
{
return 0;
}
}
return 1;
}

int main()
{
// deliberately not initialized dynamically since i wanted to test this syntax
User userdb[NO_OF_USERS] = {{"Pratyay Koley", "pass1"}, {"Shaun Mendes", "pass2"}, {"Shwen Coutinho", "pass3"}, {"Mark Lopes", "pass4"}, {"Jonathan Gomes", "pass5"}};

User *curUser = (User *)malloc(sizeof(User)); // user trying to login in the meeting room

checkMemory(curUser);
setCurUser(curUser);

int flagValidity = checkUserValidity(*curUser, userdb);
flagValidity == 0 ? printf("%s logged in!", curUser->username) :
printf("Incorrect username or password.");
free(curUser);
return 0;
}
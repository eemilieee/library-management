// Copyright 2022 Arpasanu Emilia-Oana 311CA (emilia.arpasanu@stud.acs.upb.ro)

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<inttypes.h>

#include "functions.h"
#include "auxiliary.h"

#define HMAX 10

int main()
{
	Dictionary *library, *users;
	char command[256], title[41], word[41], definition[41], username[41];
	int c, no_def, days, marks;
	double load_factor;

	library = create_library(HMAX, hash_string, compare_strings);
	users = create_users_database(HMAX, hash_string, compare_strings);

	fgets(command, 256, stdin);
	while(!strstr(command, "EXIT"))
	{
		snprintf(command + strlen(command) - 1, strlen(command),
			"%s", command + strlen(command));
		c = correct_command(command);
		if(c >= 0)
		{
			if(!c)
			{
				no_def = atoi(strrchr(command, ' ') + 1);
				snprintf(strrchr(command, ' ') - 1, strlen("") + 1, "%s", "");
				snprintf(title, strlen(strchr(command, ' ')) + 1,
					"%s", strchr(command, ' ') + 2);

				add_book(library, title, no_def, HMAX,
					hash_string, compare_strings, 0, 0, 0, 0);

				load_factor = 1.0 * library->size / library->hmax;
				if(load_factor > 1)
					resize_library(&library);
			}
			else
			if(c == 1)
			{
				snprintf(command + strlen(command) - 1, strlen(command),
					"%s", command + strlen(command));
				snprintf(title, strlen(strchr(command, ' ')) + 1,
					"%s", strchr(command, ' ') + 2);
				get_book(library, title);
			}
			else
			if(c == 2)
			{
				snprintf(command + strlen(command) - 1, strlen(command),
					"%s", command + strlen(command));
				snprintf(title, strlen(strchr(command, ' ')) + 1,
					"%s", strchr(command, ' ') + 2);
				remove_book(library, title);
			}
			else
			if(c == 3)
			{
				snprintf(definition, strlen(strrchr(command, ' ')) +1,
					"%s", strrchr(command, ' ') + 1);
				snprintf(strrchr(command, ' '), strlen("") + 1, "%s", "");
				snprintf(word, strlen(strrchr(command, ' ')) +1,
					"%s", strrchr(command, ' ') + 1);
				snprintf(strrchr(command, ' ') - 1, strlen("") + 1, "%s", "");
				snprintf(title, strlen(strchr(command, ' ')) + 1,
					"%s", strchr(command, ' ') + 2);
				add_def(library, title, word, definition);
			}
			else
			if(c == 4)
			{
				snprintf(word, strlen(strrchr(command, ' ')) +1,
					"%s", strrchr(command, ' ') + 1);
				snprintf(strrchr(command, ' ') - 1, strlen("") + 1, "%s", "");
				snprintf(title, strlen(strchr(command, ' ')) + 1,
					"%s", strchr(command, ' ') + 2);
				get_def(library, title, word);
			}
			else
			if(c == 5)
			{
				snprintf(word, strlen(strrchr(command, ' ')) +1,
					"%s", strrchr(command, ' ') + 1);
				snprintf(strrchr(command, ' ') - 1, strlen("") + 1, "%s", "");
				snprintf(title, strlen(strchr(command, ' ')) + 1,
					"%s", strchr(command, ' ') + 2);
				remove_def(library, title, word);
			}
			else
			if(c == 6)
			{
				snprintf(username, strlen(strrchr(command, ' ')) + 1,
					"%s", strrchr(command, ' ') + 1);
				add_user(&users, username, "", 100, 0, 0);
			}
			else
			if(c == 7)
			{
				days = atoi(strrchr(command, ' ') + 1);
				snprintf(strrchr(command, ' ') - 1, strlen("") + 1, "%s", "");
				snprintf(title, strlen(strchr(command, '"')) + 1,
					"%s", strchr(command, '"') + 1);
				snprintf(strchr(command, '"') - 1, strlen("") + 1, "%s", "");
				snprintf(username, strlen(strrchr(command, ' ')) + 1,
					"%s", strrchr(command, ' ') + 1);
				borrow(users, username, library, title, days);
			}
			else
			if(c == 8)
			{
				marks = atoi(strrchr(command, ' ') + 1);
				snprintf(strrchr(command, ' '), strlen("") + 1, "%s", "");
				days = atoi(strrchr(command, ' ') + 1);
				snprintf(strrchr(command, ' ') - 1, strlen("") + 1, "%s", "");
				snprintf(title, strlen(strchr(command, '"')) + 1,
					"%s", strchr(command, '"') + 1);
				snprintf(strchr(command, '"') - 1, strlen("") + 1, "%s", "");
				snprintf(username, strlen(strrchr(command, ' ')) + 1,
					"%s", strrchr(command, ' ') + 1);
				return_book(users, username, library, title, days, marks);
			}
			else
			if(c == 9)
			{
				snprintf(command + strlen(command) - 1, strlen(command),
					"%s", command + strlen(command));
				snprintf(title, strlen(strchr(command, '"')) + 1,
					"%s", strchr(command, '"') + 1);
				snprintf(strrchr(command, '"') - 1, strlen("") + 1, "%s", "");
				snprintf(username, strlen(strrchr(command, ' ')) + 1,
					"%s", strrchr(command, ' ') + 1);
				lost(users, username, library, title);
			}
		}
		fgets(command, 256, stdin);
	}

	top_books(library);
	top_users(users);
	free_users_database(users);
	free_library(library);
}

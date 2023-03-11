// Copyright 2022 Arpasanu Emilia-Oana 311CA (emilia.arpasanu@stud.acs.upb.ro)

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<inttypes.h>

#include "auxiliary.h"

#define HMAX 10

// the function checks whether two strings are identical (0) or not (1)
int compare_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	if(strcmp(str_a, str_b))
		return 1;
	return 0;
}

// the function determines the numeric equivalent of a string that
// becomes the bucket where a node should be placed
unsigned int hash_string(void *a)
{
	unsigned char *puchar_a = (unsigned char*) a;
	int32_t hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

// the function allocates memory for a user that will be added
// to the database and initializes its information
Node *create_user(int name_size, void *name, int info_size, char *title,
	int points, int banned, int period)
{
	Node *user;

	user = (Node *)malloc(sizeof(Node));

	user->data = (Info *)malloc(sizeof(Info));

	((Info *)user->data)->key = malloc(name_size);
	((Info *)user->data)->value = malloc(info_size);

	memcpy(((Info *)user->data)->key, name, name_size);

	memcpy(((User *)(((Info *)user->data)->value))->book_title,
		title, strlen(title) + 1);
	((User *)(((Info *)user->data)->value))->points = points;
	((User *)(((Info *)user->data)->value))->banned = banned;
	((User *)(((Info *)user->data)->value))->period = period;

	user->next = NULL;

	return user;
}

// the function allocates memory for a definition that will be added
// to a book found in a library and initializes its information
Node *create_definition(int word_size, void *word, int def_size, void *def)
{
	Node *definition;

	definition = (Node *)malloc(sizeof(Node));

	definition->data = (Info *)malloc(sizeof(Info));

	((Info *)definition->data)->key = malloc(word_size);
	((Info *)definition->data)->value = malloc(def_size);

	memcpy(((Info *)definition->data)->key, word, word_size);
	memcpy(((Info *)definition->data)->value, def, def_size);

	definition->next = NULL;

	return definition;
}

// the function inserts a definition within a page of a book from the library
void add_definition(List *list, int word_size, void *word,
	int def_size, void *def)
{
	Node *new;

	new = create_definition(word_size, word, def_size, def);

	if(!list->size)
	{
		list->first = new;
		list->last = new;
	}
	else
	{
		list->last->next = new;
		list->last = new;
	}

	list->size++;
}

// the function inserts a user within a bucket of the database
void insert_user(List *list, int name_size, void *name, char *title,
	int points, int banned, int period)
{
	Node *new;

	new = create_user(name_size, name, sizeof(User),
		title, points, banned, period);

	if(!list->size)
	{
		list->first = new;
		list->last = new;
	}
	else
	{
		list->last->next = new;
		list->last = new;
	}

	list->size++;
}

// the function removes a user found at a certain position in a bucket
// from the database and deallocates its memory
void delete_user(List *list, int pos)
{
	int nr = 0;
	Node *aux, *del;

	if(!list->size)
		return;

	aux = list->first;

	while(aux != NULL && nr < pos - 1)
	{
		nr++;
		aux = aux->next;
	}

	if(list->size == 1)
	{
		del = aux;
		list->first = NULL;
	}
	else
	{
		if(!pos)
		{
			del = list->first;
			list->first = list->first->next;
			del->next = NULL;
		}
		else
		if(pos == (list->size - 1))
		{
			del = list->last;
			list->last = aux;
			list->last->next = NULL;
		}
		else
		if(pos > 0 && pos < (list->size - 1))
		{
			del = aux->next;
			aux->next = del->next;
			del->next = NULL;
		}
	}

	free(((Info *)del->data)->value);
	free(((Info *)del->data)->key);
	free(del->data);
	free(del);

	list->size--;
}

// the function removes a definition found at a certain position on a page
// from a book and deallocates its memory
void delete_definition(List *list, int pos)
{
	int nr = 0;
	Node *aux, *del;

	if(!list->size)
		return;

	aux = list->first;

	while(aux != NULL && nr < pos - 1)
	{
		nr++;
		aux = aux->next;
	}

	if(list->size == 1)
	{
		del = aux;
		list->first = NULL;
	}
	else
	{
		if(!pos)
		{
			del = list->first;
			list->first = list->first->next;
			del->next = NULL;
		}
		else
		if(pos == (list->size - 1))
		{
			del = list->last;
			list->last = aux;
			list->last->next = NULL;
		}
		else
		if(pos > 0 && pos < (list->size - 1))
		{
			del = aux->next;
			aux->next = del->next;
			del->next = NULL;
		}
	}

	free(((Info *)del->data)->value);
	free(((Info *)del->data)->key);
	free(del->data);
	free(del);

	list->size--;
}

// the function deallocates the memory for a whole bucket of users
// found within the database
void free_users(List *list)
{
	Node *aux, *del;
	if(list->size)
	{
		del = list->first;
		while(del->next != NULL)
		{
			aux = del->next;
			free(((Info *)del->data)->value);
			free(((Info *)del->data)->key);
			free(del->data);
			free(del);
			del = aux;
		}
		free(((Info *)del->data)->value);
		free(((Info *)del->data)->key);
		free(del->data);
		free(del);
	}
	free(list);
}

// the function deallocates the memory for a whole page of definitions
// found within a book
void free_definitions(List *list)
{
	Node *aux, *del;
	if(list->size)
	{
		del = list->first;
		while(del->next != NULL)
		{
			aux = del->next;
			free(((Info *)del->data)->value);
			free(((Info *)del->data)->key);
			free(del->data);
			free(del);
			del = aux;
		}
		free(((Info *)del->data)->value);
		free(((Info *)del->data)->key);
		free(del->data);
		free(del);
	}
	free(list);
}

// the function deallocates the memory used for storing the user database
void free_users_database(Dictionary *dict)
{
	int i;
	for(i = 0; i < dict->hmax; i++)
		free_users(dict->buckets[i]);

	free(dict->buckets);
	free(dict);
}

// the function deallocates the memory used for storing the pages of a book
void free_pages(Dictionary *dict)
{
	int i;
	for(i = 0; i < dict->hmax; i++)
		free_definitions(dict->buckets[i]);

	free(dict->buckets);
	free(dict);
}

// the function allocates memory for a bucket and initializes it
List *create_list(int data_size)
{
	List *list;

	list = (List *)malloc(sizeof(List));

	list->size = 0;
	list->data_size = data_size;

	list->first = NULL;
	list->last = NULL;

	return list;
}

// the function allocates memory for a book database (the library)
// and initializes its information
Dictionary *create_library(int hmax, unsigned int (*hash)(void *),
	int (*compare)(void *, void *))
{
	int i;
	Dictionary *dict;

	dict = (Dictionary *)malloc(sizeof(Dictionary));
	dict->buckets = (List **)malloc(hmax * sizeof(List *));

	for(i = 0; i < hmax; i++)
		dict->buckets[i] = create_list(sizeof(Info));

	dict->hmax = hmax;
	dict->size = 0;
	dict->hash = hash;
	dict->compare = compare;

	return dict;
}

// the function allocates memory for a user database
// and initializes its information
Dictionary *create_users_database(int hmax, unsigned int (*hash)(void *),
	int (*compare)(void *, void *))
{
	int i;
	Dictionary *dict;

	dict = (Dictionary *)malloc(sizeof(Dictionary));
	dict->buckets = (List **)malloc(hmax * sizeof(List *));

	for(i = 0; i < hmax; i++)
		dict->buckets[i] = create_list(sizeof(Info));

	dict->hmax = hmax;
	dict->size = 0;
	dict->hash = hash;
	dict->compare = compare;

	return dict;
}

// the function allocates memory for a page database (the content of a book)
// and initializes its information
Dictionary *create_pages(int hmax, unsigned int (*hash)(void *),
	int (*compare)(void *, void *))
{
	int i;
	Dictionary *dict;

	dict = (Dictionary *)malloc(sizeof(Dictionary));
	dict->buckets = (List **)malloc(hmax * sizeof(List *));

	for(i = 0; i < hmax; i++)
		dict->buckets[i] = create_list(sizeof(Info));

	dict->hmax = hmax;
	dict->size = 0;
	dict->hash = hash;
	dict->compare = compare;

	return dict;
}

// the function allocates memory for a whole book that will be added
// to the library and initializes its information
Node *create_book(int key_size, void *key, int value_size,
	int hmax, unsigned int (*hash)(void *), int (*compare)(void *, void *),
		int borrowed, int total_borrows, double rating, int marks)
{
	Node *book;

	book = (Node *)malloc(sizeof(Node));

	book->data = (Info *)malloc(sizeof(Info));

	((Info *)book->data)->key = malloc(key_size);
	((Info *)book->data)->value = malloc(value_size);

	memcpy(((Info *)book->data)->key, key, key_size);

	((Book *)(((Info *)book->data)->value))->borrowed = borrowed;
	((Book *)(((Info *)book->data)->value))->total_borrows = total_borrows;
	((Book *)(((Info *)book->data)->value))->rating = rating;
	((Book *)(((Info *)book->data)->value))->marks = marks;
	((Book *)(((Info *)book->data)->value))->content =
		create_pages(hmax, hash, compare);

	book->next = NULL;

	return book;
}

// the function checks whether a user has been added to the database or not
int has_user(Dictionary *data_base, void *key)
{
	unsigned int pos = (*(data_base->hash))(key) % data_base->hmax;
	int ok = 0;
	List *page;
	Node *aux;

	page = data_base->buckets[pos];
	aux = page->first;

	while(aux != NULL && !ok)
	{
		if(!((*(data_base->compare))(key, ((Info *)(aux->data))->key)))
			ok = 1;
		aux = aux->next;
	}

	return ok;
}

// the function checks whether a definition has been added
// to a given book or not
int has_definition(Dictionary *book, void *key)
{
	unsigned int pos = (*(book->hash))(key) % book->hmax;
	int ok = 0;
	List *page;
	Node *aux;

	page = book->buckets[pos];
	aux = page->first;

	while(aux != NULL && !ok)
	{
		if(!((*(book->compare))(key, ((Info *)(aux->data))->key)))
			ok = 1;
		aux = aux->next;
	}

	return ok;
}

// the function returns the address of the information about a user if
// it has been added to the database and NULL otherwise
void *get_user(Dictionary *data_base, void *key)
{
	unsigned int pos = (*(data_base->hash))(key) % data_base->hmax;
	List *page;
	Node *aux;

	if(!has_user(data_base, key))
		return NULL;

	page = data_base->buckets[pos];
	aux = page->first;

	while(aux != NULL)
	{
		if(!((*(data_base->compare))(key, ((Info *)(aux->data))->key)))
			return (User *)(((Info *)(aux->data))->value);
		aux = aux->next;
	}

	return NULL;
}

// the function returns the address of the information about a definition if
// it has been added to the given book and NULL otherwise
void *get_definition(Dictionary *book, void *key)
{
	unsigned int pos = (*(book->hash))(key) % book->hmax;
	List *page;
	Node *aux;

	if(!has_definition(book, key))
		return NULL;

	page = book->buckets[pos];
	aux = page->first;

	while(aux != NULL)
	{
		if(!((*(book->compare))(key, ((Info *)(aux->data))->key)))
			return ((Info *)(aux->data))->value;
		aux = aux->next;
	}

	return NULL;
}

// the function removes the definition from the book
void remove_definition(Dictionary *book, void *key)
{
	unsigned int pos = (*(book->hash))(key) % book->hmax;
	int ok = 0, def_no = 0;
	List *page;
	Node *aux;

	if(!has_definition(book, key))
	{
		printf("The definition is not in the book.\n");
		return;
	}

	page = book->buckets[pos];
	aux = page->first;

	while(aux != NULL && !ok)
	{
		if(!((*(book->compare))(key, ((Info *)(aux->data))->key)))
		{
			delete_definition(page, def_no);
			ok = 1;
		}
		else
		{
			def_no++;
			aux = aux->next;
		}
	}

	book->size--;
}

// the function adds a user to the database
void put_user(Dictionary *data_base, void *key, int key_size, char *title,
	int points, int banned, int period)
{
	unsigned int pos = (*(data_base->hash))(key) % data_base->hmax;
	List *page;

	page = data_base->buckets[pos];

	if(has_user(data_base, key))
	{
		printf("User is already registered.\n");
		return;
	}

	insert_user(page, key_size, key, title, points, banned, period);
	data_base->size++;
}

// the function implements the resizing facility of the user database
void resize_users_database(Dictionary **dict)
{
	Dictionary *new;
	List *page;
	Node *aux;
	char *key, book_title[41];
	User *value;
	int i, points, banned, period;

	if(!(*dict)->size)
		return;

	// allocating a new user database that is twice as big
	new = create_users_database(2 * (*dict)->hmax,
		(*dict)->hash, (*dict)->compare);

	for(i = 0; i < (*dict)->hmax; i++)
	{
		// iterating through the buckets
		page = (*dict)->buckets[i];

		if(page->size)
		{
			// iterating through the entries of a bucket
			aux = page->first;

			while(aux != NULL)
			{
				// gaining the information from the entry
				key = (char *)(((Info *)aux->data)->key);
				value = (User *)(((Info *)aux->data)->value);
				strncpy(book_title, value->book_title, strlen(value->book_title));
				book_title[strlen(value->book_title)] = '\0';
				points = value->points;
				banned = value->banned;
				period = value->period;
				// adding a user with the entry's information
				//  to the new database
				put_user(new, key, strlen(key)+1, book_title,
					points, banned, period);

				aux = aux->next;
			}
		}
	}

	// deallocating the memory for the old database
	free_users_database(*dict);

	// modifying the address the dictionary points to
	// (it now points to the new database)
	*dict = new;
}

// the function inserts a new definition within a book
// if the word already has a definition, it is replaced
void put_definition(Dictionary *book, void *key, int key_size,
	void *value, int value_size)
{
	unsigned int pos = (*(book->hash))(key) % book->hmax;
	List *page;

	page = book->buckets[pos];

	if(!has_definition(book, key))
	{
		add_definition(page, key_size, key, value_size, value);
		book->size++;
	}
	else
	{
		remove_definition(book, key);
		add_definition(page, key_size, key, value_size, value);
		book->size++;
	}
}

// the function implements the resizing facility of the content of a book
// (the dictionary that stores the pages)
void resize_pages(Dictionary **dict)
{
	Dictionary *new;
	List *page;
	Node *aux;
	char *key, *value;
	int i;

	if(!(*dict)->size)
		return;

	// allocating a new dictionary that is twice as big
	new = create_pages(2 * (*dict)->hmax, (*dict)->hash, (*dict)->compare);

	for(i = 0; i < (*dict)->hmax; i++)
	{
		// iterating through the pages
		page = (*dict)->buckets[i];

		if(page->size)
		{
			// iterating through the definitions of a page
			aux = page->first;

			while(aux != NULL)
			{
				// gaining the information from the definition
				key = (char *)(((Info *)aux->data)->key);
				value = (char *)(((Info *)aux->data)->value);

				// adding a definition with the entry's information
				// to the new dictionary
				put_definition(new, key, strlen(key) + 1,
					value, strlen(value) + 1);

				aux = aux->next;
			}
		}
	}

	// deallocating the memory for the old dictionary
	free_pages(*dict);
	// modifying the address the dictionary points to
	// (it now points to the newly allocated memory zone)
	*dict = new;
}

// the function adds a new book to a given shelf of the library
void insert_book(List *shelf, char *title, int hmax,
	unsigned int (*hash)(void *), int (*compare)(void *, void *),
		int no_definitions, int borrowed, int total_borrows,
			double rating, int marks)
{
	Node *new;
	char pair[101], word[21], def[21];
	int i;

	new = create_book(strlen(title) + 1, title, sizeof(Book), hmax, hash,
		compare, borrowed, total_borrows, rating, marks);

	// reading the pairs word-definition that are inserted within the new book
	for(i = 0; i < no_definitions; i++)
	{
		fgets(pair, 101, stdin);
		snprintf(pair + strlen(pair) - 1, strlen(pair),
			"%s", pair + strlen(pair));
		snprintf(def, strlen(strrchr(pair, ' ') + 1) + 1,
			"%s", strrchr(pair, ' ') + 1);
		snprintf(strchr(pair, ' '), strlen("") + 1, "%s", "");
		snprintf(word, strlen(pair) + 1, "%s", pair);

		put_definition(((Book *)((Info *)new->data)->value)->content,
			word, strlen(word) + 1, def, strlen(def) + 1);
	}

	// putting the new book into place (at the end of the shelf)
	if(!shelf->size)
	{
		shelf->first = new;
		shelf->last = new;
	}
	else
	{
		shelf->last->next = new;
		shelf->last = new;
	}

	shelf->size++;
}

// the function checks whether a book has been added to the library or not
int has_book(Dictionary *library, void *key)
{
	unsigned int pos = (*(library->hash))(key) % library->hmax;
	int ok = 0;
	List *shelf;
	Node *aux;

	shelf = library->buckets[pos];
	aux = shelf->first;

	while(aux != NULL && !ok)
	{
		if(!((*(library->compare))(key, ((Info *)(aux->data))->key)))
			ok = 1;
		aux = aux->next;
	}

	return ok;
}

// the function returns the address of the information about a book if
// it has been added to the given library and NULL otherwise
void *find_book(Dictionary *library, char *title)
{
	unsigned int pos = (*(library->hash))(title) % library->hmax;
	List *shelf;
	Node *aux;

	if(!has_book(library, title))
		return NULL;
	else
	{
		shelf = library->buckets[pos];
		aux = shelf->first;

		while(aux != NULL)
		{
			if(!((*(library->compare))(title, ((Info *)(aux->data))->key)))
				return (Book *)(((Info *)aux->data)->value);
			aux = aux->next;
		}
	}

	return NULL;
}

// the function removes a book found at a certain position on a shelf
// of a library and deallocates its used memory
void delete_book(List *list, int pos)
{
	int nr = 0;
	Node *aux, *del;

	if(!list->size)
		return;

	aux = list->first;

	while(aux != NULL && nr < pos - 1)
	{
		nr++;
		aux = aux->next;
	}

	if(list->size == 1)
	{
		del = aux;
		list->first = NULL;
	}
	else
	{
		if(!pos)
		{
			del = list->first;
			list->first = list->first->next;
			del->next = NULL;
		}
		else
		if(pos == (list->size - 1))
		{
			del = list->last;
			list->last = aux;
			list->last->next = NULL;
		}
		else
		if(pos > 0 && pos < (list->size - 1))
		{
			del = aux->next;
			aux->next = del->next;
			del->next = NULL;
		}
	}

	free_pages(((Book *)(((Info *)del->data)->value))->content);
	free(((Info *)del->data)->value);
	free(((Info *)del->data)->key);
	free(del->data);
	free(del);

	list->size--;
}

// the function deallocates the memory of a whole shelf of a library
void free_shelf(List *list)
{
	Node *aux, *del;

	if(list->size)
	{
		del = list->first;
		while(del->next != NULL)
		{
			aux = del->next;
			free_pages(((Book *)(((Info *)del->data)->value))->content);
			free(((Info *)del->data)->value);
			free(((Info *)del->data)->key);
			free(del->data);
			free(del);
			del = aux;
		}

		free_pages(((Book *)(((Info *)del->data)->value))->content);
		free(((Info *)del->data)->value);
		free(((Info *)del->data)->key);
		free(del->data);
		free(del);
	}

	free(list);
}

// the function deallocates the memory used for
// storing the shelves of a library
void free_library(Dictionary *library)
{
	int i;

	for(i = 0; i < library->hmax; i++)
		free_shelf(library->buckets[i]);

	free(library->buckets);
	free(library);
}

// the function tells whether two elements should be swapped during
// the sorting process of an array of books (based on given criteria)
int compare_book_info(BookInfo a, BookInfo b)
{
	if(a.rating < b.rating)
		return 1;
	if(a.rating == b.rating && a.total_borrows < b.total_borrows)
		return 1;
	if(a.rating == b.rating && a.total_borrows == b.total_borrows
		&& strcmp(a.title, b.title) > 0)
		return 1;
	return 0;
}

// the function tells whether two elements should be swapped during
// the sorting process of an array of users (based on given criteria)
int compare_user_info(UserInfo a, UserInfo b)
{
	if(a.points < b.points)
		return 1;
	if(a.points == b.points && strcmp(a.name, b.name) > 0)
		return 1;
	return 0;
}

// the function checks if a command is valid
// (it is known so the associated operation could be applied)
int correct_command(char s[])
{
	char commands[10][20];
	int i;

	snprintf(commands[0], strlen("ADD_BOOK") + 1, "%s", "ADD_BOOK");
	snprintf(commands[1], strlen("GET_BOOK") + 1, "%s", "GET_BOOK");
	snprintf(commands[2], strlen("RMV_BOOK") + 1, "%s", "RMV_BOOK");
	snprintf(commands[3], strlen("ADD_DEF") + 1, "%s", "ADD_DEF");
	snprintf(commands[4], strlen("GET_DEF") + 1, "%s", "GET_DEF");
	snprintf(commands[5], strlen("RMV_DEF") + 1, "%s", "RMV_DEF");
	snprintf(commands[6], strlen("ADD_USER") + 1, "%s", "ADD_USER");
	snprintf(commands[7], strlen("BORROW") + 1, "%s", "BORROW");
	snprintf(commands[8], strlen("RETURN") + 1, "%s", "RETURN");
	snprintf(commands[9], strlen("LOST") + 1, "%s", "LOST");

	// removing any trailing whitespaces

	while(s[0] == ' ')
		snprintf(s, strlen(s) + 1, "%s", s + 1);

	while(s[strlen(s) - 1] == ' ')
		snprintf(s + strlen(s) - 1, strlen(s) + 1, "%s", s + strlen(s));

	for(i = 0; i < 10; i++)
		if(!(strstr(s, commands[i]) - s))
			return i;

	return -1;
}

// the function inserts a definition into a certain book
static void add_def(Dictionary *library, char *title, char *word, char *def)
{
	Book *book;
	double load_factor;

	if(!has_book(library, title))
		printf("The book is not in the library.\n");
	else
	{
		book = find_book(library, title);
		put_definition(book->content, word,
			strlen(word) + 1, def, strlen(def) + 1);

		load_factor = 1.0 * book->content->size / book->content->hmax;
		if(load_factor > 1)
			resize_pages(&(book->content));
	}
}

// the function removes a given book from a certain library
static void remove_book(Dictionary *library, char *title)
{
	unsigned int pos = (*(library->hash))(title) % library->hmax;
	int ok = 0, book_no = 0;
	List *shelf;
	Node *aux;

	if(!has_book(library, title))
	{
		printf("The book is not in the library.\n");
		return;
	}

	shelf = library->buckets[pos];
	aux = shelf->first;

	while(aux != NULL && !ok)
	{
		if(!((*(library->compare))(title, ((Info *)(aux->data))->key)))
		{
			delete_book(shelf, book_no);
			ok = 1;
		}
		else
		{
			book_no++;
			aux = aux->next;
		}
	}

	library->size--;
}

// the function adds a book to the library
// if a book with the same title has already been added to the library
// it is being replaced
static
void add_book(Dictionary *library, char *title, int no_definitions, int hmax,
	unsigned int (*hash)(void *), int (*compare)(void *, void *),
	int borrowed, int total_borrows, double rating, int marks)
{
	unsigned int pos = (*(library->hash))(title) % library->hmax;
	List *shelf;

	shelf = library->buckets[pos];

	if(has_book(library, title))
		remove_book(library, title);

	insert_book(shelf, title, hmax, hash, compare, no_definitions,
		borrowed, total_borrows, rating, marks);

	library->size++;
}

// the function implements the resizing facility of a library
void resize_library(Dictionary **dict)
{
	Dictionary *new, *content;
	List *shelf, *page;
	Node *aux1, *aux2;
	char *key, *word, *def;
	Book *value;
	int i, j, borrowed, total_borrows, marks;
	double rating;

	if(!(*dict)->size)
		return;

	// allocating a new library that is twice as big
	new = create_library(2 * (*dict)->hmax, (*dict)->hash, (*dict)->compare);

	for(i = 0; i < (*dict)->hmax; i++)
	{
		// iterating through the shelves
		shelf = (*dict)->buckets[i];

		if(shelf->size)
		{
			// iterating through the books of the shelf
			aux1 = shelf->first;
			while(aux1 != NULL)
			{
				// gaining the information from the book
				key = (char *)(((Info *)aux1->data)->key);
				value = (Book *)(((Info *)aux1->data)->value);
				borrowed = value->borrowed;
				total_borrows = value->total_borrows;
				rating = value->rating;
				marks = value->marks;

				// adding a book with the entry's information
				// to the new library (with no definitions)
				add_book(new, key, 0, new->hmax, new->hash, new->compare,
					borrowed, total_borrows, rating, marks);

				// iterating through the pages of the book
				content = value->content;
				if(content->size)
				{
					for(j = 0; j < content->hmax; j++)
					{
						page = content->buckets[j];

						if(page->size)
						{
							// iterating through the definitions of the page
							aux2 = page->first;
							while(aux2 != NULL)
							{
								word = (char *)(((Info *)aux2->data)->key);
								def = (char *)(((Info *)aux2->data)->value);
								// adding the definitions
								// to the newly inserted book
								add_def(new, key, word, def);
								aux2 = aux2->next;
							}
						}
					}
				}

				aux1 = aux1->next;
			}
		}
	}

	// deallocating the memory for the old library
	free_library(*dict);
	// modifying the address the old library points to
	// (it now points to the newly allocated one)
	*dict = new;
}

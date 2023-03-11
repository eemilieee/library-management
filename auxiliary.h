// Copyright 2022 Arpasanu Emilia-Oana 311CA (emilia.arpasanu@stud.acs.upb.ro)
#ifndef AUXILIARY_H_
#define AUXILIARY_H_

typedef struct User User;

// required information about a user found within the database
struct User
{
	char book_title[41];
	int points;
	int banned;
	int period;
};

typedef struct Node Node;

// a node which is found within a bucket (list) of a dictionary
// (the entity used to store the information in the database)
struct Node
{
	Node *next;
	void *data;
};

typedef struct List List;

// the structure of a bucket found in a dictionary
// (it becomes a shelf/page)
struct List
{
	Node *first, *last;
	int size;
	int data_size;
};

typedef struct Info Info;

// the pair that defines the stored information from the dictionary
// (a "unique identifier" and the data)
struct Info
{
	void *key;
	void *value;
};

typedef struct Dictionary Dictionary;

// the structure of a dictionary (it becomes the database)
struct Dictionary
{
	List **buckets;
	// the number of entries
	int size;
	int hmax;
	unsigned int (*hash)(void *);
	int (*compare)(void *, void *);
};

typedef struct Book Book;

// required information about a book found within the library
struct Book
{
	int borrowed;
	int total_borrows;
	double rating;
	int marks;
	Dictionary *content;
};

typedef struct BookInfo BookInfo;

struct BookInfo
{
	char title[41];
	double rating;
	int total_borrows;
};

typedef struct UserInfo UserInfo;

struct UserInfo
{
	char name[41];
	int points;
};

int compare_strings(void *a, void *b);
unsigned int hash_string(void *a);
int compare_book_info(BookInfo a, BookInfo b);
int compare_user_info(UserInfo a, UserInfo b);
int correct_command(char s[]);

List *create_list(int data_size);

Dictionary *create_library(int hmax, unsigned int (*hash)(void *),
	int (*compare)(void *, void *));

Node *create_book(int key_size, void *key, int value_size,
	int hmax, unsigned int (*hash)(void *), int (*compare)(void *, void *),
		int borrowed, int total_borrows, double rating, int marks);

void insert_book(List *shelf, char *title, int hmax,
	unsigned int (*hash)(void *), int (*compare)(void *, void *),
		int no_definitions, int borrowed, int total_borrows,
			double rating, int marks);

int has_book(Dictionary *library, void *key);

void *find_book(Dictionary *library, char *title);

void delete_book(List *list, int pos);

void free_shelf(List *list);

void resize_library(Dictionary **dict);

void free_library(Dictionary *library);

Dictionary *create_pages(int hmax, unsigned int (*hash)(void *),
	int (*compare)(void *, void *));

Node *create_definition(int word_size, void *word, int def_size, void *def);

void add_definition(List *list, int word_size, void *word,
	int def_size, void *def);

void put_definition(Dictionary *book, void *key, int key_size,
	void *value, int value_size);

int has_definition(Dictionary *book, void *key);

void *get_definition(Dictionary *book, void *key);

void delete_definition(List *list, int pos);

void remove_definition(Dictionary *book, void *key);

void free_definitions(List *list);

void resize_pages(Dictionary **dict);

void free_pages(Dictionary *dict);

Dictionary *create_users_database(int hmax, unsigned int (*hash)(void *),
	int (*compare)(void *, void *));

Node *create_user(int name_size, void *name, int info_size, char *title,
	int points, int banned, int period);

void insert_user(List *list, int name_size, void *name, char *title,
	int points, int banned, int period);

void put_user(Dictionary *data_base, void *key, int key_size, char *title,
	int points, int banned, int period);

int has_user(Dictionary *data_base, void *key);

void *get_user(Dictionary *data_base, void *key);

void delete_user(List *list, int pos);

void resize_users_database(Dictionary **dict);

void free_users(List *list);

void free_users_database(Dictionary *dict);

#endif  // AUXILIARY_H_

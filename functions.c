// Copyright 2022 Arpasanu Emilia-Oana 311CA (emilia.arpasanu@stud.acs.upb.ro)

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<inttypes.h>

#include "functions.h"
#include "auxiliary.h"

#define HMAX 10

// the function prints the information about a user from the database
void show_user(Dictionary *data_base, char *name)
{
	User *user;

	if(!has_user(data_base, name))
		return;

	user = get_user(data_base, name);

	if(!user->banned)
		printf("Name:%s Points:%d\n", name, user->points);
}

// the function adds a new user to the database
// if needed, it is resized (the load factor is greater than 1)
void add_user(Dictionary **data_base, char *name, char *title,
	int points, int banned, int period)
{
	double load_factor;

	if(has_user(*data_base, name))
	{
		printf("User is already registered.\n");
		return;
	}

	put_user(*data_base, name, strlen(name) + 1, title,
		points, banned, period);

	load_factor = 1.0 * (*data_base)->size / (*data_base)->hmax;
	if(load_factor > 1)
		resize_users_database(data_base);
}

// the function removes a given book from a certain library
void remove_book(Dictionary *library, char *title)
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

// the function prints the information of a certain book from the library
void get_book(Dictionary *library, char *title)
{
	Book *book;

	if(!has_book(library, title))
		printf("The book is not in the library.\n");
	else
	{
		book = find_book(library, title);
		printf("Name:%s Rating:%.3f Purchases:%d\n", title,
			book->rating, book->total_borrows);
	}
}

// the function inserts a definition into a certain book
void add_def(Dictionary *library, char *title, char *word, char *def)
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

// the function prints the information of a certain definition found in a book
// of the library
void get_def(Dictionary *library, char *title, char *word)
{
	Book *book;
	void *search;

	if(!has_book(library, title))
		printf("The book is not in the library.\n");
	else
	{
		book = find_book(library, title);
		search = get_definition(book->content, word);
		if(search == NULL)
			printf("The definition is not in the book.\n");
		else
			printf("%s\n", (char *)search);
	}
}

// the function removes a definition found in a given book of the library
void remove_def(Dictionary *library, char *title, char *word)
{
	Book *book;

	if(!has_book(library, title))
		printf("The book is not in the library.\n");
	else
	{
		book = find_book(library, title);
		remove_definition(book->content, word);
	}
}

// the function sets the given book as borrowed and updates the
// information of the borrower(the estimated period of time needed to read it)
void borrow(Dictionary *data_base, char *name, Dictionary *library,
	char *title, int days)
{
	Book *book;
	User *user;

	user = get_user(data_base, name);
	if(user == NULL)
		printf("You are not registered yet.\n");
	else
	{
		if(user->banned)
			printf("You are banned from this library.\n");
		else
		{
			if(strlen(user->book_title))
					printf("You have already borrowed a book.\n");
			else
			{
				book = find_book(library, title);
				if(book == NULL)
					printf("The book is not in the library.\n");
				else
				{
					if(book->borrowed)
						printf("The book is borrowed.\n");
					else
					{
						snprintf(user->book_title, strlen(title) + 1, "%s", title);
						book->borrowed = 1;
						user->period = days;
					}
				}
			}
		}
	}
}

// the function sets the given book as available and updates the
// information about the book (rating and purchases) and the
// borrower (the cumulated number of points, the title of a currently
// borrowed book and if he becomes banned)
void return_book(Dictionary *data_base, char *name, Dictionary *library,
	char *title, int days, int marks)
{
	Book *book;
	User *user;

	user = get_user(data_base, name);
	if(user == NULL)
		printf("You are not registered yet.\n");
	else
	{
		if(user->banned)
			printf("You are banned from this library.\n");
		else
		{
			if(strcmp(user->book_title, title))
				printf("You didn't borrow this book.\n");
			else
			{
				book = find_book(library, title);
				if(user->period < days)
					user->points -= (2 * (days - user->period));
				else
					user->points += (user->period - days);
				book->marks += marks;
				book->total_borrows++;
				book->rating = 1.0 * book->marks/book->total_borrows;
				book->borrowed = 0;
				user->period = 0;
				snprintf(user->book_title, strlen("") + 1, "%s", "");
				if(user->points < 0)
				{
					printf("The user %s has been banned from this library.\n", name);
					user->banned = 1;
				}
			}
		}
	}
}

// the function removes the book from the library and updates the information
// about the borrower (the cumulated number of points and if he becomes banned)
void lost(Dictionary *data_base, char *name, Dictionary *library, char *title)
{
	Book *book;
	User *user;

	user = get_user(data_base, name);
	if(user == NULL)
		printf("You are not registered yet.\n");
	else
	{
		if(user->banned)
			printf("You are banned from this library.\n");
		else
		{
			book = find_book(library, title);
			if(book == NULL)
				printf("The book is not in the library.\n");
			else
			{
				if(strcmp(user->book_title, title))
					printf("You didn't borrow this book.\n");
				else
				{
					user->points -= 50;
					user->period = 0;
					snprintf(user->book_title, strlen("") + 1, "%s", "");
					remove_book(library, title);
					if(user->points < 0)
					{
						printf("The user %s has been banned from this library.\n", name);
						user->banned = 1;
					}
				}
			}
		}
	}
}

// the function prints the information of all the books found within
// the library in a sorted order using an auxiliary array to determine
// the succession of the keys with which the details are gathered from the
// library
void top_books(Dictionary *library)
{
	int i, j, nr = 0;
	BookInfo *v, temp;
	List *shelf;
	Book *book;
	Node *aux;

	printf("Books ranking:\n");

	if(!library->size)
		return;

	v = (BookInfo *)malloc(library->size * sizeof(BookInfo));

	for(i = 0; i < library->hmax; i++)
	{
		shelf = library->buckets[i];

		if(shelf->size)
		{
			aux = shelf->first;

			while(aux != NULL)
			{
				book = (Book *)(((Info *)aux->data)->value);
				v[nr].total_borrows = book->total_borrows;
				v[nr].rating = book->rating;
				snprintf(v[nr].title, strlen((char *)(((Info *)aux->data)->key)) + 1,
					"%s", (char *)(((Info *)aux->data)->key));
				nr++;
				aux = aux->next;
			}
		}
	}

	for(i = 0; i < library->size - 1; i++)
		for(j = i + 1; j < library->size; j++)
			if(compare_book_info(v[i], v[j]))
			{
				temp = v[i];
				v[i] = v[j];
				v[j] = temp;
			}

	for(i = 0; i < library->size; i++)
	{
		printf("%d. ", i + 1);
		get_book(library, v[i].title);
	}

	free(v);
}

// the function prints the information of all the users who are not banned
// found within the database in a sorted order using an auxiliary array
// to determine the succession of the keys with which the details are gathered
// from the database
void top_users(Dictionary *data_base)
{
	int i, j, nr = 0;
	UserInfo *v, temp;
	List *page;
	User *user;
	Node *aux;

	printf("Users ranking:\n");

	if(!data_base->size)
		return;

	v = (UserInfo *)malloc(data_base->size * sizeof(UserInfo));

	for(i = 0; i < data_base->hmax; i++)
	{
		page = data_base->buckets[i];

		if(page->size)
		{
			aux = page->first;

			while(aux != NULL)
			{
				user = (User *)(((Info *)aux->data)->value);
				if(!user->banned)
				{
					v[nr].points = user->points;
					snprintf(v[nr].name, strlen((char *)(((Info *)aux->data)->key)) + 1,
						"%s", (char *)(((Info *)aux->data)->key));
					nr++;
				}
				aux = aux->next;
			}
		}
	}

	for(i = 0; i < nr - 1; i++)
		for(j = i + 1; j < nr; j++)
			if(compare_user_info(v[i], v[j]))
			{
				temp = v[i];
				v[i] = v[j];
				v[j] = temp;
			}

	for(i = 0; i < nr; i++)
	{
		printf("%d. ", i + 1);
		show_user(data_base, v[i].name);
	}

	free(v);
}

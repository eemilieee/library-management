// Copyright 2022 Arpasanu Emilia-Oana 311CA (emilia.arpasanu@stud.acs.upb.ro)
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "auxiliary.h"

void add_book(Dictionary *library, char *title, int no_definitions, int hmax,
	unsigned int (*hash)(void *), int (*compare)(void *, void *),
	int borrowed, int total_borrows, double rating, int marks);

void get_book(Dictionary *library, char *title);

void remove_book(Dictionary *library, char *title);

void add_def(Dictionary *library, char *title, char *word, char *def);

void get_def(Dictionary *library, char *title, char *word);

void remove_def(Dictionary *library, char *title, char *word);

void add_user(Dictionary **data_base, char *name, char *title,
	int points, int banned, int period);

void show_user(Dictionary *data_base, char *name);

void borrow(Dictionary *data_base, char *name, Dictionary *library,
	char *title, int days);

void return_book(Dictionary *data_base, char *name, Dictionary *library,
	char *title, int days, int marks);

void lost(Dictionary *data_base, char *name, Dictionary *library, char *title);

void top_books(Dictionary *library);

void top_users(Dictionary *data_base);

#endif  // FUNCTIONS_H_

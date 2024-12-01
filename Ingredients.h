#ifndef _INGREDIENTS_H_
#define _INGREDIENTS_H_

#include "Arduino.h"

#define nb_ingredients 11
#define nb_cocktails 19
#define nb_noncocktails 7
#define nb_prime 11

struct Ingredient
{
	String IngredientName;
  int r;
  int g;
  int b;
  int step;
  float volume;
};

struct Recipe
{
  String RecipeName;
  int IngredientVolume[11];
};

String getRecipeIngredients(Recipe myRecipe);

extern struct Ingredient Grenadine;
extern struct Ingredient ZuckerSirup;
extern struct Ingredient Lime;
extern struct Ingredient Orange;
extern struct Ingredient Ananas;
extern struct Ingredient Cola;
extern struct Ingredient Ingwer;
extern struct Ingredient Soda;
extern struct Ingredient Vodka;
extern struct Ingredient Tequila;
extern struct Ingredient Rum;


extern Ingredient Ingredients_List[11];
extern Recipe Alcoholic_List[nb_cocktails];
extern Recipe NonAlcoholic_List[nb_cocktails];
extern Recipe Prime_List[nb_cocktails];

#endif
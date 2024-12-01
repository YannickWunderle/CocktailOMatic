#include "Ingredients.h"

Ingredient Grenadine = {"Grenadine",100,0,0,-104,0};
Ingredient ZuckerSirup = {"Sirup",25,25,25,-110,0};
Ingredient Lime= {"Limette",25,75,0,-112,0};
Ingredient Orange = {"Orange",75,25,0,-120,0};
Ingredient Ananas = {"Ananas",25,85,10,-112,0};
Ingredient Cola = {"Cola",75,0,25,-112,0};
Ingredient Ingwer = {"Ingwer",50,50,0,-100,0};
Ingredient Soda = {"Soda",0,0,100,-112,0};
Ingredient Vodka = {"Vodka",25,25,100,-100,0};
Ingredient Tequila = {"Tequila",50,0,40,-128,0};
Ingredient Rum = {"Rum",25,0,75,-110,0};
                                                  //Grenadine, Sirup, Lime,Orange, Ananas, Cola, Ingwer, Soda, Vodka, Tequila, Rum
extern struct Recipe Tropical_Sunset =       {"Gehirntot",{0,  0,      0,    0,     0,      0,    0,      0,    10,       10,      10}};
extern struct Recipe Daiquiri =               {"Daiquiri",{0,  2,      3,    0,     0,      0,    0,      0,    0,       0,      5}};
extern struct Recipe Planeters =       {"Planeters Punch",{1,  1,      3,    6,     0,      0,    0,      0,    0,       0,      6}};
extern struct Recipe Mojito =                   {"Mojito",{0,  2,      2,    0,     0,      0,    0,      6,    0,       0,      6}};
extern struct Recipe Tequila_Mojito =   {"Tequila Mojito",{0,  2,      2,    0,     0,      0,    0,      8,    0,       5,      0}};
extern struct Recipe Tequila_Sunrise = {"Tequila Sunrise",{2,  0,      1,    10,     0,      0,    0,      0,    0,       6,      0}};
extern struct Recipe Julie =                     {"Julie",{2,  0,      0,    0,     10,      0,    0,      0,    0,       5,      0}};
extern struct Recipe Tommys =         {"Tommys Margarita",{0,  2,      3,    0,     0,      0,    0,      0,    0,       6,      0}};
extern struct Recipe Cuba_Libre =           {"Cuba Libre",{0,  0,      1,    0,     0,      10,    0,      0,    0,       0,      4}};
extern struct Recipe Batanga =                 {"Batanga",{0,  0,      3,    0,     0,      11,    0,      0,    0,       4,      0}};
extern struct Recipe Lover =                 {"Mr. Lover",{0,  0,      3,    0,     6,      0,    0,      0,    0,       2,      2}};
extern struct Recipe Bitch =              {"Skinny Bitch",{0,  0,      3,    0,     0,      0,    0,      8,    4,       0,      0}};
extern struct Recipe Moscow_M =            {"Moscow Mule",{0,  0,      2,    0,     0,      0,    10,      0,    6,       0,      0}};
extern struct Recipe SexOtB =         {"Sex on the Beach",{1,  0,      2,    6,     6,      0,    0,      0,    5,       0,      0}};
extern struct Recipe LongIIT =     {"Long Island Ice Tea",{0,  2,      2,    0,     0,      12,    0,      0,    2,       2,      2}};
extern struct Recipe Painkiller =           {"Painkiller",{0,  0,      0,    4,     10,      0,    0,      0,    0,       0,      8}};
extern struct Recipe DarknS =          {"Dark and Stormy",{0,  0,      4,    0,     0,      0,    12,      0,    0,       0,      6}};
extern struct Recipe T_Mule =             {"Tequila Mule",{0,  0,      2,    0,     0,      0,    12,      0,    0,       6,      0}};
extern struct Recipe GingerCocktail =           {"Ginger",{0,  0,      2,    4,     4,      0,    10,      0,    0,       4,      0}};
extern struct Recipe Ambassador =           {"Ambassador",{0,  1,      0,    6,     0,      0,    0,      0,    0,       2,      0}};

//----------------------Alkoholfrei---------------------------------------------------------------------
                                                  //Grenadine, Sirup, Lime,Orange, Ananas, Cola, Ingwer, Soda, Vodka, Tequila, Rum
extern struct Recipe Sober_Sunset =       {"Sober_Sunset",{1,  1,      1,    3,     3,      0,    0,      2,    0,       0,      0}};
extern struct Recipe Cola_Sauer =            {"Cola-Lime",{0,  1,      1,    0,     0,      6,    0,      0,    0,       0,      0}};
extern struct Recipe Fruit_Punch =         {"Fruit Punch",{2,  1,      3,    0,     4,      0,    0,      0,    0,       0,      0}};
extern struct Recipe Ginger_Sweet =       {"Sweet Ginger",{1,  1,      0,    0,     0,      0,    6,      2,    0,       0,      0}};
extern struct Recipe Ananas_drink =                 {"Ananas",{0,  0,      0,    0,     3,      0,    0,      2,    0,       0,      0}};
extern struct Recipe Orange_drink =                   {"Orange",{0,  0,      0,    3,     0,      0,    0,      2,    0,       0,      0}};
extern struct Recipe Cola_drink =                       {"Cola",{0,  0,      0,    0,     0,      1,    0,      0,    0,       0,      0}};

//----------------------Prime and Shots---------------------------------------------------------------------
                                                  //Grenadine, Sirup, Lime,Orange, Ananas, Cola, Ingwer, Soda, Vodka, Tequila, Rum
extern struct Recipe pur_Grenadine =         {"Grenadine",{1,  0,      0,    0,     0,      0,    0,      0,    0,       0,      0}};
extern struct Recipe pur_Sirup =                 {"Sirup",{0,  1,      0,    0,     0,      0,    0,      0,    0,       0,      0}};
extern struct Recipe pur_Lime =                {"Limette",{0,  0,      1,    0,     0,      0,    0,      0,    0,       0,      0}};
extern struct Recipe pur_Orange =               {"Orange",{0,  0,      0,    1,     0,      0,    0,      0,    0,       0,      0}};
extern struct Recipe pur_Ananas =               {"Ananas",{0,  0,      0,    0,     1,      0,    0,      0,    0,       0,      0}};
extern struct Recipe pur_Cola =                   {"Cola",{0,  0,      0,    0,     0,      1,    0,      0,    0,       0,      0}};
extern struct Recipe pur_Ingwer =               {"Ingwer",{0,  0,      0,    0,     0,      0,    1,      0,    0,       0,      0}};
extern struct Recipe pur_Soda =                   {"Soda",{0,  0,      0,    0,     0,      0,    0,      1,    0,       0,      0}};
extern struct Recipe pur_Vodka =                 {"Vodka",{0,  0,      0,    0,     0,      0,    0,      0,    1,       0,      0}};
extern struct Recipe pur_Tequila =             {"Tequila",{0,  0,      0,    0,     0,      0,    0,      0,    0,       1,      0}};
extern struct Recipe pur_Rum =                     {"Rum",{0,  0,      0,    0,     0,      0,    0,      0,    0,       0,      1}};
extern struct Recipe pur_all =               {"Prime all",{1,  1,      1,    1,     1,      1,    1,      1,    1,       1,      1}};
extern struct Recipe flush_all =               {"Flush all 1 Liter",{1,  1,      1,    1,     1,      1,    1,      1,    1,       1,      1}};

extern Ingredient Ingredients_List[11] = {Grenadine,ZuckerSirup,Lime, Orange, Ananas, Cola, Ingwer, Soda, Vodka, Tequila, Rum};
extern Recipe Alcoholic_List[nb_cocktails] = {Daiquiri,Planeters,Mojito,Cuba_Libre,DarknS,Painkiller,Lover,LongIIT,Batanga,Tequila_Sunrise,Tequila_Mojito,Julie,Tommys,T_Mule,GingerCocktail,Ambassador,Moscow_M,SexOtB,Bitch};
extern Recipe NonAlcoholic_List[nb_cocktails] = {Sober_Sunset,Cola_Sauer,Fruit_Punch,Ginger_Sweet,Ananas_drink,Orange_drink,Cola_drink};
extern Recipe Prime_List[nb_cocktails] = {pur_Grenadine,pur_Sirup,pur_Lime,pur_Orange,pur_Ananas,pur_Cola,pur_Ingwer,pur_Soda,pur_Vodka,pur_Tequila,pur_Rum,pur_all,flush_all};

String getRecipeIngredients(Recipe myRecipe) {
  String s = "";
  for (int i = 0; i < nb_ingredients; i++) {
    if (myRecipe.IngredientVolume[i] > 0) {
      s += Ingredients_List[i].IngredientName;
      s +="; ";
    }
    
  }
  return s;
};




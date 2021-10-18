// Libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[])
{
    char action[40];

    // tant qu'il n'y a pas une action valide
    while (1)
    {
        // demande l'action
        printf("Voulez vous ajouter un exercice ou voir votre progrès ? ");
        scanf("%s", action);

        // transformer l'entrée en minuscules
        for (int i = 0, n = strlen(action); i < n; i++)
        {
            action[i] = tolower(action[i]);
        }
        // si c'est ajouter
        if (strcmp(action, "ajouter") == 0)
        {
            // demander le langage utilisé, le nom et la difficulté
            char language[40];
            char name[160];
            char difficulty[60];
            printf("Entrez le langage utilisé pour cet exercice : ");
            scanf("%s", language);

            char temp;
            printf("Entrez le nom de l'exercice : ");
            // permettre de rentrer aussi des espaces
            scanf("%c", &temp);
            scanf("%[^\n]", name);

            printf("Entrez la difficulté que vous avez ressenti lors de cet exercice : ");
            // permettre de rentrer aussi des espaces
            scanf("%c", &temp);
            scanf("%[^\n]", difficulty);

            // récuperer la date actuelle
            time_t rawtime;
            struct tm *timeinfo;

            time(&rawtime);
            timeinfo = localtime(&rawtime);

            // ouvrir le fichier des stats
            FILE *file = fopen("statsC.csv", "a");

            // si le fichier n'existe pas
            if (!file)
            {
                return 1;
            }

            // écrire dans le fichier la date, le langage, le nom et la difficulté
            fprintf(file, "%d%d%d, %s, %s, %s\n", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, language, name,
                    difficulty);

            // fermer le fichier
            fclose(file);
            return 0;
        }
        // si l'action est lire ou voir
        else if (strcmp(action, "lire") == 0 || strcmp(action, "voir") == 0)
        {
            // ouvrir le fichier des stats
            FILE *file = fopen("statsC.csv", "rb");

            // si le fichier n'existe pas
            if (!file)
            {
                printf("Vous n'avez pas encore entrer d'exercices !");
                return 1;
            }
            // mettre le pointer de lecture a la fin
            fseek(file, 0, SEEK_END);

            // récuperer la taille du fichier
            int size = ftell(file);
            // mettre le pointer de lecture au debut
            fseek(file, 0, SEEK_SET);
            // sauvegarder tout les caractères dans un tableau
            unsigned char bytes[size];
            // lire le fichier et sauvegarder le resultat dans le tableau bytes
            fread(bytes, size, 1, file);

            int lines = 0;

            for (int i = 0; i < size; i++)
            {
                // incrementer lines par 1 quand il y a un retour a la ligne
                if (bytes[i] == '\n')
                {
                    lines++;
                }
            }

            // s'il y a plus d'une ligne de stats
            if (lines > 1)
            {
                char sort[40];
                // tant qu'il n'y a pas une réponse valide
                while (1)
                {
                    // demander si on trie la liste
                    printf("Voulez vous trier cette liste d'exercice ? ");
                    scanf("%s", sort);

                    // convertir sort en minuscules
                    for (int i = 0, n = strlen(sort); i < n; i++)
                    {
                        sort[i] = tolower(sort[i]);
                    }

                    // si la réponse est 'oui' ou 'o'
                    if (strcmp(sort, "oui") == 0 || strcmp(sort, "o") == 0)
                    {
                        char typeSort[40];
                        // tant qu'il n'y a pas une réponse valide
                        while (1)
                        {
                            // demander le type de tri (date ou langage)
                            printf("Voulez vous trier par date ou bien par le langage utilisé ? ");
                            scanf("%s", typeSort);

                            // convertir typeSort en minuscules
                            for (int i = 0, n = strlen(typeSort); i < n; i++)
                            {
                                typeSort[i] = tolower(typeSort[i]);
                            }
                            // si on tri par la date
                            if (strcmp(typeSort, "date") == 0)
                            {
                                char line[256];

                                int fsize = 0;
                                // aller au début du fichier
                                fseek(file, 0, SEEK_SET);

                                // on récupere le nombre de lignes (resultat dans fsize)
                                while (fgets(line, sizeof(line), file))
                                {
                                    fsize++;
                                }

                                // tableau en 2d (lignes et contenu de la ligne)
                                char bytesSortByDate[fsize][256];

                                // on retourne au début du fichier
                                fseek(file, 0, SEEK_SET);

                                // on récupere les stats dans le tableau en 2D
                                for (int i = 0; i < fsize; i++)
                                {
                                    fgets(bytesSortByDate[i], sizeof(bytesSortByDate[i]), file);
                                }

                                /*
                                    On récupere le début de chaque ligne dans un tableau en 1D pour faire un tri a bulle
                                */
                                // variables pour récuperer le début de chaque ligne
                                long val = 0;
                                int tmp;

                                long dateValue[fsize];
                                // pour chaque ligne
                                for (int j = 0; j < fsize; j++)
                                {
                                    val = 0;
                                    tmp = 0;
                                    // les 8 premiers elements (les chiffres)
                                    for (int i = 0; i < 8; i++)
                                    {
                                        // convertir ce caractère en int
                                        tmp = (int) bytesSortByDate[j][i] - '0';
                                        // on inverse l'ordre (le premier chiffre est le plus grand donc 10000000,
                                        // on diminue en divisant par 10 puissance i (donc a la fin on est a 1))
                                        val += tmp * (10000000 / (pow(10, i)));
                                    }
                                    dateValue[j] = val;
                                }
                                // initialisation d'une variable temporaire pour swaper les valeurs dans le tri a bulle
                                char swap[256];
                                // TRI A BULLES
                                for (int c = 0 ; c < fsize - 1; c++)
                                {
                                    for (int d = 0 ; d < fsize - c - 1; d++)
                                    {
                                        if (dateValue[d] > dateValue[d + 1])
                                        {
                                            // copie des valeurs avec strcpy
                                            strcpy(swap, bytesSortByDate[d]);
                                            strcpy(bytesSortByDate[d], bytesSortByDate[d + 1]);
                                            strcpy(bytesSortByDate[d + 1], swap);
                                        }
                                    }
                                }
                                // affichage du tableau qui est trier par la date
                                for (int i = 0; i < fsize; i++)
                                {
                                    printf("%s", bytesSortByDate[i]);
                                }
                                return 0;

                            }
                            // si on trie par le langage utilisé
                            else if (strcmp(typeSort, "langage") == 0)
                            {
                                char line[256];

                                int fsize = 0;
                                // aller au début du fichier
                                fseek(file, 0, SEEK_SET);

                                // récuperer le nombre de ligne dans fsize
                                while (fgets(line, sizeof(line), file))
                                {
                                    fsize++;
                                }

                                // une variable pour le tri et une autre pour l'affichage
                                char bytesSortByLang[fsize][256];
                                char bytesSortByLangCopy[fsize][256];

                                // aller au début du fichier
                                fseek(file, 0, SEEK_SET);

                                // récuperer le texte
                                for (int i = 0; i < fsize; i++)
                                {
                                    fgets(bytesSortByLang[i], sizeof(bytesSortByLang[i]), file);
                                }
                                // aller au début du fichier
                                fseek(file, 0, SEEK_SET);
                                // récuperer le texte
                                for (int i = 0; i < fsize; i++)
                                {
                                    fgets(bytesSortByLangCopy[i], sizeof(bytesSortByLangCopy[i]), file);
                                }

                                // délimiteur pour spliter le ligne
                                char delim[] = ", ";
                                char *ptr;

                                // stockage des résultats
                                char languageValue[fsize][256];

                                int counter;

                                for (int j = 0; j < fsize; j++)
                                {
                                    // récuperer l'element du split de la ligne j
                                    // on utilise la copie car cette opération change la chaine de caracteres
                                    // pour plus de simplicité, on a fait deux tableaux pour garder l'original lors de l'affichage
                                    ptr = strtok(bytesSortByLangCopy[j], delim);
                                    counter = 0;
                                    // tant que le pointeur n'est pas nul
                                    while (ptr != NULL)
                                    {
                                        // on veux le langage donc la 2è valeur, soit 1
                                        if (counter == 1)
                                        {
                                            // copie dans langageValue
                                            strcpy(languageValue[j], ptr);
                                        }
                                        counter++;
                                        ptr = strtok(NULL, delim);
                                    }
                                }

                                // TRI A BULLES

                                char temp[256];

                                for (int j = 0; j < fsize - 1; j++)
                                {
                                    for (int i = j + 1; i < fsize; i++)
                                    {
                                        if (strcmp(languageValue[j], languageValue[i]) > 0)
                                        {
                                            strcpy(temp, bytesSortByLang[j]);
                                            strcpy(bytesSortByLang[j], bytesSortByLang[i]);
                                            strcpy(bytesSortByLang[i], temp);
                                        }
                                    }
                                }
                                // affichage
                                for (int i = 0; i < fsize; i++)
                                {
                                    printf("%s", bytesSortByLang[i]);
                                }
                                return 0;
                            }
                        }
                    }
                    // si on ne veux pas trier la liste
                    else if (strcmp(sort, "non") == 0 || strcmp(sort, "n") == 0)
                    {
                        for (int i = 0; i < size; i++)
                        {
                            printf("%c", bytes[i]);
                        }
                        return 0;
                    }
                }
            }
            // s'il n'y a qu'une ligne
            else
            {
                for (int i = 0; i < size; i++)
                {
                    printf("%c", bytes[i]);
                }
                return 0;
            }
            // fermer le fichier
            fclose(file);
        }
    }

}

# Libraries
from datetime import datetime
import os
import re


def main():
    # tant qu'il n'y a pas eu un mot valide
    while True:
        # demande de l'action a faire
        action = input("Voulez vous ajouter un exercice ou voir votre progrès ? ")
        # vérifier que l'action est ajouter
        if action.lower() == "ajouter":
            # demande du langage utilisé, le nom et la difficulté
            language = input("Entrez le langage utilisé pour cet exercice : ")
            name = input("Entrez le nom de l'exercice : ")
            difficulty = input("Entrez la difficulté que vous avez ressenti lors de cet exercice : ")
            now = datetime.now()
            # ouvrir le fichier des stats
            with open("statsPY.csv", "a") as f:
                # écrire les entrées dans ce fichier
                f.write((f"{now.strftime('%d-%m-%Y')}, {language}, {name}, {difficulty}\n"))
        # si l'action est lire ou voir
        elif action.lower() == "lire" or action.lower() == "voir":
            # vérification que le fichier existe
            if os.path.exists("statsPY.csv"):
                # ouverture du ficher
                with open("statsPY.csv", "r") as f:
                    # récuperer le contenu
                    data = f.readlines()
                    # s'il y a plus d'une ligne
                    if len(data) > 1:
                        # tant qu'il n'y a pas un autre mot valide
                        while True:
                            # demander si on veux trier l'affichage
                            sort = input("Voulez vous trier votre liste d'exercice ? ")
                            # si la réponse est oui
                            if re.search("^o(ui)?$", sort, re.IGNORECASE):
                                # tant qu'il n'y a pas un mot valide
                                while True:
                                    # demande du type de tri
                                    typeSort = input("Voulez vous trier par date ou bien par le langage utilisé ? ")
                                    # si c'est un tri de date
                                    if typeSort.lower() == "date":
                                        # tri du fichier
                                        dataSortByDate = sorted(data, key=lambda x: x[0:9][::-1])
                                        # affichage des stats
                                        print()
                                        for line in dataSortByDate:
                                            print(line, end="")
                                        return 0
                                    # si on tri par le langage utilisé
                                    elif typeSort.lower() == "langage":
                                        # tri du ficher par la langue
                                        dataSortByLanguage = sorted(data, key=lambda x: x.split(", ")[1])
                                        # affichage
                                        print()
                                        for line in dataSortByLanguage:
                                            print(line, end="")
                                        return 0
                            # si on ne veux pas trier
                            elif re.search("^n(on)?$", sort, re.IGNORECASE) or re.search("^n(o)?$", sort, re.IGNORECASE):
                                # affichage
                                print()
                                for line in data:
                                    print(line, end="")
                                return 0
                    else:
                        # affichage
                        print()
                        for line in data:
                            print(line, end="")
                        return 0
            # s'il n'y a pas de fichier déjà existant
            else:
                print("Vous n'avez pas encore entrer d'exercices !")
                return 1


main()
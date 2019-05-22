def PathFinding(): 

#-------------------------------recupération et traitement du fichier json contenant coord des obstacles----------------------------
    import time #pour tests a enlever après
    import json
    
    Xliste=[]
    Yliste=[]
    ListeObstacle=[]
    CoordObstacle=[]
    with open('Description_Environnement_SansAccent_V10-05-17.js') as json_data: #recup le fichier référancant obstacles, terrain ...
        data_dict = json.load(json_data)
        
    iObstacle=0
    iCoord=0
        
    obstacle_dict=data_dict.get("obstacles") #recupère LISTE des obstacles
    while (iObstacle<len(obstacle_dict)):
        type_Obstacle=obstacle_dict[iObstacle].get("type")
        coordonnees_Obstacle=obstacle_dict[iObstacle].get("coordonnees") #recupère LISTE des coord (sous forme dico) des différents obstacles
        
        if (type_Obstacle=="polygone"):
            while iCoord<len(coordonnees_Obstacle):
            
            
                x_y=coordonnees_Obstacle[iCoord].get("point") #recupère le dictionnaire contenant x et y
                
                x=x_y.get("x")
                Xliste.append(x)
                y=x_y.get("y")
                Yliste.append(y)
                
                iCoord=iCoord+1
            iCoord=0
                
                
                
            X1=min(Xliste) #on arrondit les polygones à des grands carrés contenant le dit polygone
            CoordObstacle.append(X1)
            Y1=min(Yliste)
            CoordObstacle.append(Y1)
            X2=max(Xliste)
            CoordObstacle.append(X2)
            Y2=max(Yliste)
            CoordObstacle.append(Y2)
               
            ListeObstacle.append(CoordObstacle)
            CoordObstacle=[]
            Xliste=[]
            Yliste=[]
                
                
        if (type_Obstacle=="cercle"):
            x_y_centre=coordonnees_Obstacle[0].get("centre")
            x_centre=x_y_centre.get("x")
            y_centre=x_y_centre.get("y")
                
            rayon=coordonnees_Obstacle[1].get("rayon")
                
            CoordObstacle=[]
            #on arondit également les cercles à des carrés les incluants
            X1=x_centre-rayon
            CoordObstacle.append(X1)
            Y1=y_centre-rayon
            CoordObstacle.append(Y1)
            X2=x_centre+rayon
            CoordObstacle.append(X2)
            Y2=y_centre+rayon
            CoordObstacle.append(Y2)
        
            ListeObstacle.append(CoordObstacle)
            CoordObstacle=[]
            Xliste=[]
            Yliste=[]
             
                
            
                   
        iObstacle=iObstacle+1
                
    
#coordonnées obstacles renvoyées sous forme d'une liste (1 élément par obstacle) de liste (d'éléments x1, y1, x2, y2)  
#------------------------------------------------fin traitement du fichier obstacle json---------------------------------------------------------     

#-------------------------------recupération et traitement du fichier json contenant coord depart etapes arrivée----------------------------


    with open('Evolution_Base_Roulante_SansAccent_V10-05-17.js') as json_data: #recup le fichier référancant obstacles, terrain ...
        chemin_dict = json.load(json_data)
        depart=chemin_dict.get("depart")
        arrivee=chemin_dict.get("arrivee")
        etapes=chemin_dict.get("etapes") #renvoie liste de dico
        
        ietapes=0
        etapeX=[]
        etapeY=[]
        etapeAngle=[]
        
        coord_actuelle_depart=depart.get("coordonnees")
        coord_actuelleX=coord_actuelle_depart.get("x")
        coord_actuelleY=coord_actuelle_depart.get("y")
        angle_depart=depart.get("angle")
        
        coord_arrivee=arrivee.get("coordonnees")
        angle_arrivee=arrivee.get("angle")
        Xcoord_arrivee=coord_arrivee.get("x")
        Ycoord_arrivee=coord_arrivee.get("y")
        
        while(ietapes<len(etapes)):
            coord_etape=etapes[ietapes].get("coordonnees")
            Xcoord_etape=coord_etape.get("x")
            Ycoord_etape=coord_etape.get("y")
            angle_etape=etapes[ietapes].get("angle")
            
            if (type(angle_etape)!=int and type(angle_etape)!=float): #cas ou pas de changement d'angle
                angle_etape=0
            
            etapeX.append(Xcoord_etape)
            etapeY.append(Ycoord_etape)
            etapeAngle.append(angle_etape)
            
            ietapes=ietapes+1
        
        etapeX.append(Xcoord_arrivee)
        etapeY.append(Ycoord_arrivee)
        etapeAngle.append(angle_arrivee) 
        
        
        
        
#-------------------------------------------------------------Test--------------------------------------------------------------------------------        
        #""" 
        print("début test json")
        print(Xliste)
        print(Yliste)
        print(ListeObstacle)
        print(etapeX)
        print(etapeY)
        print(etapeAngle)
        print(coord_actuelleX)
        print(coord_actuelleY)
        print(angle_depart)
        print("fin test json")
       #"""
        
        
 #------------------------------------------------fin traitement du fichier obstacle json---------------------------------------------------------            
   



# sortie fichier json:
#        ListeObstacle: [[X1,Y1,X2,Y2],[X1,Y1,X2,Y2],[X1,Y1,X2,Y2] ...]    
#        etapeX: [X1, X2, X3 ...] 
#        etapeY: [Y1, Y2, Y3 ...]
#        etapeAngle: [angle1, angle2, angle3 ...]
#        coord_actuelleX= x
#        coord_actuelleY= y
#        angle_depart= a (degré)
        
#-----------------------------------------------------début algorithme PathFinding-------------------------------------------------        
    nbDeplacement=1 #nb de cases de 50*50 parcourues apr robot (le terrain, obstacles, déplacements étant des multiples de cases de 50)

    coord_actuelle=[coord_actuelleX, coord_actuelleY] #sous forme de liste de coordonnées (plus simple pour les comparaison de coordonnées entre éléments)
    i_etapes=0
    
    Angle_actuel=0
    
    deplacements_faits=[] #liste des deplacements ex: (droite, droite , gauche, haut, bas ...)
    
    #coord_obstacle=[coord_obstacleX-coord_actuelle[0], coord_obstacleY-coord_actuelle[1]] #attention prend en compte qu'un seul obstacle; distance en cases entre obstacle et robot
    #coord_obstacle=(i/50 for i in coord_obstacle)

    while(i_etapes<len(etapeX)):
        cases_totales=abs(etapeX[i_etapes]-coord_actuelle[0]) + abs(etapeY[i_etapes]-coord_actuelle[1]) #distance en cases entre position finale et robot
        #cases_totales=cases_totales/50
        
        
        listeDroite=[]
        listeGauche=[]
        listeHaut=[]
        listeBas=[]
        
        listeMin=[]
        
        dejaAjout=0
        
        itest=0
        
        #deplacements_possibles={"droite":listeDroite,"gauche":listeGauche,"haut":listeHaut,"bas":listeBas} #dictionnaire avec en clé le type de deplacement (droite, gauche, haut, bas) et en valeur la distance en cases par rapport a la position de depart
        
        while (cases_totales > 2):#tant que robot pas arrivé (>25 car approximation d'arrivée)
            
            cases_totales=(abs(etapeX[i_etapes]-coord_actuelle[0]) + abs(etapeY[i_etapes]-coord_actuelle[1]))//50 
            position_droite=[coord_actuelle[0]+50, coord_actuelle[1]]
            position_gauche=[coord_actuelle[0]-50, coord_actuelle[1]]
            position_haute=[coord_actuelle[0], coord_actuelle[1]+50]
            position_basse=[coord_actuelle[0], coord_actuelle[1]-50]
    
            """
            print("droite")
            print(position_droite[0])
            print(position_droite[1])
            print("gauche")
            print(position_gauche[0])
            print(position_gauche[1])
            print("haute")
            print(position_haute[0])
            print(position_haute[1])
            print("basse")
            print(position_basse[0])
            print(position_basse[1])
            print("obstacle")
            print("xmin")
            print(ListeObstacle[0][0])
            print("ymin")
            print(ListeObstacle[0][1])
            print("xmax")
            print(ListeObstacle[0][2])
            print("ymax")
            print(ListeObstacle[0][3])
            """
            #print("yo")
            
            while (itest<len(ListeObstacle)):
                if (((position_droite[0] < ListeObstacle[itest][0]) or (position_droite[0] > ListeObstacle[itest][2])) and ((position_droite[1] < ListeObstacle[itest][1]) or (position_droite[1] > ListeObstacle[itest][3])) and dejaAjout!=1):
                    cases_totales=(abs(etapeX[i_etapes]-position_droite[0]) + abs(etapeY[i_etapes]-position_droite[1]))//50
                    listeDroite.append(nbDeplacement+cases_totales)
                    #deplacements_possibles["listeDroite"].append(nbDeplacement+cases_totales) 
                    dejaAjout=1
                itest=itest+1
            dejaAjout=0
            itest=0
            
            while (itest<len(ListeObstacle)):
                if (((position_gauche[0] < ListeObstacle[itest][0]) or (position_gauche[0] > ListeObstacle[itest][2])) and ((position_gauche[1] < ListeObstacle[itest][1]) or (position_gauche[1] > ListeObstacle[itest][3])) and dejaAjout!=1):
                    cases_totales=(abs(etapeX[i_etapes]-position_gauche[0]) + abs(etapeY[i_etapes]-position_gauche[1]))//50
                    listeGauche.append(nbDeplacement+cases_totales)
                    #deplacements_possibles["listeGauche"].append(nbDeplacement+cases_totales) 
                    dejaAjout=1
                itest=itest+1
            dejaAjout=0
            itest=0
            
            while (itest<len(ListeObstacle)):
                if (((position_haute[0] < ListeObstacle[itest][0]) or (position_haute[0] > ListeObstacle[itest][2])) and ((position_haute[1] < ListeObstacle[itest][1]) or (position_haute[1] > ListeObstacle[itest][3])) and dejaAjout!=1):
                    cases_totales=(abs(etapeX[i_etapes]-position_haute[0]) + abs(etapeY[i_etapes]-position_haute[1]))//50 
                    listeHaut.append(nbDeplacement+cases_totales)
                    #deplacements_possibles["listeHaut"].append(nbDeplacement+cases_totales); 
                    dejaAjout=1
                itest=itest+1
            dejaAjout=0
            itest=0
            
            while (itest<len(ListeObstacle)):            
                if (((position_basse[0] < ListeObstacle[itest][0]) or (position_basse[0] > ListeObstacle[itest][2])) and ((position_basse[1] < ListeObstacle[itest][1]) or (position_basse[1] > ListeObstacle[itest][3])) and dejaAjout!=1):
                    cases_totales=(abs(etapeX[i_etapes]-position_basse[0]) + abs(etapeY[i_etapes]-position_basse[1]))//50 
                    listeBas.append(nbDeplacement+cases_totales)
                    #deplacements_possibles["listeBas"].append(nbDeplacement+cases_totales);
                    dejaAjout=1
                itest=itest+1
            dejaAjout=0
            itest=0
                            
            nbDeplacement = nbDeplacement + 1
            #print(listeDroite)
            #print(listeGauche)
            #print(listeHaut)
            #print(listeBas)
            
            
            if (listeDroite != []):               
                listeMin.append(min(listeDroite))
            if (listeGauche != []): 
                listeMin.append(min(listeGauche))
            if (listeHaut != []): 
                listeMin.append(min(listeHaut))
            if (listeBas != []): 
                listeMin.append(min(listeBas))
        
            minimum=min(listeMin)
            Index=listeMin.index(minimum)
            
            #___  0=droite ___ 1=gauche ___ 2=haut ____ 3=bas ____
            if (Index==0):
                listeDroite.remove(minimum) #ca marche ?
                deplacements_faits.append([50,0,0]) #recupere le deplacement correspondant au chemin le plus court
                coord_actuelle=position_droite
            if (Index==1):
                listeGauche.remove(minimum)
                deplacements_faits.append([-50,0,0])
                coord_actuelle=position_gauche
            if (Index==2):
                listeHaut.remove(minimum)
                deplacements_faits.append([0,50,0])
                coord_actuelle=position_haute
            if (Index==3):
                listeBas.remove(minimum)
                deplacements_faits.append([0,-50,0])
                coord_actuelle=position_basse
            
            #print(coord_actuelle)
            #print(nbDeplacement)
            #print(listeMin)
            #time.sleep(1)
            #print(itest)
            #print(cases_totales)
            
            listeMin=[]
        
        deplacement_complementaire=[etapeX[i_etapes]-coord_actuelle[0],etapeY[i_etapes]-coord_actuelle[1],etapeAngle[i_etapes]-Angle_actuel]
        deplacements_faits.append(deplacement_complementaire)
        coord_actuelle=[etapeX[i_etapes],etapeY[i_etapes]]
        Angle_actuel=etapeAngle[i_etapes]
        i_etapes=i_etapes+1
        print(deplacements_faits)
        print(i_etapes)
        
    print(deplacements_faits)
    with open('deplacement.json', 'w') as f:
        json.dump(deplacements_faits, f)
                
 #!!!!!!!!!!!!!!!!!!!!!!!!!    problème cas où égalité de minimum ----> on veut garder le minimum correspondant au "nbDeplacement" 
 #!!!!!!!!!!!!!!!!!!!!!!!!!    le plus élevé (pour être dans la continuité du mvment)       
         
        

PathFinding()
